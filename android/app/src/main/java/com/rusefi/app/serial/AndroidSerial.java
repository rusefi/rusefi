package com.rusefi.app.serial;

import android.annotation.SuppressLint;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.widget.TextView;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.opensr5.io.DataListener;
import com.rusefi.app.PermissionGrantedAction;
import com.rusefi.app.rusEFI;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.dfu.DfuLogic;
import com.rusefi.io.ByteReader;
import com.rusefi.io.serial.AbstractIoStream;

import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.Date;
import java.util.List;

public class AndroidSerial extends AbstractIoStream {
    private static final int ST_CDC = 0x5740;
    private final IncomingDataBuffer dataBuffer;

    private UsbSerialPort usbSerialPort;

    public static List<UsbSerialDriver> findUsbSerial(UsbManager usbManager) {
        ProbeTable customTable = UsbSerialProber.getDefaultProbeTable();
        customTable.addProduct(DfuLogic.ST_VENDOR, ST_CDC, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);

        return prober.findAllDrivers(usbManager);
    }

    public AndroidSerial(UsbSerialPort usbSerialPort) {
        this.usbSerialPort = usbSerialPort;
        dataBuffer = createDataBuffer("");
    }

    private static UsbSerialDriver getSerialDriver(rusEFI rusEFI, TextView mStatusView, UsbManager usbManager, PermissionGrantedAction action) {
        List<UsbSerialDriver> availableDrivers = findUsbSerial(usbManager);
        if (availableDrivers.isEmpty()) {
            mStatusView.setText("Serial not found");
            rusEFI.visibleLogAppend("No serial devices " + new Date() + "\n");
            return null;
        }
        mStatusView.setText("rusEFI: " + availableDrivers.size() + " device(s)");

        UsbSerialDriver driver = availableDrivers.get(0);

        UsbDevice usbDevice = driver.getDevice();
        if (!usbManager.hasPermission(usbDevice)) {
            mStatusView.setText("Need permission");
            rusEFI.requestUsbPermission(usbDevice, action);
            return null;
        }
        return driver;
    }

    @SuppressLint("SetTextI18n")
    @Nullable
    public static AndroidSerial getAndroidSerial(rusEFI rusEFI, TextView mStatusView, UsbManager usbManager) {
        // todo: should support separate actions not default to dashboard!
        UsbSerialDriver driver = getSerialDriver(rusEFI, mStatusView, usbManager, PermissionGrantedAction.DASHBOARD);
        if (driver == null) {
            // error already reported to UI or permission request was fired
            return null;
        }
        UsbDeviceConnection connection = usbManager.openDevice(driver.getDevice());
        if (connection == null) {
            // add UsbManager.requestPermission(driver.getDevice(), ..) handling here
            mStatusView.setText("Unable to open serial");
            return null;
        }

        UsbSerialPort port = driver.getPorts().get(0); // Most devices have just one port (port 0)
        try {
            port.open(connection);
            port.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
        } catch (IOException e) {
            mStatusView.append("Error opening " + e);
            return null;
        }

        return new AndroidSerial(port);
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void setInputListener(DataListener listener) {
        ByteReader reader = buffer -> usbSerialPort.read(buffer, 5000);
        ByteReader.runReaderLoop("", listener, reader, this);
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        usbSerialPort.write(bytes, 1000);
    }
}
