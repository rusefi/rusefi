package com.rusefi.app;

import android.hardware.usb.UsbManager;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.rusefi.dfu.DfuLogic;

import java.util.List;

public class AndroidSerial {
    private static final int ST_CDC = 0x5740;

    static List<UsbSerialDriver> findUsbSerial(UsbManager usbManager) {
        ProbeTable customTable = UsbSerialProber.getDefaultProbeTable();
        customTable.addProduct(DfuLogic.ST_VENDOR, ST_CDC, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);

        return prober.findAllDrivers(usbManager);
    }
}
