/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/sun/hardware/battery/BnBattery.h>
#include <aidl/vendor/sun/hardware/battery/ChargingStatus.h>
#include <aidl/vendor/sun/hardware/battery/Feature.h>
#include <log/log.h>

#undef LOG_TAG
#define LOG_TAG       "vendor.sun.hardware.battery-service.oplus"
#define DEBUG_ENABLED 0

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace battery {

struct NodeInfo {
    const std::string path;
    const std::string readDisabledPrefix;
    const std::string writeEnableValue;
    const std::string writeDisableValue;
};

struct FeatureNode {
    const NodeInfo chargingSuspendNode;
    const NodeInfo voocChgingNode;
    const NodeInfo wirelessQuietModeNode;
    const NodeInfo wirelessRxNode;
    const NodeInfo wirelessTxNode;
};

struct DeviceInfo {
    const int supportedFeatures;
    const FeatureNode featureNode;
    const ChargingStatus fastChargingStatus; 
};

struct Battery : public BnBattery {
    Battery();
private:
    const DeviceInfo* mDeviceInfo;
public:
    ndk::ScopedAStatus hasFeature(Feature feature, bool* _aidl_return) override;
    ndk::ScopedAStatus isFeatureEnabled(Feature feature, bool* _aidl_return) override;
    ndk::ScopedAStatus setFeatureEnabled(Feature feature, bool enabled) override;
    ndk::ScopedAStatus readChargingStatus(ChargingStatus* _aidl_return) override;
};

}  // namespace battery
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
