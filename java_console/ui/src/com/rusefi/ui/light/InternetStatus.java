package com.rusefi.ui.light;

import com.rusefi.Timeouts;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.HttpURLConnection;
import java.net.URL;

public class InternetStatus {
    public static final String GOOGLE = "http://google.com";
    private final JLabel status = new JLabel();

    public InternetStatus() {
        new Thread(() -> {
            while (true) {
                boolean isConnected = isServerReachable();
                try {
                    SwingUtilities.invokeAndWait(new Runnable() {
                        @Override
                        public void run() {
                            if (isConnected) {
                                status.setText("online");
                                status.setForeground(Color.green);
                            } else {
                                status.setText("offline");
                                status.setForeground(Color.red);
                            }
                        }
                    });
                    Thread.sleep(Timeouts.SECOND);
                } catch (InterruptedException | InvocationTargetException e) {
                    // ignore
                }
            }
        }).start();
    }

    private static boolean isServerReachable() {
        try {
            URL urlServer = new URL(GOOGLE);
            HttpURLConnection urlConn = (HttpURLConnection) urlServer.openConnection();
            urlConn.setConnectTimeout(3000); //<- 3Seconds Timeout
            urlConn.connect();
            if (urlConn.getResponseCode() == 200) {
                return true;
            } else {
                return false;
            }
        } catch (IOException e1) {
            return false;
        }
    }

    public Component getContent() {
        return status;
    }
}
