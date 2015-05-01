package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.core.Sensor;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.PopupMenuButton;
import com.rusefi.ui.widgets.RpmCommand;
import com.rusefi.ui.widgets.SensorGauge;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 2/5/13
 * (c) Andrey Belomutskiy
 */
public class GaugesPanel {
    private static final Sensor[] DEFAULT_LAYOUT = {
            Sensor.RPM,
            Sensor.MAF,
            Sensor.CLT,
            Sensor.IAT,
            Sensor.TPS,
            Sensor.MAP,
            Sensor.MAP_RAW,
            Sensor.T_CHARGE,
            Sensor.DWELL1,
            Sensor.DWELL0,
            Sensor.DUTY0,
            Sensor.ADVANCE0,
            Sensor.FUEL,
            Sensor.BARO,
            Sensor.FUEL_CLT,
            Sensor.FUEL_IAT,
            Sensor.FUEL_LAG,
            Sensor.AFR,
            Sensor.DEFAULT_FUEL,
            Sensor.TIMING,
            Sensor.VREF

    };

    static {
        if (DEFAULT_LAYOUT.length != SizeSelectorPanel.WIDTH * SizeSelectorPanel.HEIGHT)
            throw new IllegalStateException("Not expected " + DEFAULT_LAYOUT.length);
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel box2 = new JPanel(new GridLayout(3, 5));

    private boolean showRpmPanel = true;
    private boolean showMessagesPanel = true;
    private JPanel lowerRpmPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
    private JPanel smallMessagePanel = new JPanel(new BorderLayout());

    public GaugesPanel(Node config) {
//        Radial radial2 = createRadial("title");

        MessagesPanel mp = new MessagesPanel(config, false);
        smallMessagePanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        smallMessagePanel.add(BorderLayout.CENTER, mp.getMessagesScroll());

        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = FileLog.getDate() + "_gauges.png";

                UiUtils.saveImageWithPrompt(fileName, content, box2);
            }
        });

        JPanel rightUpperPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));


        final JPopupMenu selectorMenu = new JPopupMenu();
        selectorMenu.add(new SizeSelectorPanel(new SizeSelectorPanel.SizeSelectorListener() {
            @Override
            public void onSelected(int row, int column) {
                System.out.println("new size " + row + "/" + column);
            }
        }));

        lowerRpmPanel.add(new RpmLabel(15).getContent());

        JButton selector = new JButton("O");
        selector.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Component c = (Component) e.getSource();
                selectorMenu.show(c, -1, c.getHeight());
            }
        });
        rightUpperPanel.add(selector);

        JPopupMenu menu = new JPopupMenu();
        final JCheckBoxMenuItem showRpmItem = new JCheckBoxMenuItem("Show RPM");
        showRpmItem.setSelected(showRpmPanel);
        showRpmItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                GaugesPanel.this.showRpmPanel = showRpmItem.isSelected();
                applyShowFlags();
            }
        });
        menu.add(showRpmItem);
        JCheckBoxMenuItem showCommandsItem = new JCheckBoxMenuItem("Show Commands");
        showCommandsItem.setSelected(showMessagesPanel);
        menu.add(showCommandsItem);
        menu.add(new JPopupMenu.Separator());
        menu.add(new JPopupMenu("Reset Config"));

        JButton menuButton = new PopupMenuButton("#", menu);
        rightUpperPanel.add(menuButton);

        JPanel leftUpperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        leftUpperPanel.add(saveImageButton);
        leftUpperPanel.add(new RpmLabel(2).getContent());

        JPanel upperPanel = new JPanel(new BorderLayout());
        upperPanel.add(leftUpperPanel, BorderLayout.CENTER);
        upperPanel.add(rightUpperPanel, BorderLayout.EAST);

        box2.add(SensorGauge.createGauge(Sensor.RPM));
        box2.add(SensorGauge.createGauge(Sensor.MAF));
        box2.add(SensorGauge.createGauge(Sensor.CLT));
        box2.add(SensorGauge.createGauge(Sensor.IAT));
        box2.add(SensorGauge.createGauge(Sensor.TPS));
        box2.add(SensorGauge.createGauge(Sensor.MAP));
        box2.add(SensorGauge.createGauge(Sensor.MAP_RAW));
        box2.add(SensorGauge.createGauge(Sensor.T_CHARGE));
        box2.add(SensorGauge.createGauge(Sensor.DWELL1));
        box2.add(SensorGauge.createGauge(Sensor.DWELL0));
        box2.add(SensorGauge.createGauge(Sensor.DUTY0));
        box2.add(SensorGauge.createGauge(Sensor.ADVANCE0));
        box2.add(SensorGauge.createGauge(Sensor.FUEL));
        box2.add(SensorGauge.createGauge(Sensor.BARO));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_CLT));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_IAT));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_LAG));
        box2.add(SensorGauge.createGauge(Sensor.AFR));
        box2.add(SensorGauge.createGauge(Sensor.DEFAULT_FUEL));
        box2.add(SensorGauge.createGauge(Sensor.TIMING));

        JPanel middlePanel = new JPanel(new BorderLayout());
        middlePanel.add(box2, BorderLayout.CENTER);
        middlePanel.add(lowerRpmPanel, BorderLayout.SOUTH);

        //add(rpmGauge);
        content.add(upperPanel, BorderLayout.NORTH);
        content.add(middlePanel, BorderLayout.CENTER);
        content.add(smallMessagePanel, BorderLayout.EAST);
        content.add(new WarningPanel().getPanel(), BorderLayout.SOUTH);
//        add(new JLabel("fd"), BorderLayout.EAST);
        applyShowFlags();
    }

    private void applyShowFlags() {
        lowerRpmPanel.setVisible(showRpmPanel);

    }

    public JComponent getContent() {
        return content;
    }
}
