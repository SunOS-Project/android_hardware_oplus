package android.hardware.camera2;

import android.content.Context;
import android.os.IBinder;
import android.os.Parcel;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.util.Log;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.CaptureResult;
import android.hardware.camera2.IOplusCameraManager;
import android.hardware.camera2.impl.CameraMetadataNative;
import android.hardware.camera2.marshal.MarshalRegistry;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.lang.reflect.Field;

public class OplusCameraManager {

    private static OplusCameraManager sOplusCameraManager = null;

    private static final String TAG = "OplusCameraManager";

    public static OplusCameraManager getInstance() {
        if (sOplusCameraManager == null) {
            sOplusCameraManager = new OplusCameraManager();
        }
        return sOplusCameraManager;
    }

    public static Object getEmptyCameraMetadataNative(long[] metadataPtr) {
        CameraMetadataNative meta = new CameraMetadataNative();
        if (metadataPtr != null && metadataPtr.length > 0) {
            metadataPtr[0] = meta.getMetadataPtr();
        }
        return meta;
    }

    public static <T> T metaDataValueConvert(CaptureResult.Key<T> key, int i, byte[] bArr) {
        if (key != null && bArr != null) {
            return (T) MarshalRegistry.getMarshaler(key.getNativeKey().getTypeReference(), i).unmarshal(ByteBuffer.wrap(bArr).order(ByteOrder.nativeOrder()));
        }
        return null;
    }

    public static int getMetadataTag(CaptureResult.Key key) {
        if (key != null) {
            return key.getNativeKey().getTag();
        }
        return -1;
    }

    public static TotalCaptureResult generateTotalCaptureResult(Object meta, long frameId) {
        if (meta == null || !(meta instanceof CameraMetadataNative)) {
            return null;
        }
        TotalCaptureResult r = new TotalCaptureResult((CameraMetadataNative) meta, 0);
        try {
            Field numField = CaptureResult.class.getDeclaredField("mFrameNumber");
            numField.setAccessible(true);
            numField.setLong(r, frameId);
        } catch (IllegalAccessException | NoSuchFieldException e) {
            e.printStackTrace();
        }
        return r;
    }
    
    public void preOpenCamera(Context context) {
        if (context == null) {
            throw new IllegalArgumentException("context was null");
        }
        try {
            OplusCameraManagerGlobal.get().preOpenCamera(context.getOpPackageName());
        } catch (CameraAccessException | RemoteException e) {
            e.printStackTrace();
        }
    }

    public void setCallInfo() {
        try {
            OplusCameraManagerGlobal.get().setCallInfo();
        } catch (CameraAccessException | RemoteException e) {
            e.printStackTrace();
        }
    }

    public void setDeathRecipient(IBinder client) {
        if (client == null) {
            throw new IllegalArgumentException("client was null");
        }
        try {
            OplusCameraManagerGlobal.get().setDeathRecipient(client);
        } catch (CameraAccessException | RemoteException e) {
            e.printStackTrace();
        }
    }

    public void sendOplusExtCamCmd(Context context, IOplusCameraManager.Cmd cmd, int[] args) {
        if (context == null) {
            throw new IllegalArgumentException("context was null");
        }
        try {
            String packageName = context.getOpPackageName();
            OplusCameraManagerGlobal.get().sendOplusExtCamCmd(packageName, cmd, args);
        } catch (CameraAccessException | RemoteException e) {
            e.printStackTrace();
        }
    }

    private static final class OplusCameraManagerGlobal implements IBinder.DeathRecipient {

        private static final String TAG = "OplusCameraManagerGlobal";

        private static final String CAMERA_SERVICE_BINDER_NAME = "media.camera";
        private static final String DESCRIPTOR = "android.hardware.media";

        private static final int SET_DEATH_RECIPIENT = 10002;
        private static final int SET_CALL_INFO = 10006;
        private static final int PRE_OPEN_CAMERA = 10014;
        private static final int SEND_OPLUS_EXT_CAM_CMD = 10015;

        private static final OplusCameraManagerGlobal gOplusCameraManagerGlobal =
                new OplusCameraManagerGlobal();

        private static final boolean sCameraServiceDisabled =
                SystemProperties.getBoolean("config.disable_cameraservice", false);

        private final Object mLock = new Object();

        private IBinder mRemote = null;

        public static OplusCameraManagerGlobal get() {
            return gOplusCameraManagerGlobal;
        }

        private void connectCameraServiceLocked() {
            if (mRemote != null || sCameraServiceDisabled) {
                return;
            }
            mRemote = ServiceManager.getService(CAMERA_SERVICE_BINDER_NAME);
            if (mRemote != null) {
                try {
                    mRemote.linkToDeath(this, 0);
                } catch (RemoteException e) {
                    Log.e(TAG, "link to death failed");
                }
            }
        }

        public IBinder getCameraServiceRemote() {
            synchronized (mLock) {
                connectCameraServiceLocked();
                if (mRemote == null && !sCameraServiceDisabled) {
                    Log.e(TAG, "Camera service is unavailable");
                }
            }
            return mRemote;
        }

        public void preOpenCamera(String packageName) throws CameraAccessException, RemoteException {
            final IBinder remote = getCameraServiceRemote();
            if (remote == null) {
                throw new CameraAccessException(2, "Camera service is currently unavailable");
            }
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            try {
                data.writeInterfaceToken(DESCRIPTOR);
                data.writeInt(1);
                mRemote.transact(PRE_OPEN_CAMERA, data, reply, 0);
                reply.readException();
                data.recycle();
                reply.recycle();
                Log.i(TAG, "preOpenCamera, the " + packageName + " preOpenSend");
            } catch (Throwable th) {
                data.recycle();
                reply.recycle();
                throw th;
            }
        }

        public void setCallInfo() throws CameraAccessException, RemoteException {
            final IBinder remote = getCameraServiceRemote();
            if (remote == null) {
                throw new CameraAccessException(2, "Camera service is currently unavailable");
            }
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            try {
                data.writeInterfaceToken(DESCRIPTOR);
                mRemote.transact(SET_CALL_INFO, data, reply, 0);
                reply.readException();
            } finally {
                data.recycle();
                reply.recycle();
            }
        }

        public void setDeathRecipient(IBinder client) throws CameraAccessException, RemoteException {
            final IBinder remote = getCameraServiceRemote();
            if (remote == null) {
                throw new CameraAccessException(2, "Camera service is currently unavailable");
            }
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            try {
                data.writeInterfaceToken(DESCRIPTOR);
                data.writeStrongBinder(client);
                mRemote.transact(SET_DEATH_RECIPIENT, data, reply, 0);
                reply.readException();
            } finally {
                data.recycle();
                reply.recycle();
            }
        }

        public void sendOplusExtCamCmd(String packageName, IOplusCameraManager.Cmd cmd, int[] args) throws CameraAccessException, RemoteException {
            Log.e(TAG, "sendOplusExtCamCmd, packageName: " + packageName + ", cmd: " + cmd);
            final IBinder remote = getCameraServiceRemote();
            if (remote == null) {
                throw new CameraAccessException(2, "Camera service is currently unavailable");
            }
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            try {
                data.writeInterfaceToken(DESCRIPTOR);
                data.writeInt(cmd.ordinal());
                data.writeIntArray(args);
                mRemote.transact(SEND_OPLUS_EXT_CAM_CMD, data, reply, 0);
                reply.readException();
                data.recycle();
                reply.recycle();
                Log.e(TAG, "sendOplusExtCamCmd complete");
            } catch (Throwable th) {
                data.recycle();
                reply.recycle();
                throw th;
            }
        }

        @Override
        public void binderDied() {
            mRemote = null;
        }
    }
}
