package com.rusefi.ui;

import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.ui.config.*;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

public class SettingsTab {
    public static final String WRITECONFIG = "writeconfig";
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel panel = new JPanel(new GridLayout(1, 3));
    private final JButton dialog = new JButton();
    private final JPanel dialogBody = new JPanel();

    public SettingsTab() {
        UiUtils.showLoadingMessage(content);
    }

    public Component createPane() {
        return content;
    }

    public void showContent() {
        final Map<String, DialogModel> dialogs = IniFileModel.getInstance().getDialogs();
        if (dialogs.isEmpty()) {
            // todo: show error label
            return;
        }

        content.removeAll();
        panel.removeAll();

        String firstDialog = new ArrayList<>(dialogs.keySet()).get(0);
        dialog.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Component c = (Component) e.getSource();

                final JPopupMenu menu = new JPopupMenu();
                menu.setLayout(new GridLayout(dialogs.keySet().size() / 2 + 1, 2));
                for (final String name : dialogs.keySet()) {
                    JMenuItem item = new JMenuItem(name);
                    item.addActionListener(new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                            selectDialog(name);
                        }
                    });
                    menu.add(item);
                }
                menu.show(c, -1, c.getHeight());
            }
        });

        selectDialog(firstDialog);

        panel.add(UiUtils.wrap(dialog));

//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.ALGORITHM, "algorithm").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGER_TYPE, "trigger type").getContent()));
//        panel.add(new BitConfigField(Fields.USEONLYFRONTFORTRIGGER, "Only Front").getContent());
//        panel.add(new BitConfigField(Fields.ISPRINTTRIGGERSYNCHDETAILS, "gap info").getContent());
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS1, "trigger #1 input").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS2, "trigger #2 input").getContent()));
////        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TRIGGERINPUTPINS3, "trigger #3 input").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.INJECTIONPINS1, "injector #1").getContent()));
//
//        panel.add(UiUtils.wrap(new BitConfigField(Fields.ISSDCARDENABLED, "SD card enabled").getContent()));
//        panel.add(UiUtils.wrap(new BitConfigField(Fields.USELCDSCREEN, "Use LCD").getContent()));
//
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.HIP9011INTHOLDPIN, "int/hold pin").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.HIP9011INTHOLDPINMODE, "int/hold pin mode").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.HIP9011GAIN, "k gain").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.KNOCKDETECTIONWINDOWSTART, "kw start").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.KNOCKDETECTIONWINDOWEND, "kw end").getContent()));
//
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.IGNITIONPINS1, "ign #1").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.IGNITIONPINS3, "ign #3").getContent()));
//
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.AFR_HWCHANNEL, "AFR channel").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.MAFADCCHANNEL, "MAF channel").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.MAP_SENSOR_HWCHANNEL, "MAP channel").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.TPSADCCHANNEL, "TPS channel").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.PEDALPOSITIONCHANNEL, "Pedal channel").getContent()));
//
//        panel.add(UiUtils.wrap(new BitConfigField(Fields.ISCANENABLED, "CAN enabled").getContent()));
//        panel.add(UiUtils.wrap(new BitConfigField(Fields.CANREADENABLED, "CAN read").getContent()));
//        panel.add(UiUtils.wrap(new BitConfigField(Fields.CANWRITEENABLED, "CAN write").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.CANTXPIN, "CAN TX").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.CANRXPIN, "CAN RX").getContent()));
//
//
//        panel.add(UiUtils.wrap(new ConfigField(Fields.GLOBALFUELCORRECTION, "fuel corr").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.GLOBALTRIGGERANGLEOFFSET, "trig offset").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.CRANKING_BASEFUEL, "Cranking Fuel").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.IGNITIONOFFSET, "Ignition offset").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.CRANKINGTIMINGANGLE, "Cranking timing").getContent()));
//
//        panel.add(UiUtils.wrap(new ConfigField(Fields.ADDEDTOWALLCOEF, "Added to wall").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.SUCKEDOFFCOEF, "Sucked off wall").getContent()));
////        panel.add(UiUtils.wrap(new ConfigField(Fields.TPSACCELENRICHMENTTHRESHOLD, "TPS accel threshold").getContent()));
//        panel.add(UiUtils.wrap(new ConfigField(Fields.TPSACCELENRICHMENTMULTIPLIER, "TPS accel coef").getContent()));

        panel.add(dialogBody);

        panel.add(UiUtils.wrap(RecentCommands.createButton(new AtomicBoolean(), WRITECONFIG)));

        content.add(panel);
        UiUtils.trueLayout(content);
    }

    private void selectDialog(String name) {
        dialog.setText(name);
        dialogBody.removeAll();

        DialogModel m = IniFileModel.getInstance().getDialogs().get(name);

        dialogBody.setLayout(new GridLayout(m.getFields().size(), 1));

        for (DialogModel.Field f : m.getFields()) {
            if (f.getKey() == null)
                continue;

            Field field = Field.VALUES.get(f.getKey().toUpperCase());
            if (field == null)
                throw new NullPointerException("No field for " + f.getKey());
//            System.out.println("Located " + field);

            JComponent control;
            if (field.getType() == FieldType.BIT) {
                control = new BitConfigField(field, f.getUiName()).getContent();
            } else if (field.getOptions() != null) {
                control = new EnumConfigField(field, f.getUiName()).getContent();
            } else {
                control = new ConfigField(field, f.getUiName()).getContent();
            }

            dialogBody.add(control);
        }

        UiUtils.trueLayout(dialogBody);
    }
}