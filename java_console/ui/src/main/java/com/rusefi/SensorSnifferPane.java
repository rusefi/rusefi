package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.RpmLabel;
import com.rusefi.ui.RpmModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.config.ConfigField;
import com.rusefi.ui.config.EnumConfigField;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;
import java.util.List;

/**
 * Date: 12/21/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class SensorSnifferPane {
    private static final String HELP_URL = "http://rusefi.com/wiki/index.php?title=Manual:DevConsole#Analog_Chart";

    private final TreeMap<Double, Double> values = new TreeMap<>();
    private final SensorSnifferCanvas canvas = new SensorSnifferCanvas();

    private double minX;
    private double maxX;
    private double minY;
    private double maxY;

    private final JPanel content = new JPanel(new BorderLayout());
    private final AnyCommand command;

    private boolean paused = false;

    public SensorSnifferPane(UIContext uiContext, Node config) {
        uiContext.sensorSnifferCentral.addListener(new SensorSnifferCentral.AnalogChartListener() {
            @Override
            public void onAnalogChart(final String message) {
                // this callback is invoked from the connectivity thread, need to handle in AWT for thread-safety
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        unpackValues(message, values);
                        if (!paused) {
                            processValues();
                            UiUtils.trueRepaint(canvas);
                        }
                    }
                });
            }
        });

        final JPanel upperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        final JButton pauseButton = UiUtils.createPauseButton();

        JButton clearButton = UiUtils.createClearButton();
        clearButton.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clear();
                UiUtils.trueRepaint(canvas);
                setPaused(pauseButton, false);
            }
        });
        upperPanel.add(clearButton);

        JButton saveImageButton = UiUtils.createSaveImageButton();
        upperPanel.add(saveImageButton);
        saveImageButton.addActionListener(new ActionListener() {
                                          @Override
                                          public void actionPerformed(ActionEvent e) {
                                              int rpm = RpmModel.getInstance().getValue();
                                              String fileName = Logger.getDate() + "_rpm_" + rpm + "_sensor" + ".png";

                                              UiUtils.saveImageWithPrompt(fileName, upperPanel, canvas);
                                          }
                                      }
        );

        upperPanel.add(pauseButton);
        upperPanel.add(new RpmLabel(uiContext,2).getContent());

        command = AnyCommand.createField(uiContext, config, true, false);
        upperPanel.add(command.getContent());

        upperPanel.add(new URLLabel(EngineSnifferPanel.HELP_TEXT, HELP_URL));
        pauseButton.addActionListener(new
                                              ActionListener() {
                                                  @Override
                                                  public void actionPerformed(ActionEvent e) {
                                                      setPaused(pauseButton, !paused);
                                                  }
                                              }
        );

//        upperPanel.setBorder(BorderFactory.createLineBorder(Color.orange));
        content.add(upperPanel, BorderLayout.NORTH);

        content.add(canvas, BorderLayout.CENTER);

        final JPanel lowerPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
        lowerPanel.setBorder(BorderFactory.createLineBorder(Color.cyan));
        content.add(lowerPanel, BorderLayout.SOUTH);

        lowerPanel.add(new EnumConfigField(uiContext, Fields.SENSORCHARTMODE, "Mode").getContent());
        lowerPanel.add(new ConfigField(uiContext, Fields.SENSORSNIFFERRPMTHRESHOLD, "RPM threshold").getContent());
    }

    private void setPaused(JButton pauseButton, boolean isPaused) {
        paused = isPaused;
        UiUtils.setPauseButtonText(pauseButton, paused);
    }

    private void clear() {
        minX = maxX = minY = maxY = 0;
        values.clear();
    }

    private void processValues() {
        List<Double> keys = new ArrayList<>(values.keySet());
        minX = keys.get(0);
        maxX = keys.get(keys.size() - 1);
        FileLog.MAIN.logLine("Analog chart from " + minX + " to " + maxX);

        TreeSet<Double> sortedValues = new TreeSet<>();
        sortedValues.addAll(values.values());
        List<Double> values = new ArrayList<>(sortedValues);

        minY = values.get(0);
        maxY = values.get(values.size() - 1);
    }

    public JComponent getPanel() {
        return content;
    }

    public ActionListener getTabSelectedListener() {
        return new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                command.requestFocus();
            }
        };
    }

    private class SensorSnifferCanvas extends JComponent {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            Dimension size = getSize();

            g.drawString("X range from " + minX + " to " + maxX, 4, 20);
            g.drawString("Y range from " + minY + " to " + maxY, 4, 40);

            int prevX = 0;
            int prevY = size.height;

            double bX = size.width / (maxX - minX);
            double bY = size.height / (maxY - minY);

            for (Map.Entry<Double, Double> e : values.entrySet()) {
                int x = (int) ((e.getKey() - minX) * bX);
                int y = size.height - (int) ((e.getValue() - minY) * bY);

                g.drawLine(prevX, prevY, x, y);
                prevX = x;
                prevY = y;
            }
        }
    }

    private static void unpackValues(String chart, TreeMap<Double, Double> destination) {
        destination.clear();

        String[] tokens = chart.split("\\|");
        for (int i = 0; i < tokens.length - 1; ) {
            String key = tokens[i++];
            String value = tokens[i++];

            destination.put(Double.parseDouble(key), Double.parseDouble(value));
        }
    }
}
