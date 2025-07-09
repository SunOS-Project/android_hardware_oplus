/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "../InputFFVibrator.h"

static const FirmwarePathConfig FIRMWARE_PATH_CONFIG_WAFFLE = {
    .crispPathPrefix = "/odm/etc/vibrator/9999/def/effect_",
    .gentlePathPrefix = "/odm/etc/vibrator/9999/soft/effect_"
};

static std::unordered_map<Effect, CustomEffectStyleSet> EFFECTS_MAP_WAFFLE {
    // AOSP effects
    { Effect::CLICK, {
        .crisp = {
            .firmware_id = { 4 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 309 },
            .min_interval = 80
        }
    }},
    { Effect::DOUBLE_CLICK, {
        .crisp = {
            .firmware_id = { 8, 8 },
            .sleep_time = { 150 }
        },
        .gentle = {
            .firmware_id = { 6, 6 },
            .sleep_time = { 150 }
        }
    }},
    { Effect::TICK, {
        .crisp = {
            .firmware_id = { 309 },
            .min_interval = 120
        },
        .gentle = {
            .firmware_id = { 309 },
            .min_interval = 120
        }
    }},
    { Effect::THUD, {
        .crisp = {
            .firmware_id = { 4 }
        },
        .gentle = {
            .firmware_id = { 309 }
        }
    }},
    { Effect::POP, {
        .crisp = {
            .firmware_id = { 4 }
        },
        .gentle = {
            .firmware_id = { 309 }
        }
    }},
    { Effect::HEAVY_CLICK, {
        .crisp = {
            .firmware_id = { 8 }
        },
        .gentle = {
            .firmware_id = { 6 }
        }
    }},
    // Custom effects
    { Effect::DURATION_STRENGTH_LEVEL1, {
        .crisp = {
            .firmware_id = { 111 },
            .scale = { 0.6 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 4 },
            .scale = { 0.18 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL2, {
        .crisp = {
            .firmware_id = { 111 },
            .scale = { 0.7 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 4 },
            .scale = { 0.26 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL3, {
        .crisp = {
            .firmware_id = { 111 },
            .scale = { 0.8 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 4 },
            .scale = { 0.34 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL4, {
        .crisp = {
            .firmware_id = { 112 },
            .scale = { 0.8 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 2 },
            .scale = { 0.42 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL5, {
        .crisp = {
            .firmware_id = { 112 },
            .scale = { 0.9 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 2 },
            .scale = { 0.50 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL6, {
        .crisp = {
            .firmware_id = { 112 },
            .scale = { 1.0 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 2 },
            .scale = { 0.68 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL7, {
        .crisp = {
            .firmware_id = { 8 },
            .scale = { 0.7 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 6 },
            .scale = { 0.76 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL8, {
        .crisp = {
            .firmware_id = { 8 },
            .scale = { 0.8 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 6 },
            .scale = { 0.84 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL9, {
        .crisp = {
            .firmware_id = { 8 },
            .scale = { 0.9 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 6 },
            .scale = { 0.92 },
            .min_interval = 80
        }
    }},
    { Effect::DURATION_STRENGTH_LEVEL10, {
        .crisp = {
            .firmware_id = { 8 },
            .scale = { 1.0 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 6 },
            .scale = { 1.0 },
            .min_interval = 80
        }
    }},
    { Effect::RINGTONE_WALTZ, {
        .crisp = {
            .firmware_id = { 41 }
        },
        .gentle = {
            .firmware_id = { 41 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::RINGTONE_CUT, {
        .crisp = {
            .firmware_id = { 42 }
        },
        .gentle = {
            .firmware_id = { 42 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::RINGTONE_CLOCK, {
        .crisp = {
            .firmware_id = { 43 }
        },
        .gentle = {
            .firmware_id = { 43 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::RINGTONE_SHORT, {
        .crisp = {
            .firmware_id = { 45 }
        },
        .gentle = {
            .firmware_id = { 45 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::ALERT_SLIDER_BOTTOM, {
        .crisp = {
            .firmware_id = { 308 }
        },
        .gentle = {
            .firmware_id = { 308 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::ALERT_SLIDER_MIDDLE, {
        .crisp = {
            .firmware_id = { 8, 8 },
            .sleep_time = { 150 }
        },
        .gentle = {
            .firmware_id = { 6, 6 },
            .sleep_time = { 150 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::BACK_GESTURE, {
        .crisp = {
            .firmware_id = { 309 }
        },
        .gentle = {
            .firmware_id = { 309 }
        }
    }},
    { Effect::BUTTON_CLICK, {
        .crisp = {
            .firmware_id = { 112 }
        },
        .gentle = {
            .firmware_id = { 2 }
        }
    }},
    { Effect::CLEAR_ALL_NOTIFICATION, {
        .crisp = {
            .firmware_id = { 364 }
        },
        .gentle = {
            .firmware_id = { 364 }
        }
    }},
    { Effect::CLEAR_ALL_RECENT, {
        .crisp = {
            .firmware_id = { 363 }
        },
        .gentle = {
            .firmware_id = { 363 }
        }
    }},
    { Effect::KEYBOARD_PRESS, {
        .crisp = {
            .firmware_id = { 8, 8 },
            .sleep_time = { 100 },
            .min_interval = 80
        },
        .gentle = {
            .firmware_id = { 6, 6 },
            .sleep_time = { 100 },
            .min_interval = 80
        }
    }},
    { Effect::PLUG_IN, {
        .crisp = {
            .firmware_id = { FIRMWARE_ID_MAX + 200, 108 },
            .sleep_time = { 600 }
        },
        .gentle = {
            .firmware_id = { FIRMWARE_ID_MAX + 200, 108 },
            .sleep_time = { 600 },
            .style = static_cast<uint32_t>(Style::CRISP)
        }
    }},
    { Effect::SCREEN_OFF, {
        .crisp = {
            .firmware_id = { 315 }
        },
        .gentle = {
            .firmware_id = { 315 }
        }
    }},
    { Effect::SCREEN_ON, {
        .crisp = {
            .firmware_id = { 4 }
        },
        .gentle = {
            .firmware_id = { 4 }
        }
    }},
    { Effect::SCREENSHOT, {
        .crisp = {
            .firmware_id = { 318 }
        },
        .gentle = {
            .firmware_id = { 318 }
        }
    }},
    { Effect::SLIDER_EDGE, {
        .crisp = {
            .firmware_id = { 106 }
        },
        .gentle = {
            .firmware_id = { 106 }
        }
    }},
    { Effect::SLIDER_STEP, {
        .crisp = {
            .firmware_id = { 105 },
            .min_interval = 120
        },
        .gentle = {
            .firmware_id = { 105 },
            .min_interval = 120
        }
    }},
    { Effect::SWITCH_TOGGLE, {
        .crisp = {
            .firmware_id = { 112 }
        },
        .gentle = {
            .firmware_id = { 2 }
        }
    }},
    { Effect::UNIFIED_ERROR, {
        .crisp = {
            .firmware_id = { 46 }
        },
        .gentle = {
            .firmware_id = { 46 }
        }
    }},
    { Effect::UNIFIED_SUCCESS, {
        .crisp = {
            .firmware_id = { 8 }
        },
        .gentle = {
            .firmware_id = { 6 }
        }
    }}
};

class waffle_0916t_vibrator : public InputFFVibrator {
public:
    waffle_0916t_vibrator() : InputFFVibrator(
        &LEVEL_RANGE_CONFIG_OP11_12_12R,
        &FIRMWARE_PATH_CONFIG_WAFFLE,
        EFFECTS_MAP_WAFFLE
    ) {}

    bool isMultipleStyleSupported() override {
        return true;
    }

    const char* getName() override {
        return "waffle_0916t_vibrator";
    }
};
