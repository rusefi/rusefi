package com.rusefi.ui.livedocs;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Multi-line horizontal flow panel.
 *
 *
 * (c) Andrey Belomutskiy 2013-2019
 */
public class ActionPanel {
    List<RefreshActions> actionsList = new ArrayList<>();
    private final JPanel panel = new JPanel(new MigLayout());
    private JPanel currentLinePanel;

    public ActionPanel(String title) {
        panel.setBorder(BorderFactory.createTitledBorder(title));
    }

    public JPanel getPanel() {
        return panel;
    }

    public void newLine() {
        initIfNeeded();
        currentLinePanel = null;
    }

    public void addControl(JComponent component) {
        initIfNeeded();
        currentLinePanel.add(component);
    }

    private void initIfNeeded() {
        if (currentLinePanel == null) {
            currentLinePanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
            panel.add(currentLinePanel, "wrap");
        }
    }
}
