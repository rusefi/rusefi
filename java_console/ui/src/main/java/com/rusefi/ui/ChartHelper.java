package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.models.Range;
import com.rusefi.models.XYData;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.util.JTextFieldWithWidth;
//import net.ericaro.surfaceplotter.DefaultSurfaceModel;
//import net.ericaro.surfaceplotter.JSurfacePanel;
//import net.ericaro.surfaceplotter.Mapper;
//import net.ericaro.surfaceplotter.surface.SurfaceModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static com.rusefi.models.Utils.parseIntWithReason;

/**
 * Date: 1/22/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
/*
public class ChartHelper {
    private ChartHelper() {
    }

    public static JPanel create3DControl(final XYData data, SurfaceModel surfaceModel, String title) {
        final JPanel result = new JPanel(new BorderLayout());
        result.setBorder(BorderFactory.createLineBorder(Color.red));

        final JSurfacePanel jsp = new JSurfacePanel(surfaceModel);
        jsp.setTitleText(title);
        jsp.setConfigurationVisible(true);
        jsp.getSurface().setXLabel("RPM");
        jsp.getSurface().setYLabel("MAF voltage");
        result.add(BorderLayout.CENTER, jsp);

        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = FileLog.getDate() + "_3d.png";

                UiUtils.saveImageWithPrompt(fileName, result, jsp);
            }
        });

        JPanel upperPanel = new JPanel(new FlowLayout());
        upperPanel.add(saveImageButton);
        result.add(upperPanel, BorderLayout.NORTH);

        JPanel bottomPanel = new JPanel(new FlowLayout());
        bottomPanel.add(new JLabel("RPM: "));

        final JTextField xField = new JTextFieldWithWidth("1200", 150);
        final JTextField yField = new JTextFieldWithWidth("50", 150);
        bottomPanel.add(xField);
        bottomPanel.add(yField);

        final JLabel currentValue = new JLabel();
        bottomPanel.add(currentValue);

        result.add(bottomPanel, BorderLayout.SOUTH);

        ActionListener updateValue = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int rpm = parseIntWithReason(xField.getText(), "CH xField");

                double y = Double.parseDouble(yField.getText());

                currentValue.setText("" + data.getValue(rpm, y));
            }
        };
        xField.addActionListener(updateValue);
        yField.addActionListener(updateValue);

        updateValue.actionPerformed(null);

        return result;
    }

    public static DefaultSurfaceModel createDefaultSurfaceModel(final XYData data) {
        return createDefaultSurfaceModel(data, null);
    }

    public static DefaultSurfaceModel createDefaultSurfaceModel(final XYData data, final XYData data2) {
        Range xRange = new Range((float) data.getMinXValue(), (float) data.getMaxXValue());
        Range yRange = new Range((float) data.getMinYValue(), (float) data.getMaxYValue());

        return createDefaultSurfaceModel(data, xRange, yRange, data2);
    }

    public static DefaultSurfaceModel createDefaultSurfaceModel(final XYData data, Range xRange, Range yRange) {
        return createDefaultSurfaceModel(data, xRange, yRange, null);
    }

    public static DefaultSurfaceModel createDefaultSurfaceModel(final XYData data, Range xRange, Range yRange, final XYData data2) {
        final DefaultSurfaceModel sm = new DefaultSurfaceModel();

        sm.setPlotFunction2(false);

        sm.setCalcDivisions(50);
        sm.setDispDivisions(50);
        sm.setContourLines(10);

        sm.setXMin(xRange.getMin());
        sm.setXMax(xRange.getMax());
        setYRange(yRange, sm);

        sm.setBoxed(true);
        sm.setDisplayXY(true);
        sm.setExpectDelay(false);
        sm.setAutoScaleZ(true);
        sm.setDisplayZ(true);
        sm.setMesh(true);
        sm.setPlotType(SurfaceModel.PlotType.SURFACE);
        if (data2 == null)
            sm.setFirstFunctionOnly(true);
        else
            sm.setBothFunction(true);

        sm.setPlotColor(data2 != null ? SurfaceModel.PlotColor.FOG : SurfaceModel.PlotColor.SPECTRUM);
        sm.setMapper(new Mapper() {
            public float f1(float x, float y) {
                return data.getValue(x, y);
            }

            public float f2(float x, float y) {
                if (data2 == null)
                    return 0;
                return data2.getValue(x, y);
            }
        });
        sm.plot().execute();
        return sm;
    }

    public static void setYRange(Range yRange, DefaultSurfaceModel sm) {
        sm.setYMin(yRange.getMin());
        sm.setYMax(yRange.getMax());
    }
}
*/