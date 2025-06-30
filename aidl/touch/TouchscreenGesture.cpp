/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include <android-base/file.h>
#include <android-base/strings.h>

#include <OplusTouchConstants.h>
#include <TouchscreenGestureConfig.h>

using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGestureEnableIndepPath = "/proc/touchpanel/double_tap_enable_indep";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

TouchscreenGesture::TouchscreenGesture(std::shared_ptr<IOplusTouch> oplusTouch)
    : mOplusTouch(std::move(oplusTouch)) {}

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    for (const auto& [id, name] : kGestureNames) {
        if (kSupportedGestures & (1 << id)) {
            gestures.push_back({static_cast<int>(gestures.size()), name, kGestureStartKey + id});
        }
    }

    *_aidl_return = gestures;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    int contents = 0;

    if (std::string tmp; mOplusTouch) {
        mOplusTouch->touchReadNodeFile(DEFAULT_TP_IC_ID, DOUBLE_TAP_INDEP_NODE, &tmp);
        contents = std::stoi(tmp, nullptr, 16);
    } else if (ReadFileToString(kGestureEnableIndepPath, &tmp)) {
        contents = std::stoi(Trim(tmp), nullptr, 16);
    } else {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (enabled) {
        contents |= (1 << (gesture.keycode - kGestureStartKey));
    } else {
        contents &= ~(1 << (gesture.keycode - kGestureStartKey));
    }

    if (mOplusTouch) {
        mOplusTouch->touchWriteNodeFileOneWay(DEFAULT_TP_IC_ID, DOUBLE_TAP_ENABLE_NODE, "1");
        mOplusTouch->touchWriteNodeFileOneWay(DEFAULT_TP_IC_ID, DOUBLE_TAP_INDEP_NODE,
                                              std::to_string(contents));
    } else if (!WriteStringToFile(std::to_string(contents), kGestureEnableIndepPath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
