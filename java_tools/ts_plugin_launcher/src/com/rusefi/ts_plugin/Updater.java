package com.rusefi.ts_plugin;

import com.rusefi.autoupdate.AutoupdateUtil;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URLClassLoader;

import static com.rusefi.ts_plugin.TsPluginLauncher.VERSION;

public class Updater {
    private static final String PLUGIN_BODY_JAR = "rusefi_plugin_body.jar";
    private static final String TITLE = "rusEFI plugin installer " + VERSION;

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public Updater() {
        content.add(new JLabel("" + VERSION));

        String version = null;
        if (new File(PLUGIN_BODY_JAR).exists()) {
            version = getVersion();
        }

        JButton download = new JButton("Download latest");
        if (version != null) {
            JButton run = new JButton("Run " + version);
            run.addActionListener(new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    try {
                        startPlugin();
                    } catch (IllegalAccessException | MalformedURLException | ClassNotFoundException | InstantiationException ex) {
                        run.setText(e.toString());
                    }
                }
            });

            content.add(run);
        }

        download.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new Thread(() -> startDownload(download)).start();
            }
        });

        content.add(download);
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
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                download.setEnabled(false);
            }
        });

        try {
            AutoupdateUtil.ConnectionAndMeta connectionAndMeta = new AutoupdateUtil.ConnectionAndMeta(PLUGIN_BODY_JAR).invoke();

            AutoupdateUtil.downloadAutoupdateFile(PLUGIN_BODY_JAR, connectionAndMeta.getHttpConnection(), connectionAndMeta.getCompleteFileSize(),
                    TITLE);

            startPlugin();

        } catch (IOException | ClassNotFoundException | InstantiationException | IllegalAccessException e) {
            download.setEnabled(true);
        }
    }

    private void startPlugin() throws MalformedURLException, ClassNotFoundException, InstantiationException, IllegalAccessException {
        Class clazz = getPluginClass();
        TsPluginBody instance = (TsPluginBody) clazz.newInstance();
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                replaceWith(instance);
            }
        });
    }

    private static Class getPluginClass() throws MalformedURLException, ClassNotFoundException {
        URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(PLUGIN_BODY_JAR);
        return Class.forName("com.rusefi.ts_plugin.PluginEntry", true, jarClassLoader);
    }

    private void replaceWith(TsPluginBody instance) {
        content.removeAll();
        content.add(instance.getContent());
        AutoupdateUtil.trueLayout(content.getParent());
    }

    public JPanel getContent() {
        return content;
    }
}
