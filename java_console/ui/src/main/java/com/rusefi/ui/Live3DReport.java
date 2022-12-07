package com.rusefi.ui;

//import com.rusefi.EcuStimulator;
import com.rusefi.core.MessagesCentral;
import com.rusefi.models.Point3D;
import com.rusefi.models.Range;
import com.rusefi.models.XYData;
//import net.ericaro.surfaceplotter.DefaultSurfaceModel;

import javax.swing.*;
import java.awt.*;

/**
 * 7/22/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
/*
public class Live3DReport {
    public static final Range KEY_RANGE = new Range(1.5f, 4.0f);
    private final XYData primary = new XYData();
    private final XYData secondary = null;//new XYData();
    private final JPanel control;

    private static final String KEY = "map_adjusted: ";

    public Live3DReport() {
        final DefaultSurfaceModel model = ChartHelper.createDefaultSurfaceModel(primary, EcuStimulator.RPM_RANGE, KEY_RANGE, secondary);

//        primary.fill(EcuStimulator.RPM_RANGE, KEY_RANGE, 16, 1);

        control = ChartHelper.create3DControl(primary, model, "Live Data");

//        addPoint("1000 3 0.9", model);
//        addPoint("1000 320 90", model);
//        addPoint("1000 340 90", model);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (!message.startsWith(KEY))
                    return;
                message = message.substring(KEY.length());
                addPoint(message, model);
            }
        });
    }

    private void addPoint(String message, DefaultSurfaceModel model) {
        String[] v = message.split(" ");
        if (v.length != 3)
            return;

        int rpm = Integer.parseInt(v[0]);
        float key = Integer.parseInt(v[1]) / 100.0f;
        float value = Integer.parseInt(v[2]) / 100.0f;

        primary.setPoint(new Point3D(rpm, key, value));
        primary.saveToFile("_mult.csv");
        model.plot().execute();
    }

    public Component getControl() {
        return control;
    }
}
*/