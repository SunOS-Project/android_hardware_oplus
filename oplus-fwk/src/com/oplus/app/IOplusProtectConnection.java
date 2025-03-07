package com.oplus.app;

import android.os.Binder;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Parcel;
import android.os.RemoteException;

public interface IOplusProtectConnection extends IInterface {
    String DESCRIPTOR = "com.oplus.app.IOplusProtectConnection";

    void onError(int i) throws RemoteException;
    void onSuccess() throws RemoteException;
    void onTimeout() throws RemoteException;

    abstract class Stub extends Binder implements IOplusProtectConnection {
        static final int TRANSACTION_onError = 2;
        static final int TRANSACTION_onSuccess = 1;
        static final int TRANSACTION_onTimeout = 3;

        public Stub() {
            attachInterface(this, DESCRIPTOR);
        }

        public static IOplusProtectConnection asInterface(IBinder iBinder) {
            if (iBinder == null) {
                return null;
            }
            IInterface queryLocalInterface = iBinder.queryLocalInterface(DESCRIPTOR);
            if (queryLocalInterface instanceof IOplusProtectConnection) {
                return (IOplusProtectConnection) queryLocalInterface;
            }
            return new Proxy(iBinder);
        }

        @Override
        public IBinder asBinder() {
            return this;
        }

        @Override
        public boolean onTransact(int code, Parcel data, Parcel reply, int flags) throws RemoteException {
            if (code >= TRANSACTION_onSuccess && code <= 16777215) {
                data.enforceInterface(DESCRIPTOR);
            }
            if (code == 0x5f4e5446) {
                reply.writeString(DESCRIPTOR);
                return true;
            }
            switch (code) {
                case TRANSACTION_onSuccess:
                    onSuccess();
                    return true;
                case TRANSACTION_onError:
                    int readInt = data.readInt();
                    onError(readInt);
                    return true;
                case TRANSACTION_onTimeout:
                    onTimeout();
                    return true;
                default:
                    return super.onTransact(code, data, reply, flags);
            }
        }

        private static class Proxy implements IOplusProtectConnection {
            private final IBinder mRemote;

            Proxy(IBinder remote) {
                mRemote = remote;
            }

            @Override
            public IBinder asBinder() {
                return mRemote;
            }

            @Override
            public void onSuccess() throws RemoteException {
                Parcel data = Parcel.obtain();
                try {
                    data.writeInterfaceToken(DESCRIPTOR);
                    mRemote.transact(TRANSACTION_onSuccess, data, null, IBinder.FLAG_ONEWAY);
                } finally {
                    data.recycle();
                }
            }

            @Override
            public void onError(int errorCode) throws RemoteException {
                Parcel data = Parcel.obtain();
                try {
                    data.writeInterfaceToken(DESCRIPTOR);
                    data.writeInt(errorCode);
                    mRemote.transact(TRANSACTION_onError, data, null, IBinder.FLAG_ONEWAY);
                } finally {
                    data.recycle();
                }
            }

            @Override
            public void onTimeout() throws RemoteException {
                Parcel data = Parcel.obtain();
                try {
                    data.writeInterfaceToken(DESCRIPTOR);
                    mRemote.transact(TRANSACTION_onTimeout, data, null, IBinder.FLAG_ONEWAY);
                } finally {
                    data.recycle();
                }
            }
        }
    }
}
