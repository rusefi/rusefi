package com.rusefi.ui.livedocs;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public class IfConditionPanel {
    private final List<RefreshActions> actionsList = new ArrayList<>();
    private final JPanel panel = new JPanel(new MigLayout());

    public IfConditionPanel(JPanel result, List<RefreshActions> combined) {
        // todo: do we really need this proxy panel or can we just use parameter value?
        panel.add(result);
        // todo: do we really need this list or can we just use parameter value?
        actionsList.addAll(combined);
    }

    public List<RefreshActions> getActionsList() {
        return actionsList;
    }

    public JPanel getPanel() {
        return panel;
    }
}
