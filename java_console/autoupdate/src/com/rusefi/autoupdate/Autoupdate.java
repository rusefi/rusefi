package com.rusefi.autoupdate;

import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Date;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class Autoupdate {
    private static final String BUNDLE_NAME_FILE = "bundle_name.ini";
    private static final String AUTOUPDATE_MODE = "autoupdate";
    private static final int BUFFER_SIZE = 32 * 1024;

    public static void main(String[] args) {
        UpdateMode mode = getMode();
        if (mode != UpdateMode.NEVER) {
            String bundleFullName = readBundleFullName();
            if (bundleFullName != null) {
                System.out.println("Handling " + bundleFullName);
                handleBundle(bundleFullName, mode);
            }
        }

        startConsole(args);
    }

    private static void startConsole(String[] args) {
    }

    private static UpdateMode getMode() {
        String value = getConfig().getRoot().getProperty(AUTOUPDATE_MODE);
        try {
            return UpdateMode.valueOf(value);
        } catch (Throwable e) {
            return UpdateMode.ASK;
        }
    }

    private static void handleBundle(String bundleFullName, UpdateMode mode) {
        try {
            String zipFileName = bundleFullName + "_autoupdate" + ".zip";
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(zipFileName).invoke();

            if (hasExistingFile(zipFileName, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
                System.out.println("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
                return;
            }

            if (mode != UpdateMode.ALWAYS) {
                boolean doUpdate = askUserIfUpdateIsDesired();
                if (!doUpdate)
                    return;
            }

            HttpURLConnection httpConnection = connectionAndMeta.getHttpConnection();
            long completeFileSize = connectionAndMeta.getCompleteFileSize();
            long lastModified = connectionAndMeta.getLastModified();

            System.out.println(bundleFullName + " " + completeFileSize + " bytes, last modified " + new Date(lastModified));

            BufferedInputStream in = new BufferedInputStream(httpConnection.getInputStream());
            FileOutputStream fos = new FileOutputStream(zipFileName);
            BufferedOutputStream bout = new BufferedOutputStream(fos, BUFFER_SIZE);
            byte[] data = new byte[BUFFER_SIZE];
            long downloadedFileSize = 0;
            int newDataSize;

            int printedPercentage = 0;

            while ((newDataSize = in.read(data, 0, BUFFER_SIZE)) >= 0) {
                downloadedFileSize += newDataSize;

                // calculate progress
                final int currentProgress = (int) ((((double) downloadedFileSize) / ((double) completeFileSize)) * 100000d);

                int currentPercentage = (int) (100L * downloadedFileSize / completeFileSize);
                if (currentPercentage > printedPercentage + 5) {
                    System.out.println("Downloaded " + currentPercentage + "%");
                    printedPercentage = currentPercentage;
                }

//            // update progress bar
//            SwingUtilities.invokeLater(new Runnable() {
//
//                @Override
//                public void run() {
//                    jProgressBar.setValue(currentProgress);
//                }
//            });

                bout.write(data, 0, newDataSize);
            }
            bout.close();
            in.close();

            File file = new File(zipFileName);
            file.setLastModified(lastModified);
            System.out.println("Downloaded " + file.length() + " bytes");


        } catch (IOException e) {

        }
    }

    private static boolean askUserIfUpdateIsDesired() {
        AtomicBoolean doUpdate = new AtomicBoolean();
        CountDownLatch frameClosed = new CountDownLatch(1);

        FrameHelper frameHelper = new FrameHelper() {
            @Override
            protected void onWindowClosed() {
                frameClosed.countDown();
            }
        };
        JPanel choice = new JPanel(new BorderLayout());

        choice.add(new JLabel("Do you want to update bundle to latest version?"), BorderLayout.NORTH);

        JPanel middle = new JPanel(new FlowLayout());

        JButton never = new JButton("Never");
        never.setBackground(Color.red);
        never.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                getConfig().getRoot().setProperty(AUTOUPDATE_MODE, UpdateMode.NEVER.toString());
                frameHelper.getFrame().dispose();
            }
        });
        middle.add(never);

        JButton no = new JButton("No");
        no.setBackground(Color.red);
        no.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frameHelper.getFrame().dispose();
            }
        });
        middle.add(no);

        JButton once = new JButton("Once");
        once.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                doUpdate.set(true);
                frameHelper.getFrame().dispose();
            }
        });
        middle.add(once);

        JButton always = new JButton("Always");
        always.setBackground(Color.green);
        always.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                getConfig().getRoot().setProperty(AUTOUPDATE_MODE, UpdateMode.ALWAYS.toString());
                doUpdate.set(true);
                frameHelper.getFrame().dispose();
            }
        });
        middle.add(always);


        choice.add(middle, BorderLayout.CENTER);

        frameHelper.showFrame(choice, true);
        try {
            frameClosed.await();
        } catch (InterruptedException e) {
            // ignore
        }
        return doUpdate.get();
    }

    private static boolean hasExistingFile(String zipFileName, long completeFileSize, long lastModified) {
        File file = new File(zipFileName);
        return file.length() == completeFileSize && file.lastModified() == lastModified;
    }

    private static String readBundleFullName() {
        try {
            BufferedReader r = new BufferedReader(new FileReader(BUNDLE_NAME_FILE));
            String fullName = r.readLine();
            fullName = fullName.trim();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (IOException e) {
            return null;
        }
    }

    enum UpdateMode {
        ALWAYS,
        NEVER,
        ASK
    }

    private static class ConnectionAndMeta {
        private String zipFileName;
        private HttpURLConnection httpConnection;
        private long completeFileSize;
        private long lastModified;

        public ConnectionAndMeta(String zipFileName) {
            this.zipFileName = zipFileName;
        }

        public HttpURLConnection getHttpConnection() {
            return httpConnection;
        }

        public long getCompleteFileSize() {
            return completeFileSize;
        }

        public long getLastModified() {
            return lastModified;
        }

        public ConnectionAndMeta invoke() throws IOException {
            URL url = new URL("https://rusefi.com/build_server/autoupdate/" + zipFileName);
            httpConnection = (HttpURLConnection) url.openConnection();
            completeFileSize = httpConnection.getContentLength();
            lastModified = httpConnection.getLastModified();
            return this;
        }
    }
}
