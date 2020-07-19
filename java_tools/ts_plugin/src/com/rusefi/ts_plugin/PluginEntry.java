package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.ts_plugin.util.ManifestHelper;
import com.rusefi.tune.xml.Constant;

import javax.swing.*;
import java.util.function.Supplier;

/**
 * TsPlugin launcher creates an instance of this class via reflection.
 */
public class PluginEntry implements TsPluginBody {
    private final JTabbedPane tabbedPane = new JTabbedPane();

    /**
     * the real constructor - this one is invoked via reflection
     */
    public PluginEntry() {
        this(ControllerAccess::getInstance);
    }

    public PluginEntry(Supplier<ControllerAccess> controllerAccessSupplier) {
        System.out.println("PluginEntry init " + this);

        UploadTab uploadTab = new UploadTab(controllerAccessSupplier);
        BroadcastTab broadcastTab = new BroadcastTab();
        RemoteTab remoteTab = new RemoteTab();

        tabbedPane.addTab("Upload", uploadTab.getContent());
        tabbedPane.addTab("Broadcast", broadcastTab.getContent());
        tabbedPane.addTab("Remote ECU", remoteTab.getContent());
    }

    public static boolean isEmpty(Constant constant) {
        if (constant == null)
            return true;
        return isEmpty(constant.getValue());
    }

    private static boolean isEmpty(String value) {
        return value == null || value.trim().length() == 0;
    }

    @Override
    public JComponent getContent() {
        return tabbedPane;
    }
/*
    public void close() {
        PersistentConfiguration.getConfig().save();
    }
*/

    /**
     * this method is invoked by refection
     *
     * @see TsPluginBody#GET_VERSION
     */
    @SuppressWarnings("unused")
    public static String getVersion() {
        return ManifestHelper.getVersion();
    }
}
