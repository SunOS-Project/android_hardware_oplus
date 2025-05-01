/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <dirent.h>
#include <linux/input.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "Constant.h"
#include "VibratorExt.h"

using ::aidl::vendor::sun::hardware::vibratorExt::BaseVibrator;
using ::aidl::vendor::sun::hardware::vibratorExt::Effect;
using ::aidl::vendor::sun::hardware::vibratorExt::LevelRangeConfig;
using ::aidl::vendor::sun::hardware::vibratorExt::Style;

struct KernelEffectStream {
    uint32_t              effect_id;
    uint32_t              length;
    uint32_t              play_rate_hz = PLAY_RATE_HZ_DEFAULT;
    const int8_t          *data;
};

struct CustomEffect {
    std::vector<uint32_t> firmware_id;
    std::vector<uint32_t> sleep_time;
    std::vector<float>    scale;
    bool                  fixed_scale = false;
    uint32_t              min_interval = EFFECT_INTERNAL_MS_DEFAULT;
    uint32_t              style = STYLE_FOLLOW_USER_MAGIC;
};

struct CustomEffectStyleSet {
    CustomEffect          crisp;
    CustomEffect          gentle;
};

struct FirmwarePathConfig {
    const std::string     crispPathPrefix;
    const std::string     gentlePathPrefix;
};

class InputFFVibrator : public BaseVibrator {
protected:
    InputFFVibrator(
        const LevelRangeConfig* levelRangeConfig,
        const FirmwarePathConfig* firmwarePathConfig,
        std::unordered_map<Effect, CustomEffectStyleSet> effectMap
    ) : BaseVibrator(levelRangeConfig),
        mEffectMap(effectMap), mFirmwarePathConfig(firmwarePathConfig) {}
private:
    std::unordered_map<Effect, CustomEffectStyleSet> mEffectMap;
    std::unordered_map<Style, std::unordered_map<uint32_t, KernelEffectStream>> mFirmwareEffect;
    std::unordered_map<Style, std::unordered_map<uint32_t, std::vector<int8_t>>> mFirmwareFifoData;

    const FirmwarePathConfig* mFirmwarePathConfig;

    int mVibratorFd = INVALID_VALUE;
    int16_t mCurrentAppId = INVALID_VALUE;

    Style mCurrentStyle = Style::CRISP;
    Effect mLastEffectId = Effect::DURATION_DEFAULT;
    bool mWaitForInterval = false;

    std::filesystem::path getFirmwarePath(uint32_t effectId, Style style) {
        std::string prefix;
        if (isMultipleStyleSupported()) {
            prefix = style == Style::CRISP ? mFirmwarePathConfig->crispPathPrefix : mFirmwarePathConfig->gentlePathPrefix;
        } else {
            prefix = mFirmwarePathConfig->crispPathPrefix;
        }
        return prefix + std::to_string(effectId) + ".bin";
    }

    KernelEffectStream* loadFirmwareEffect(uint32_t effectId, Style style, bool silent) {
        if (DEBUG_ENABLED) {
            ALOGD("loadFirmwareEffect, effectId: %d, style: %d", effectId, static_cast<int>(style));
        }
        auto& effectDataMap = mFirmwareEffect[style];
        auto it = effectDataMap.find(effectId);
        if (it != effectDataMap.end()) {
            return &it->second;
        }

        std::filesystem::path filePath = getFirmwarePath(effectId, style);
        std::ifstream data(filePath, std::ios::in | std::ios::binary);
        if (!data.is_open()) {
            if (!silent) {
                ALOGE("loadFirmwareEffect, failed to open %s for effect %d", filePath.string().c_str(), effectId);
            }
            return nullptr;
        }

        uint32_t fileSize = std::filesystem::file_size(filePath);
        std::vector<int8_t> fifoData(fileSize);
        data.read(reinterpret_cast<char*>(fifoData.data()), fileSize);

        auto& fifoDataMap = mFirmwareFifoData[style];
        auto fifoResult = fifoDataMap.emplace(effectId, std::move(fifoData));
        std::unique_ptr<KernelEffectStream> effectStream = std::make_unique<KernelEffectStream>(
                effectId, fileSize, PLAY_RATE_HZ_DEFAULT, fifoResult.first->second.data());
        if (effectStream) {
            auto effectResult = effectDataMap.emplace(effectId, *effectStream);
            return &effectResult.first->second;
        }

        return nullptr;
    }

    CustomEffect* getEffect(Effect effectId, Style style) {
        if (mEffectMap.find(effectId) == mEffectMap.end()) {
            return nullptr;
        }
        auto& effectStyleSet = mEffectMap[effectId];
        if (!isMultipleStyleSupported() || style == Style::CRISP) {
            return &effectStyleSet.crisp;
        }
        return &effectStyleSet.gentle;
    }

    int16_t getMagnitude(Effect effectId, float scale, bool fixedScale, bool hapticFeedback) {
        int16_t magnitude;
        if (mAmplitude == 1.0 || fixedScale) {
            if (hapticFeedback) {
                magnitude = (int16_t) ((mHapticLevel * (MAX_MAGNITUDE - MIN_MAGNITUDE)
                        / mLevelRangeConfig->hapticLevelRange.maxLevel + MIN_MAGNITUDE) * scale);
            } else if (effectId == Effect::DURATION_NOTIFICATION) {
                magnitude = (int16_t) ((mNotificationLevel * (MAX_MAGNITUDE - MIN_MAGNITUDE)
                        / mLevelRangeConfig->notificationLevelRange.maxLevel + MIN_MAGNITUDE) * scale);
            } else {
                magnitude = (int16_t) ((mAlarmCallLevel * (MAX_MAGNITUDE - MIN_MAGNITUDE)
                        / mLevelRangeConfig->alarmCallLevelRange.maxLevel + MIN_MAGNITUDE) * scale);
            }
        } else {
            magnitude = (int16_t) (((uint8_t) (mAmplitude * 0xff) * (MAX_MAGNITUDE - MIN_MAGNITUDE)
                    / 255 + MIN_MAGNITUDE) * scale);
        }
        if (DEBUG_ENABLED) {
            ALOGD("getMagnitude, effectId: %d, scale: %f, fixedScale: %d, hapticFeedback: %d, result: %hd",
                    static_cast<int>(effectId), scale, fixedScale, hapticFeedback, magnitude);
        }
        return magnitude;
    }
public:
    void initVibrator() override {
        DIR *dp = opendir(INPUT_DIR);
        if (!dp) {
            ALOGE("initVibrator, open %s failed, errno: %d", INPUT_DIR, errno);
            return;
        }

        uint8_t ffBitmask[FF_CNT / 8];
        memset(ffBitmask, 0, sizeof(ffBitmask));

        char devicename[PATH_MAX];
        dirent *dir;
        int fd;
        int ret;
        while ((dir = readdir(dp))) {
            if (dir->d_name[0] == '.' &&
                    (dir->d_name[1] == '\0' ||
                    (dir->d_name[1] == '.' && dir->d_name[2] == '\0'))) {
                continue;
            }

            snprintf(devicename, PATH_MAX, "%s%s", INPUT_DIR, dir->d_name);
            fd = TEMP_FAILURE_RETRY(open(devicename, O_RDWR));
            if (fd < 0) {
                ALOGE("initVibrator, open %s failed, errno: %d", devicename, errno);
                continue;
            }

            char name[NAME_BUF_SIZE];
            ret = TEMP_FAILURE_RETRY(ioctl(fd, EVIOCGNAME(sizeof(name)), name));
            if (ret == INVALID_VALUE) {
                ALOGE("initVibrator, get input device name %s failed, errno: %d", devicename, errno);
                close(fd);
                continue;
            }

            if (strcmp(name, "qcom-hv-haptics")) {
                close(fd);
                continue;
            }

            if (DEBUG_ENABLED) {
                ALOGD("initVibrator, %s is detected at %s", name, devicename);
            }
            ret = TEMP_FAILURE_RETRY(ioctl(fd, EVIOCGBIT(EV_FF, sizeof(ffBitmask)), ffBitmask));
            if (ret == INVALID_VALUE) {
                ALOGE("initVibrator, ioctl failed, errno: %d", errno);
                close(fd);
                continue;
            }

            if (test_bit(FF_CONSTANT, ffBitmask) ||
                    test_bit(FF_PERIODIC, ffBitmask)) {
                mVibratorFd = fd;
                break;
            }

            close(fd);
        }

        closedir(dp);

        // Pre-loading all effect firmware data
        for (uint32_t i = 0; i < 500; i++) {
            loadFirmwareEffect(i, Style::CRISP, true);
            if (isMultipleStyleSupported()) {
                loadFirmwareEffect(i, Style::GENTLE, true);
            }
        }
    }

    long vibratorOn(Effect effectId, long duration, bool hapticFeedback) override {
        if (mVibratorFd < 0) {
            ALOGE("vibratorOn, input device is unavailable");
            vibratorOff();
            return INVALID_VALUE;
        }
        if (mWaitForInterval) {
            if (effectId == mLastEffectId) {
                if (DEBUG_ENABLED) {
                    ALOGD("vibratorOn, waiting for interval, ignore same effect");
                }
                return INVALID_VALUE;
            }
            mWaitForInterval = false;
        }

        long playLengthMs = 0;
        int ret;
        CustomEffect *customEffect = getEffect(effectId, mCurrentStyle);
        int cycles = customEffect ? customEffect->firmware_id.size() : 1;
        if (DEBUG_ENABLED) {
            ALOGD("vibratorOn, isEffect: %d, cycles size: %d", customEffect != nullptr, cycles);
        }

        for (int i = 0; i < cycles; i++) {
            if (mCurrentAppId != INVALID_VALUE) {
                TEMP_FAILURE_RETRY(ioctl(mVibratorFd, EVIOCRMFF, mCurrentAppId));
                mCurrentAppId = INVALID_VALUE;
            }

            int16_t magnitude = customEffect ? getMagnitude(
                effectId, /* effectId */
                customEffect->scale.size() != 0 ? customEffect->scale[i] : 1.0, /* scale */
                customEffect->fixed_scale, /* fixedScale */
                true /* hapticFeedback */
            ) : getMagnitude(
                effectId, /* effectId */
                1.0, /* scale */
                false, /* fixedScale */
                false /* hapticFeedback */
            );

            const uint32_t firmwareId = customEffect ? customEffect->firmware_id[i] : 0;
            const Style useStyle = !customEffect || customEffect->style == STYLE_FOLLOW_USER_MAGIC ?
                    mCurrentStyle : static_cast<Style>(customEffect->style);
            const KernelEffectStream *stream = customEffect && firmwareId <= FIRMWARE_ID_MAX
                    ? loadFirmwareEffect(firmwareId, useStyle, false) : nullptr;
            if (customEffect && firmwareId <= FIRMWARE_ID_MAX && !stream) {
                ALOGE("vibratorOn, isEffect but firmware %d not available", firmwareId);
                vibratorOff();
                return INVALID_VALUE;
            }
            const long length = customEffect && !stream ? firmwareId - FIRMWARE_ID_MAX : duration;
            if (DEBUG_ENABLED) {
                ALOGD("vibratorOn, firmwareId: %d, length=%lld, style=%d",
                        firmwareId, length, static_cast<int>(useStyle));
            }

            ff_effect effect;
            input_event play;
            memset(&effect, 0, sizeof(effect));
            if (stream) {
                effect.type = FF_PERIODIC;
                effect.u.periodic.waveform = FF_CUSTOM;
                effect.u.periodic.magnitude = magnitude;
                effect.u.periodic.custom_data = (int16_t *) stream;
                effect.u.periodic.custom_len = sizeof(*stream);
            } else {
                effect.type = FF_CONSTANT;
                effect.u.constant.level = magnitude;
                effect.replay.length = length;
            }

            effect.id = mCurrentAppId;
            effect.replay.delay = 0;

            ret = TEMP_FAILURE_RETRY(ioctl(mVibratorFd, EVIOCSFF, &effect));
            if (ret == INVALID_VALUE) {
                ALOGE("vibratorOn, ioctl EVIOCSFF failed, errno: %d", -errno);
                vibratorOff();
                return INVALID_VALUE;
            }

            mCurrentAppId = effect.id;
            if (stream) {
                playLengthMs += ((stream->length * 1000) / PLAY_RATE_HZ_DEFAULT) + 1;
            } else {
                playLengthMs += length;
            }

            play.value = 1;
            play.type = EV_FF;
            play.code = mCurrentAppId;
            play.time.tv_sec = 0;
            play.time.tv_usec = 0;
            ret = TEMP_FAILURE_RETRY(write(mVibratorFd, (const void*) &play, sizeof(play)));
            if (ret == INVALID_VALUE) {
                ALOGE("vibratorOn, write failed, errno: %d", -errno);
                vibratorOff();
                return INVALID_VALUE;
            }

            if (i != cycles - 1 && customEffect && customEffect->sleep_time.size() != 0 && customEffect->sleep_time[i] > 0) {
                playLengthMs += customEffect->sleep_time[i];
                usleep(customEffect->sleep_time[i] * 1000);
            }
        }
        mAmplitude = 1.0;

        if (DEBUG_ENABLED) {
            ALOGD("vibratorOn, done, playLengthMs=%lld", playLengthMs);
        }

        if (customEffect && playLengthMs < customEffect->min_interval) {
            std::thread([=] {
                mWaitForInterval = true;
                mLastEffectId = effectId;
                usleep((customEffect->min_interval - playLengthMs) * 1000);
                mWaitForInterval = false;
            }).detach();
            return customEffect->min_interval;
        }
        return playLengthMs;
    }

    void vibratorOff() override {
        if (DEBUG_ENABLED) {
            ALOGD("vibratorOff, waitForInterval: %d", mWaitForInterval);
        }
        if (mWaitForInterval) {
            return;
        }
        if (mCurrentAppId != INVALID_VALUE) {
            TEMP_FAILURE_RETRY(ioctl(mVibratorFd, EVIOCRMFF, mCurrentAppId));
        }
        mCurrentAppId = INVALID_VALUE;
        mAmplitude = 1.0;
    }

    bool isEffectSupported(Effect effectId) override {
        return mEffectMap.find(effectId) != mEffectMap.end() ||
                effectId == Effect::DURATION_ALARM_CALL ||
                effectId == Effect::DURATION_NOTIFICATION;
    }

    void setHapticStyle(Style style) override {
        if (isMultipleStyleSupported()) {
            mCurrentStyle = style;
        }
    }

    bool isHapticStyleSupported(Style style) override {
        if (isMultipleStyleSupported()) {
            return style == Style::CRISP || style == Style::GENTLE;
        }
        return style == Style::CRISP;
    }

    virtual bool isMultipleStyleSupported() = 0;
};
