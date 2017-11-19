package com.rusefi.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * @author Andrey Belomutskiy *
 * 11/19/2017
 */
public class OlderDiscoveryChecbbox {
    public static boolean olderMode;
    private final JCheckBox checkBox = new JCheckBox("Older ST-link mode");
    private Component button;

    public OlderDiscoveryChecbbox() {
        checkBox.addActionListener(e -> olderMode = checkBox.isSelected());
    }

    public Component getButton() {
        return checkBox;
    }
}
