package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.*;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.models.Utils;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.function.Supplier;
import java.util.stream.Collectors;


public class KnockAnalyzerTab {

    private enum CanvasType {
        CT_ALL,
        CT_SENSORS,
        CT_CYLINDERS,
    }

    private final Supplier<ControllerAccess> controllerAccessSupplier;
    String ecuControllerName;

    private final JComponent content = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    JComponent allDraw = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));

    JComponent canvasesComponent = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    private final JLabel status = new JLabel();
    private final JButton buttonStartStop = new JButton("Start");

    private final JButton buttonAll = new JButton("All");
    private final JButton buttonSensors = new JButton("Sensors");
    private final JButton buttonCylinders = new JButton("Cylinders");

    private boolean started = false;
    private boolean flushed = false;

    private final int[] line_sum_index = {0};
    private int channel = 0;
    private int cylinder = 0;

    private float[] values = new float[64];

    private int cylindersCount = 0;

    private CanvasType canvasType = CanvasType.CT_ALL;

    private ArrayList<KnockCanvas> canvases = new ArrayList<>();
    private final KnockMagnitudeCanvas magnituges = new KnockMagnitudeCanvas();

    public KnockAnalyzerTab(Supplier<ControllerAccess> controllerAccessSupplier) {
        this.controllerAccessSupplier = controllerAccessSupplier;
        ecuControllerName = this.controllerAccessSupplier.get().getEcuConfigurationNames()[0];

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStart", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {

                    int frequency = (int)v;
                    canvases.forEach(c -> c.setFrequencyStart(frequency));
                    magnituges.setFrequencyStart(frequency);
                }
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStep", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {

                    float frequencyStep = (float)v;
                    canvases.forEach(c -> c.setFrequencyStep(frequencyStep));
                    magnituges.setFrequencyStep(frequencyStep);
                }
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockSpectrumChannelCyl", (name, v) -> {

                long value = (long)v;

                flush();

                KnockAnalyzerTab.this.channel = (int)(value >>> 8) & 0xFF;
                KnockAnalyzerTab.this.cylinder =  (int)(value & 0xFF);
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            ControllerParameter cylindersCountParameter = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(ecuControllerName, "cylindersCount");
            if(cylindersCountParameter != null) {
                double value = cylindersCountParameter.getScalarValue();
                KnockAnalyzerTab.this.cylindersCount = (int)(value);
            }
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            String[] outputChannelNames = this.controllerAccessSupplier.get().getOutputChannelServer().getOutputChannels(ecuControllerName);

            String[] spectrums = Arrays.stream(outputChannelNames)
                .filter((n) -> n.indexOf("m_knockSpectrum") >= 0)
                .collect(Collectors.toList())
                .toArray(new String[0]);

            int checksum = 0;
            for (int i = 0; i< 16; ++i) {
                checksum += i;
            }

            for (int i = 0; i< 16; ++i){
                try {

                    String name = spectrums[i];
                    int finalChecksum = checksum;
                    controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, name, (name1, v) -> {
                        if(!started)
                        {
                            // SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayout(content));
                            return;
                        }

                        flushed = false;

                        String indexStr = name1.substring(15);
                        int index = Integer.parseInt(indexStr) - 1;

                        long value = (long)v;

                        long a = (value >>> 24) & 0xFF;
                        long b = (value >>> 16) & 0xFF;
                        long c = (value >>> 8) & 0xFF;
                        long d =  value & 0xFF;

                        values[index * 4] = a;
                        values[(index * 4) + 1] = b;
                        values[(index * 4) + 2] = c;
                        values[(index * 4) + 3] = d;

                        line_sum_index[0] += index;
                        if(line_sum_index[0] >= finalChecksum) {

                            flush();

                            line_sum_index[0] = 0;
                        }
                    });
                } catch (ControllerException ee) {
                    System.out.println(ee.getMessage());
                }
            }

        } catch (ControllerException e) {

        }

        buttonStartStop.addActionListener(e -> {
                boolean enabled = this.getEnabledEcu();
                KnockAnalyzerTab.this.setStartState(!enabled);
                KnockAnalyzerTab.this.setEnabledEcu(!enabled);
        });

        JComponent buttons = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));

        buttons.add(status);
        buttons.add(buttonStartStop);
        buttons.add(buttonAll);
        buttons.add(buttonSensors);
        buttons.add(buttonCylinders);

        buttonAll.addActionListener(e -> {
            canvasType = CanvasType.CT_ALL;
            createCanvas(canvasType);
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });
        buttonSensors.addActionListener(e -> {
            canvasType = CanvasType.CT_SENSORS;
            createCanvas(canvasType);
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });
        buttonCylinders.addActionListener(e -> {
            canvasType = CanvasType.CT_CYLINDERS;
            createCanvas(canvasType);
            buttonStartStop.doClick();
            buttonStartStop.doClick();
        });

        content.add(buttons);

        JScrollPane canvasScroll = new JScrollPane(canvasesComponent, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        canvasScroll.setPreferredSize(new Dimension(840, 800));
        canvasScroll.setMinimumSize(new Dimension(840, 800));
        canvasScroll.setMaximumSize(new Dimension(840, 800));
        allDraw.add(canvasScroll);
        content.add(allDraw);

        JComponent magnituges = this.magnituges.getComponent();
        magnituges.setFocusable(true);
        magnituges.setFocusTraversalKeysEnabled(false);
        magnituges.setFocusable(true);
        magnituges.setDoubleBuffered(true);
        magnituges.setPreferredSize(new Dimension(760, 200));
        magnituges.setMinimumSize(new Dimension(760, 200));
        allDraw.add(magnituges);

        createCanvas(CanvasType.CT_ALL);

        boolean enabled = this.getEnabledEcu();
        this.setStartState(enabled);

        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayout(content));
    }

    private void flush() {

        if(!started || flushed)
        {
            return;
        }

        switch (canvasType) {
            case CT_ALL:
                canvases.forEach(canvas -> {
                    canvas.processValues(values);
                    canvas.getComponent().repaint();
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

        canvases.forEach(canvas -> {
            canvas.getComponent().repaint();
        });

        for (int i = 0; i < values.length; ++i) {
            values[i] = 0;
        }

        flushed = true;
    }

    private void initCanvas(KnockMotionListener kmml, KnockMouseListener kml, KnockKeyListener l, JComponent canvas) {
        canvas.setFocusTraversalKeysEnabled(false);
        canvas.addMouseMotionListener(kmml);
        canvas.addMouseListener(kml);
        canvas.addKeyListener(l);
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
                createCanvas(1,1);
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

    public void refreshCanvases() {
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayout(canvasesComponent));
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayout(content));
    }

    public void createCanvas(int number, int divider) {
        KnockCanvas canvas = new KnockCanvas(number, divider);
        KnockMouseListener kml = new KnockMouseListener(canvas);
        KnockMotionListener kmml = new KnockMotionListener(canvas, this.magnituges);
        KnockKeyListener l = new KnockKeyListener();
        initCanvas(kmml, kml, l, canvas.getComponent());
        canvasesComponent.add(canvas.getComponent());
        canvases.add(canvas);
    }

    public void createCanvasSensors() {
        this.createCanvas(1, 2);
        this.createCanvas(2, 2);
    }

    public void createCanvasCylinders() {
        for(int i = 0; i < this.cylindersCount; ++i){
            this.createCanvas(i + 1, this.cylindersCount);
        }
    }

    public boolean getEnabledEcu() {
        try {
            String ecuControllerName = this.controllerAccessSupplier.get().getEcuConfigurationNames()[0];
            ControllerParameter enable = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(ecuControllerName, "enableKnockSpectrogram");
            String enabled = enable.getStringValue();
            return enabled.indexOf("true") > 0;
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        return false;
    }

    public void setEnabledEcu(boolean enabled) {
        try {
            String ecuControllerName = this.controllerAccessSupplier.get().getEcuConfigurationNames()[0];
            controllerAccessSupplier.get().getControllerParameterServer().updateParameter(ecuControllerName, "enableKnockSpectrogram", enabled ? 1.0 : 0.0);
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }
    }

    public void setStartState(boolean enabled) {
        this.started = enabled;

        buttonStartStop.setText(this.started ? "Stop" : "Start");

        this.line_sum_index[0] = 0;

        if(this.started) {
            canvases.forEach(canvas -> {
                canvas.resetPeak();
            });
        }

        refreshCanvases();
    }

    public boolean getStartState() {
        return this.started;
    }

    public JComponent getContent() {
        return content;
    }

    public class KnockKeyListener extends KeyAdapter implements ActionListener {

        @Override
        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_RIGHT) {

            }
            if (e.getKeyCode() == KeyEvent.VK_UP) {

            }
            if (e.getKeyCode() == KeyEvent.VK_LEFT) {

            }
            if (e.getKeyCode() == KeyEvent.VK_DOWN) {

            }
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            //drawPanel.repaint();
        }
    }

    public class KnockMotionListener implements MouseMotionListener {

        private KnockCanvas knockCanvas;
        private KnockMagnitudeCanvas magnitugesCanvas;

        KnockMotionListener(KnockCanvas canvas, KnockMagnitudeCanvas magnituges) {
            knockCanvas = canvas;
            magnitugesCanvas = magnituges;
        }

        @Override
        public void mouseDragged(MouseEvent e) {

        }

        @Override
        public void mouseMoved(MouseEvent e) {
            knockCanvas.mouse_x = e.getX();
            knockCanvas.mouse_y = e.getY();

            float[] magnitudes = knockCanvas.getCurrentMouseMagnitudes();

            magnitugesCanvas.processValues(magnitudes);
        }
    }

    public class KnockMouseListener implements MouseListener {

        private KnockCanvas knockCanvas;

        KnockMouseListener(KnockCanvas canvas) {
            knockCanvas = canvas;
        }

        @Override
        public void mouseClicked(MouseEvent e) {
            knockCanvas.setupFrequencyByClick(e.getX(), e.getY());
        }

        @Override
        public void mousePressed(MouseEvent e) {

        }

        @Override
        public void mouseReleased(MouseEvent e) {

        }

        @Override
        public void mouseEntered(MouseEvent e) {

        }

        @Override
        public void mouseExited(MouseEvent e) {

        }
    }
}
