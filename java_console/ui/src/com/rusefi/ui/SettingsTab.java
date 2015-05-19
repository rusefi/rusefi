package com.rusefi.ui;

import com.rusefi.config.Fields;
import com.rusefi.ui.config.EnumConfigField;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class SettingsTab {
    private final JPanel panel = new JPanel(new GridLayout(3, 3));

    public SettingsTab() {
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.algorithm, "algorithm").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.triggerType, "trigger type").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.triggerInputPins1, "trigger #1 input").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.triggerInputPins2, "trigger #2 input").getContent()));
//        panel.add(UiUtils.wrap(new EnumConfigField(Fields.triggerInputPins3, "trigger #3 input").getContent()));
        panel.add(UiUtils.wrap(new EnumConfigField(Fields.injector1, "injector #1").getContent()));

        panel.add(UiUtils.wrap(RecentCommands.createButton(new AtomicBoolean(), "writeconfig")));
    }

    public Component createPane() {
        return panel;
    }
}