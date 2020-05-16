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

    static DataListener dataListener = new DataListener() {
        public void onDataArrived(byte freshData[]) {
            LinkManager.engineState.processNewData(new String(freshData), LinkManager.ENCODER);
        }
    };

}
