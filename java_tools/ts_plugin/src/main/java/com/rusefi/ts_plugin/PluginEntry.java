package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.ts_plugin.util.ManifestHelper;
import com.rusefi.tune.xml.Constant;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.Field;
import java.util.function.Supplier;

/**
 * {@link TsPluginLauncher} creates an instance of this class via reflection.
 * @see UploadTab upload tune & TODO upload logs
 * @see RemoteTab remote ECU access & control
 * @see BroadcastTab offer your ECU for remove access & control
 * @see PluginBodySandbox
 */
public class PluginEntry implements TsPluginBody {
    private final JPanel content = new JPanel(new BorderLayout());

    static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");


    /**
     * the real constructor - this one is invoked via reflection
     */
    @SuppressWarnings("unused")
    public PluginEntry() {
        this(ControllerAccess::getInstance);
    }

    public PluginEntry(Supplier<ControllerAccess> controllerAccessSupplier) {
        System.out.println("PluginEntry init " + this);

        if (isLauncherTooOld()) {
            content.add(new JLabel("Please manually install latest plugin version"));
            return;
        }

        UploadTab uploadTab = new UploadTab(controllerAccessSupplier);
        BroadcastTab broadcastTab = new BroadcastTab();
        RemoteTab remoteTab = new RemoteTab();

        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Upload", uploadTab.getContent());
        tabbedPane.addTab("Broadcast", broadcastTab.getContent());
        tabbedPane.addTab("Remote ECU", remoteTab.getContent());
        tabbedPane.addTab("Read SD Card", new SdCardReader().getContent());
        content.add(tabbedPane);
    }

    private boolean isLauncherTooOld() {
        try {
            // at some point we did not have this field so using reflection for the next couple of months
            Field field = TsPluginLauncher.class.getField("BUILD_VERSION");
            int launcherVersion = (int) field.get(null);
            System.out.println("Launcher version " + launcherVersion + " detected");
            return false;
        } catch (NoSuchFieldException | IllegalAccessException e) {
            return true;
        }
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
        return content;
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
