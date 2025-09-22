package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.rusefi.core.ui.AutoupdateUtil;
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
 * Launch {@link TsPluginUi#PLUGIN_UI_IMPL_CLASS} via reflection.
 */
public class Updater {
    private static final Logging log = getLogging(Updater.class);

    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");
    private final Object lock = new Object();
    private TsPluginUi pluginUiInstance;

    public Updater() {
        content.add(new JLabel(VERSION));

        content.add(new JLabel(LOGO));

        Thread t = new Thread(() -> {
            try {
                startPluginUi();
            } catch (MalformedURLException | ClassNotFoundException | InstantiationException |
                     IllegalAccessException | InterruptedException | InvocationTargetException e) {
                log.error("Error launching " + e, e);
            }
        });

        t.setName("RE Plugin Fetcher");
        t.setDaemon(true);
        t.start();
    }


    private void startPluginUi() throws MalformedURLException, ClassNotFoundException, InstantiationException, IllegalAccessException, InterruptedException, InvocationTargetException {
        log.info("Starting plugin " + this);
        Class clazz = getPluginClass(TsPluginUi.PLUGIN_UI_IMPL_CLASS);
        synchronized (lock) {
            if (pluginUiInstance != null) {
                log.info("Not starting second instance");
                return; // avoid having two instances running
            }
            SwingUtilities2.invokeAndWait(() -> {
                try {
                    pluginUiInstance = (TsPluginUi) clazz.newInstance();
                    replaceWith(pluginUiInstance);
                } catch (InstantiationException e) {
                    throw new RuntimeException(e);
                } catch (IllegalAccessException e) {
                    throw new RuntimeException(e);
                }
            });
        }
    }

    public static Class getPluginClass(String className) throws MalformedURLException, ClassNotFoundException {
        log.info("Using " + TsPluginBodyFetcher.LOCAL_JAR_FILE_NAME);
        URLClassLoader jarClassLoader = AutoupdateUtil.getClassLoaderByJar(TsPluginBodyFetcher.LOCAL_JAR_FILE_NAME);
        return Class.forName(className, true, jarClassLoader);
    }

    private void replaceWith(TsPluginUi instance) {
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
