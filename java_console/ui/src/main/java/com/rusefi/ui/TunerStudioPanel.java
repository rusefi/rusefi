package com.rusefi.ui;

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

        add(container);
    }
}
