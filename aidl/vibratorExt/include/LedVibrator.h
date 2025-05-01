/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "Constant.h"
#include "VibratorExt.h"

using ::aidl::vendor::sun::hardware::vibratorExt::BaseVibrator;
using ::aidl::vendor::sun::hardware::vibratorExt::Effect;
using ::aidl::vendor::sun::hardware::vibratorExt::LevelRangeConfig;
using ::aidl::vendor::sun::hardware::vibratorExt::Style;

class LedVibrator : public BaseVibrator {
protected:
    LedVibrator(
        const LevelRangeConfig* levelRangeConfig,
        std::unordered_map<Effect, std::vector<std::pair<std::string, std::string>>> effectMap,
        std::vector<std::pair<std::string, std::string>> vmaxGainListAlarmNotifcation,
        std::vector<std::pair<std::string, std::string>> vmaxGainListDuration,
        std::vector<std::pair<std::string, std::string>> vmaxGainListHaptic
    ) : BaseVibrator(levelRangeConfig),
        mEffectMap(effectMap),
        mVmaxGainListAlarmNotifcation(vmaxGainListAlarmNotifcation),
        mVmaxGainListDuration(vmaxGainListDuration),
        mVmaxGainListHaptic(vmaxGainListHaptic) {}

    int writeNode(const std::string& path, const char* value) {
        int fd = TEMP_FAILURE_RETRY(open(path.c_str(), O_WRONLY));
        if (fd < 0) {
            ALOGE("open %s failed, errno: %d", path.c_str(), errno);
            return -errno;
        }

        int ret = TEMP_FAILURE_RETRY(write(fd, value, strlen(value) + 1));
        if (ret == -1) {
            ret = -errno;
        } else if (ret != static_cast<int>(strlen(value) + 1)) {
            /* even though EAGAIN is an errno value that could be set
            by write() in some cases, none of them apply here.  So, this return
            value can be clearly identified when debugging and suggests the
            caller that it may try to call vibrator_on() again */
            ret = -EAGAIN;
        } else {
            ret = 0;
        }

        errno = 0;
        close(fd);

        return ret;
    }
private:
    std::unordered_map<Effect, std::vector<std::pair<std::string, std::string>>> mEffectMap;
    std::vector<std::pair<std::string, std::string>> mVmaxGainListAlarmNotifcation;
    std::vector<std::pair<std::string, std::string>> mVmaxGainListDuration;
    std::vector<std::pair<std::string, std::string>> mVmaxGainListHaptic;

    bool mRampPlaying = false;
    bool mStopNextRamp = false;

    void playRampDown(long duration) {
        if (mRampPlaying) {
            mStopNextRamp = true;
            usleep(RAMP_INTERVAL_MS * 1000 + 2000);
            mStopNextRamp = false;
            if (mRampPlaying) {
                ALOGE("playRampDown, skip because waited for stop but still playing");
                return;
            }
        }

        char tmp[32];
        float currentGain = RAMP_GAIN_MAX;
        float gainDecrement = (RAMP_GAIN_MAX - RAMP_GAIN_MIN) / ((duration * 0.25) / RAMP_INTERVAL_MS);

        snprintf(tmp, sizeof(tmp), "%lu\n", duration);
        writeNode(DURATION_PATH, tmp);
        writeNode(VMAX_PATH, "0x30");
        snprintf(tmp, sizeof(tmp), "0x%x", (int) currentGain);
        writeNode(GAIN_PATH, tmp);
        writeNode(ACTIVATE_PATH, "1");

        mRampPlaying = true;
        usleep(RAMP_INTERVAL_MS * 1000);

        currentGain -= gainDecrement;
        while (currentGain >= RAMP_GAIN_MIN) {
            if (mStopNextRamp) {
                writeNode(ACTIVATE_PATH, "0");
                mRampPlaying = false;
                break;
            }

            snprintf(tmp, sizeof(tmp), "0x%x", (int) currentGain);
            writeNode(GAIN_PATH, tmp);

            usleep(RAMP_INTERVAL_MS * 1000);
            currentGain -= gainDecrement;
        }

        writeNode(ACTIVATE_PATH, "0");
        mRampPlaying = false;
    }

    std::vector<std::pair<std::string, std::string>>& getVmaxGainList(Effect effectId) {
        if (effectId == Effect::DURATION_ALARM_CALL ||
                effectId == Effect::DURATION_NOTIFICATION) {
            return mVmaxGainListAlarmNotifcation;
        }
        if (effectId >= Effect::RINGTONE_WALTZ &&
                effectId <= Effect::RINGTONE_SHORT) {
            return mVmaxGainListAlarmNotifcation;
        }
        return mVmaxGainListHaptic;
    }

    std::pair<int, int> getVmaxGainWithAmplitude(Effect effectId, float amplitude) {
        auto& vmaxGainList = getVmaxGainList(effectId);
        int minVmax = std::stoi(vmaxGainList[0].first, nullptr, 16);
        int maxVmax = std::stoi(vmaxGainList[vmaxGainList.size() - 1].first, nullptr, 16);
        int minGain = std::stoi(vmaxGainList[0].second, nullptr, 16);
        int maxGain = std::stoi(vmaxGainList[vmaxGainList.size() - 1].second, nullptr, 16);
        if (effectId == Effect::SLIDER_STEP) {
            minGain = 48;  // Use smaller strength for slider haptic
        }
        return std::make_pair((int) (minVmax + (maxVmax - minVmax) * amplitude),
                (int) (minGain + (maxGain - minGain) * amplitude));
    }

    std::pair<std::string, std::string>& getVmaxGainWithLevel(Effect effectId, int level) {
        if (effectId >= Effect::DURATION_STRENGTH_LEVEL1 &&
                effectId <= Effect::DURATION_STRENGTH_LEVEL10) {
            return mVmaxGainListDuration[static_cast<int>(effectId) -
                    static_cast<int>(Effect::DURATION_STRENGTH_LEVEL1) + level - 1];
        }
        return getVmaxGainList(effectId)[level - 1];
    }
public:
    long vibratorOn(Effect effectId, long duration, bool hapticFeedback) override {
        if (mRampPlaying) {
            mStopNextRamp = true;
            usleep(RAMP_INTERVAL_MS * 1000 + 2000);
            mStopNextRamp = false;
            if (mRampPlaying) {
                ALOGE("vibratorOn, skip because waited for ramp stop but still playing");
                return INVALID_VALUE;
            }
        }
        if (effectId == Effect::RAMP_DOWN) {
            std::thread([=] {
                playRampDown(duration);
            }).detach();
            return duration;
        }

        int ret = 0;
        char tmp[32];

        if (!hapticFeedback) {
            snprintf(tmp, sizeof(tmp), "%lu\n", duration);
            ret |= writeNode(DURATION_PATH, tmp);
        }

        for (auto& p : mEffectMap[effectId]) {
            if (p.first == PLACEHOLDER_VMAX_GAIN) {
                if (mAmplitude == 1.0) {
                    int level = hapticFeedback ? mHapticLevel :
                            effectId == Effect::DURATION_NOTIFICATION ? mNotificationLevel :
                            mAlarmCallLevel;
                    auto& vmaxGainStrPair = getVmaxGainWithLevel(effectId, level);
                    ret |= writeNode(VMAX_PATH, vmaxGainStrPair.first.c_str());
                    ret |= writeNode(GAIN_PATH, vmaxGainStrPair.second.c_str());
                } else {
                    float scale = hapticFeedback ? mHapticStrengthScale :
                            effectId == Effect::DURATION_NOTIFICATION ? mNotificationStrengthScale :
                            mAlarmCallStrengthScale;
                    auto vmaxGainIntPair = getVmaxGainWithAmplitude(effectId, mAmplitude * scale);
                    mAmplitude = 1.0;
                    snprintf(tmp, sizeof(tmp), "0x%x", vmaxGainIntPair.first);
                    ret |= writeNode(VMAX_PATH, tmp);
                    snprintf(tmp, sizeof(tmp), "0x%x", vmaxGainIntPair.second);
                    ret |= writeNode(GAIN_PATH, tmp);
                }
            } else if (p.first == PLACEHOLDER_RETURN) {
                mAmplitude = 1.0;
                if (ret == 0) {
                    return std::stoull(p.second.c_str(), NULL, 0);
                }
            } else if (p.first == PLACEHOLDER_SLEEP) {
                usleep(atoi(p.second.c_str()) * 1000);
            } else {
                ret |= writeNode(p.first.c_str(), p.second.c_str());
            }
        }
        mAmplitude = 1.0;
        if (ret == 0) {
            return duration;
        }
        ALOGE("vibratorOn, write node failed");
        return INVALID_VALUE;
    }

    void vibratorOff() override {
        if (mRampPlaying) {
            mStopNextRamp = true;
            usleep(RAMP_INTERVAL_MS * 1000 + 2000);
            mStopNextRamp = false;
        }

        mAmplitude = 1.0;
        int ret = 0;
        ret |= writeNode(ACTIVATE_PATH, "0");
        ret |= writeNode(RTP_PATH, "0");
        if (ret != 0) {
            ALOGE("vibratorOff, write node failed");
        }
    }

    bool isEffectSupported(Effect effectId) override {
        return mEffectMap.find(effectId) != mEffectMap.end() || effectId == Effect::RAMP_DOWN;
    }

    void setHapticStyle(Style style) override {}
    bool isHapticStyleSupported(Style style) override { return style == Style::CRISP; }
};
