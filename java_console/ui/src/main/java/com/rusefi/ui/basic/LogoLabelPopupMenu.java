package com.rusefi.ui.basic;

import com.rusefi.NamedThreadFactory;
import com.rusefi.panama.PanamaClient;

import javax.swing.*;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class LogoLabelPopupMenu extends JPopupMenu {
    private final static Executor UPLOAD_EXECUTOR = Executors.newSingleThreadExecutor(new NamedThreadFactory("panama client"));

    public LogoLabelPopupMenu(String panamaUrl) {


        JMenuItem instanceNameMenuItem = new JMenuItem("Instance name");
        instanceNameMenuItem.addActionListener(
            e -> InstanceNameEditor.INSTANCE.editInstanceName(instanceNameMenuItem)
        );

        JMenuItem uploadAction = new JMenuItem("Upload Tune");
        uploadAction.setEnabled(InstanceNameEditor.loadInstanceName() != null);
        uploadAction.addActionListener(e -> uploadTune(panamaUrl));

        add(uploadAction);
        add(instanceNameMenuItem);
    }

    private static void uploadTune(String panamaUrl) {
        UPLOAD_EXECUTOR.execute(() -> doUploadTune(panamaUrl));
    }

    private static void doUploadTune(String panamaUrl) {
        // todo!
        // IniField mcuSerialField = PanamaHelper.getIniField(linkManager);
//        if (mcuSerialField == null) {
//           addMessage("Please update firmware to use this feature");
        // return;
//        }
// todo: grab current calibrations and save fresh MSQ
        int mcuSerial = 1231234; // todo

        PanamaClient.uploadFile(panamaUrl,
            /* todo MSQ file */ null,
            InstanceNameEditor.loadInstanceName(),
            mcuSerial
        );
    }
}
