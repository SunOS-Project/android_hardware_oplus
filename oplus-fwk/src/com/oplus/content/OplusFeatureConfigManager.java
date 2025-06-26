/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.content;

import android.os.SystemProperties;
import android.text.TextUtils;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class OplusFeatureConfigManager {
    public static OplusFeatureConfigManager sOplusFeatureConfigManager = null;

    public final List<OnFeatureActionObserver> observers = new CopyOnWriteArrayList<>();

    public static OplusFeatureConfigManager getInstance() {
        if (sOplusFeatureConfigManager == null) {
            sOplusFeatureConfigManager = new OplusFeatureConfigManager();
        }
        return sOplusFeatureConfigManager;
    }

    public boolean hasFeature(String name) {
        if ("oplus.software.vibrator_lmvibrator".equals(name)) {
            // OnePlus 7/8 series use old OnePlus base camera. Don't let this feature break haptic feedback.
            if (useOnePlusBaseCamera()) {
                return false;
            }
            return true;
        }
        if ("oplus.hardware.type.rear_IR".equals(name)) {
            return true;
        }
        return false;
    }

    private static boolean useOnePlusBaseCamera() {
        return !TextUtils.isEmpty(SystemProperties.get("ro.oplus.version.base"));
    }

    public interface OnFeatureObserver {
        default void onFeatureUpdate(List<String> features) {}
    }

    public interface OnFeatureActionObserver {
        void onFeaturesActionUpdate(String featureName, String action, IOplusFeatureConfigManager.FeatureID featureID);
    }

    public boolean registerFeatureActionObserver(OnFeatureActionObserver observer) {
        if (observer != null) {
            observers.add(observer);
            return true;
        }
        return false;
    }

    public boolean unregisterFeatureActionObserver(OnFeatureActionObserver observer) {
        return observers.remove(observer);
    }
}
