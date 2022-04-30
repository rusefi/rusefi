package com.rusefi.ui.light;

import com.rusefi.NamedThreadFactory;
import com.rusefi.Timeouts;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ThreadFactory;

public class InternetStatus {
    private final static ThreadFactory THREAD_FACTORY = new NamedThreadFactory("InternetStatus");
    private static final String GOOGLE = "http://google.com";
    private final JPanel panel = new JPanel();
    private final JLabel status = new JLabel();

    public InternetStatus() {
        panel.add(status);
        Font defaultFont = status.getFont();
        status.setFont(new Font(defaultFont.getName(), defaultFont.getStyle(), 2 * defaultFont.getSize()));

        THREAD_FACTORY.newThread(() -> {
            while (true) {
                boolean isConnected = isServerReachable();
                try {
                    SwingUtilities.invokeAndWait(new Runnable() {
                        @Override
                        public void run() {
                            if (isConnected) {
                                status.setText("online");
                                panel.setBackground(Color.green);
                            } else {
                                status.setText("offline");
                                panel.setBackground(Color.red);
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
            return urlConn.getResponseCode() == 200;
        } catch (IOException e1) {
            return false;
        }
    }

    public Component getContent() {
        return panel;
    }
}
