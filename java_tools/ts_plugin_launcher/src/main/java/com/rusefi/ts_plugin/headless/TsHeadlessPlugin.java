package com.rusefi.ts_plugin.headless;

import com.devexperts.logging.Logging;

import java.net.MalformedURLException;

import static com.rusefi.ts_plugin.Updater.getPluginClass;

public interface TsHeadlessPlugin {
    String PLUGIN_HEADLESS_IMPL_CLASS = "com.rusefi.ts_plugin.headless.TsHeadlessPluginImpl";

    Logging log = Logging.getLogging(TsHeadlessPlugin.class);

    static void start() {
        try {
            Class clazz = getPluginClass(PLUGIN_HEADLESS_IMPL_CLASS);
            TsHeadlessPlugin instance = (TsHeadlessPlugin) clazz.newInstance();
            instance.run();
        } catch (ClassNotFoundException | MalformedURLException | InstantiationException | IllegalAccessException e) {
            log.warn("Error " + e, e);
        }
    }

    void run();
}
