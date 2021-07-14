package com.rusefi.autoupdate;

import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.shared.FileUtil;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URLClassLoader;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

public class Autoupdate {
    private static final String TITLE = "rusEFI Bundle Updater 20200607";
    private static final String BUNDLE_NAME_FILE = "../bundle_name.txt";
    private static final String AUTOUPDATE_MODE = "autoupdate";
    private static final String RUSEFI_CONSOLE_JAR = "rusefi_console.jar";
    private static final String COM_RUSEFI_LAUNCHER = "com.rusefi.Launcher";

    public static void main(String[] args) {
        String bundleFullName = readBundleFullName();

        if (args.length > 0 && args[0].equalsIgnoreCase("release")) {
            System.out.println("Release update requested");
            handleBundle(bundleFullName, UpdateMode.ALWAYS, ConnectionAndMeta.BASE_URL_RELEASE);
            return;
        }

        System.out.println("Latest update requested");
        UpdateMode mode = getMode();
        if (mode != UpdateMode.NEVER) {
            if (bundleFullName != null) {
                System.out.println("Handling " + bundleFullName);
                handleBundle(bundleFullName, mode, ConnectionAndMeta.BASE_URL_LATEST);
            }
        } else {
            System.out.println("Update mode: NEVER");
        }
        startConsole(args);
    }

    private static void startConsole(String[] args) {
        try {
            // we want to make sure that files are available to write so we use reflection to get lazy class initialization
            System.out.println("Running rusEFI console with " + Arrays.toString(args));
            // since we are overriding file we cannot just use static java classpath while launching
            URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(RUSEFI_CONSOLE_JAR);

            Class mainClass = Class.forName(COM_RUSEFI_LAUNCHER, true, jarClassLoader);
            Method mainMethod = mainClass.getMethod("main", args.getClass());
            mainMethod.invoke(null, new Object[]{args});
        } catch (ClassNotFoundException | IllegalAccessException | InvocationTargetException | NoSuchMethodException | MalformedURLException e) {
            System.out.println(e);
        }
    }

    private static UpdateMode getMode() {
        String value = PersistentConfiguration.getConfig().getRoot().getProperty(AUTOUPDATE_MODE);
        try {
            return UpdateMode.valueOf(value);
        } catch (Throwable e) {
            return UpdateMode.ASK;
        }
    }

    private static void handleBundle(String bundleFullName, UpdateMode mode, String baseUrl) {
        try {
            String zipFileName = bundleFullName + "_autoupdate" + ".zip";
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(zipFileName).invoke(baseUrl);
            System.out.println("Remote file " + zipFileName);
            System.out.println("Server has " + connectionAndMeta.getCompleteFileSize() + " from " + new Date(connectionAndMeta.getLastModified()));

            if (AutoupdateUtil.hasExistingFile(zipFileName, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
                System.out.println("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
                return;
            }

            if (mode != UpdateMode.ALWAYS) {
                boolean doUpdate = askUserIfUpdateIsDesired();
                if (!doUpdate)
                    return;
            }

            // todo: user could have waited hours to respond to question above, we probably need to re-establish connection
            long completeFileSize = connectionAndMeta.getCompleteFileSize();
            long lastModified = connectionAndMeta.getLastModified();

            System.out.println(bundleFullName + " " + completeFileSize + " bytes, last modified " + new Date(lastModified));

            AutoupdateUtil.downloadAutoupdateFile(zipFileName, connectionAndMeta, TITLE);

            File file = new File(zipFileName);
            file.setLastModified(lastModified);
            System.out.println("Downloaded " + file.length() + " bytes");

            FileUtil.unzip(zipFileName, new File(".."));
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    private static boolean askUserIfUpdateIsDesired() {
        CountDownLatch frameClosed = new CountDownLatch(1);

        if (AutoupdateUtil.runHeadless) {
            // todo: command line ask for options
            return true;
        }

        return askUserIfUpdateIsDesiredWithGUI(frameClosed);
    }

    private static boolean askUserIfUpdateIsDesiredWithGUI(CountDownLatch frameClosed) {
        AtomicBoolean doUpdate = new AtomicBoolean();

        FrameHelper frameHelper = new FrameHelper() {
            @Override
            protected void onWindowClosed() {
                frameClosed.countDown();
            }
        };
        frameHelper.getFrame().setTitle(TITLE);
        JPanel choice = new JPanel(new BorderLayout());

        choice.add(new JLabel("Do you want to update bundle to latest version?"), BorderLayout.NORTH);

        JPanel middle = new JPanel(new FlowLayout());

        JButton never = new JButton("Never");
        never.setBackground(Color.red);
        never.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                PersistentConfiguration.getConfig().getRoot().setProperty(AUTOUPDATE_MODE, UpdateMode.NEVER.toString());
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
                PersistentConfiguration.getConfig().getRoot().setProperty(AUTOUPDATE_MODE, UpdateMode.ALWAYS.toString());
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

    public static String readBundleFullName() {
        try {
            BufferedReader r = new BufferedReader(new FileReader(BUNDLE_NAME_FILE));
            String fullName = r.readLine();
            fullName = fullName.trim();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (IOException e) {
            System.err.println(new Date() + ": Error reading " + BUNDLE_NAME_FILE);
            return null;
        }
    }

    enum UpdateMode {
        ALWAYS,
        NEVER,
        ASK
    }

}
