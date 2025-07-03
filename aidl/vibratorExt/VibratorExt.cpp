/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/StubVibrator.h"
#include "include/VibratorExt.h"
#include "include/device/aston_0809.h"
#include "include/device/corvette_0916t.h"
#include "include/device/dodge_0916t.h"
#include "include/device/kona_0815.h"
#include "include/device/lahaina_0809.h"
#include "include/device/salami_0916.h"
#include "include/device/waffle_0916t.h"

#include <android-base/properties.h>

using ::android::base::GetProperty;
using ::android::base::SetProperty;

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace vibratorExt {

VibratorExt::VibratorExt() : mVibrator(nullptr) {
    std::string deviceName = GetProperty("ro.sun.device", "");
    if (DEBUG_ENABLED) {
        ALOGD("device name: %s", deviceName.c_str());
    }

    // OnePlus Ace 3 Pro
    if (deviceName == "corvette") {
        mVibrator = new corvette_0916t_vibrator();
        SetProperty("sys.nameless.feature.vibrator.external_control", "true");
        SetProperty("sys.nameless.feature.vibrator.richtap", "true");
        SetProperty("sys.nameless.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.nameless.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }

    // OnePlus 13
    if (deviceName == "dodge") {
        mVibrator = new dodge_0916t_vibrator();
        SetProperty("sys.sun.feature.vibrator.external_control", "true");
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }
    // OnePlus 12
    if (deviceName == "waffle") {
        mVibrator = new waffle_0916t_vibrator();
        SetProperty("sys.sun.feature.vibrator.external_control", "true");
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }
    // OnePlus 12R
    if (deviceName == "aston") {
        mVibrator = new aston_0916_vibrator();
        SetProperty("sys.sun.feature.vibrator.external_control", "true");
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }
    // OnePlus 11
    if (deviceName == "salami") {
        mVibrator = new salami_0916_vibrator();
        SetProperty("sys.sun.feature.vibrator.external_control", "true");
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }
    // OnePlus 9/9 Pro
    if (deviceName == "lemonade" || deviceName == "lemonadep") {
        mVibrator = new lahaina_0809_vibrator();
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }
    // OnePlus 8/8 Pro/8T/9R
    if (deviceName == "instantnoodle" || deviceName == "instantnoodlep" || deviceName == "kebab" || deviceName == "lemonades") {
        mVibrator = new kona_0815_vibrator();
        SetProperty("sys.sun.feature.vibrator.richtap", "true");
        SetProperty("sys.sun.feature.vibrator.richtap_dev", "/dev/awinic_haptic");
        SetProperty("sys.sun.feature.vibrator.richtap_config", "/odm/etc/aac_richtap.config");
        goto END;
    }

    ALOGE("device is not unsupported");
    mVibrator = new StubVibrator();
    goto END;

END:
    if (DEBUG_ENABLED) {
        ALOGD("vibrator name: %s", mVibrator->getName());
    }

    SetProperty("sys.sun.feature.vibrator.ready", "1");
}

ndk::ScopedAStatus VibratorExt::initVibrator() {
    if (DEBUG_ENABLED) {
        ALOGD("initVibrator");
    }
    mVibrator->initVibrator();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::vibratorOn(Effect effectId, long duration, long* _aidl_return) {
    if (effectId == Effect::DURATION_DEFAULT) {
        effectId = durationToEffectId(duration);
        if (DEBUG_ENABLED) {
            ALOGD("vibratorOn, effectId was DURATION_DEFAULT, converted to %d", static_cast<int>(effectId));
        }
    }
    if (!mVibrator->isEffectSupported(effectId)) {
        *_aidl_return = -1;
        if (static_cast<int>(effectId) != EFFECT_TEXTURE_TICK) {
            ALOGE("vibratorOn, effectId %d is not supported", static_cast<int>(effectId));
        }
        vibratorOff();
        return ndk::ScopedAStatus::ok();
    }
    bool hapticFeedback = isHapticFeedback(effectId);
    if (duration <= 0 && !hapticFeedback) {
        *_aidl_return = -1;
        ALOGE("vibratorOn, effect %d is not haptic feedback while duration is invalid", static_cast<int>(effectId));
        vibratorOff();
        return ndk::ScopedAStatus::ok();
    }
    if ((effectId == Effect::DURATION_ALARM_CALL || effectId == Effect::DURATION_NOTIFICATION)
            && duration <= 50) {
        if (DEBUG_ENABLED) {
            ALOGD("vibratorOn, use level 10");
        }
        effectId = Effect::DURATION_STRENGTH_LEVEL10;
    }
    *_aidl_return = mVibrator->vibratorOn(effectId, duration, hapticFeedback);
    if (DEBUG_ENABLED) {
        ALOGD("vibratorOn, effectId: %d, duration: %lld, ret: %lld", static_cast<int>(effectId), duration, *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::vibratorOff() {
    if (DEBUG_ENABLED) {
        ALOGD("vibratorOff");
    }
    mVibrator->vibratorOff();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::setAmplitude(float amplitude) {
    if (amplitude <= 0.0 || amplitude > 1.0) {
        ALOGE("setAmplitude, invalid amplitude %f", amplitude);
        return ndk::ScopedAStatus::ok();
    }
    if (DEBUG_ENABLED) {
        ALOGD("setAmplitude, amplitude: %f", amplitude);
    }
    mVibrator->setAmplitude(amplitude);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::setHapticStyle(Style style) {
    if (DEBUG_ENABLED) {
        ALOGD("setHapticStyle, style: %d", static_cast<int>(style));
    }
    mVibrator->setHapticStyle(style);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::getStrengthLevelRange(Type type, LevelRange* _aidl_return) {
    *_aidl_return = mVibrator->getStrengthLevelRange(type);
    if (DEBUG_ENABLED) {
        ALOGD("getStrengthLevelRange, type: %d, ret: { max: %d, default: %d }",
                static_cast<int>(type), (*_aidl_return).maxLevel, (*_aidl_return).defaultLevel);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::setStrengthLevel(Type type, int level) {
    if (level < 0) {
        ALOGE("setStrengthLevel, invalid level %d", level);
        return ndk::ScopedAStatus::ok();
    }
    if (DEBUG_ENABLED) {
        ALOGD("setStrengthLevel, type: %d, level: %d", static_cast<int>(type), level);
    }
    mVibrator->setStrengthLevel(type, level);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::isEffectSupported(Effect effectId, bool* _aidl_return) {
    *_aidl_return = mVibrator->isEffectSupported(effectId);
    if (DEBUG_ENABLED) {
        ALOGD("isEffectSupported, effectId: %d, ret: %d", static_cast<int>(effectId), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VibratorExt::isHapticStyleSupported(Style style, bool* _aidl_return) {
    *_aidl_return = mVibrator->isHapticStyleSupported(style);
    if (DEBUG_ENABLED) {
        ALOGD("isHapticStyleSupported, style: %d, ret: %d", static_cast<int>(style), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace vibratorExt
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
