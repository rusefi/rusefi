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
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.opensr5.Logger;
import com.rusefi.dfu.DfuCommmand;
import com.rusefi.dfu.DfuConnection;
import com.rusefi.dfu.DfuImage;
import com.rusefi.dfu.DfuLogic;
import com.rusefi.dfu.FlashRange;
import com.rusefi.dfu.android.DfuDeviceLocator;
import com.rusefi.dfu.commands.DfuCommandGetStatus;
import com.rusefi.io.DfuHelper;
import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.shared.FileUtil;
import com.rusefi.*;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.Date;
import java.util.List;

import static android.hardware.usb.UsbConstants.USB_DIR_IN;
import static android.hardware.usb.UsbConstants.USB_DIR_OUT;

public class rusEFI extends Activity {
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    public static final String FILE = "rusefi_bundle_mre_f4_autoupdate.zip";

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


        mStatusView = (TextView) findViewById(R.id.text_status);
        mResultView = (TextView) findViewById(R.id.text_result);

        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver, filter);

        mStatusView.setText("Hello");


        final File localFolder = getExternalFilesDir(null);
        final String localFullFile = localFolder + File.separator + FILE;
        localDfuImageFileName = localFolder + File.separator + "rusefi_mre_f4.dfu";

        if (new File(localFullFile).exists()) {
            mResultView.append(FILE + " found!\n");
            uncompressFile(localFullFile, localFolder, localDfuImageFileName);
        } else {
            mResultView.append(FILE + " not found!\n");

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        ConnectionAndMeta c = new ConnectionAndMeta(FILE).invoke();
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

        DfuConnection c = new DfuConnection() {
            @Override
            public FlashRange getFlashRange() {
                return dfu.getFlashRange();
            }

            @Override
            public int getTransferSize() {
                return dfu.getTransferSize();
            }

            @Override
            public int receiveData(DfuCommmand dfuCommmand, short wValue, ByteBuffer byteBuffer) {
                return transfer(dfu.getConnection(), USB_DIR_IN, dfuCommmand.getValue(), wValue, byteBuffer);
            }

            @Override
            public int sendData(DfuCommmand dfuCommmand, short wValue, ByteBuffer byteBuffer) {
                return transfer(dfu.getConnection(), USB_DIR_OUT, dfuCommmand.getValue(), wValue, byteBuffer);
            }
        };


        DfuLogic.Logger logger = DfuLogic.Logger.CONSOLE;
        try {
//            DfuCommandGetStatus.State state = DfuCommandGetStatus.read(c);
//            mResultView.append("State " + state + "\n");

            DfuCommandGetStatus.DeviceStatus state = DfuCommandGetStatus.read(logger, c);

            if (state.getState() == DfuCommandGetStatus.State.APP_IDLE) {
                System.out.println("Wow?");


                c.sendData(DfuCommmand.DETACH, (short) DFU_DETACH_TIMEOUT, ByteBuffer.allocateDirect(0));


            }

            if (state.getState() == DfuCommandGetStatus.State.APP_IDLE) {
                System.out.println("Wow?2");


                c.sendData(DfuCommmand.DETACH, (short) DFU_DETACH_TIMEOUT, ByteBuffer.allocateDirect(0));


            }


            DfuLogic.uploadImage(logger, c, dfuImage, dfu.getFlashRange());

        } catch (IllegalStateException e) {
            mResultView.append("Error " + e + "\n");
        }
    }

    private static int transfer(UsbDeviceConnection connection, int direction, int request, short wValue, ByteBuffer byteBuffer) {
        return connection.controlTransfer(REQUEST_TYPE_CLASS | RECIPIENT_INTERFACE | direction, request,
                wValue, 0, byteBuffer.array(), byteBuffer.limit(), 500);
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
