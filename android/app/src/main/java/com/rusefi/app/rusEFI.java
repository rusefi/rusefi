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

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.util.Linkify;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.devexperts.logging.Logging;
import com.google.android.material.snackbar.BaseTransientBottomBar;
import com.google.android.material.snackbar.Snackbar;
import com.rusefi.Callable;
import com.rusefi.Timeouts;
import com.rusefi.app.serial.AndroidSerial;
import com.rusefi.app.util.AndroidUtil;
import com.rusefi.auth.AuthTokenUtil;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.dfu.DfuConnection;
import com.rusefi.dfu.DfuImage;
import com.rusefi.dfu.DfuLogic;
import com.rusefi.dfu.android.AndroidDfuConnection;
import com.rusefi.dfu.android.DfuDeviceLocator;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.DfuHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.ErrorInfoCommand;
import com.rusefi.io.serial.StreamConnector;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.proxy.NetworkConnectorContext;
import com.rusefi.ui.StatusConsumer;

import java.io.IOException;
import java.util.Date;

public class rusEFI extends Activity {
    private final static Logging log = Logging.getLogging(rusEFI.class);
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

//    private static final byte REQUEST_TYPE_CLASS = 32;
//    private static final byte RECIPIENT_INTERFACE = 0x01;
//
//    protected static final int DFU_DETACH_TIMEOUT = 1000;

    private static final String VERSION = "rusEFI app v0.20220524\n";

    private static final int LOCAL_PORT = 29001;

    /* UI elements */
    private TextView mStatusView;
    private TextView mResultView; // global dump of all messages
    private TextView broadcastStatus;
    private EditText authTokenUI;
    private TextView authStatusMessage;
    private TextView authStatusClickableUrl;

    private UsbManager usbManager;
    private DfuUpload dfuUpload;
    private SoundBroadcast soundBroadcast = new SoundBroadcast();
    private PermissionGrantedAction onPermissionGrantedAction;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        log.info("onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usb);

        /**
         * We need to make sure that WiFi is available for us, this might be related to screen on?
         */
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        findViewById(R.id.buttonSound).setVisibility(View.GONE);
        findViewById(R.id.buttonDfu).setVisibility(View.GONE);

        Button view = findViewById(R.id.localBroadcast);
        view.setText("Local broadcast on " + LOCAL_PORT);

        broadcastStatus = findViewById(R.id.broadcastStatus);
        broadcastStatus.setVisibility(View.GONE);

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // turn on screen while ADB debugging idle phone
        AndroidUtil.turnScreenOn(this);

        mStatusView = findViewById(R.id.text_status);
        mResultView = findViewById(R.id.text_result);
        authTokenUI = findViewById(R.id.authToken);
        authTokenUI.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void afterTextChanged(Editable editable) {
                String text = authTokenUI.getText().toString();
                if (AuthTokenUtil.isToken(text)) {
                    SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(rusEFI.this);
                    SharedPreferences.Editor editor = preferences.edit();
                    editor.putString(AuthTokenUtil.AUTH_TOKEN, text);
                    editor.apply();
                    authStatusMessage.setVisibility(View.GONE);
                    authStatusClickableUrl.setVisibility(View.GONE);
                }
            }
        });

        authStatusMessage = findViewById(R.id.authStatus1);
        authStatusClickableUrl = findViewById(R.id.authStatus2);
        authStatusClickableUrl.setText(AuthTokenUtil.TOKEN_PROFILE_URL);
        Linkify.addLinks(authStatusClickableUrl, Linkify.WEB_URLS);

        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver, filter);

        visibleLogAppend(VERSION);

        dfuUpload = new DfuUpload(this);

        //dfuUpload.downloadFileIfNotPresent(mResultView);

        String authToken = readPersistedAuthToken();
        authTokenUI.setText(authToken);
        int visibility = AuthTokenUtil.isToken(authToken) ? View.GONE : View.VISIBLE;
        authStatusMessage.setVisibility(visibility);
        authStatusClickableUrl.setVisibility(visibility);

//        switchOrProgramDfu();

//        SoundBroadcast.checkOrRequestPermission(this);
    }

    private String readPersistedAuthToken() {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(rusEFI.this);
        return preferences.getString(AuthTokenUtil.AUTH_TOKEN, "");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mUsbReceiver);
    }

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if (onPermissionGrantedAction == PermissionGrantedAction.DFU) {

                            UsbDevice dfuDevice = DfuDeviceLocator.findDevice(usbManager);
                            doDfuUpdate(dfuDevice);
                        } else if (onPermissionGrantedAction == PermissionGrantedAction.DASHBOARD) {
                            connectDashboard();
                        }
                    }
                }
            }
        }
    };

    private void switchOrProgramDfu() {
        UsbDevice dfuDevice = DfuDeviceLocator.findDevice(usbManager);
        if (dfuDevice == null) {
            visibleLogAppend("No DFU device\n");
            switchToDfu();
            // once device is in DFU mode we expect what exactly to happen?
        } else if (!usbManager.hasPermission(dfuDevice)) {
            requestUsbPermission(dfuDevice, PermissionGrantedAction.DFU);
        } else {
            doDfuUpdate(dfuDevice);
        }
    }

    public void visibleLogAppend(String s) {
        CharSequence current = mResultView.getText();
        mResultView.setText(s + "\n" + current);
    }

    @SuppressLint("SetTextI18n")
    private void switchToDfu() {
        AndroidSerial serial = AndroidSerial.getAndroidSerial(this, mStatusView, usbManager);
        if (serial == null) {
            // error already reported to mStatusView
            return;
        }

        visibleLogAppend("Switching to DFU\n");
        DfuHelper.sendDfuRebootCommand(serial, StatusConsumer.VOID);
    }

    public void requestUsbPermission(UsbDevice usbDevice, PermissionGrantedAction action) {
        // why do we not have similar 'request serial USB permission'?
        onPermissionGrantedAction = action;
        PendingIntent mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
        usbManager.requestPermission(usbDevice, mPermissionIntent);
    }

    @SuppressLint("SetTextI18n")
    private void doDfuUpdate(UsbDevice dfuDevice) {
        mStatusView.setText("rusEFI: DFU detected");
        DfuDeviceLocator.Result dfu = new DfuDeviceLocator().openDfu(usbManager, dfuDevice);

        DfuImage dfuImage = new DfuImage();
        dfuImage.read(dfuUpload.localDfuImageFileName);
        visibleLogAppend("Image size " + dfuImage.getImageSize() + "\n");

        DfuConnection connection = new AndroidDfuConnection(dfu.getConnection(), dfu.getInterfaceIndex(), dfu.getTransferSize(), dfu.getFlashRange());

        DfuLogic.Logger logger = DfuLogic.Logger.CONSOLE;
        try {
//            DfuCommandGetStatus.State state = DfuCommandGetStatus.read(c);
//            mResultView.append("State " + state + "\n");

            DfuLogic.uploadImage(logger, connection, dfuImage, dfu.getFlashRange());
        } catch (IllegalStateException e) {
            visibleLogAppend("Error " + e + "\n");
        }
    }

    public void onConnectButton(View view) {
        connectDashboard();
    }

    /**
     * MRE + USB cable + Android + WiFi I get about 8Hz Runtime Data Rate :( 
     */
    public void onLocalBroadcast(View view) {
        AndroidSerial serial = AndroidSerial.getAndroidSerial(this, mStatusView, usbManager);
        if (serial == null) {
            // error already reported to mStatusView
            return;
        }
        try {
            BinaryProtocolProxy.createProxy(serial, LOCAL_PORT, new BinaryProtocolProxy.ClientApplicationActivityListener() {
                @Override
                public void onActivity() {

                }
            }, StatusConsumer.ANONYMOUS);
            visibleLogAppend("Running TCP/IP proxy on " + LOCAL_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Called when the user touches a button
     */
    public void sendMessage(View view) {
        if (view.getId() == R.id.buttonDfu) {
            switchOrProgramDfu();
        } else if (view.getId() == R.id.buttonSound) {
            soundBroadcast.start();
        } else if (view.getId() == R.id.buttonBroadcast) {
            startService(new Intent(this, SerialService.class));

            AndroidSerial serial = AndroidSerial.getAndroidSerial(this, mStatusView, usbManager);
            if (serial == null) {
                // error already reported to mStatusView
                Snackbar mySnackbar = Snackbar.make(view, "No ECU detected", BaseTransientBottomBar.LENGTH_LONG);
                mySnackbar.show();
                return;
            }

            LinkManager linkManager = new LinkManager();
            linkManager.setConnector(new StreamConnector(linkManager, new Callable<IoStream>() {
                @Override
                public IoStream call() {
                    return serial;
                }
            }));
            linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(true),
                    new ConnectionStateListener() {
                        @Override
                        public void onConnectionEstablished() {
                            mResultView.post(() -> visibleLogAppend(new Date() + " On connection established\n"));

                            NetworkConnectorContext context = new NetworkConnectorContext();
                            NetworkConnector.ActivityListener oncePerSecondStatistics = new NetworkConnector.ActivityListener() {
                                long previousTime;

                                @Override
                                public void onActivity(IoStream targetEcuSocket) {
                                    long now = System.currentTimeMillis();
                                    if (now - previousTime < Timeouts.SECOND) {
                                        // only update status once per second
                                        return;
                                    }
                                    previousTime = now;
                                    broadcastStatus.post(() -> broadcastStatus.setText(targetEcuSocket.getBytesIn() + "/" + targetEcuSocket.getBytesOut()));
                                }
                            };
                            NetworkConnector.NetworkConnectorResult result = new NetworkConnector().start(NetworkConnector.Implementation.Android,
                                    readPersistedAuthToken(), context, new NetworkConnector.ReconnectListener() {
                                        @Override
                                        public void onReconnect() {
                                        }
                                    }, linkManager, oncePerSecondStatistics);

                            mResultView.post(() -> visibleLogAppend(new Date() + " Broadcast: " + result + "\n"));

                        }

                        @Override
                        public void onConnectionFailed(String errorMessage) {
                            mResultView.post(() -> visibleLogAppend("Connection failed " + errorMessage + " \n"));
                        }
                    });

            Snackbar mySnackbar = Snackbar.make(view, "Broadcasting with " + readPersistedAuthToken(), BaseTransientBottomBar.LENGTH_LONG);
            mySnackbar.show();


        }
    }

    private void connectDashboard() {
        AndroidSerial serial = AndroidSerial.getAndroidSerial(this, mStatusView, usbManager);
        if (serial == null) {
            // error already reported to mStatusView
            return;
        }
        try {
            String signature = BinaryProtocol.getSignature(serial);
            visibleLogAppend("Connected to " + signature);
            ErrorInfoCommand.send(serial);
            String configError = ErrorInfoCommand.getResponse(serial.getDataBuffer());
            if (configError != null)
                visibleLogAppend("[CRITICAL] " + configError);

        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    protected void onNewIntent(Intent intent) {
    }
}
