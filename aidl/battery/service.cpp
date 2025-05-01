/*
 * Copyright (C) 2025 The Nameless-CLO Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/Battery.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using ::aidl::vendor::sun::hardware::battery::Battery;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Battery> battery = ndk::SharedRefBase::make<Battery>();
    if (!battery) {
        return EXIT_FAILURE;
    }

    const std::string instance = std::string() + Battery::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(battery->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
