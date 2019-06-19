package com.rusefi.ui.livedocs;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class ActionPanel {
    List<RefreshActions> actionsList = new ArrayList<>();
    private final JPanel panel = new JPanel(new MigLayout());
    private JPanel currentLinePanel;

    public ActionPanel(String title) {
        panel.setBorder(BorderFactory.createTitledBorder(title));
    }

    public ActionPanel(JPanel result, List<RefreshActions> combined) {
        panel.add(result);
        actionsList.addAll(combined);
    }

    public JPanel getPanel() {
        return panel;
    }

    public void newLine() {
        initIfNeeded();
        currentLinePanel = null;
    }

    private void initIfNeeded() {
        if (currentLinePanel == null) {
            currentLinePanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
            panel.add(currentLinePanel, "wrap");
        }
    }

    public void addControl(JComponent component) {
        initIfNeeded();
        currentLinePanel.add(component);
    }
}
