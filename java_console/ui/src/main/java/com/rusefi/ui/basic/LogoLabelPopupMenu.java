package com.rusefi.ui.basic;

import javax.swing.*;

public class LogoLabelPopupMenu extends JPopupMenu {
    public LogoLabelPopupMenu() {
        super();
        final JMenuItem instanceNameMenuItem = new JMenuItem("Instance name");
        instanceNameMenuItem.addActionListener(
            e -> InstanceNameEditor.INSTANCE.editInstanceName(instanceNameMenuItem)
        );
        add(instanceNameMenuItem);
    }
}
