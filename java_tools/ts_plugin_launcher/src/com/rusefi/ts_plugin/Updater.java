package com.rusefi.ts_plugin;

import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.shared.FileUtil;
import org.jetbrains.annotations.Nullable;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URLClassLoader;
import java.util.Date;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.ts_plugin.TsPluginLauncher.VERSION;

/**
 * Download fresh copy of {@link #PLUGIN_BODY_JAR} and launch {@link #PLUGIN_ENTRY_CLASS} via reflection.
 */
public class Updater {
    private static final String PLUGIN_ENTRY_CLASS = "com.rusefi.ts_plugin.PluginEntry";
    private static final String PLUGIN_BODY_JAR = "rusefi_plugin_body.jar";
    private static final String LOCAL_JAR_FILE_NAME = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + PLUGIN_BODY_JAR;
    private static final String TITLE = "rusEFI plugin installer " + VERSION;

    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");
    private final JLabel countDownLabel = new JLabel();
    private final AtomicInteger autoStartCounter = new AtomicInteger(4);
    private TsPluginBody instance;
    private final Timer timer = new Timer(1000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            if (autoStartCounter.decrementAndGet() == 0) {
                timer.stop();
                try {
                    if (shouldAutoStart) {
                        shouldAutoStart = false;
                        System.out.println("Auto-starting startPlugin");
                        startPlugin();
                    }
                } catch (IllegalAccessException | MalformedURLException | ClassNotFoundException | InstantiationException ex) {
                    ex.printStackTrace();
                    JOptionPane.showMessageDialog(content, "Error " + ex);
                }
            } else {
                countDownLabel.setText("Will auto-start in " + autoStartCounter + " seconds");
            }
        }
    });

    private volatile boolean shouldAutoStart = true;

    public Updater() {
        content.add(new JLabel("" + VERSION));

        content.add(new JLabel(LOGO));

        String version = null;
        File localFile = new File(LOCAL_JAR_FILE_NAME);
        if (localFile.exists()) {
            version = getVersion();
        }

        JButton download = new JButton("Update plugin");
        JButton run = createRunThisVersionButton(version);

        new Thread(new Runnable() {
            @Override
            public void run() {
                ConnectionAndMeta connectionAndMeta;
                try {
                    connectionAndMeta = new ConnectionAndMeta(PLUGIN_BODY_JAR).invoke(ConnectionAndMeta.BASE_URL_LATEST);
                } catch (Exception e) {
                    e.printStackTrace();
                    return;
                }
                System.out.println("Server has " + connectionAndMeta.getCompleteFileSize() + " from " + new Date(connectionAndMeta.getLastModified()));

                if (AutoupdateUtil.hasExistingFile(LOCAL_JAR_FILE_NAME, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
                    System.out.println("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
                    SwingUtilities.invokeLater(() -> {
                        download.setText("We have latest plugin version");
                        download.setEnabled(false);
                    });
                    return;
                }

            }
        }).start();

        download.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (run != null)
                    run.setEnabled(false);
                cancelAutoStart();
                new Thread(() -> startDownload(download)).start();
            }
        });

        content.add(download);
    }

    @Nullable
    private JButton createRunThisVersionButton(String version) {
        if (version == null)
            return null;
        JButton run = new JButton("Run Version " + version);
        run.setBackground(new Color(0x90EE90));
        run.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    cancelAutoStart();
                    System.out.println("run startPlugin");
                    startPlugin();
                } catch (IllegalAccessException | MalformedURLException | ClassNotFoundException | InstantiationException ex) {
                    run.setText(e.toString());
                }
            }
        });
        content.add(run);

        content.add(countDownLabel);
        timer.start();
        return run;
    }

    private void cancelAutoStart() {
        timer.stop();
        shouldAutoStart = false;
    }

    private String getVersion() {
        try {
            Class clazz = getPluginClass();
            Method method = clazz.getMethod(TsPluginBody.GET_VERSION);
            return (String) method.invoke(null);
        } catch (NoSuchMethodException | MalformedURLException | ClassNotFoundException | IllegalAccessException | InvocationTargetException e) {
            return null;
        }
    }

    private void startDownload(JButton download) {
        System.out.println("startDownload");
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                download.setEnabled(false);
            }
        });

        try {
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(PLUGIN_BODY_JAR).invoke(ConnectionAndMeta.BASE_URL_LATEST);

            AutoupdateUtil.downloadAutoupdateFile(LOCAL_JAR_FILE_NAME, connectionAndMeta,
                    TITLE);
            System.out.println("Downloaded, now startPlugin");
            startPlugin();

        } catch (Exception e) {
            e.printStackTrace();
            download.setEnabled(true);
        }
    }

    private void startPlugin() throws MalformedURLException, ClassNotFoundException, InstantiationException, IllegalAccessException {
        System.out.println("Starting plugin " + this);
        Class clazz = getPluginClass();
        synchronized (this) {
            if (instance != null) {
                System.out.println("Not starting second instance");
                return; // avoid having two instances running
            }
            instance = (TsPluginBody) clazz.newInstance();
        }
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                replaceWith(instance);
            }
        });
    }

    private static Class getPluginClass() throws MalformedURLException, ClassNotFoundException {
        URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(LOCAL_JAR_FILE_NAME);
        return Class.forName(PLUGIN_ENTRY_CLASS, true, jarClassLoader);
    }

    private void replaceWith(TsPluginBody instance) {
        content.removeAll();
        content.add(instance.getContent());
        AutoupdateUtil.trueLayout(content.getParent());
        Window windowAncestor = SwingUtilities.getWindowAncestor(content);
        AutoupdateUtil.pack(windowAncestor);
    }

    public JPanel getContent() {
        return content;
    }
}
