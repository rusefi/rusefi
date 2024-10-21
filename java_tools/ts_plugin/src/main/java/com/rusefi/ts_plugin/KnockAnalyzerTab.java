package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.OutputChannelClient;
import com.rusefi.core.ui.AutoupdateUtil;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Arrays;
import java.util.function.Supplier;
import java.util.stream.Collectors;


public class KnockAnalyzerTab {

    private final Supplier<ControllerAccess> controllerAccessSupplier;

    private final JComponent content = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    private final JLabel status = new JLabel();
    private final JButton button = new JButton("Start");

    private boolean started = false;

    private final int[] line_sum_index = {0};
    private int channel = 0;
    private int cylinder = 0;

    private final KnockCanvas canvas = new KnockCanvas();
    private final KnockCanvas canvas2 = new KnockCanvas();
    private final KnockMagnitudeCanvas magnituges = new KnockMagnitudeCanvas();

    public KnockAnalyzerTab(Supplier<ControllerAccess> controllerAccessSupplier) {
        var cc = this;
        this.controllerAccessSupplier = controllerAccessSupplier;

        this.setStartButtonState();
        this.setStartButtonState();

        String ecuControllerName = this.controllerAccessSupplier.get().getEcuConfigurationNames()[0];

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStart", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {
                    canvas.setFrequencyStart((int)v);
                    canvas2.setFrequencyStart((int)v);
                    magnituges.setFrequencyStart((int)v);
                    //canvas.repaint();
                }
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockFrequencyStep", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {
                    canvas.setFrequencyStep((float)v);
                    canvas2.setFrequencyStep((float)v);
                    magnituges.setFrequencyStep((float)v);
                    //canvas.repaint();
                }
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        try {
            String[] outputChannelNames = this.controllerAccessSupplier.get().getOutputChannelServer().getOutputChannels(ecuControllerName);

            var spectrums = Arrays.stream(outputChannelNames)
                .filter((n) -> n.indexOf("m_knockSpectrum") >= 0)
                .collect(Collectors.toList())
                .toArray(new String[0]);

            float[] values = new float[64];

            button.setText(this.getEnabled() == "true" ? "Stop" : "Start");

            int checksum = 0;
            for(var i = 0; i< 16; ++i) {
                checksum += i;
            }

            for(var i = 0; i< 16; ++i){
                try {

                    var name = spectrums[i];
                    int finalChecksum = checksum;
                    controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, name, new OutputChannelClient() {
                        @Override
                        public void setCurrentOutputChannelValue(String name, double v) {
                            //value.setText(String.valueOf(v));

                            if(!started)
                            {
                                canvas.repaint();
                                canvas2.repaint();
                                return;
                            }

                            var indexStr = name.substring(15);
                            var index = Integer.parseInt(indexStr) - 1;
                            //values[index] = (int)v;

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

                                if(channel == 1) {
                                    canvas.processValues(values);
                                    canvas.repaint();
                                }
                                else {
                                    canvas2.processValues(values);
                                    canvas2.repaint();
                                }

                                line_sum_index[0] = 0;
                            }
                        }
                    });
                } catch (ControllerException ee) {
                    System.out.println(ee.getMessage());
                }
            }

        } catch (ControllerException e) {

        }

        try {
            controllerAccessSupplier.get().getOutputChannelServer().subscribe(ecuControllerName, "m_knockSpectrumChannelCyl", new OutputChannelClient() {
                @Override
                public void setCurrentOutputChannelValue(String name, double v) {

                    long value = (long)v;

                    cc.channel = (int)(value >>> 8) & 0xFF;
                    cc.cylinder =  (int)(value & 0xFF);

                }
            });
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

                try {
                    var started = cc.setStartButtonState();
                    controllerAccessSupplier.get().getControllerParameterServer().updateParameter(ecuControllerName, "enableKnockSpectrogram", started ? 1.0 : 0.0);
                } catch (ControllerException ee) {
                    System.out.println(ee.getMessage());
                }
            }
        });


        content.add(status);
        content.add(button);

        KnockMouseListener kml = new KnockMouseListener(this.canvas);
        KnockMotionListener kmml = new KnockMotionListener(this.canvas, this.magnituges);
        KnockKeyListener l = new KnockKeyListener();
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
        content.add(canvas);


        KnockMouseListener kml2 = new KnockMouseListener(this.canvas2);
        KnockMotionListener kmml2 = new KnockMotionListener(this.canvas2, this.magnituges);
        KnockKeyListener l2 = new KnockKeyListener();
        canvas.setFocusTraversalKeysEnabled(false);
        canvas.addMouseMotionListener(kmml2);
        canvas.addMouseListener(kml2);
        canvas.addKeyListener(l2);
        canvas2.setFocusable(true);
        canvas2.setFocusTraversalKeysEnabled(false);
        canvas2.setFocusable(true);
        canvas2.setDoubleBuffered(true);
        canvas2.setPreferredSize(new Dimension(800, 200));
        canvas2.setMinimumSize(new Dimension(800, 200));
        content.add(canvas2);


        magnituges.setFocusable(true);
        magnituges.setFocusTraversalKeysEnabled(false);
        magnituges.setFocusable(true);
        magnituges.setDoubleBuffered(true);
        magnituges.setPreferredSize(new Dimension(800, 200));
        magnituges.setMinimumSize(new Dimension(800, 200));
        content.add(magnituges);

        AutoupdateUtil.trueLayout(content);
    }

    public String getEnabled() {
        try {
            String ecuControllerName = this.controllerAccessSupplier.get().getEcuConfigurationNames()[0];
            var enable = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(ecuControllerName, "enableKnockSpectrogram");
            String enabled = enable.getStringValue();
            return enabled;
        } catch (ControllerException ee) {
            System.out.println(ee.getMessage());
        }

        return "false";
    }

    public boolean setStartButtonState() {
        String enabled = this.getEnabled();

        this.started = enabled.indexOf("true") > 0 ? false : true;

        button.setText(this.started ? "Stop" : "Start");

        this.line_sum_index[0] = 0;

        if(this.started == true) {
            this.canvas.resetPeak();
        }

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
