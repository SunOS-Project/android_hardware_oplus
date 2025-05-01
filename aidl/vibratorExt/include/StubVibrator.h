/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "Constant.h"
#include "VibratorExt.h"

using ::aidl::vendor::sun::hardware::vibratorExt::BaseVibrator;
using ::aidl::vendor::sun::hardware::vibratorExt::Effect;
using ::aidl::vendor::sun::hardware::vibratorExt::Style;
using ::aidl::vendor::sun::hardware::vibratorExt::Type;

class StubVibrator : public BaseVibrator {
public:
    StubVibrator() : BaseVibrator(nullptr) {}

    void initVibrator() override {
        ALOGE("initVibrator, stub");
    }

    long vibratorOn(Effect effectId, long duration, bool hapticFeedback) override {
        ALOGE("vibratorOn, stub");
        return INVALID_VALUE;
    }

    void vibratorOff() override {
        ALOGE("vibratorOff, stub");
    }

    void setHapticStyle(Style style) override {
        ALOGE("setHapticStyle, stub");
    }

    bool isEffectSupported(Effect effectId) override {
        ALOGE("isEffectSupported, stub");
        return false;
    }

    bool isHapticStyleSupported(Style style) override {
        ALOGE("isHapticStyleSupported, stub");
        return false;
    }

    void setAmplitude(float amplitude) override {
        ALOGE("setAmplitude, stub");
    }

    const LevelRange getStrengthLevelRange(Type type) override {
        ALOGE("getStrengthLevelRange, stub");
        return { .maxLevel = 0, .defaultLevel = 0 };
    }

    void setStrengthLevel(Type type, int level) override {
        ALOGE("setStrengthLevel, stub");
    }

    const char* getName() override {
        return "StubVibrator";
    }
};
