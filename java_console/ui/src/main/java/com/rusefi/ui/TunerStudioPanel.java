package com.rusefi.ui;

import com.rusefi.StartupFrame;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.tools.TunerStudioHelper;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

public class TunerStudioPanel extends JPanel {
    public TunerStudioPanel(Runnable onDone) {
        super(new GridBagLayout());

        JPanel container = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.CENTER, 5, 5));

        JLabel label = new JLabel("<html><center>TunerStudio is running.<br/>Do you want to close it?</center></html>");
        label.setFont(label.getFont().deriveFont(label.getFont().getSize() * 2.0f));
        label.setHorizontalAlignment(SwingConstants.CENTER);
        container.add(label);

        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.CENTER, 20, 0));
        JButton yes = new JButton("Yes");
        yes.setFont(yes.getFont().deriveFont(yes.getFont().getSize() * 2.0f));
        yes.addActionListener(e -> {
            new Thread(() -> TunerStudioHelper.attemptClosingTunerStudio()).start();
            onDone.run();
        });
        JButton no = new JButton("No");
        no.setFont(no.getFont().deriveFont(no.getFont().getSize() * 2.0f));
        no.addActionListener(e -> onDone.run());
        buttons.add(yes);
        buttons.add(no);
        container.add(buttons);

        JCheckBox autoClose = new JCheckBox("Auto-close TS");
        autoClose.setSelected(PersistentConfiguration.getBoolProperty(StartupFrame.AUTO_CLOSE_TS, false));
        autoClose.addActionListener(e -> PersistentConfiguration.setBoolProperty(StartupFrame.AUTO_CLOSE_TS, autoClose.isSelected()));

        JCheckBox checkTs = new JCheckBox("Check if TS is running");
        checkTs.setSelected(PersistentConfiguration.getBoolProperty(StartupFrame.CHECK_TS_RUNNING, true));
        checkTs.addActionListener(e -> PersistentConfiguration.setBoolProperty(StartupFrame.CHECK_TS_RUNNING, checkTs.isSelected()));

        container.add(autoClose);
        container.add(checkTs);

        add(container);
    }
}
