package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.enums.engine_type_e;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PresetsPane {
    private final UIContext uiContext;

    private final JPanel content = new JPanel(new GridLayout(4, 5));

    public PresetsPane(UIContext uiContext) {
        this.uiContext = uiContext;
        content.add(new SetEngineTypeCommandControl(uiContext,"Frankenso Miata NA6 Stage 0", "/engines/miata_na.png", engine_type_e.MIATA_NA6_VAF.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Frankenso Miata NA6 Stage 1", "/engines/miata_na.png", Fields.ET_FRANKENSO_MIATA_NA6).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Frankenso Miata NB2", "/engines/miata_nb.png", engine_type_e.MAZDA_MIATA_2003.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata NA6 MAP", "engines/miata_na.png", engine_type_e.MRE_MIATA_NA6_MAP.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata NA6 VAF", "engines/miata_na.png", engine_type_e.MRE_MIATA_NA6_VAF.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata 94 MAP", "engines/miata_na.png", engine_type_e.MRE_MIATA_94_MAP.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata NB2 MAP", "engines/miata_nb.png", engine_type_e.MRE_MIATA_NB2_MAP.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata NB2 MAF", "engines/miata_nb.png", engine_type_e.MRE_MIATA_NB2_MAF.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Proteus NB2 MAP", "engines/miata_nb.png", engine_type_e.PROTEUS_MIATA_NB2.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Hellen72 NB2 MAP", "engines/miata_nb.png", engine_type_e.HELLEN_NB2.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE Miata Defaults", "", engine_type_e.MICRO_RUS_EFI.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Frankenso V12", "/engines/v12", engine_type_e.BMW_M73_PROTEUS.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "ETB bench", "", engine_type_e.ETB_BENCH_ENGINE.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "Minimal", "", engine_type_e.MINIMAL_PINS.ordinal()).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE old test", "", Fields.ET_MRE_OLD_TEST_BOARD).getContent());
        content.add(new SetEngineTypeCommandControl(uiContext, "MRE new test", "", Fields.ET_MRE_NEW_TEST_BOARD).getContent());
    }

    public JPanel getContent() {
        return content;
    }

    private class SetEngineTypeCommandControl extends FixedCommandControl {
        private final String labelTest;

        public SetEngineTypeCommandControl(UIContext uiContext, String labelTest, String imageFileName, int engineType) {
            super(uiContext, labelTest, imageFileName, CommandControl.SET, "set " + Fields.CMD_ENGINE_TYPE + " " + engineType);
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

                uiContext.getCommandQueue().write(getCommand());
            };
        }
    }
}
