package com.rusefi.ui;

import com.rusefi.EcuStimulator;
import com.irnems.Launcher;
import com.irnems.core.EngineTimeListener;
import com.irnems.core.Sensor;
import com.rusefi.ui.widgets.*;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.widgets.SensorGauge;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 1/7/13
 * (c) Andrey Belomutskiy
 */
public class RpmPanel {
    private RpmControl rpmControl = new RpmControl();
    // this label displays real RPM received from ECU
    // that's for CKP signal emulation
    public final WaveInfoPanel wave0 = new WaveInfoPanel(0);
    public final WaveInfoPanel wave1 = new WaveInfoPanel(1);
    public final WaveInfoPanel wave2 = new WaveInfoPanel(2);

    public RpmPanel() {
        rpmControl.setSize(15);
    }

    private WaveInfoPanel findWavePanel(int index) {
        WaveInfoPanel wave;
        if (index == 0)
            wave = wave0;
        else if (index == 1)
            wave = wave1;
        else
            throw new IllegalStateException("unexpected index " + index);
        return wave;
    }


    public JComponent createRpmPanel() {
        JPanel controls = createControls();

        JPanel gauges = new JPanel(new GridLayout(2, 3));
        gauges.setBorder(BorderFactory.createLineBorder(Color.black));
//        gauges.add(GaugesPanel.createCoolantGauge());
        gauges.add(SensorGauge.createGauge(Sensor.DWELL0));
        gauges.add(SensorGauge.createGauge(Sensor.DUTY0));
        gauges.add(SensorGauge.createGauge(Sensor.FUEL));
        //gauges.add(GaugesPanel.createGauge(Sensor.ADVANCE0));

        gauges.add(SensorGauge.createGauge(Sensor.VREF, PotCommand.VOLTAGE_CORRECTION));
        gauges.add(SensorGauge.createGauge(Sensor.MAF));
        gauges.add(SensorGauge.createGauge(Sensor.DWELL1));
//        gauges.add(GaugesPanel.createGauge(Sensor.ADVANCE1));
//        gauges.add(GaugesPanel.createGauge(Sensor.MAF));


        final Timer reconnectTimer = new Timer(10000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                LinkManager.restart();
            }
        });
        reconnectTimer.restart();

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                /**
                 * this timer will reconnect
                 */
                postponeReconnecting(reconnectTimer);

            }
        });

        JComponent rpmPanel = new JPanel(new BorderLayout());
        rpmPanel.setBorder(BorderFactory.createLineBorder(Color.white));

        rpmPanel.add(rpmControl.getContent(), BorderLayout.NORTH);
        rpmPanel.add(controls, BorderLayout.WEST);
        rpmPanel.add(gauges, BorderLayout.CENTER);
        MsgPanel msgPanel = new MsgPanel(false);
        rpmPanel.add(msgPanel.getContent(), BorderLayout.EAST);

        return rpmPanel;
    }

    private void postponeReconnecting(Timer timer2) {
        timer2.restart();
    }

    private JPanel createControls() {
        JPanel controls = new JPanel(new MigLayout());
        controls.setBorder(BorderFactory.createLineBorder(Color.red));
        controls.add(new RpmCommand(), "grow, wrap");
//        controls.add(new PotCommand(0).panel, "grow, wrap");
//        controls.add(new PotCommand(1).panel, "grow, wrap");
        controls.add(new AnyCommand(), "grow, wrap");

        controls.add(new MafCommand(), "grow, wrap");

        controls.add(wave0.getControl(), "grow, wrap");
        controls.add(wave1.getControl(), "grow, wrap");
        controls.add(wave2.getControl(), "grow, wrap");


        controls.add(new AdcDebugControl().getControl(), "grow, wrap");

        controls.add(new InjectorControl(0, Sensor.INJECTOR_0_STATUS).getControl(), "grow, wrap");
        controls.add(new InjectorControl(1, Sensor.INJECTOR_1_STATUS).getControl(), "grow, wrap");
        controls.add(new InjectorControl(2, Sensor.INJECTOR_2_STATUS).getControl(), "grow, wrap");
        controls.add(new InjectorControl(3, Sensor.INJECTOR_3_STATUS).getControl(), "grow, wrap");

        controls.add(new LogModeWidget().getPanel(), "grow, wrap");

        return controls;
    }
}

