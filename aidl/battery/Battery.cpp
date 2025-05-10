/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/Config.h"

#include <android-base/file.h>
#include <android-base/properties.h>

using ::android::base::GetProperty;
using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace sun {
namespace hardware {
namespace battery {

Battery::Battery() : mDeviceInfo(nullptr) {
    std::string deviceName = GetProperty("ro.sun.device", "");
    int prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));
    int rfVersion = std::stoi(GetProperty("ro.boot.rf_version", "0"));
    if (DEBUG_ENABLED) {
        ALOGD("device name: %s, prjname: %d, rf version: %d", deviceName.c_str(), prjname, rfVersion);
    }

    // OnePlus 13
    if (deviceName == "dodge") {
        mDeviceInfo = &CONFIG_OP13;
        goto END;
    }
    // OnePlus 12
    if (deviceName == "waffle") {
        mDeviceInfo = &CONFIG_OP12;
        goto END;
    }
    // OnePlus 12R
    if (deviceName == "aston") {
        mDeviceInfo = &CONFIG_OP12R;
        goto END;
    }
    // OnePlus 11
    if (deviceName == "salami") {
        mDeviceInfo = &CONFIG_OP11;
        goto END;
    }
    // OnePlus 9 Pro
    if (deviceName == "lemonadep") {
        mDeviceInfo = &CONFIG_OP9P;
        goto END;
    }
    // OnePlus 8/8T/9R
    if (deviceName == "instantnoodle" || deviceName == "kebab" || deviceName == "lemonades") {
        mDeviceInfo = &CONFIG_OP8_8T_9R;
        goto END;
    }
    // OnePlus 8 Pro
    if (deviceName == "instantnoodlep") {
        mDeviceInfo = &CONFIG_OP8P;
        goto END;
    }

    if (prjname <= 0) {
        ALOGE("device prjname is empty");
        mDeviceInfo = &CONFIG_EMPTY_FEATURE;
        goto END;
    }
    switch (prjname) {
        case 19825:      // OnePlus 9
        case 20854:      // OnePlus 9 TMO
            switch (rfVersion) {
                case 11: // CN
                case 13: // IN
                    mDeviceInfo = &CONFIG_OP9_CN_IN;
                    break;
                case 12: // TMO
                case 21: // EU
                case 22: // NA
                    mDeviceInfo = &CONFIG_OP9_TMO_EU_GLO;
                    break;
                default:
                    ALOGE("device rf version is unsupported");
                    mDeviceInfo = &CONFIG_EMPTY_FEATURE;
                    goto END;
            }
            break;
        default:
            ALOGE("device prjname is unsupported");
            mDeviceInfo = &CONFIG_EMPTY_FEATURE;
            goto END;
    }

END:
    if (DEBUG_ENABLED) {
        ALOGD("device supported feature: %d", mDeviceInfo->supportedFeatures);
    }
}

ndk::ScopedAStatus Battery::hasFeature(Feature feature, bool* _aidl_return) {
    *_aidl_return = mDeviceInfo->supportedFeatures & static_cast<int>(feature);
    if (DEBUG_ENABLED) {
        ALOGD("hasFeature, feature: %d, ret: %d", static_cast<int>(feature), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Battery::isFeatureEnabled(Feature feature, bool* _aidl_return) {
    hasFeature(feature, _aidl_return);
    if (!*_aidl_return) {
        *_aidl_return = false;
        ALOGE("isFeatureEnabled, feature %d is not supported", static_cast<int>(feature));
        return ndk::ScopedAStatus::ok();
    }

    bool ret = false;
    std::string value;
    switch (feature) {
        case Feature::SUSPEND_CHARGING:
            ret = ReadFileToString(mDeviceInfo->featureNode.chargingSuspendNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.chargingSuspendNode.readDisabledPrefix);
            }
            break;
        case Feature::WIRELESS_CHARGING_RX:
            ret = ReadFileToString(mDeviceInfo->featureNode.wirelessRxNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.wirelessRxNode.readDisabledPrefix);
            }
            break;
        case Feature::WIRELESS_CHARGING_TX:
            ret = ReadFileToString(mDeviceInfo->featureNode.wirelessTxNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.wirelessTxNode.readDisabledPrefix);
            }
            break;
        case Feature::WIRELESS_CHARGING_QUIET_MODE:
            ret = ReadFileToString(mDeviceInfo->featureNode.wirelessQuietModeNode.path, &value);
            if (ret) {
                *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.wirelessQuietModeNode.readDisabledPrefix);
            }
            break;
        default:
            *_aidl_return = false;
            ALOGE("isFeatureEnabled, feature %d doesn't exist", static_cast<int>(feature));
            return ndk::ScopedAStatus::ok();
    }
    if (!ret) {
        ALOGE("isFeatureEnabled, failed to read for feature: %d", static_cast<int>(feature));
    } if (DEBUG_ENABLED) {
        ALOGD("isFeatureEnabled, feature: %d, enabled: %d", static_cast<int>(feature), *_aidl_return);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Battery::setFeatureEnabled(Feature feature, bool enabled) {
    bool featureSupported = false;
    hasFeature(feature, &featureSupported);
    if (!featureSupported) {
        ALOGE("setFeatureEnabled, feature %d is not supported", static_cast<int>(feature));
        return ndk::ScopedAStatus::ok();
    }

    bool ret = false;
    switch (feature) {
        case Feature::SUSPEND_CHARGING:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.chargingSuspendNode.writeEnableValue
                                            : mDeviceInfo->featureNode.chargingSuspendNode.writeDisableValue,
                                    mDeviceInfo->featureNode.chargingSuspendNode.path, true);
            break;
        case Feature::WIRELESS_CHARGING_RX:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.wirelessRxNode.writeEnableValue
                                            : mDeviceInfo->featureNode.wirelessRxNode.writeDisableValue,
                                    mDeviceInfo->featureNode.wirelessRxNode.path, true);
            break;
        case Feature::WIRELESS_CHARGING_TX:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.wirelessTxNode.writeEnableValue
                                            : mDeviceInfo->featureNode.wirelessTxNode.writeDisableValue,
                                    mDeviceInfo->featureNode.wirelessTxNode.path, true);
            break;
        case Feature::WIRELESS_CHARGING_QUIET_MODE:
            ret = WriteStringToFile(enabled ? mDeviceInfo->featureNode.wirelessQuietModeNode.writeEnableValue
                                            : mDeviceInfo->featureNode.wirelessQuietModeNode.writeDisableValue,
                                    mDeviceInfo->featureNode.wirelessQuietModeNode.path, true);
            break;
        default:
            ALOGE("setFeatureEnabled, feature %d doesn't exist", static_cast<int>(feature));
            return ndk::ScopedAStatus::ok();
    }
    if (!ret) {
        ALOGE("setFeatureEnabled failed, feature: %d, enabled: %d", static_cast<int>(feature), enabled);
    } else if (DEBUG_ENABLED) {
        ALOGD("setFeatureEnabled success, feature: %d, enabled: %d", static_cast<int>(feature), enabled);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Battery::readChargingStatus(ChargingStatus* _aidl_return) {
    std::string value;
    if (!ReadFileToString(mDeviceInfo->featureNode.voocChgingNode.path, &value)) {
        ALOGE("readChargingStatus failed");
        *_aidl_return = ChargingStatus::UNKNOWN;
        return ndk::ScopedAStatus::ok();
    }
    *_aidl_return = !value.starts_with(mDeviceInfo->featureNode.voocChgingNode.readDisabledPrefix)
            ? mDeviceInfo->fastChargingStatus : ChargingStatus::UNKNOWN;
    if (DEBUG_ENABLED) {
        ALOGD("readChargingStatus, status: %d", static_cast<int>(*_aidl_return));
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace battery
}  // namespace hardware
}  // namespace sun
}  // namespace vendor
}  // namespace aidl
