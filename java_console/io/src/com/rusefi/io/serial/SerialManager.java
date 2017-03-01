package com.rusefi.io.serial;

import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.LinkManager;

/**
 * 7/9/13
 * (c) Andrey Belomutskiy
 */
class SerialManager {
    public static String port;

//    private static boolean closed;

    static DataListener dataListener = new DataListener() {
        public void onDataArrived(byte freshData[]) {
            //                jTextAreaIn.append(string);
            LinkManager.engineState.processNewData(new String(freshData), LinkManager.ENCODER);
        }
    };
    public static ConnectionStateListener listener;

    /*
    static String[] findSerialPorts() {
        List<String> result = new ArrayList<String>();

        Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier portIdentifier = portEnum.nextElement();
            System.out.println(portIdentifier.getName() + " - " + getPortTypeName(portIdentifier.getPortType()));
            if (portIdentifier.getPortType() == CommPortIdentifier.PORT_SERIAL)
                result.add(portIdentifier.getName());
        }
        return result.toArray(new String[result.size()]);
    }

    static String getPortTypeName(int portType) {
        switch (portType) {
            case CommPortIdentifier.PORT_I2C:
                return "I2C";
            case CommPortIdentifier.PORT_PARALLEL:
                return "Parallel";
            case CommPortIdentifier.PORT_RAW:
                return "Raw";
            case CommPortIdentifier.PORT_RS485:
                return "RS485";
            case CommPortIdentifier.PORT_SERIAL:
                return "Serial";
            default:
                return "unknown type";
        }
    }
  */
    //    public static void close() {
//        closed = true;
//        SerialIO.getInstance().stop();
//    }
}
