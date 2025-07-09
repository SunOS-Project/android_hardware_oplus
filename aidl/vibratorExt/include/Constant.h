/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "VibratorExt.h"

using ::aidl::vendor::sun::hardware::vibratorExt::Effect;
using ::aidl::vendor::sun::hardware::vibratorExt::LevelRange;
using ::aidl::vendor::sun::hardware::vibratorExt::LevelRangeConfig;

#define test_bit(bit, array) ((array) [(bit) / 8] & (1 << ((bit) % 8)))

#define RAMP_INTERVAL_MS 5

#define RAMP_GAIN_MIN 8.0    // 0x08
#define RAMP_GAIN_MAX 128.0  // 0x80

#define MIN_MAGNITUDE 0x1111
#define MAX_MAGNITUDE 0x7fff

#define INVALID_VALUE -1
#define NAME_BUF_SIZE 32

#define EFFECT_TEXTURE_TICK 21

#define FIRMWARE_ID_MAX 999
#define PLAY_RATE_HZ_DEFAULT 24000
#define EFFECT_INTERNAL_MS_DEFAULT 50
#define STYLE_FOLLOW_USER_MAGIC 100

static const char *INPUT_DIR = "/dev/input/";

static const std::string ACTIVATE_PATH = "/sys/class/leds/vibrator/activate";
static const std::string BRIGHTNESS_PATH = "/sys/class/leds/vibrator/brightness";
static const std::string DURATION_PATH = "/sys/class/leds/vibrator/duration";
static const std::string F0_PATH = "/sys/class/leds/vibrator/f0";
static const std::string F0_DATA_PATH = "/sys/class/leds/vibrator/f0_data";
static const std::string GAIN_PATH = "/sys/class/leds/vibrator/gain";
static const std::string RTP_PATH = "/sys/class/leds/vibrator/rtp";
static const std::string VMAX_PATH = "/sys/class/leds/vibrator/vmax";
static const std::string WAVEFORM_INDEX_PATH = "/sys/class/leds/vibrator/waveform_index";

static const std::string PLACEHOLDER_VMAX_GAIN = "P";
static const std::string PLACEHOLDER_SLEEP = "S";
static const std::string PLACEHOLDER_RETURN = "R";

static const LevelRangeConfig UNIFIED_LEVEL_RANGE_CONFIG = {
    .alarmCallLevelRange = {
        .maxLevel = 5,
        .defaultLevel = 5
    },
    .hapticLevelRange = {
        .maxLevel = 14,
        .defaultLevel = 14
    },
    .notificationLevelRange = {
        .maxLevel = 5,
        .defaultLevel = 5
    }
};

static const LevelRangeConfig LEVEL_RANGE_CONFIG_OP11_12_12R = {
    .alarmCallLevelRange = {
        .maxLevel = 5,
        .defaultLevel = 4
    },
    .hapticLevelRange = {
        .maxLevel = 14,
        .defaultLevel = 11
    },
    .notificationLevelRange = {
        .maxLevel = 5,
        .defaultLevel = 4
    }
};

static Effect durationToEffectId(long duration) {
    if (duration < 17) {
        return Effect::DURATION_STRENGTH_LEVEL3;
    } else if (duration < 34) {
        return Effect::DURATION_STRENGTH_LEVEL6;
    } else if (duration < 51) {
        return Effect::DURATION_STRENGTH_LEVEL10;
    } else if (duration < 300) {
        return Effect::DURATION_NOTIFICATION;
    } else {
        return Effect::DURATION_ALARM_CALL;
    }
}

static bool isHapticFeedback(Effect effectId) {
    if (effectId < Effect::DURATION_DEFAULT) {
        return true;
    }
    if (effectId >= Effect::DURATION_STRENGTH_LEVEL1 &&
            effectId <= Effect::DURATION_STRENGTH_LEVEL10) {
        return true;
    }
    if (effectId > Effect::CUSTOM_EFFECT_START &&
            effectId < Effect::CUSTOM_EFFECT_END) {
        return true;
    }
    return false;
}
