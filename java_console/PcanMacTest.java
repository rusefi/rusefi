import peak.can.basic.PCANBasic;

public class PcanMacTest {
    public static void main(String[] args) {
        PCANBasic can = new PCANBasic();
        boolean ok = can.initializeAPI();
        System.out.println("initializeAPI = " + ok);
        if (ok) {
            System.out.println("status = " + can.Initialize(peak.can.basic.TPCANHandle.PCAN_USBBUS1,
                peak.can.basic.TPCANBaudrate.PCAN_BAUD_500K,
                peak.can.basic.TPCANType.PCAN_TYPE_NONE, 0, (short) 0));
        }
    }
}
