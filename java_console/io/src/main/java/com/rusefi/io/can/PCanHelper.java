package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.rusefi.core.OsUtil;
import com.rusefi.util.HexBinary;
import org.jetbrains.annotations.NotNull;
import peak.can.basic.*;

import static com.devexperts.logging.Logging.getLogging;
import static peak.can.basic.TPCANMessageType.PCAN_MESSAGE_STANDARD;

public class PCanHelper {
    private static Logging log = getLogging(PCanHelper.class);

    public static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;

    // [tag:pcan_500k_sjw] BTR0/BTR1: retain 500 kbit/s and 87.5% sampling,
    // but allow two time quanta for resynchronization. With one Tq, Windows
    // PCAN/GPEC2A bulk reads can deliver repeated CAN frames before completing.
    private static final TPCANBaudrate WINDOWS_500K = TPCANBaudrate.valueOf(0x401c);

    public static TPCANBaudrate get500kBaudrate() {
        return get500kBaudrate(OsUtil.isWindows());
    }

    static TPCANBaudrate get500kBaudrate(boolean windows) {
        // Keep the existing timing on native backends not tested with custom BTRs.
        return windows ? WINDOWS_500K : TPCANBaudrate.PCAN_BAUD_500K;
    }

    /**
     * Whether the PCANBasic native layer can be used in this process, without touching any CAN channel.
     * <p>
     * On Windows the JNI DLL links PEAK's PCANBasic.dll, so a successful load means the driver is installed.
     * On macOS the JNI bridge (misc/pcanbasic_jni_macos) dlopen()s MacCAN's libPCBUSB lazily, so the class
     * may load fine while {@code initializeAPI()} still reports that MacCAN is missing. Linux has no
     * PCANBasic binding at all (SocketCAN is used there) and the class fails to link.
     * <p>
     * Never throws: the port scanner calls this on every scan cycle.
     */
    public static boolean isNativeApiAvailable() {
        try {
            return new PCANBasic().initializeAPI();
        } catch (Throwable e) {
            // UnsatisfiedLinkError / NoClassDefFoundError when neither JNI library is on java.library.path
            log.info("PCANBasic native API unavailable: " + e);
            return false;
        }
    }

    @NotNull
    public static PCANBasic create() {
        PCANBasic can = new PCANBasic();
        can.initializeAPI();
        return can;
    }

    public static TPCANStatus init(PCANBasic can) {
        return can.Initialize(CHANNEL, get500kBaudrate(), TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
    }

    public static TPCANStatus send(PCANBasic can, int id, byte[] payLoad) {
        log.info(String.format("Sending id=%x %s", id, HexBinary.printByteArray(payLoad)));
        TPCANMsg msg = new TPCANMsg(id, PCAN_MESSAGE_STANDARD.getValue(),
                (byte) payLoad.length, payLoad);
        return can.Write(CHANNEL, msg);
    }

    public static void debugPacket(TPCANMsg rx) {
        if (log.debugEnabled())
            log.debug("Got [" + rx + "] id=" + String.format("%X", rx.getID()) + " len=" + rx.getLength() + ": " + HexBinary.printByteArray(rx.getData()));
    }
}
