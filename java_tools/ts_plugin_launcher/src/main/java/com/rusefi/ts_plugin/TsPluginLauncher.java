package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ApplicationPlugin;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.ts_plugin.headless.TsHeadlessPlugin;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

/**
 * This class is the more permanent part of the plugin, it's responsible for refreshing and launcher PluginEntry via reflections.
 * which downloads the main more volatile UI part (PluginEntry)
 *
 * by the way TS installs stuff into %user%\.efianalytics\TunerStudio\plugins folder
 * @see PluginLauncherSandbox sandbox for this
 * @see PluginEntry
 * @see Updater
 */
public class TsPluginLauncher implements ApplicationPlugin {
    public static final int BUILD_VERSION = 7;
    static final String VERSION = "2025.alpha." + BUILD_VERSION;
    private static final Logging log = getLogging(TsPluginLauncher.class);
    private static final String HELP_URL = "https://wiki.rusefi.com/TS-Plugin";

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public TsPluginLauncher() {
        log.info("init " + this);
        Thread pluginFetchThread = new Thread(() -> {
            // first download current version of actual plugin
            TsPluginBodyFetcher.downloadLatestIfNeeded();
            // now run background logic
            TsHeadlessPlugin.start();
        }, "pluginFetchThread");
        pluginFetchThread.start();
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
        log.info("displayPlugin " + signature);
        // todo: smarter implementation one day
        TsHeadlessPlugin.displayPlugin(signature);
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
                log.info("Create Updater " + this + " " + rusEFIVersion.CONSOLE_VERSION);
                Updater updater = new Updater();
                content.add(updater.getContent());
            }
        }
        return content;
    }

    @Override
    public void close() {
        log.info("TsPluginLauncher.close " + this);
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

    @Override
    public String toString() {
        return super.toString() + " " + getVersion();
    }
}
