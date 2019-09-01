package com.rusefi.ui.livedocs;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Multi-line horizontal flow panel.
 * <p>
 * <p>
 * (c) Andrey Belomutskiy 2013-2019
 */
public class ActionPanel {
    private final RefreshActionsMap refreshActions = new RefreshActionsMap();
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

    public RefreshActionsMap getRefreshActions() {
        return refreshActions;
    }

    public void actionsListAdd(LiveDataContext context, RefreshActions refreshAction) {
        refreshActions.put(context, refreshAction);
    }

    public void actionsListAddAll(RefreshActionsMap actions) {
        refreshActions.addAll(actions);
    }
}
