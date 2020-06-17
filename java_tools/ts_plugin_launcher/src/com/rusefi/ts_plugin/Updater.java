package com.rusefi.ts_plugin;

import com.rusefi.autoupdate.AutoupdateUtil;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.net.URLClassLoader;

public class Updater {
    private static final String PLUGIN_BODY_JAR = "rusefi_plugin_body.jar";
    private static final String TITLE = "rusEFI plugin installer " + TsPluginLauncher.VERSION;

    private final JPanel content = new JPanel();

    public Updater() {
        JButton download = new JButton("Download");

        download.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new Thread(() -> startDownload(download)).start();


            }
        });

        content.add(download);
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

            URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(PLUGIN_BODY_JAR);

            Class clazz = Class.forName("com.rusefi.ts_plugin.PluginEntry", true, jarClassLoader);
            TsPluginBody instance = (TsPluginBody) clazz.newInstance();
            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    replaceWith(instance);
                }
            });

        } catch (IOException | ClassNotFoundException | InstantiationException | IllegalAccessException e) {
            download.setEnabled(true);

        }

    }

    private void replaceWith(TsPluginBody instance) {
        content.removeAll();
        content.add(instance.getContent());
    }

    public JPanel getContent() {
        return content;
    }
}
