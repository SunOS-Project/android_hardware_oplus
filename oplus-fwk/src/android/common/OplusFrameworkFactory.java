/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package android.common;

public class OplusFrameworkFactory implements IOplusCommonFactory {
    private static OplusFrameworkFactory sOplusFrameworkFactory = null;

    public static OplusFrameworkFactory getInstance() {
        if (sOplusFrameworkFactory == null) {
            sOplusFrameworkFactory = new OplusFrameworkFactory();
        }
        return sOplusFrameworkFactory;
    }

    @Override
    public boolean isValid(int index) {
        final boolean validOplus =
                index < OplusFeatureList.OplusIndex.EndOplusFrameworkFactory.ordinal() &&
                index > OplusFeatureList.OplusIndex.StartOplusFrameworkFactory.ordinal();
        final boolean vaildOplusOs =
                index < OplusFeatureList.OplusIndex.EndOplusOsFrameworkFactory.ordinal() &&
                index > OplusFeatureList.OplusIndex.StartOplusOsFrameworkFactory.ordinal();
        return vaildOplusOs || validOplus;
    }
}
