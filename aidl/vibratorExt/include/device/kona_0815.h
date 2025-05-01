/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "../LedVibrator.h"

using ::aidl::vendor::sun::hardware::vibratorExt::Effect;

static std::unordered_map<Effect, std::vector<std::pair<std::string, std::string>>> EFFECTS_MAP_KONA {
    // AOSP effects
    { Effect::CLICK, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "2" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "30" }
    }},
    { Effect::DOUBLE_CLICK, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "150" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "200" }
    }},
    { Effect::TICK, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "30" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "50" }
    }},
    { Effect::THUD, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "2" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "30" }
    }},
    { Effect::POP, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "4" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "30" }
    }},
    { Effect::HEAVY_CLICK, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "30" }
    }},
    // Custom effects
    { Effect::DURATION_STRENGTH_LEVEL1, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "16" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL2, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "16" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL3, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "16" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL4, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "17" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL5, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "17" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL6, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "17" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL7, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL8, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL9, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_STRENGTH_LEVEL10, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
    }},
    { Effect::DURATION_NOTIFICATION, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { ACTIVATE_PATH, "1" }
    }},
    { Effect::DURATION_ALARM_CALL, {
        { RTP_PATH, "0" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { ACTIVATE_PATH, "1" }
    }},
    // Ringtone effects
    { Effect::RINGTONE_WALTZ, {
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { RTP_PATH, "41" }
    }},
    { Effect::RINGTONE_CUT, {
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { RTP_PATH, "42" }
    }},
    { Effect::RINGTONE_CLOCK, {
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { RTP_PATH, "43" }
    }},
    { Effect::RINGTONE_SHORT, {
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { RTP_PATH, "45" }
    }},
    { Effect::ALERT_SLIDER_BOTTOM, { // Use hardcoded gain & vmax for this effect
        { RTP_PATH, "0" },
        { DURATION_PATH, "200" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x30" },
        { ACTIVATE_PATH, "1" },
        { PLACEHOLDER_RETURN, "200" }
    }},
    { Effect::ALERT_SLIDER_MIDDLE, { // Use hardcoded gain & vmax for this effect
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x16" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "30" },
        { DURATION_PATH, "30" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x30" },
        { ACTIVATE_PATH, "1" },
        { PLACEHOLDER_SLEEP, "100" },
        { DURATION_PATH, "10" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x16" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "100" }
    }},
    { Effect::BUTTON_CLICK, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "13" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
    { Effect::CLEAR_ALL_NOTIFICATION, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "120" }
    }},
    { Effect::CLEAR_ALL_RECENT, {  // Use hardcoded gain & vmax for this effect
        { RTP_PATH, "0" },
        { DURATION_PATH, "100" },
        { GAIN_PATH, "0x40" },
        { VMAX_PATH, "0x0a" },
        { ACTIVATE_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x2c" },
        { VMAX_PATH, "0x08" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x18" },
        { VMAX_PATH, "0x06" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x04" },
        { VMAX_PATH, "0x04" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x16" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "120" }
    }},
    { Effect::KEYBOARD_PRESS, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "100" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "120" }
    }},
    { Effect::PLUG_IN, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "200" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { ACTIVATE_PATH, "1" },
        { PLACEHOLDER_SLEEP, "500" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "1" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "1" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "1" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "50" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "950" }
    }},
    { Effect::SCREEN_OFF, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "3" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "80" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "1" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "100" }
    }},
    { Effect::SCREEN_ON, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "2" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
    { Effect::SCREENSHOT, {  // Use hardcoded gain & vmax for this effect
        { RTP_PATH, "0" },
        { DURATION_PATH, "100" },
        { GAIN_PATH, "0x40" },
        { VMAX_PATH, "0x0a" },
        { ACTIVATE_PATH, "1" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x2c" },
        { VMAX_PATH, "0x08" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x18" },
        { VMAX_PATH, "0x06" },
        { PLACEHOLDER_SLEEP, "20" },
        { GAIN_PATH, "0x04" },
        { VMAX_PATH, "0x04" },
        { PLACEHOLDER_SLEEP, "20" },
        { DURATION_PATH, "10" },
        { GAIN_PATH, "0x80" },
        { VMAX_PATH, "0x16" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "120" }
    }},
    { Effect::SLIDER_EDGE, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
    { Effect::SLIDER_STEP, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "1" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
    { Effect::SWITCH_TOGGLE, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "4" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
    { Effect::UNIFIED_ERROR, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "40" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_SLEEP, "40" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "7" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "100" }
    }},
    { Effect::UNIFIED_SUCCESS, {
        { RTP_PATH, "0" },
        { DURATION_PATH, "10" },
        { PLACEHOLDER_VMAX_GAIN, "0" },
        { WAVEFORM_INDEX_PATH, "5" },
        { BRIGHTNESS_PATH, "1" },
        { PLACEHOLDER_RETURN, "15" }
    }},
};

static std::vector<std::pair<std::string, std::string>> VMAX_GAIN_LIST_ALARM_NOTIFICATION_KONA {
    { "0x16", "0x40" },
    { "0x16", "0x50" },
    { "0x23", "0x60" },
    { "0x23", "0x70" },
    { "0x30", "0x80" },
};
static std::vector<std::pair<std::string, std::string>> VMAX_GAIN_LIST_DURATION_KONA {
    { "0x02", "0x12" },
    { "0x02", "0x17" },
    { "0x04", "0x1c" },
    { "0x04", "0x21" },
    { "0x06", "0x26" },
    { "0x06", "0x2b" },
    { "0x08", "0x30" },
    { "0x08", "0x35" },
    { "0x0a", "0x3a" },
    { "0x0a", "0x3f" },
    { "0x0c", "0x44" },
    { "0x0c", "0x49" },
    { "0x0e", "0x4e" },
    { "0x0e", "0x53" },
    { "0x10", "0x58" },
    { "0x10", "0x5d" },
    { "0x12", "0x62" },
    { "0x12", "0x67" },
    { "0x14", "0x6c" },
    { "0x14", "0x71" },
    { "0x16", "0x76" },
    { "0x16", "0x7b" },
    { "0x16", "0x80" },
};
static std::vector<std::pair<std::string, std::string>> VMAX_GAIN_LIST_HAPTIC_KONA {
    { "0x02", "0x48" },
    { "0x02", "0x50" },
    { "0x02", "0x58" },
    { "0x04", "0x60" },
    { "0x04", "0x68" },
    { "0x06", "0x70" },
    { "0x08", "0x78" },
    { "0x0a", "0x80" },
    { "0x0c", "0x80" },
    { "0x0e", "0x80" },
    { "0x10", "0x80" },
    { "0x12", "0x80" },
    { "0x14", "0x80" },
    { "0x16", "0x80" },
};

class kona_0815_vibrator : public LedVibrator {
public:
    kona_0815_vibrator() : LedVibrator(
        &UNIFIED_LEVEL_RANGE_CONFIG,
        EFFECTS_MAP_KONA,
        VMAX_GAIN_LIST_ALARM_NOTIFICATION_KONA,
        VMAX_GAIN_LIST_DURATION_KONA,
        VMAX_GAIN_LIST_HAPTIC_KONA
    ) {}

    void initVibrator() override {
        writeNode(F0_PATH, "1735");    // Load aw8697_haptic_174.bin
        writeNode(F0_DATA_PATH, "8");  // TODO: Shoule we read from /mnt/vendor/persist/engineermode/f0_cali_data ?
    }

    const char* getName() override {
        return "kona_0815_vibrator";
    }
};
