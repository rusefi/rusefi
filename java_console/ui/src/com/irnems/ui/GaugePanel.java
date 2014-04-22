package com.irnems.ui;

import com.irnems.core.Sensor;
import com.irnems.core.SensorCentral;
import com.irnems.ui.widgets.MafCommand;
import com.irnems.ui.widgets.PotCommand;
import com.irnems.ui.widgets.RpmCommand;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.ColorDef;

import javax.swing.*;
import java.awt.*;

/**
 * Date: 2/5/13
 * (c) Andrey Belomutskiy
 */
public class GaugePanel extends JComponent {
    private static final int ADC_MAX_VALUE = 255; // mazda ECU
//    private static final int ADC_MAX_VALUE = 4095; // discovery board

    public GaugePanel() {
        setLayout(new GridLayout(1, 3));


//        Radial radial2 = createRadial("title");

        JPanel box2 = new JPanel(new GridLayout(3, 5));


        box2.add(createControls());
        box2.add(createGauge(Sensor.T_CHARGE));
        box2.add(createGauge(Sensor.DWELL1));
        box2.add(createGauge(Sensor.DWELL0));
        box2.add(createGauge(Sensor.DUTY0));
        box2.add(createGauge(Sensor.ADVANCE0));
        box2.add(createGauge(Sensor.MAF));
        box2.add(createGauge(Sensor.FUEL));
        box2.add(createGauge(Sensor.FUEL_BASE));
        box2.add(createGauge(Sensor.FUEL_CLT));
        box2.add(createGauge(Sensor.FUEL_IAT));
        box2.add(createGauge(Sensor.FUEL_LAG));

//        box2.add(createGauge(Sensor.TABLE_SPARK));


//        box2.add(createGauge(Sensor.DUTY1));
//        box2.add(createGauge(Sensor.ADVANCE1));
        box2.add(createGauge(Sensor.INTAKE_AIR));
        //box2.add(createGauge(Sensor.INTAKE_AIR_WIDTH));
        box2.add(createGauge(Sensor.COOLANT));
//        box2.add(createGauge(Sensor.COOLANT_WIDTH));

        box2.add(createGauge(Sensor.MAP));
        box2.add(createGauge(Sensor.MAP_RAW));
        box2.add(createGauge(Sensor.THROTTLE));
//        box2.add(createGauge(Sensor.VREF, PotCommand.VOLTAGE_CORRECTION));
//        box2.add(createGauge(Sensor.VREF_WIDTH));

//        box2.add(createGauge(Sensor.ADC_FAST));
//        box2.add(createGauge(Sensor.ADC_FAST_AVG));


        box2.add(createGauge(Sensor.AFR));
        box2.add(createGauge(Sensor.DEFAULT_FUEL));

        box2.add(createGauge(Sensor.TIMING));

        box2.add(createRpmGauge());

        //add(rpmGauge);
        add(box2);
//        add(new JLabel("fd"), BorderLayout.EAST);
    }

    private Component createControls() {
        JPanel controls = new JPanel(new GridLayout(2, 1));
        controls.add(new RpmCommand());
        controls.add(new MafCommand());
        return controls;
    }

    private Radial createRpmGauge() {
        final Radial rpmGauge = createRadial("RPM", "", 8000, 0);
        RpmModel.getInstance().addListener(new RpmModel.RpmListener() {
            public void onRpmChange(RpmModel rpm) {
                rpmGauge.setValue(rpm.getValue());
            }
        });
        rpmGauge.setMaxMeasuredValueVisible(true);
        return rpmGauge;
    }

    public static Component createGauge(final Sensor sensor) {
        return createGauge(sensor, 1);
    }

    public static Component createGauge(final Sensor sensor, final double correction) {
        final Radial gauge = createRadial(sensor.getName(), sensor.getUnits(), sensor.getMaxValue(), sensor.getMinValue());

        gauge.setBackgroundColor(sensor.getColor());

        SensorCentral.getInstance().addListener(sensor, new SensorCentral.AdcListener() {
            public void onAdcUpdate(SensorCentral model, double value) {
                gauge.setValue(value * correction);
            }
        });
        gauge.setLcdDecimals(2);
        return gauge;
    }

    private static Radial createRadial(String title, String units, double maxValue, double minValue) {
//        final Section[] SECTIONS =
//                {
//                        new Section(0, to, Color.red)
//                };

        Radial radial1 = new Radial();
//        radial1.setSections(SECTIONS);
        radial1.setTitle(title);
        radial1.setUnitString(units);

        //radial1.setTrackStop(to);

        radial1.setMinValue(minValue);
        radial1.setMaxValue(maxValue);
        radial1.setThresholdVisible(false);
        radial1.setPointerColor(ColorDef.RED);

        radial1.setValue(0);
        return radial1;
    }
}
