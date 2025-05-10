/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "Battery.h"

using ::aidl::vendor::sun::hardware::battery::ChargingStatus;
using ::aidl::vendor::sun::hardware::battery::DeviceInfo;
using ::aidl::vendor::sun::hardware::battery::Feature;
using ::aidl::vendor::sun::hardware::battery::FeatureNode;
using ::aidl::vendor::sun::hardware::battery::NodeInfo;

static const NodeInfo VOOC_CHARING_NODE = {
    .path = "/sys/class/oplus_chg/battery/voocchg_ing",
    .readDisabledPrefix = "0"
};
static const NodeInfo CHARGING_SUSPEND_NODE = {
    .path = "/sys/class/oplus_chg/battery/mmi_charging_enable",
    .readDisabledPrefix = "1",
    .writeEnableValue = "0",
    .writeDisableValue = "1"
};
static const NodeInfo WIRELESS_RX_NODE = {
    .path = "/proc/wireless/enable_rx",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo WIRELESS_TX_NODE = {
    .path = "/proc/wireless/enable_tx",
    .readDisabledPrefix = "disable",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo WIRELESS_QUIET_MODE_NODE = {
    .path = "/proc/wireless/quiet_mode",
    .readDisabledPrefix = "false",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};
static const NodeInfo WIRELESS_QUIET_MODE_NEW_NODE = {
    .path = "/proc/wireless/user_sleep_mode",
    .readDisabledPrefix = "0",
    .writeEnableValue = "1",
    .writeDisableValue = "0"
};

static const DeviceInfo CONFIG_OP13 = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_RX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_TX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_QUIET_MODE),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE,
        .wirelessRxNode = WIRELESS_RX_NODE,
        .wirelessTxNode = WIRELESS_TX_NODE,
        .wirelessQuietModeNode = WIRELESS_QUIET_MODE_NEW_NODE
    },
    .fastChargingStatus = ChargingStatus::VOOC_CHARGING
};

static const DeviceInfo CONFIG_OP12 = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_RX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_TX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_QUIET_MODE),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE,
        .wirelessRxNode = WIRELESS_RX_NODE,
        .wirelessTxNode = WIRELESS_TX_NODE,
        .wirelessQuietModeNode = WIRELESS_QUIET_MODE_NEW_NODE
    },
    .fastChargingStatus = ChargingStatus::VOOC_CHARGING
};

static const DeviceInfo CONFIG_OP12R = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE
    },
    .fastChargingStatus = ChargingStatus::VOOC_CHARGING
};

static const DeviceInfo CONFIG_OP11 = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE
    },
    .fastChargingStatus = ChargingStatus::VOOC_CHARGING
};

static const DeviceInfo CONFIG_OP9P = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_RX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_TX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_QUIET_MODE),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE,
        .wirelessRxNode = WIRELESS_RX_NODE,
        .wirelessTxNode = WIRELESS_TX_NODE,
        .wirelessQuietModeNode = WIRELESS_QUIET_MODE_NEW_NODE
    },
    .fastChargingStatus = ChargingStatus::WARP_CHARGING
};

static const DeviceInfo CONFIG_OP9_CN_IN = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE
    },
    .fastChargingStatus = ChargingStatus::WARP_CHARGING
};

static const DeviceInfo CONFIG_OP9_TMO_EU_GLO = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_RX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_TX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_QUIET_MODE),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE,
        .wirelessRxNode = WIRELESS_RX_NODE,
        .wirelessTxNode = WIRELESS_TX_NODE,
        .wirelessQuietModeNode = WIRELESS_QUIET_MODE_NEW_NODE
    },
    .fastChargingStatus = ChargingStatus::WARP_CHARGING
};

static const DeviceInfo CONFIG_OP8_8T_9R = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE
    },
    .fastChargingStatus = ChargingStatus::WARP_CHARGING
};

static const DeviceInfo CONFIG_OP8P = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_RX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_TX) |
                         static_cast<int>(Feature::WIRELESS_CHARGING_QUIET_MODE),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE,
        .wirelessRxNode = WIRELESS_RX_NODE,
        .wirelessTxNode = WIRELESS_TX_NODE,
        .wirelessQuietModeNode = WIRELESS_QUIET_MODE_NODE
    },
    .fastChargingStatus = ChargingStatus::WARP_CHARGING
};

static const DeviceInfo CONFIG_OPNCE4 = {
    .supportedFeatures = static_cast<int>(Feature::SUSPEND_CHARGING),
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE,
        .chargingSuspendNode = CHARGING_SUSPEND_NODE
    },
    .fastChargingStatus = ChargingStatus::VOOC_CHARGING
};

static const DeviceInfo CONFIG_EMPTY_FEATURE = {
    .supportedFeatures = 0,
    .featureNode = {
        .voocChgingNode = VOOC_CHARING_NODE
    },
    .fastChargingStatus = ChargingStatus::FAST_CHARGING
};
