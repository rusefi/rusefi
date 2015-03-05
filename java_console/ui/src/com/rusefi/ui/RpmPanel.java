package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.widgets.*;
import com.rusefi.ui.widgets.SensorGauge;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;

/**
 * Date: 1/7/13
 * (c) Andrey Belomutskiy
 */
public class RpmPanel {
    private final Node config;
    private RpmLabel rpmLabel = new RpmLabel();
    // that's for CKP signal emulation
    public final WaveInfoPanel wave0 = new WaveInfoPanel(0);
    public final WaveInfoPanel wave1 = new WaveInfoPanel(1);
    public final WaveInfoPanel wave2 = new WaveInfoPanel(2);

    public RpmPanel(Node config) {
        this.config = config;
        rpmLabel.setSize(15);
    }

    public JComponent createRpmPanel() {
        JPanel controls = createControls();

        JPanel gauges = new JPanel(new GridLayout(2, 3));
        gauges.setBorder(BorderFactory.createLineBorder(Color.black));
        gauges.add(SensorGauge.createGauge(Sensor.CLT));
        gauges.add(SensorGauge.createGauge(Sensor.FUEL));
        gauges.add(SensorGauge.createGauge(Sensor.TIMING));

        gauges.add(SensorGauge.createGauge(Sensor.VBATT));
        gauges.add(SensorGauge.createGauge(Sensor.MAF));
        gauges.add(SensorGauge.createGauge(Sensor.TPS));

        ConnectionWatchdog.start();

        JPanel smallMessagePanel = new JPanel(new BorderLayout());
        MessagesPanel mp = new MessagesPanel(config);
        smallMessagePanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        smallMessagePanel.add(BorderLayout.CENTER, mp.getMessagesScroll());

        JPanel msgPanel = new JPanel(new BorderLayout());
        msgPanel.add(new AnyCommand(config).getContent(), BorderLayout.NORTH);
        msgPanel.add(smallMessagePanel, BorderLayout.CENTER);

        JComponent rpmPanel = new JPanel(new BorderLayout());
        rpmPanel.setBorder(BorderFactory.createLineBorder(Color.white));

        rpmPanel.add(rpmLabel.getContent(), BorderLayout.NORTH);
        rpmPanel.add(controls, BorderLayout.WEST);
        rpmPanel.add(gauges, BorderLayout.CENTER);
        rpmPanel.add(msgPanel, BorderLayout.EAST);
        rpmPanel.add(new WarningPanel().getPanel(), BorderLayout.SOUTH);

        return rpmPanel;
    }

    private JPanel createControls() {
        JPanel controls = new JPanel(new MigLayout());
        controls.setBorder(BorderFactory.createLineBorder(Color.red));
        controls.add(new RpmCommand(), "grow, wrap");
//        controls.add(new PotCommand(0).panel, "grow, wrap");
//        controls.add(new PotCommand(1).panel, "grow, wrap");

        controls.add(wave0.getControl(), "grow, wrap");
        controls.add(wave1.getControl(), "grow, wrap");
        controls.add(wave2.getControl(), "grow, wrap");


        controls.add(new AdcDebugControl().getControl(), "grow, wrap");

//        controls.add(new InjectorControl(0, Sensor.INJECTOR_0_STATUS).getControl(), "grow, wrap");
//        controls.add(new InjectorControl(1, Sensor.INJECTOR_1_STATUS).getControl(), "grow, wrap");
//        controls.add(new InjectorControl(2, Sensor.INJECTOR_2_STATUS).getControl(), "grow, wrap");
//        controls.add(new InjectorControl(3, Sensor.INJECTOR_3_STATUS).getControl(), "grow, wrap");

        controls.add(new LogModeWidget().getPanel(), "grow, wrap");

        return controls;
    }
}

