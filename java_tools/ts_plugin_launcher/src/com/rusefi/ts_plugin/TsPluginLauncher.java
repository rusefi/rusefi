package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ApplicationPlugin;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

/**
 * by the way TS installs stuff into %user%\.efianalytics\TunerStudio\plugins folder
 */
public class TsPluginLauncher implements ApplicationPlugin {
    static final String VERSION = "alpha2_2020";
    private static final String HELP_URL = "https://github.com/rusefi/rusefi/wiki/TS-Plugin";

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public TsPluginLauncher() {
        content.add(new Updater().getContent());
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
        return content;
    }

    @Override
    public void close() {
        System.out.printf("TsPlugin#close");
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
