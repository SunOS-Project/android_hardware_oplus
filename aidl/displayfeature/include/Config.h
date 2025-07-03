/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "DisplayFeature.h"

using ::aidl::vendor::sun::hardware::displayfeature::DeviceInfo;
using ::aidl::vendor::sun::hardware::displayfeature::Feature;
using ::aidl::vendor::sun::hardware::displayfeature::NodeInfo;

static const std::string SEED_PATH = "/sys/kernel/oplus_display/seed";
static const std::string TP_DIRECTION_PATH = "/proc/touchpanel/oplus_tp_direction";

static const NodeInfo DC_DIMMING_NODE = {
    .path = "/sys/kernel/oplus_display/dimlayer_hbm",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo ONE_PULSE_NODE = {
    .path = "/sys/kernel/oplus_display/pwm_onepulse",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo HBM_NODE = {
    .path = "/sys/kernel/oplus_display/hbm",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo HIGH_TOUCH_SAMPLE_NODE = {
    .path = "/proc/touchpanel/game_switch_enable",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo LTPO_MIN_FPS_NODE = {
    .path = "/sys/kernel/oplus_display/min_fps",
    .writeEnableValue = "1",
    .writeDisableValue = "120"
};
static const NodeInfo LTPO_TEST_TE_NODE = {
    .path = "/sys/kernel/oplus_display/test_te",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo OPLUS_TOUCH_HIGH_TOUCH_SAMPLE = {
    .oplusTouchFeatureId = 26,
    .readDisabledPrefix = "0",
    .writeEnableValue = "3",
    .writeDisableValue = "0"
};
static const NodeInfo OPLUS_TOUCH_DT2W = {
    .oplusTouchFeatureId = 1,
    .writeEnableValue = "1"
};
static const NodeInfo OPLUS_TOUCH_GESTURE_WITHOUT_S = {
    .oplusTouchFeatureId = 21,
    .writeEnableValue = "78068"
};
static const NodeInfo OPLUS_TOUCH_GESTURE_REPORT = {
    .oplusTouchFeatureId = 2
};

static const std::unordered_map<int, std::string> COLOR_MODE_MAP_0_256_258_259 = {
    { 0, "102" },
    { 256, "101" },
    { 258, "101" },
    { 259, "100" }
};
static const std::unordered_map<int, std::string> COLOR_MODE_MAP_303_301_307 = {
    { 303, "102" },
    { 301, "101" },
    { 307, "100" }
};

static const std::unordered_map<int, std::string> TP_ORIENTATION_MAP = {
    { 0, "0" },
    { 90, "1" },
    { 270, "2" }
};

static const DeviceInfo CONFIG_OP13 = {
    .supportedFeatures = static_cast<int>(Feature::LTPO),
    .featureNode = {
        .ltpoNode = { LTPO_MIN_FPS_NODE, LTPO_TEST_TE_NODE }
    },
    .colorModeMap = COLOR_MODE_MAP_303_301_307,
    .useOplusTouch = true,
    .touchGestureInitNode = { OPLUS_TOUCH_DT2W, OPLUS_TOUCH_GESTURE_WITHOUT_S },
    .touchGestureReportNode = OPLUS_TOUCH_GESTURE_REPORT
};

static const DeviceInfo CONFIG_OPACE3P = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH) |
                         static_cast<int>(Feature::LTPO),
    .featureNode = {
        .dcDimmingNode = ONE_PULSE_NODE,
        .highTouchSampleNode = OPLUS_TOUCH_HIGH_TOUCH_SAMPLE,
        .ltpoNode = { LTPO_MIN_FPS_NODE, LTPO_TEST_TE_NODE }
    },
    .useOplusTouch = true,
    .touchGestureInitNode = { OPLUS_TOUCH_DT2W, OPLUS_TOUCH_GESTURE_WITHOUT_S },
    .touchGestureReportNode = OPLUS_TOUCH_GESTURE_REPORT
};

static const DeviceInfo CONFIG_OP12 = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH) |
                         static_cast<int>(Feature::LTPO),
    .featureNode = {
        .dcDimmingNode = ONE_PULSE_NODE,
        .highTouchSampleNode = OPLUS_TOUCH_HIGH_TOUCH_SAMPLE,
        .ltpoNode = { LTPO_MIN_FPS_NODE, LTPO_TEST_TE_NODE }
    },
    .colorModeMap = COLOR_MODE_MAP_303_301_307,
    .useOplusTouch = true,
    .touchGestureInitNode = { OPLUS_TOUCH_DT2W, OPLUS_TOUCH_GESTURE_WITHOUT_S },
    .touchGestureReportNode = OPLUS_TOUCH_GESTURE_REPORT
};

static const DeviceInfo CONFIG_OP12R = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH),
    .featureNode = {
        .dcDimmingNode = ONE_PULSE_NODE,
        .highTouchSampleNode = OPLUS_TOUCH_HIGH_TOUCH_SAMPLE,
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .useOplusTouch = true,
    .touchGestureInitNode = { OPLUS_TOUCH_GESTURE_WITHOUT_S },
    .touchGestureReportNode = OPLUS_TOUCH_GESTURE_REPORT
};

static const DeviceInfo CONFIG_OP11 = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH) |
                         static_cast<int>(Feature::LTPO),
    .featureNode = {
        .dcDimmingNode = ONE_PULSE_NODE,
        .highTouchSampleNode = OPLUS_TOUCH_HIGH_TOUCH_SAMPLE,
        .ltpoNode = { LTPO_MIN_FPS_NODE, LTPO_TEST_TE_NODE }
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .useOplusTouch = true,
    .touchGestureInitNode = { OPLUS_TOUCH_DT2W, OPLUS_TOUCH_GESTURE_WITHOUT_S },
    .touchGestureReportNode = OPLUS_TOUCH_GESTURE_REPORT
};

static const DeviceInfo CONFIG_OP9P = {
    .supportedFeatures = static_cast<int>(Feature::HBM_MODE) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH) |
                         static_cast<int>(Feature::SDR2HDR) |
                         static_cast<int>(Feature::MEMC_FHD),
    .featureNode = {
        .hbmNode = HBM_NODE,
        .highTouchSampleNode = HIGH_TOUCH_SAMPLE_NODE
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .displayOrientationMap = TP_ORIENTATION_MAP
};

static const DeviceInfo CONFIG_OP9 = {
    .supportedFeatures = static_cast<int>(Feature::HBM_MODE) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH),
    .featureNode = {
        .hbmNode = HBM_NODE,
        .highTouchSampleNode = HIGH_TOUCH_SAMPLE_NODE
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .displayOrientationMap = TP_ORIENTATION_MAP
};

static const DeviceInfo CONFIG_OP8_8T_9R = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HBM_MODE) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH),
    .featureNode = {
        .dcDimmingNode = DC_DIMMING_NODE,
        .hbmNode = HBM_NODE,
        .highTouchSampleNode = HIGH_TOUCH_SAMPLE_NODE
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .displayOrientationMap = TP_ORIENTATION_MAP
};

static const DeviceInfo CONFIG_OP8P = {
    .supportedFeatures = static_cast<int>(Feature::DC_DIMMING) |
                         static_cast<int>(Feature::HBM_MODE) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH) |
                         static_cast<int>(Feature::SDR2HDR) |
                         static_cast<int>(Feature::MEMC_FHD) |
                         static_cast<int>(Feature::MEMC_QHD),
    .featureNode = {
        .dcDimmingNode = DC_DIMMING_NODE,
        .hbmNode = HBM_NODE,
        .highTouchSampleNode = HIGH_TOUCH_SAMPLE_NODE
    },
    .colorModeMap = COLOR_MODE_MAP_0_256_258_259,
    .displayOrientationMap = TP_ORIENTATION_MAP
};

static const DeviceInfo CONFIG_OPNCE4 = {
    .supportedFeatures = static_cast<int>(Feature::HBM_MODE) |
                         static_cast<int>(Feature::HIGH_SAMPLE_TOUCH),
    .featureNode = {
        .hbmNode = HBM_NODE,
        .highTouchSampleNode = HIGH_TOUCH_SAMPLE_NODE
    },
    .displayOrientationMap = TP_ORIENTATION_MAP
};

static const DeviceInfo CONFIG_EMPTY {
    .supportedFeatures = 0
};
