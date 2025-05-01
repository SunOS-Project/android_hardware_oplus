/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/DisplayFeature.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using ::aidl::vendor::sun::hardware::displayfeature::DisplayFeature;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<DisplayFeature> displayFeature = ndk::SharedRefBase::make<DisplayFeature>();
    if (!displayFeature) {
        return EXIT_FAILURE;
    }

    const std::string instance = std::string() + DisplayFeature::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(displayFeature->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
