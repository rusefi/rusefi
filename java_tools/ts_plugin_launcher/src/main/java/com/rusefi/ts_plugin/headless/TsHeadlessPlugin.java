package com.rusefi.ts_plugin.headless;

import com.devexperts.logging.Logging;

import java.net.MalformedURLException;

import static com.rusefi.ts_plugin.Updater.getPluginClass;

public interface TsHeadlessPlugin {
    String PLUGIN_HEADLESS_IMPL_CLASS = "com.rusefi.ts_plugin.headless.TsHeadlessPluginImpl";

    Logging log = Logging.getLogging(TsHeadlessPlugin.class);

    class Holder {
        private static volatile TsHeadlessPlugin INSTANCE;
    }

    static TsHeadlessPlugin getOrCreateInstance() {
        TsHeadlessPlugin local = Holder.INSTANCE;
        if (local != null) {
            return local;
        }
        synchronized (Holder.class) {
            if (Holder.INSTANCE == null) {
                try {
                    Class clazz = getPluginClass(PLUGIN_HEADLESS_IMPL_CLASS);
                    if (clazz == null) {
                        return null;
                    }
                    Holder.INSTANCE = (TsHeadlessPlugin) clazz.newInstance();
                    log.info("Created TsHeadlessPluginImpl instance");
                } catch (Exception e) {
                    log.warn("Error creating TsHeadlessPluginImpl instance: " + e, e);
                    return null;
                }
            }
            return Holder.INSTANCE;
        }
    }

    static void start() {
        try {
            TsHeadlessPlugin instance = getOrCreateInstance();
            instance.run();
        } catch (Exception e) {
            log.warn("Error " + e, e);
        }
    }

    static boolean displayPlugin(String signature) {
        try {
            TsHeadlessPlugin instance = getOrCreateInstance();
            if (instance == null) {
                log.error("Unexpected null instance of TsHeadlessPlugin");
                return false;
            }
            return instance.getDisplayPlugin(signature);
        } catch (Exception e) {
            log.warn("Error " + e, e);
            return false;
        }
    }

    void run();
    boolean getDisplayPlugin(String serialSignature);
}
