package com.rusefi.ui;

import com.rusefi.config.Fields;
import com.rusefi.ui.config.BitConfigField;
import com.rusefi.ui.config.ConfigField;
import com.rusefi.ui.config.EnumConfigField;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class SettingsTab {
    private final JPanel panel = new JPanel(new GridLayout(6, 3));
    private final JPanel content = new JPanel(new BorderLayout());

    public SettingsTab() {
        UiUtils.showLoadingMessage(content);
   }

    public Component createPane() {
        return content;
    }

    public void showContent() {
        content.removeAll();

        panel.add(UiUtils.wrap(new EnumConfigField(Fields.ALGORITHM, "algorithm").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGER_TYPE, "trigger type").getContent()));
        panel.add(new BitConfigField(Fields.USEONLYFRONTFORTRIGGER, "Only Front").getContent());
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS1, "trigger #1 input").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS2, "trigger #2 input").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS3, "trigger #3 input").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.INJECTIONPINS1, "injector #1").getContent()));

        panel.add(UiUtils.wrap(new BitConfigField(Fields.ISSDCARDENABLED, "SD card enabled").getContent()));

        panel.add(UiUtils.wrap(new EnumConfigField(Fields.HIP9011INTHOLDPIN, "int/hold pin").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.HIP9011INTHOLDPINMODE, "int/hold pin mode").getContent()));


        panel.add(UiUtils.wrap(new EnumConfigField(Fields.AFR_HWCHANNEL, "AFR channel").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.MAFADCCHANNEL, "MAF channel").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.PEDALPOSITIONCHANNEL, "Pedal channel").getContent()));

        panel.add(UiUtils.wrap(new BitConfigField(Fields.ISCANENABLED, "CAN enabled").getContent()));
        panel.add(UiUtils.wrap(new BitConfigField(Fields.CANREADENABLED, "CAN read").getContent()));
        panel.add(UiUtils.wrap(new BitConfigField(Fields.CANWRITEENABLED, "CAN write").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.CANTXPIN, "CAN TX").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.CANRXPIN, "CAN RX").getContent()));


        panel.add(UiUtils.wrap(new ConfigField(Fields.GLOBALFUELCORRECTION, "fuel corr").getContent()));
        panel.add(UiUtils.wrap(new ConfigField(Fields.GLOBALTRIGGERANGLEOFFSET, "trig offset").getContent()));
        panel.add(UiUtils.wrap(new ConfigField(Fields.CRANKING_BASEFUEL, "Cranking Fuel").getContent()));
        panel.add(UiUtils.wrap(new ConfigField(Fields.IGNITIONOFFSET, "Ignition offset").getContent()));
        panel.add(UiUtils.wrap(new ConfigField(Fields.CRANKINGTIMINGANGLE, "Cranking timing").getContent()));

        panel.add(UiUtils.wrap(RecentCommands.createButton(new AtomicBoolean(), "writeconfig")));

        content.add(panel);
    }
}