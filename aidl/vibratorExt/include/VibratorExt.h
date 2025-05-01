/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/sun/hardware/vibratorExt/BnVibratorExt.h>
#include <aidl/vendor/sun/hardware/vibratorExt/Effect.h>
#include <aidl/vendor/sun/hardware/vibratorExt/LevelRange.h>
#include <aidl/vendor/sun/hardware/vibratorExt/Style.h>
#include <aidl/vendor/sun/hardware/vibratorExt/Type.h>
#include <log/log.h>

#include <thread>
#include <unordered_map>
#include <vector>

#undef LOG_TAG
#define LOG_TAG       "vendor.sun.hardware.vibratorExt-service.oplus"
#define DEBUG_ENABLED 0

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace vibratorExt {

struct LevelRangeConfig {
    const LevelRange alarmCallLevelRange;
    const LevelRange hapticLevelRange;
    const LevelRange notificationLevelRange;
};

class BaseVibrator {
protected:
    BaseVibrator(const LevelRangeConfig* levelRangeConfig) { mLevelRangeConfig = levelRangeConfig; }

    const LevelRangeConfig* mLevelRangeConfig;

    float mAmplitude = 1.0;

    int mAlarmCallLevel;
    float mAlarmCallStrengthScale;
    int mHapticLevel;
    float mHapticStrengthScale;
    int mNotificationLevel;
    float mNotificationStrengthScale;
public:
    virtual ~BaseVibrator() = default;
    virtual void initVibrator() = 0;
    virtual long vibratorOn(Effect effectId, long duration, bool hapticFeedback) = 0;
    virtual void vibratorOff() = 0;
    virtual void setHapticStyle(Style style) = 0;
    virtual bool isEffectSupported(Effect effectId) = 0;
    virtual bool isHapticStyleSupported(Style style) = 0;

    virtual void setAmplitude(float amplitude) { mAmplitude = amplitude; }
    virtual const LevelRange getStrengthLevelRange(Type type) {
        switch (type) {
            case Type::ALARM_CALL:
                return mLevelRangeConfig->alarmCallLevelRange;
            case Type::HAPTIC:
                return mLevelRangeConfig->hapticLevelRange;
            case Type::NOTIFICATION:
                return mLevelRangeConfig->notificationLevelRange;
            default:
                ALOGE("getStrengthLevelRange, unknown vibration type: %d", static_cast<int>(type));
                return { .maxLevel = 0, .defaultLevel = 0 };
        }
    }
    virtual void setStrengthLevel(Type type, int level) {
        switch (type) {
            case Type::ALARM_CALL:
                mAlarmCallLevel = level;
                mAlarmCallStrengthScale = (float) mAlarmCallLevel /
                        mLevelRangeConfig->alarmCallLevelRange.maxLevel;
                break;
            case Type::HAPTIC:
                mHapticLevel = level;
                mHapticStrengthScale = (float) mHapticLevel /
                        mLevelRangeConfig->hapticLevelRange.maxLevel;
                break;
            case Type::NOTIFICATION:
                mNotificationLevel = level;
                mNotificationStrengthScale = (float) mNotificationLevel /
                        mLevelRangeConfig->notificationLevelRange.maxLevel;
                break;
            default:
                ALOGE("setStrengthLevel, unknown vibration type: %d", static_cast<int>(type));
                break;
        }
    }

    virtual const char* getName() = 0;
};

struct VibratorExt : public BnVibratorExt {
    VibratorExt();
private:
    BaseVibrator* mVibrator;
public:
    ndk::ScopedAStatus initVibrator() override;
    ndk::ScopedAStatus vibratorOn(Effect effectId, long duration, long* _aidl_return) override;
    ndk::ScopedAStatus vibratorOff() override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setHapticStyle(Style style) override;
    ndk::ScopedAStatus getStrengthLevelRange(Type type, LevelRange* _aidl_return) override;
    ndk::ScopedAStatus setStrengthLevel(Type type, int level) override;
    ndk::ScopedAStatus isEffectSupported(Effect effectId, bool* _aidl_return) override;
    ndk::ScopedAStatus isHapticStyleSupported(Style style, bool* _aidl_return) override;
};

}  // namespace vibratorExt
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
