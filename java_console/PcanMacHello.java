import peak.can.basic.*;

/** End-to-end test through the real peak.can.basic Java classes + the fixed
 * libpcanbasic_jni.dylib: send the console ISO-TP hello on 0x710 and wait for
 * the ECU's 0x720 response. Usage:
 *   java -Djava.library.path=java_console -cp console/rusefi_console.jar:. PcanMacHello
 */
public class PcanMacHello {
    static String hex(byte[] d) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < d.length; i++) sb.append(String.format("%02X ", d[i]));
        return sb.toString();
    }

    public static void main(String[] args) throws Exception {
        PCANBasic can = new PCANBasic();
        if (!can.initializeAPI()) {
            System.out.println("initializeAPI failed");
            return;
        }
        TPCANStatus st = can.Initialize(TPCANHandle.PCAN_USBBUS1, TPCANBaudrate.PCAN_BAUD_500K,
                TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        System.out.println("Initialize: " + st);
        if (st != TPCANStatus.PCAN_ERROR_OK) return;

        TPCANMsg hello = new TPCANMsg(0x710, TPCANMessageType.PCAN_MESSAGE_STANDARD.getValue(),
                (byte) 8, new byte[]{0x07, 0x00, 0x01, 0x53, 0x20, 0x60, (byte) 0xEF, (byte) 0xC3});
        st = can.Write(TPCANHandle.PCAN_USBBUS1, hello);
        System.out.println("Write: " + st);

        long t0 = System.currentTimeMillis();
        int responses = 0, frames = 0;
        while (System.currentTimeMillis() - t0 < 3000) {
            TPCANMsg m = new TPCANMsg();
            TPCANTimestamp ts = new TPCANTimestamp();
            TPCANStatus rs = can.Read(TPCANHandle.PCAN_USBBUS1, m, ts);
            if (rs != TPCANStatus.PCAN_ERROR_OK) {
                Thread.sleep(1);
                continue;
            }
            frames++;
            if (m.getID() == 0x720) {
                responses++;
                System.out.println("GOT 720 len=" + m.getLength() + " data: " + hex(m.getData()));
            }
        }
        System.out.println("rx frames=" + frames + " 720_responses=" + responses);
        can.Uninitialize(TPCANHandle.PCAN_USBBUS1);
        System.exit(responses > 0 ? 0 : 2);
    }
}
