package android.hardware.camera2;

public interface IOplusCameraManager {

    enum Cmd {
        CMD_NONE,
        CMD_PRE_CAPTURE,
        CMD_PRE_OPEN,
        CMD_PRE_EVLIST,
        CMD_READ_MEM,
        CMD_PRE_PRO_MANUAL_WB,
        CMD_UI_EVENT,
        CMD_PRE_CLOSE
    }

}
