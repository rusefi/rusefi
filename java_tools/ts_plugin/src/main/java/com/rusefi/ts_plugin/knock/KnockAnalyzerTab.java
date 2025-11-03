package com.rusefi.ts_plugin.knock;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.*;
import com.rusefi.core.ui.AutoupdateUtil;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Arrays;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;


public class KnockAnalyzerTab {
    private static final Logging log = getLogging(KnockAnalyzerTab.class);

    private static final int PREFERRED_WIDTH = 800;
    private static final int PREF_HEIGHT = 200;

    // todo: share value with C++ via some .txt?
    private final static int COMPRESSED_SPECTRUM_PROTOCOL_SIZE = 16;

    public static final String CYLINDERS_COUNT = "cylindersCount";
    public static final String ENABLE_KNOCK_SPECTROGRAM = "enableKnockSpectrogram";
    private static final int MAG_WIDTH = 760;
    private static final int MAG_HEIGHT = 200;
    public static final int VALUE_COUNT = 64;

    private enum CanvasType {
        COMBINED,
        CT_SENSORS,
        CT_CYLINDERS,
    }

    private final Supplier<ControllerAccess> controllerAccessSupplier;

    private final JComponent content = new JPanel(new BorderLayout());

    private final JComponent canvasesPanel = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    private final JButton buttonStartStop = new JButton("Start");

    private boolean started = false;
    private boolean flushed = false;

    private final int[] line_sum_index = {0};
    private int currentChannel = 0;
    private int currentCylinder = 0;

    private final float[] values = new float[VALUE_COUNT];

    private int cylindersCount = 0;

    private CanvasType canvasType = CanvasType.COMBINED;

    private final java.util.List<KnockCanvas> canvases = new CopyOnWriteArrayList<>();
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
            toggleStartStopState();
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
            canvasType = CanvasType.COMBINED;
            createCanvas();
            toggle();
        });
        buttonSensors.addActionListener(e -> {
            canvasType = CanvasType.CT_SENSORS;
            createCanvas();
            toggle();
        });
        buttonCylinders.addActionListener(e -> {
            canvasType = CanvasType.CT_CYLINDERS;
            createCanvas();
            toggle();
        });

        content.add(buttons, BorderLayout.NORTH);

        JScrollPane canvasScroll = new JScrollPane(canvasesPanel, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
//        canvasScroll.setPreferredSize(new Dimension(840, 800));
//        canvasScroll.setMinimumSize(new Dimension(840, 800));
//        canvasScroll.setMaximumSize(new Dimension(840, 800));
        JComponent allDraw = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        allDraw.add(canvasScroll);
        content.add(allDraw, BorderLayout.CENTER);

        JComponent magnitudes = this.magnitudes.getComponent();
        magnitudes.setFocusable(true);
        magnitudes.setFocusTraversalKeysEnabled(false);
        magnitudes.setFocusable(true);
        magnitudes.setDoubleBuffered(true);
        magnitudes.setPreferredSize(new Dimension(MAG_WIDTH, MAG_HEIGHT));
        magnitudes.setMinimumSize(new Dimension(MAG_WIDTH, MAG_HEIGHT));
        allDraw.add(magnitudes);

        createCanvas();

        boolean enabled = getEnabledOnEcuSide();
        this.setStartState(enabled);

        refreshCanvases();
    }

    private void toggleStartStopState() {
        boolean enabled = getEnabledOnEcuSide();
        KnockAnalyzerTab.this.setStartState(!enabled);
        KnockAnalyzerTab.this.setEnabledOnEcuSide(!enabled);
    }

    private void toggle() {
        // todo: address this hack?
        // invert, reset canvas, re-start on ECU side along the way?!
        toggleStartStopState();
        toggleStartStopState();
    }

    private int prevFrequency = -1;
    private float prevFrequencyStep = -1;

    private void subscribe(Supplier<ControllerAccess> controllerAccessSupplier, String ecuControllerName) {
        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStart", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {
                    int frequency = (int) v;
                    if (prevFrequency != frequency) {
                        prevFrequency = frequency;
                        System.out.println("frequency: " + frequency);
                    }
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
                    if (prevFrequencyStep != frequencyStep) {
                        prevFrequencyStep = frequencyStep;
                        System.out.println("frequencyStep: " + frequencyStep);
                    }
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

                KnockAnalyzerTab.this.currentChannel = (int) (value >>> 8) & 0xFF;
                KnockAnalyzerTab.this.currentCylinder = (int) (value & 0xFF);
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
            for (int i = 0; i < COMPRESSED_SPECTRUM_PROTOCOL_SIZE; ++i) {
                checksum += i;
            }

            if (outputChannelNames.length != 0)
                for (int i = 0; i < COMPRESSED_SPECTRUM_PROTOCOL_SIZE; ++i) {
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
            case COMBINED:
                canvases.forEach(canvas -> {
                    canvas.processValues(values);
                });
                break;
            case CT_SENSORS:
                assert currentChannel < canvases.size();
                canvases.get(currentChannel).processValues(values);
                break;
            case CT_CYLINDERS:
                assert currentCylinder < canvases.size();
                canvases.get(currentCylinder).processValues(values);
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
        canvas.setPreferredSize(new Dimension(PREFERRED_WIDTH, PREF_HEIGHT));
        canvas.setMinimumSize(new Dimension(PREFERRED_WIDTH, PREF_HEIGHT));
    }

    public void createCanvas() {

        this.clearCanvas();

        switch (canvasType) {
            case COMBINED:
                createCanvas(1, 1, true);
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
        canvasesPanel.removeAll();
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
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(canvasesPanel));
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(content));
    }

    private void createCanvas(int number, int divider, boolean isCombined) {
        KnockCanvas canvas = new KnockCanvas(number, divider, isCombined);
        KnockMouseListener kml = new KnockMouseListener(canvas);
        KnockMotionListener kmml = new KnockMotionListener(canvas, this.magnitudes);
        initCanvas(kmml, kml, canvas.getComponent());
        canvasesPanel.add(canvas.getComponent());
        canvases.add(canvas);
    }

    public void createCanvasSensors() {
        this.createCanvas(1, 2, false);
        this.createCanvas(2, 2, false);
    }

    public void createCanvasCylinders() {
        for (int i = 0; i < this.cylindersCount; ++i) {
            this.createCanvas(i + 1, this.cylindersCount, false);
        }
    }

    private boolean getEnabledOnEcuSide() {
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

    private void setEnabledOnEcuSide(boolean enabled) {
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
            knockCanvas.setMousePosition(e.getX(), e.getY());

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
