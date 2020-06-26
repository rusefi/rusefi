package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PresetsPane {
    // see rusefi_enums.h
    private static final int TEST_V_12 = 49;
    private static final int ETB_BENCH = 58;
    private static final int MINIMAL_PINS = 99;

    private JPanel content = new JPanel(new GridLayout(2, 4));

    public PresetsPane(UIContext uiContext) {
        content.add(new SetEngineTypeCommandControl("Frankenso Miata NA6 Stage 0", "/engines/miata_na.png", Fields.ET_FRANKENSO_MIATA_NA6_VAF).getContent());
        content.add(new SetEngineTypeCommandControl("Frankenso Miata NA6 Stage 1", "/engines/miata_na.png", Fields.ET_FRANKENSO_MIATA_NA6).getContent());
        content.add(new SetEngineTypeCommandControl("Frankenso Miata NB2", "/engines/miata_nb.png", Fields.ET_FRANKENSO_MIATA_NB2).getContent());
        content.add(new SetEngineTypeCommandControl("MRE Miata NB2 MAF", "engines/miata_nb.png", Fields.ET_MRE_MIATA_NB2_MAF).getContent());
        content.add(new SetEngineTypeCommandControl("Frankenso V12", "/engines/v12", TEST_V_12).getContent());
        content.add(new SetEngineTypeCommandControl("ETB bench", "", ETB_BENCH).getContent());
        content.add(new SetEngineTypeCommandControl("Minimal", "", MINIMAL_PINS).getContent());
        content.add(new SetEngineTypeCommandControl("MRE old test", "", Fields.ET_MRE_OLD_TEST_BOARD).getContent());
        content.add(new SetEngineTypeCommandControl("MRE new test", "", Fields.ET_MRE_NEW_TEST_BOARD).getContent());
    }

    public JPanel getContent() {
        return content;
    }

    private class SetEngineTypeCommandControl extends FixedCommandControl {
        private final String labelTest;

        public SetEngineTypeCommandControl(String labelTest, String imageFileName, int engineType) {
            super(labelTest, imageFileName, CommandControl.SET, "set " + Fields.CMD_ENGINE_TYPE + " " + engineType);
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
