package com.oplus.inner.hardware.camera2.impl;

import android.hardware.camera2.impl.CameraMetadataNative;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.TotalCaptureResult;

public class CameraMetadataNativeWrapper {

    public static long getMetadataPtr(Object metadata) {
        if (metadata == null) {
            throw new IllegalArgumentException("Metadata object is null.");
        }

        if (metadata instanceof CameraCharacteristics) {
            CameraMetadataNative nativeMetadata = ((CameraCharacteristics) metadata).getNativeMetadata();
            if (nativeMetadata == null) {
                throw new IllegalArgumentException("CameraCharacteristics' native metadata is null.");
            }
            metadata = nativeMetadata;
        } else if (metadata instanceof TotalCaptureResult) {
            CameraMetadataNative nativeMetadata = ((TotalCaptureResult) metadata).getNativeMetadata();
            if (nativeMetadata == null) {
                throw new IllegalArgumentException("TotalCaptureResult's native metadata is null.");
            }
            metadata = nativeMetadata;
        }

        if (!(metadata instanceof CameraMetadataNative)) {
            throw new IllegalArgumentException("Invalid metadata object.");
        }

        return ((CameraMetadataNative) metadata).getMetadataPtr();
    }
}
