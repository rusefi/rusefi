package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.widgets.MafCommand;
import com.rusefi.ui.widgets.RpmCommand;
import com.rusefi.ui.widgets.SensorGauge;
import eu.hansolo.steelseries.gauges.Radial;

import javax.swing.*;
import java.awt.*;

/**
 * Date: 2/5/13
 * (c) Andrey Belomutskiy
 */
public class GaugesPanel  {
    private static final int ADC_MAX_VALUE = 255; // mazda ECU
//    private static final int ADC_MAX_VALUE = 4095; // discovery board

    private final JPanel content = new JPanel(new BorderLayout());

    public static void main(String[] args) {
        FrameHelper fh = new FrameHelper();
        fh.showFrame(new GaugesPanel().getContent());
    }

    public GaugesPanel() {
//        Radial radial2 = createRadial("title");

        JPanel box2 = new JPanel(new GridLayout(3, 5));

        box2.add(createControls());
        box2.add(createRpmGauge());
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
        //box2.add(createGauge(Sensor.FUEL_BASE));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_CLT));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_IAT));
        box2.add(SensorGauge.createGauge(Sensor.FUEL_LAG));

//        box2.add(createGauge(Sensor.TABLE_SPARK));


//        box2.add(createGauge(Sensor.DUTY1));
//        box2.add(createGauge(Sensor.ADVANCE1));
        //box2.add(createGauge(Sensor.INTAKE_AIR_WIDTH));
//        box2.add(createGauge(Sensor.COOLANT_WIDTH));

//        box2.add(createGauge(Sensor.VREF, PotCommand.VOLTAGE_CORRECTION));
//        box2.add(createGauge(Sensor.VREF_WIDTH));

//        box2.add(createGauge(Sensor.ADC_FAST));
//        box2.add(createGauge(Sensor.ADC_FAST_AVG));


        box2.add(SensorGauge.createGauge(Sensor.AFR));
        box2.add(SensorGauge.createGauge(Sensor.DEFAULT_FUEL));

        box2.add(SensorGauge.createGauge(Sensor.TIMING));


        //add(rpmGauge);
        content.add(box2, BorderLayout.CENTER);
        content.add(new WarningPanel().getPanel(), BorderLayout.SOUTH);
//        add(new JLabel("fd"), BorderLayout.EAST);
    }

    public JComponent getContent() {
        return content;
    }

    private Component createControls() {
        JPanel controls = new JPanel(new GridLayout(2, 1));
        controls.add(new RpmCommand());
        return controls;
    }

    private Radial createRpmGauge() {
        final Radial rpmGauge = SensorGauge.createRadial("RPM", "", 8000, 0);
        RpmModel.getInstance().addListener(new RpmModel.RpmListener() {
            public void onRpmChange(RpmModel rpm) {
                rpmGauge.setValue(rpm.getValue());
            }
        });
        rpmGauge.setMaxMeasuredValueVisible(true);
        return rpmGauge;
    }
}
