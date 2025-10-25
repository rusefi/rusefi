package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ts_plugin.auth.InstanceAuthContext;
import com.rusefi.ts_plugin.knock.KnockAnalyzerTab;
import com.rusefi.ts_plugin.ui.TuneUploadTab;
import com.rusefi.ts_plugin.util.ManifestHelper;
import com.rusefi.tune.xml.Constant;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.Field;
import java.util.function.Supplier;

/**
 * {@link TsPluginLauncher} creates an instance of this class via reflection.
 *
 * @see TuneUploadTab upload tune & TODO upload logs
 * @see RemoteTab remote ECU access & control
 * @see BroadcastTab offer your ECU for remove access & control
 * @see PluginBodySandbox
 */
public class TsPluginUiImpl implements TsPluginUi {
    private final JPanel content = new JPanel(new BorderLayout());

    public static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");

    /**
     * the real constructor - this one is invoked via reflection
     */
    @SuppressWarnings("unused")
    public TsPluginUiImpl() {
        this(ControllerAccess::getInstance);
    }

    // default singleton implementation is not amazing, let's have a safer option available for test code
    public TsPluginUiImpl(Supplier<ControllerAccess> controllerAccessSupplier) {
        System.out.println("PluginEntry init " + this);

        if (isLauncherTooOld()) {
            content.add(new JLabel("<html>Please manually install latest plugin version<br/>Usually we can update to latest version but this time there was a major change.<br/>" +
                    "Please use TunerStudio controls to update to plugin from recent rusEFI bundle."));
            return;
        }

//        TuneUploadTab tuneUploadTab = new TuneUploadTab(controllerAccessSupplier);
//        LogUploadSelector logUploadTab = new LogUploadSelector(controllerAccessSupplier);
        //BroadcastTab broadcastTab = new BroadcastTab();
        //RemoteTab remoteTab = new RemoteTab();
        KnockAnalyzerTab knockAnalyzerTab = new KnockAnalyzerTab(controllerAccessSupplier);

//        tabbedPane.addTab("Tune Upload", tuneUploadTab.getContent());
//        tabbedPane.addTab("Log Upload", logUploadTab.getContent());
//        tabbedPane.addTab("Broadcast", broadcastTab.getContent());
//        tabbedPane.addTab("Remote ECU", remoteTab.getContent());
        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Knock Analyzer", knockAnalyzerTab.getContent());

        content.add(tabbedPane);

        InstanceAuthContext.startup();
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
        return value == null || value.trim().isEmpty();
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
}
