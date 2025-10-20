package com.rusefi.ts_plugin.knock;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.*;
import com.rusefi.core.ui.AutoupdateUtil;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;


public class KnockAnalyzerTab {
    private static final Logging log = getLogging(KnockAnalyzerTab.class);

    public static final String CYLINDERS_COUNT = "cylindersCount";
    public static final String ENABLE_KNOCK_SPECTROGRAM = "enableKnockSpectrogram";

    private enum CanvasType {
        CT_ALL,
        CT_SENSORS,
        CT_CYLINDERS,
    }

    private final Supplier<ControllerAccess> controllerAccessSupplier;

    private final JComponent content = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));

    private final JComponent canvasesComponent = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    private final JButton buttonStartStop = new JButton("Start");

    private boolean started = false;
    private boolean flushed = false;

    private final int[] line_sum_index = {0};
    private int channel = 0;
    private int cylinder = 0;

    private final float[] values = new float[64];

    private int cylindersCount = 0;

    private CanvasType canvasType = CanvasType.CT_ALL;

    private final ArrayList<KnockCanvas> canvases = new ArrayList<>();
    private final KnockMagnitudeCanvas magnitudes = new KnockMagnitudeCanvas();

    public KnockAnalyzerTab(Supplier<ControllerAccess> controllerAccessSupplier) {
        this.controllerAccessSupplier = controllerAccessSupplier;

        String[] ecuConfigurationNames = this.controllerAccessSupplier.get().getEcuConfigurationNames();
        if (ecuConfigurationNames.length > 0) {
            String ecuControllerName = ecuConfigurationNames[0];
            subscribe(controllerAccessSupplier, ecuControllerName);
        } else {
            // major todo: plugin could be displayed prior to opening project!
            log.info("No configuration yet");
        }

        buttonStartStop.addActionListener(e -> {
            boolean enabled = getEnabledEcu();
            KnockAnalyzerTab.this.setStartState(!enabled);
            KnockAnalyzerTab.this.setEnabledEcu(!enabled);
        });

        JComponent buttons = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));

        JLabel status = new JLabel();
        buttons.add(status);
        buttons.add(buttonStartStop);
        JButton buttonAll = new JButton("All");
        buttons.add(buttonAll);
        JButton buttonSensors = new JButton("Sensors");
        buttons.add(buttonSensors);
        JButton buttonCylinders = new JButton("Cylinders");
        buttons.add(buttonCylinders);

        buttonAll.addActionListener(e -> {
            canvasType = CanvasType.CT_ALL;
            createCanvas(canvasType);
            // todo: address this hack!!!
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });
        buttonSensors.addActionListener(e -> {
            canvasType = CanvasType.CT_SENSORS;
            createCanvas(canvasType);
            // todo: address this hack!!!
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });
        buttonCylinders.addActionListener(e -> {
            canvasType = CanvasType.CT_CYLINDERS;
            createCanvas(canvasType);
            // todo: address this hack!!!
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });

        content.add(buttons);

        JScrollPane canvasScroll = new JScrollPane(canvasesComponent, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        canvasScroll.setPreferredSize(new Dimension(840, 800));
        canvasScroll.setMinimumSize(new Dimension(840, 800));
        canvasScroll.setMaximumSize(new Dimension(840, 800));
        JComponent allDraw = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        allDraw.add(canvasScroll);
        content.add(allDraw);

        JComponent magnitudes = this.magnitudes.getComponent();
        magnitudes.setFocusable(true);
        magnitudes.setFocusTraversalKeysEnabled(false);
        magnitudes.setFocusable(true);
        magnitudes.setDoubleBuffered(true);
        magnitudes.setPreferredSize(new Dimension(760, 200));
        magnitudes.setMinimumSize(new Dimension(760, 200));
        allDraw.add(magnitudes);

        createCanvas(CanvasType.CT_ALL);

        boolean enabled = getEnabledEcu();
        this.setStartState(enabled);

        refreshCanvases();
    }

    private void subscribe(Supplier<ControllerAccess> controllerAccessSupplier, String ecuControllerName) {
        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStart", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {

                    int frequency = (int) v;
                    canvases.forEach(c -> c.setFrequencyStart(frequency));
                    magnitudes.setFrequencyStart(frequency);
                }
            });
        } catch (ControllerException ee) {
            log.error(ee.getMessage());
        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStep", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {

                    float frequencyStep = (float) v;
                    canvases.forEach(c -> c.setFrequencyStep(frequencyStep));
                    magnitudes.setFrequencyStep(frequencyStep);
                }
            });
        } catch (ControllerException e) {
            log.error(e.getMessage());
        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockSpectrumChannelCyl", (name, v) -> {

                long value = (long) v;

                flush();

                KnockAnalyzerTab.this.channel = (int) (value >>> 8) & 0xFF;
                KnockAnalyzerTab.this.cylinder = (int) (value & 0xFF);
            });
        } catch (ControllerException e) {
            log.error(e.getMessage());
        }

        try {
            ControllerParameter cylindersCountParameter = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(ecuControllerName, CYLINDERS_COUNT);
            if (cylindersCountParameter != null) {
                double value = cylindersCountParameter.getScalarValue();
                KnockAnalyzerTab.this.cylindersCount = (int) (value);
            }
        } catch (ControllerException e) {
            log.error(e.getMessage());
        }

        try {
            String[] outputChannelNames = controllerAccessSupplier.get().getOutputChannelServer().getOutputChannels(ecuControllerName);

            String[] spectrums = Arrays.stream(outputChannelNames)
                .filter((n) -> n.contains("m_knockSpectrum")).toArray(String[]::new);

            int checksum = 0;
            for (int i = 0; i < 16; ++i) {
                checksum += i;
            }

            if (outputChannelNames.length != 0)
                for (int i = 0; i < 16; ++i) {
                    try {

                        String name = spectrums[i];
                        int finalChecksum = checksum;
                        controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, name, (name1, v) -> {
                            if (!started) {
                                refreshCanvases();
                                return;
                            }

                            flushed = false;

                            String indexStr = name1.substring(15);
                            int index = Integer.parseInt(indexStr) - 1;

                            long value = (long) v;

                            long a = (value >>> 24) & 0xFF;
                            long b = (value >>> 16) & 0xFF;
                            long c = (value >>> 8) & 0xFF;
                            long d = value & 0xFF;

                            values[index * 4] = a;
                            values[(index * 4) + 1] = b;
                            values[(index * 4) + 2] = c;
                            values[(index * 4) + 3] = d;

                            line_sum_index[0] += index;
                            if (line_sum_index[0] >= finalChecksum) {

                                flush();

                                line_sum_index[0] = 0;
                            }
                        });
                    } catch (ControllerException ee) {
                        log.error(ee.getMessage());
                    }
                }

        } catch (ControllerException e) {
            log.error(e.getMessage());
        }
    }

    private void flush() {

        if (!started || flushed) {
            return;
        }

        switch (canvasType) {
            case CT_ALL:
                canvases.forEach(canvas -> {
                    canvas.processValues(values);
                });
                break;
            case CT_SENSORS:
                assert channel < canvases.size();
                canvases.get(channel).processValues(values);
                break;
            case CT_CYLINDERS:
                assert cylinder < canvases.size();
                canvases.get(cylinder).processValues(values);
                break;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                canvases.forEach(canvas -> {
                    AutoupdateUtil.trueLayoutAndRepaint(canvas.getComponent());
                });

                Arrays.fill(values, 0);
                flushed = true;
            }
        });
    }

    private void initCanvas(KnockMotionListener kmml, KnockMouseListener kml, JComponent canvas) {
        canvas.setFocusTraversalKeysEnabled(false);
        canvas.addMouseMotionListener(kmml);
        canvas.addMouseListener(kml);
        canvas.setFocusable(true);
        canvas.setFocusTraversalKeysEnabled(false);
        canvas.setFocusable(true);
        canvas.setDoubleBuffered(true);
        canvas.setPreferredSize(new Dimension(800, 200));
        canvas.setMinimumSize(new Dimension(800, 200));
    }

    public void createCanvas(CanvasType canvasType) {

        this.clearCanvas();

        switch (canvasType) {
            case CT_ALL:
                createCanvas(1, 1);
                break;
            case CT_SENSORS:
                createCanvasSensors();
                break;
            case CT_CYLINDERS:
                createCanvasCylinders();
                break;
            default:
                throw new Error("Unknown CanvasType");
        }

        refreshCanvases();
    }

    public void clearCanvas() {
        this.canvases.clear();
        canvasesComponent.removeAll();
        this.refreshCanvases();
    }

    private long refreshThrottle;

    private void refreshCanvases() {
        long now = System.currentTimeMillis();
        boolean withRecentRefresh = now - refreshThrottle < 500/*ms*/;
        refreshThrottle = now;
        if (withRecentRefresh) {
            return;
        }
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                canvases.forEach(canvas -> {
                    AutoupdateUtil.trueLayoutAndRepaint(canvas.getComponent());
                });
            }
        });
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(canvasesComponent));
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(content));
    }

    public void createCanvas(int number, int divider) {
        KnockCanvas canvas = new KnockCanvas(number, divider);
        KnockMouseListener kml = new KnockMouseListener(canvas);
        KnockMotionListener kmml = new KnockMotionListener(canvas, this.magnitudes);
        initCanvas(kmml, kml, canvas.getComponent());
        canvasesComponent.add(canvas.getComponent());
        canvases.add(canvas);
    }

    public void createCanvasSensors() {
        this.createCanvas(1, 2);
        this.createCanvas(2, 2);
    }

    public void createCanvasCylinders() {
        for (int i = 0; i < this.cylindersCount; ++i) {
            this.createCanvas(i + 1, this.cylindersCount);
        }
    }

    private boolean getEnabledEcu() {
        try {
            ControllerAccess controllerAccess = controllerAccessSupplier.get();
            String[] ecuConfigurationNames = controllerAccess.getEcuConfigurationNames();
            if (ecuConfigurationNames.length == 0)
                return false; // project not open
            String ecuControllerName = ecuConfigurationNames[0];
            ControllerParameter enable = controllerAccess.getControllerParameterServer().getControllerParameter(ecuControllerName, ENABLE_KNOCK_SPECTROGRAM);
            String enabled = enable.getStringValue();
            return enabled.contains("true") || enabled.contains("yes");
        } catch (ControllerException ee) {
            log.error(ee.getMessage());
        }

        return false;
    }

    public void setEnabledEcu(boolean enabled) {
        try {
            String[] ecuConfigurationNames = controllerAccessSupplier.get().getEcuConfigurationNames();
            if (ecuConfigurationNames.length == 0) {
                log.warn("No project?");
                return;
            }
            String ecuControllerName = ecuConfigurationNames[0];
            controllerAccessSupplier.get().getControllerParameterServer().updateParameter(ecuControllerName, "enableKnockSpectrogram", enabled ? 1.0 : 0.0);
        } catch (ControllerException ee) {
            log.error(ee.getMessage());
        }
    }

    public void setStartState(boolean enabled) {
        this.started = enabled;

        buttonStartStop.setText(this.started ? "Stop" : "Start");

        this.line_sum_index[0] = 0;

        if (this.started) {
            canvases.forEach(canvas -> {
                canvas.resetPeak();
            });
        }

        refreshCanvases();
    }

    public JComponent getContent() {
        return content;
    }

    public class KnockMotionListener implements MouseMotionListener {

        private final KnockCanvas knockCanvas;
        private final KnockMagnitudeCanvas magnitudesCanvas;

        KnockMotionListener(KnockCanvas canvas, KnockMagnitudeCanvas magnitudes) {
            knockCanvas = canvas;
            magnitudesCanvas = magnitudes;
        }

        @Override
        public void mouseDragged(MouseEvent e) {

        }

        @Override
        public void mouseMoved(MouseEvent e) {
            knockCanvas.mouse_x = e.getX();
            knockCanvas.mouse_y = e.getY();

            float[] magnitudes = knockCanvas.getCurrentMouseMagnitudes();

            magnitudesCanvas.processValues(magnitudes);
        }
    }

    public class KnockMouseListener extends MouseAdapter {

        private final KnockCanvas knockCanvas;

        KnockMouseListener(KnockCanvas canvas) {
            knockCanvas = canvas;
        }

        @Override
        public void mouseClicked(MouseEvent e) {
/* dead code?
            knockCanvas.setupFrequencyByClick(e.getX(), e.getY());
 */
        }
    }
}
