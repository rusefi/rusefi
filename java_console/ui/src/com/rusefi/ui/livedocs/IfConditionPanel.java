package com.rusefi.ui.livedocs;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public class IfConditionPanel {
    private final RefreshActionsMap actionsList;
    private final JPanel panel = new JPanel(new MigLayout());

    public IfConditionPanel(JPanel result, RefreshActionsMap actionsList) {
        // todo: do we really need this proxy panel or can we just use parameter value?
        panel.add(result);
        this.actionsList = actionsList;
    }

    public RefreshActionsMap getActionsList() {
        return actionsList;
    }

    public JPanel getPanel() {
        return panel;
    }
}
