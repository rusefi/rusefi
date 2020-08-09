package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ApplicationPlugin;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

/**
 * This class is the more permanent part of the plug, it's responsible for refreshing and launcher PluginEntry via reflections.
 * which downloads the main more volatile UI part (PluginEntry)
 *
 * by the way TS installs stuff into %user%\.efianalytics\TunerStudio\plugins folder
 * @see PluginLauncherSandbox sandbox for this
 * see PluginEntry
 */
public class TsPluginLauncher implements ApplicationPlugin {
    public static final int BUILD_VERSION = 3;
    static final String VERSION = "2020.alpha." + BUILD_VERSION;
    private static final String HELP_URL = "https://github.com/rusefi/rusefi/wiki/TS-Plugin";

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public TsPluginLauncher() {
        System.out.println("init " + this);
    }

    @Override
    public String getIdName() {
        return "rusEFI_plugin";
    }

    @Override
    public int getPluginType() {
        return PERSISTENT_DIALOG_PANEL;
    }

    @Override
    public String getDisplayName() {
        return "rusEFI Plugin";
    }

    @Override
    public String getDescription() {
        return "A plugin for rusEFI integration";
    }

    @Override
    public void initialize(ControllerAccess controllerAccess) {
    }

    @Override
    public boolean displayPlugin(String signature) {
        System.out.println("displayPlugin " + signature);
        // todo: smarter implementation one day
        return true;
    }

    @Override
    public boolean isMenuEnabled() {
        return true;
    }

    @Override
    public String getAuthor() {
        return "rusEFI LLC";
    }

    @Override
    public JComponent getPluginPanel() {
        synchronized (this) {
            // lazy initialization since TunerStudio creates one instance only to get version information without any
            // intentions to display the UI
            if (content.getComponents().length == 0) {
                System.out.println("Create Updater " + this);
                Updater updater = new Updater();
                content.add(updater.getContent());
            }
        }
        return content;
    }

    @Override
    public void close() {
        System.out.println("TsPluginLauncher.close " + this);
    }

    @Override
    public String getHelpUrl() {
        return HELP_URL;
    }

    @Override
    public String getVersion() {
        return VERSION;
    }

    @Override
    public double getRequiredPluginSpec() {
        return PLUGIN_API_VERSION;
    }
}
