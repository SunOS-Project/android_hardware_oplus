/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/sun/hardware/displayfeature/BnDisplayFeature.h>
#include <aidl/vendor/sun/hardware/displayfeature/Command.h>
#include <aidl/vendor/sun/hardware/displayfeature/Feature.h>
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>
#include <log/log.h>

#include <unordered_map>
#include <vector>

#undef LOG_TAG
#define LOG_TAG       "vendor.sun.hardware.displayfeature-service.oplus"
#define DEBUG_ENABLED 0

using ::aidl::vendor::oplus::hardware::touch::IOplusTouch;

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace displayfeature {

struct NodeInfo {
    const std::string path;
    const std::string readDisabledPrefix;
    const std::string writeEnableValue;
    const std::string writeDisableValue;
    const int oplusTouchFeatureId;
};

struct FeatureNode {
    const NodeInfo dcDimmingNode;
    const NodeInfo hbmNode;
    const NodeInfo highTouchSampleNode;
    const std::vector<NodeInfo> ltpoNode;
};

struct DeviceInfo {
    const int supportedFeatures;
    const FeatureNode featureNode;
    const std::unordered_map<int, std::string> colorModeMap;
    const std::unordered_map<int, std::string> displayOrientationMap;

    const bool useOplusTouch = false;
    const std::vector<NodeInfo> touchGestureInitNode;
    const NodeInfo touchGestureReportNode;
};

struct DisplayFeature : public BnDisplayFeature {
    DisplayFeature();
private:
    const DeviceInfo* mDeviceInfo;
    std::shared_ptr<IOplusTouch> mOplusTouch = nullptr;
public:
    ndk::ScopedAStatus hasFeature(Feature feature, bool* _aidl_return) override;
    ndk::ScopedAStatus isFeatureEnabled(Feature feature, bool* _aidl_return) override;
    ndk::ScopedAStatus setFeatureEnabled(Feature feature, bool enabled) override;
    ndk::ScopedAStatus setColorMode(int colorMode) override;
    ndk::ScopedAStatus setDisplayOrientation(int orientation) override;
    ndk::ScopedAStatus sendCommand(Command command, int* _aidl_return) override;
    bool oplusTouchReadNode(int featureId, std::string* result);
    bool oplusTouchWriteNode(int featureId, std::string value);
};

}  // namespace displayfeature
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
