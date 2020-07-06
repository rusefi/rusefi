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
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.opensr5.Logger;
import com.rusefi.Listener;
import com.rusefi.dfu.DfuConnection;
import com.rusefi.dfu.DfuImage;
import com.rusefi.dfu.DfuLogic;
import com.rusefi.dfu.android.AndroidDfuConnection;
import com.rusefi.dfu.android.DfuDeviceLocator;
import com.rusefi.io.DfuHelper;
import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.shared.FileUtil;

import java.io.File;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.Date;
import java.util.List;

public class rusEFI extends Activity {
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    //public static final String FILE = "rusefi_bundle_mre_f4_autoupdate.zip";
    //private static final String DFU_FILE_NAME = "rusefi_mre_f4.dfu";
    private static final String BUNDLE_FILE = "rusefi_bundle_autoupdate.zip";
    private static final String DFU_FILE_NAME = "rusefi_.dfu";

    private static final byte REQUEST_TYPE_CLASS = 32;
    private static final byte RECIPIENT_INTERFACE = 0x01;


    protected static final int DFU_DETACH_TIMEOUT = 1000;

    /* UI elements */
    private TextView mStatusView;
    private TextView mResultView;

    private UsbManager usbManager;
    private String localDfuImageFileName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usb);

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // turn on scree while ADB debugging idle phone
        turnScreenOn();

        mStatusView = (TextView) findViewById(R.id.text_status);
        mResultView = (TextView) findViewById(R.id.text_result);

        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver, filter);

        mStatusView.setText("Hello");


        final File localFolder = getExternalFilesDir(null);
        final String localFullFile = localFolder + File.separator + BUNDLE_FILE;
        localDfuImageFileName = localFolder + File.separator + DFU_FILE_NAME;

        if (new File(localFullFile).exists()) {
            mResultView.append(BUNDLE_FILE + " found!\n");
            uncompressFile(localFullFile, localFolder, localDfuImageFileName);
        } else {
            mResultView.append(BUNDLE_FILE + " not found!\n");

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        ConnectionAndMeta c = new ConnectionAndMeta(BUNDLE_FILE).invoke();
                        ConnectionAndMeta.downloadFile(localFullFile, c, new ConnectionAndMeta.DownloadProgressListener() {
                            @Override
                            public void onPercentage(final int currentProgress) {
                                mResultView.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        mResultView.append("Downloading " + currentProgress + "\n");
                                    }
                                });
                            }
                        });
                        mResultView.post(new Runnable() {
                            @Override
                            public void run() {
                                mResultView.append("Downloaded! " + "\n");
                            }
                        });
                        uncompressFile(localFullFile, localFolder, localDfuImageFileName);

                    } catch (IOException | KeyManagementException | NoSuchAlgorithmException e) {
                        mResultView.post(new Runnable() {
                            @Override
                            public void run() {
                                mResultView.append("Error downloading " + e + "\n");
                            }
                        });
                    }

                }
            }).start();
        }

        handleButton();
    }

    private void turnScreenOn() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
            setTurnScreenOn(true);
        } else {
            Window window = getWindow();
            window.addFlags(WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON);
        }
    }

    private void uncompressFile(final String localFullFile, final File localFolder, final String localDfuImageFileName) {
        final Listener<Integer> onSuccess = new Listener<Integer>() {
            @Override
            public void onResult(final Integer size) {
                mResultView.post(new Runnable() {
                    @Override
                    public void run() {
                        mResultView.append(localDfuImageFileName + " File size: " + size + "\n");
                    }
                });
                DfuImage dfuImage = new DfuImage();
                dfuImage.read(localDfuImageFileName);
            }
        };

        new Thread(() -> {
            try {
                FileUtil.unzip(localFullFile, localFolder);
                final int size = (int) new File(localDfuImageFileName).length();
                onSuccess.onResult(size);

            } catch (final IOException e) {
                mResultView.post(() -> mResultView.append("Error uncompressing " + e + "\n"));
            }

        }).start();
    }

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        UsbDevice dfuDevice = DfuDeviceLocator.findDevice(usbManager);
                        dfuUpdate(dfuDevice);
//                        if (device != null) {
//                            //call method to set up device communication
//                        }
//                    } else {
                        //txtInfo.append("permission denied for device " + device);
                    }
                }
            }
        }
    };

    private void handleButton() {
        mResultView.append("rusEFI app v0.0000003\n");

        UsbDevice dfuDevice = DfuDeviceLocator.findDevice(usbManager);

        if (dfuDevice != null) {
            handleDfuDevice(dfuDevice);
        } else {
            mResultView.append("No DFU device\n");
            handleSerialDevice();
        }
//        listDevices(manager);
    }

    private void handleSerialDevice() {
        List<UsbSerialDriver> availableDrivers = AndroidSerial.findUsbSerial(usbManager);
        if (availableDrivers.isEmpty()) {
            mStatusView.setText("Not connected");
            mResultView.append("No serial devices " + new Date() + "\n");
            return;
        }
        mStatusView.setText("rusEFI: " + availableDrivers.size() + " device(s)");

        UsbSerialDriver driver = availableDrivers.get(0);
        UsbDeviceConnection connection = usbManager.openDevice(driver.getDevice());
        if (connection == null) {
            // add UsbManager.requestPermission(driver.getDevice(), ..) handling here
            mStatusView.setText("Unable to open serial");
            return;
        }

        UsbSerialPort port = driver.getPorts().get(0); // Most devices have just one port (port 0)
        try {
            port.open(connection);
            port.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);

            AndroidSerial serial = new AndroidSerial(port);
            mResultView.append("Switching to DFU\n");
            DfuHelper.sendDfuRebootCommand(serial, new StringBuilder(), Logger.CONSOLE);

        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private void handleDfuDevice(UsbDevice dfuDevice) {
        if (usbManager.hasPermission(dfuDevice)) {
            dfuUpdate(dfuDevice);
        } else {
            PendingIntent mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
            usbManager.requestPermission(dfuDevice, mPermissionIntent);
        }
    }

    private void dfuUpdate(UsbDevice dfuDevice) {
        mStatusView.setText("rusEFI: DFU detected");
        DfuDeviceLocator.Result dfu = new DfuDeviceLocator().openDfu(usbManager, dfuDevice);

        DfuImage dfuImage = new DfuImage();
        dfuImage.read(localDfuImageFileName);
        mResultView.append("Image size " + dfuImage.getImageSize() + "\n");

        DfuConnection connection = new AndroidDfuConnection(dfu.getConnection(), dfu.getInterfaceIndex(), dfu.getTransferSize(), dfu.getFlashRange());

        DfuLogic.Logger logger = DfuLogic.Logger.CONSOLE;
        try {
//            DfuCommandGetStatus.State state = DfuCommandGetStatus.read(c);
//            mResultView.append("State " + state + "\n");

            DfuLogic.uploadImage(logger, connection, dfuImage, dfu.getFlashRange());

        } catch (IllegalStateException e) {
            mResultView.append("Error " + e + "\n");
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
