/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/Config.h"

#include <android/binder_manager.h>
#include <android-base/file.h>
#include <android-base/properties.h>

using ::android::base::GetProperty;
using ::android::base::ReadFileToString;
using ::android::base::SetProperty;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace displayfeature {

DisplayFeature::DisplayFeature() : mDeviceInfo(nullptr) {
    std::string deviceName = GetProperty("ro.sun.device", "");
    if (DEBUG_ENABLED) {
        ALOGD("device name: %s", deviceName.c_str());
    }

    // OnePlus 13
    if (deviceName == "dodge") {
        mDeviceInfo = &CONFIG_OP13;
        SetProperty("sys.sun.feature.touch_gesture.bit", "78068");
        SetProperty("sys.sun.feature.touch_gesture.oplus", "true");
        goto END;
    }
    // OnePlus 12
    if (deviceName == "waffle") {
        mDeviceInfo = &CONFIG_OP12;
        SetProperty("sys.sun.feature.display.dc_alias_one_pulse", "true");
        SetProperty("sys.sun.feature.touch_gesture.bit", "78068");
        SetProperty("sys.sun.feature.touch_gesture.oplus", "true");
        goto END;
    }
    // OnePlus 12R
    if (deviceName == "aston") {
        mDeviceInfo = &CONFIG_OP12R;
        SetProperty("sys.sun.feature.display.dc_alias_one_pulse", "true");
        SetProperty("sys.sun.feature.touch_gesture.bit", "78068");
        SetProperty("sys.sun.feature.touch_gesture.oplus", "true");
        goto END;
    }
    // OnePlus 11
    if (deviceName == "salami") {
        mDeviceInfo = &CONFIG_OP11;
        SetProperty("sys.sun.feature.display.dc_alias_one_pulse", "true");
        SetProperty("sys.sun.feature.touch_gesture.bit", "78068");
        SetProperty("sys.sun.feature.touch_gesture.oplus", "true");
        goto END;
    }
    // OnePlus 9 Pro
    if (deviceName == "lemonadep") {
        mDeviceInfo = &CONFIG_OP9P;
        SetProperty("sys.sun.feature.touch_gesture.bit", "340212");
        goto END;
    }
    // OnePlus 9
    if (deviceName == "lemonade") {
        mDeviceInfo = &CONFIG_OP9;
        SetProperty("sys.sun.feature.touch_gesture.bit", "340212");
        goto END;
    }
    // OnePlus 8/8T/9R
    if (deviceName == "instantnoodle" || deviceName == "kebab" || deviceName == "lemonades") {
        mDeviceInfo = &CONFIG_OP8_8T_9R;
        SetProperty("sys.sun.feature.touch_gesture.bit", "340212");
        goto END;
    }
    // OnePlus 8 Pro
    if (deviceName == "instantnoodlep") {
        mDeviceInfo = &CONFIG_OP8P;
        SetProperty("sys.sun.feature.touch_gesture.bit", "340212");
        goto END;
    }

    ALOGE("device is not unsupported");
    mDeviceInfo = &CONFIG_EMPTY;
    goto END;

END:
    if (DEBUG_ENABLED) {
        ALOGD("device supported feature: %d", mDeviceInfo->supportedFeatures);
    }
}

ndk::ScopedAStatus DisplayFeature::hasFeature(Feature feature, bool* _aidl_return) {
    *_aidl_return = mDeviceInfo->supportedFeatures & static_cast<int>(feature);
    if (DEBUG_ENABLED) {
        ALOGD("hasFeature, feature: %d, ret: %d", static_cast<int>(feature), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayFeature::isFeatureEnabled(Feature feature, bool* _aidl_return) {
    hasFeature(feature, _aidl_return);
    if (!*_aidl_return) {
        *_aidl_return = false;
        ALOGE("isFeatureEnabled, feature %d is not supported", static_cast<int>(feature));
        return ndk::ScopedAStatus::ok();
    }

    bool ret = false;
    std::string value;
    switch (feature) {
        case Feature::DC_DIMMING:
            ret = ReadFileToString(mDeviceInfo->featureNode.dcDimmingNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.dcDimmingNode.readDisabledPrefix);
            }
            break;
        case Feature::HBM_MODE:
            ret = ReadFileToString(mDeviceInfo->featureNode.hbmNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.hbmNode.readDisabledPrefix);
            }
            break;
        case Feature::HIGH_SAMPLE_TOUCH:
            if (mDeviceInfo->useOplusTouch) {
                ret = oplusTouchReadNode(mDeviceInfo->featureNode.highTouchSampleNode.oplusTouchFeatureId, &value);
            } else {
                ret = ReadFileToString(mDeviceInfo->featureNode.highTouchSampleNode.path, &value);
            }
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.highTouchSampleNode.readDisabledPrefix);
            }
            break;
        case Feature::LTPO:
            ret = true;
            *_aidl_return = true;
            if (DEBUG_ENABLED) {
                ALOGD("isFeatureEnabled, skip for ltpo");
            }
            break;
        default:
            *_aidl_return = false;
            ALOGE("isFeatureEnabled, feature %d doesn't exist or not supported", static_cast<int>(feature));
            return ndk::ScopedAStatus::ok();
    }
    if (!ret) {
        ALOGE("isFeatureEnabled, failed to read for feature: %d", static_cast<int>(feature));
    } if (DEBUG_ENABLED) {
        ALOGD("isFeatureEnabled, feature: %d, enabled: %d", static_cast<int>(feature), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayFeature::setFeatureEnabled(Feature feature, bool enabled) {
    bool featureSupported = false;
    hasFeature(feature, &featureSupported);
    if (!featureSupported) {
        ALOGE("setFeatureEnabled, feature %d is not supported", static_cast<int>(feature));
        return ndk::ScopedAStatus::ok();
    }

    bool ret = false;
    switch (feature) {
        case Feature::DC_DIMMING:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.dcDimmingNode.writeEnableValue
                                            : mDeviceInfo->featureNode.dcDimmingNode.writeDisableValue,
                                    mDeviceInfo->featureNode.dcDimmingNode.path, true);
            break;
        case Feature::HBM_MODE:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.hbmNode.writeEnableValue
                                            : mDeviceInfo->featureNode.hbmNode.writeDisableValue,
                                    mDeviceInfo->featureNode.hbmNode.path, true);
            break;
        case Feature::HIGH_SAMPLE_TOUCH:
            if (mDeviceInfo->useOplusTouch) {
                ret = oplusTouchWriteNode(mDeviceInfo->featureNode.highTouchSampleNode.oplusTouchFeatureId,
                                          enabled ? mDeviceInfo->featureNode.highTouchSampleNode.writeEnableValue
                                                  : mDeviceInfo->featureNode.highTouchSampleNode.writeDisableValue);
            } else {
                ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.highTouchSampleNode.writeEnableValue
                                                : mDeviceInfo->featureNode.highTouchSampleNode.writeDisableValue,
                                        mDeviceInfo->featureNode.highTouchSampleNode.path, true);
            }
            break;
        case Feature::LTPO:
            ret = true;
            for (int i = 0; i < static_cast<int>(mDeviceInfo->featureNode.ltpoNode.size()); i++) {
                ret &= WriteStringToFile(enabled ? mDeviceInfo->featureNode.ltpoNode[i].writeEnableValue
                                                 : mDeviceInfo->featureNode.ltpoNode[i].writeDisableValue,
                                         mDeviceInfo->featureNode.ltpoNode[i].path, true);
            }
            break;
        default:
            ALOGE("setFeatureEnabled, feature %d doesn't exist or not supported", static_cast<int>(feature));
            return ndk::ScopedAStatus::ok();
    }
    if (!ret) {
        ALOGE("setFeatureEnabled failed, feature: %d, enabled: %d", static_cast<int>(feature), enabled);
    } else if (DEBUG_ENABLED) {
        ALOGD("setFeatureEnabled success, feature: %d, enabled: %d", static_cast<int>(feature), enabled);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayFeature::setColorMode(int colorMode) {
    if (colorMode < 0) {
        ALOGE("setColorMode, invalid colorMode %d", colorMode);
        return ndk::ScopedAStatus::ok();
    }

    if (mDeviceInfo->colorModeMap.find(colorMode) != mDeviceInfo->colorModeMap.end()) {
        if (!WriteStringToFile(mDeviceInfo->colorModeMap.at(colorMode).c_str(), SEED_PATH, true)) {
            ALOGE("setColorMode failed, colorMode: %d, seed: %s",
                    colorMode, mDeviceInfo->colorModeMap.at(colorMode).c_str());
        } else if (DEBUG_ENABLED) {
            ALOGD("setColorMode success, colorMode: %d, seed: %s",
                    colorMode, mDeviceInfo->colorModeMap.at(colorMode).c_str());
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayFeature::setDisplayOrientation(int orientation) {
    if (orientation < 0) {
        ALOGE("setDisplayOrientation, invalid orientation %d", orientation);
        return ndk::ScopedAStatus::ok();
    }

    if (mDeviceInfo->displayOrientationMap.find(orientation) != mDeviceInfo->displayOrientationMap.end()) {
        if (!WriteStringToFile(mDeviceInfo->displayOrientationMap.at(orientation).c_str(), TP_DIRECTION_PATH, true)) {
            ALOGE("setDisplayOrientation failed, orientation: %d, value: %s",
                    orientation, mDeviceInfo->displayOrientationMap.at(orientation).c_str());
        } else if (DEBUG_ENABLED) {
            ALOGD("setDisplayOrientation success, orientation: %d, value: %s",
                    orientation, mDeviceInfo->displayOrientationMap.at(orientation).c_str());
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayFeature::sendCommand(Command command, int* _aidl_return) {
    if (!mDeviceInfo->useOplusTouch) {
        *_aidl_return = -1;
        if (DEBUG_ENABLED) {
            ALOGD("sendCommand, oplus touch not supported");
        }
        return ndk::ScopedAStatus::ok();
    }

    bool ret = true;
    std::string value;
    switch (command) {
        case Command::CMD_INIT_TOUCH_GESTURE:
            for (const NodeInfo& nodeInfo : mDeviceInfo->touchGestureInitNode) {
                ret &= oplusTouchWriteNode(nodeInfo.oplusTouchFeatureId, nodeInfo.writeEnableValue);
            }
            *_aidl_return = 0;
            break;
        case Command::CMD_REPORT_TOUCH_GESTURE:
            ret = oplusTouchReadNode(mDeviceInfo->touchGestureReportNode.oplusTouchFeatureId, &value);
            if (ret) {
                *_aidl_return = std::stoi(value.substr(0, value.find(",")));
            }
            break;
        default:
            *_aidl_return = -1;
            ALOGE("sendCommand, command %d doesn't exist", static_cast<int>(command));
            return ndk::ScopedAStatus::ok();
    }
    if (!ret) {
        ALOGE("sendCommand failed, command: %d", static_cast<int>(command));
    } if (DEBUG_ENABLED) {
        ALOGD("sendCommand succuss, command: %d, result: %d", static_cast<int>(command), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

bool DisplayFeature::oplusTouchReadNode(int featureId, std::string* result) {
    if (mOplusTouch == nullptr) {
        const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
        mOplusTouch = IOplusTouch::fromBinder(ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
    }
    if (mOplusTouch == nullptr) {
        ALOGE("oplusTouchReadNode failed, oplus touch service is null");
        return false;
    }
    mOplusTouch->touchReadNodeFile(0, featureId, result);
    if (DEBUG_ENABLED) {
        ALOGD("oplusTouchReadNode success, featureId: %d, result: %s", featureId, (*result).c_str());
    }
    return true;
}

bool DisplayFeature::oplusTouchWriteNode(int featureId, std::string value) {
    if (mOplusTouch == nullptr) {
        const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
        mOplusTouch = IOplusTouch::fromBinder(ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
    }
    if (mOplusTouch == nullptr) {
        ALOGE("oplusTouchWriteNode failed, oplus touch service is null");
        return false;
    }
    int result;
    mOplusTouch->touchWriteNodeFile(0, featureId, value, &result);
    if (DEBUG_ENABLED) {
        ALOGD("oplusTouchWriteNode, featureId: %d, value: %s, result: %d", featureId, value.c_str(), result);
    }
    return result > 0;
}

}  // namespace displayfeature
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
