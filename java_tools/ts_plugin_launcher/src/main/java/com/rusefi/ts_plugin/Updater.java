package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.FileUtil;
import com.rusefi.util.SwingUtilities2;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URLClassLoader;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ts_plugin.TsPluginLauncher.VERSION;

/**
 * Download fresh copy of {@link #PLUGIN_BODY_JAR} and launch {@link #PLUGIN_ENTRY_CLASS} via reflection.
 */
public class Updater {
    private static final Logging log = getLogging(Updater.class);
    private static final String PLUGIN_ENTRY_CLASS = "com.rusefi.ts_plugin.PluginEntry";
    static final String PLUGIN_BODY_JAR = "rusefi_plugin_body.jar";
    static final String LOCAL_JAR_FILE_NAME = FileUtil.RUSEFI_SETTINGS_FOLDER + PLUGIN_BODY_JAR;

    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");
    private final Object lock = new Object();
    private TsPluginBody instance;

    public Updater() {
        content.add(new JLabel(VERSION));

        content.add(new JLabel(LOGO));

        Thread t = new Thread(() -> {
            try {
                startPlugin();
            } catch (MalformedURLException | ClassNotFoundException | InstantiationException |
                     IllegalAccessException | InterruptedException | InvocationTargetException e) {
                log.error("Error launching " + e, e);
            }
        });

        t.setName("RE Plugin Fetcher");
        t.setDaemon(true);
        t.start();
    }


    private void startPlugin() throws MalformedURLException, ClassNotFoundException, InstantiationException, IllegalAccessException, InterruptedException, InvocationTargetException {
        log.info("Starting plugin " + this);
        Class clazz = getPluginClass();
        synchronized (lock) {
            if (instance != null) {
                log.info("Not starting second instance");
                return; // avoid having two instances running
            }
            SwingUtilities2.invokeAndWait(() -> {
                try {
                    instance = (TsPluginBody) clazz.newInstance();
                    replaceWith(instance);
                } catch (InstantiationException e) {
                    throw new RuntimeException(e);
                } catch (IllegalAccessException e) {
                    throw new RuntimeException(e);
                }
            });
        }
    }

    private static Class getPluginClass() throws MalformedURLException, ClassNotFoundException {
        log.info("Using " + LOCAL_JAR_FILE_NAME);
        URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(LOCAL_JAR_FILE_NAME);
        return Class.forName(PLUGIN_ENTRY_CLASS, true, jarClassLoader);
    }

    private void replaceWith(TsPluginBody instance) {
        content.removeAll();
        content.add(instance.getContent());
        AutoupdateUtil.trueLayoutAndRepaint(content.getParent());
        Window windowAncestor = SwingUtilities.getWindowAncestor(content);
        AutoupdateUtil.pack(windowAncestor);
    }

    public JPanel getContent() {
        return content;
    }
}
