/*
 * Copyright 2017, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.rusefi.app;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.io.IOException;
import java.util.Date;
import java.util.List;
import java.util.concurrent.Executors;

public class rusEFI extends Activity {
    private static final int VENDOR_ST = 0x0483;
    private static final int ST_CDC = 0x5740;

    /* UI elements */
    private TextView mStatusView;
    private TextView mResultView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usb);

        mStatusView = (TextView) findViewById(R.id.text_status);
        mResultView = (TextView) findViewById(R.id.text_result);


        mStatusView.setText("Hello");

        handleButton();
    }

    private void handleButton() {
        mResultView.append("rusEFI app v0.0000001\n");
        UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);

//        listDevices(manager);

        ProbeTable customTable = new ProbeTable();
        customTable.addProduct(VENDOR_ST, ST_CDC, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);
        // todo: handle both custom ST CDC and default hard-coded USB chips

        List<UsbSerialDriver> availableDrivers = prober.findAllDrivers(manager);
        if (availableDrivers.isEmpty()) {
            mStatusView.setText("Not connected");
            mResultView.append("No devices " + new Date());
            return;
        }
        mStatusView.setText("rusEFI: " + availableDrivers.size() + " device(s)");

        UsbSerialDriver driver = availableDrivers.get(0);
        UsbDeviceConnection connection = manager.openDevice(driver.getDevice());
        if (connection == null) {
            // add UsbManager.requestPermission(driver.getDevice(), ..) handling here
            mStatusView.setText("Unable to open");
            return;
        }

        UsbSerialPort port = driver.getPorts().get(0); // Most devices have just one port (port 0)
        try {
            port.open(connection);
            port.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);

            port.write("t".getBytes(), 500);

            SerialInputOutputManager usbIoManager = new SerialInputOutputManager(port, new SerialInputOutputManager.Listener() {
                @Override
                public void onNewData(final byte[] data) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mResultView.append(new String(data));
                        }
                    });
                }

                @Override
                public void onRunError(Exception e) {
                }
            });
            Executors.newSingleThreadExecutor().submit(usbIoManager);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void listDevices(UsbManager manager) {
        for (final UsbDevice usbDevice : manager.getDeviceList().values()) {
            mResultView.append(usbDevice.getDeviceName() + " " + usbDevice.getVendorId() + " " + usbDevice.getProductId() + "\n");
        }
    }

    /**
     * Called when the user touches the button
     */
    public void sendMessage(View view) {
        handleButton();
    }

    @Override
    protected void onNewIntent(Intent intent) {
    }
}
