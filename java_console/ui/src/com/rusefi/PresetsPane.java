package com.rusefi;

import com.rusefi.io.CommandQueue;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public class PresetsPane {
    // see rusefi_enums.h
    private static final int MRE_BOARD_TEST = 30;
    private static final int MIATA_NA6_MAP = 41;
    private static final int MIATA_NA6_VAF = 57;
    private static final int MAZDA_MIATA_2003 = 47;
    private static final int TEST_V_12 = 49;
    private static final int ETB_BENCH = 58;
    private static final int MINIMAL_PINS = 99;

    private JPanel content = new JPanel(new GridLayout(2, 4));

    public PresetsPane() {
        content.add(new SetEngineTypeCommandControl("Miata NA6 Stage 0", "engines/miata_na.png", MIATA_NA6_VAF).getContent());
        content.add(new SetEngineTypeCommandControl("Miata NA6 Stage 1", "engines/miata_na.png", MIATA_NA6_MAP).getContent());
        content.add(new SetEngineTypeCommandControl("Miata NB2", "engines/miata_nb.png", MAZDA_MIATA_2003).getContent());
        content.add(new SetEngineTypeCommandControl("Frankenso V12", "engines/v12", TEST_V_12).getContent());
        content.add(new SetEngineTypeCommandControl("ETB bench", "", ETB_BENCH).getContent());
        content.add(new SetEngineTypeCommandControl("Minimal", "", MINIMAL_PINS).getContent());
        content.add(new SetEngineTypeCommandControl("MRE test", "", MRE_BOARD_TEST).getContent());
    }

    public JPanel getContent() {
        return content;
    }

    private class SetEngineTypeCommandControl extends FixedCommandControl {
        private final String labelTest;

        public SetEngineTypeCommandControl(String labelTest, String imageFileName, int engineType) {
            super(labelTest, imageFileName, CommandControl.SET, "set engine_type " + engineType);
            this.labelTest = labelTest;
        }

        @NotNull
        @Override
        protected ActionListener createButtonListener() {
            return e -> {
                int dialogResult = JOptionPane.showConfirmDialog(panel, "Do you really want to reset all settings to " + labelTest,
                        "Warning", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;

                CommandQueue.getInstance().write(getCommand());
            };
        }
    }
}
