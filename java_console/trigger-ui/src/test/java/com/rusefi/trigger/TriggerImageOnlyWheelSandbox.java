package com.rusefi.trigger;

import com.rusefi.enums.trigger_type_e;

import javax.swing.*;

/**
 * Handy manual sandbox which renders ONLY the round part (the trigger wheel) of the trigger image
 * for {@link trigger_type_e#TT_VVT_BOSCH_QUICK_START} using triggers.txt data.
 * <p>
 * This is a stripped-down counterpart of {@link TriggerImage} - it skips the up/down waveform image,
 * the markdown generation and PNG export, showing just the round wheel so one can iterate on the
 * wheel rendering logic in isolation. The actual wheel rendering is reused from
 * {@link TriggerImage#createWheelPanel} to avoid code duplication.
 * <p>
 * Run {@link #main(String[])} to pop up a window with the Bosch Quick Start wheel.
 */
public class TriggerImageOnlyWheelSandbox {
    private static final trigger_type_e TARGET = trigger_type_e.TT_VVT_BOSCH_QUICK_START;

    public static void main(String[] args) {
        String workingFolder = args.length > 0 ? args[0] : TriggerWheelInfo.DEFAULT_WORK_FOLDER;

        SwingUtilities.invokeLater(() -> TriggerWheelInfo.readWheels(workingFolder, wheelInfo -> {
            if (TriggerImage.findByOrdinal(wheelInfo.getId()) != TARGET)
                return;
            showWheel(wheelInfo);
        }));
    }

    private static void showWheel(TriggerWheelInfo wheelInfo) {
        JFrame frame = new JFrame("Bosch Quick Start trigger wheel");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(TriggerImage.createWheelPanel(wheelInfo.getFirstWheeTriggerSignals(), true, wheelInfo));
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
