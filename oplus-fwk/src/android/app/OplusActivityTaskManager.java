package android.app;

public class OplusActivityTaskManager {

    public static final OplusActivityTaskManager INSTANCE = new OplusActivityTaskManager();

    public OplusActivityTaskManager() {
    }

    public static OplusActivityTaskManager getInstance() {
        return INSTANCE;
    }

    public boolean isFloatingWindowEnabled() {
        return false;
    }
}
