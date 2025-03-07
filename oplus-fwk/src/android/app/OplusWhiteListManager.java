package android.app;

import android.content.Context;

import java.util.ArrayList;

import com.oplus.app.IOplusProtectConnection;

public class OplusWhiteListManager {

    public OplusWhiteListManager(Context context) {}

    public ArrayList<String> getStageProtectListFromPkg(String calledPkg, int type) {
        return new ArrayList<>();
    }

    public void addStageProtectInfo(String pkg, long timeout) {}

    public void addStageProtectInfo(String pkg, String tag, long duration, IOplusProtectConnection callback) {}
 
    public void removeStageProtectInfo(String pkg) {}
}
