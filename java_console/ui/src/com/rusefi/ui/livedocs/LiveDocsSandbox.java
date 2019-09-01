package com.rusefi.ui.livedocs;

import com.rusefi.ldmp.generated.ThermistorsMeta;
import com.rusefi.ui.livedocs.controls.Toolbox;
import com.rusefi.ui.util.FrameHelper;
import net.miginfocom.swing.MigLayout;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.config.generated.Fields.LDS_CLT_STATE_INDEX;
import static com.rusefi.config.generated.Fields.LDS_IAT_STATE_INDEX;
import static com.rusefi.ui.livedocs.controls.Toolbox.setTransparentLineBorder;

public class LiveDocsSandbox {
    public static JLabel comment = new JLabel("Prototype Comments appear here");

    public static void main(String[] args) {
        comment.setForeground(Color.blue);

        JPanel panels = new JPanel(new MigLayout("fillx, gap 0, insets 0"));
        panels.add(LiveDocPanel.createPanel("Coolant Sensor", ThermistorsMeta.CONTENT, "CLT", LDS_CLT_STATE_INDEX), "wrap");
        panels.add(LiveDocPanel.createPanel("Intake Air Sensor", ThermistorsMeta.CONTENT, "IAT", LDS_IAT_STATE_INDEX), "wrap");

        panels.add(getTChargePanel(), "wrap, grow");

        panels.add(comment);

        new FrameHelper().showFrame(panels);
    }

    private static Component getTChargePanel() {
        JPanel panel = new JPanel(new MigLayout("gap 0, insets 0"));
        panel.setBorder(BorderFactory.createTitledBorder("Air Charge Temperature"));
        panel.setBorder(BorderFactory.createLineBorder(Color.CYAN));

        JPanel topPanel = new JPanel(new MigLayout());
        setTransparentLineBorder(topPanel);
        topPanel.add(new JLabel("top"));

        JPanel bottomPanel = new JPanel(new MigLayout());
        setTransparentLineBorder(bottomPanel);
        bottomPanel.add(new JLabel("bottom"));

        addBranch(panel, true, topPanel, bottomPanel);

        return panel;
    }

    private static void addBranch(JPanel panel, boolean b, JPanel topPanel, JPanel bottomPanel) {
        //IfBranch branch;
        JPanel topWrapped = wrapWithOffset(topPanel);
        JPanel bottomWrapped = wrapWithOffset(bottomPanel);

        topPanel.setBorder(BorderFactory.createLineBorder(Color.green));
        bottomPanel.setBorder(BorderFactory.createLineBorder(Color.red));
        Toolbox.setEnabledRecursive(bottomPanel, false);


        panel.add(new JLabel("If xa"), "wrap");
        panel.add(topWrapped, "wrap");
        panel.add(new JLabel("else"), "wrap");
        panel.add(bottomWrapped, "wrap");

    }

    @NotNull
    public static JPanel wrapWithOffset(JPanel panel) {
        JPanel topWrapped = new JPanel(new FlowLayout());
        topWrapped.add(new JLabel(" "));
        topWrapped.add(panel);
        return topWrapped;
    }
}
