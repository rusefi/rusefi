package com.rusefi.ui.knock;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.tools.ConsoleTools;
import com.rusefi.ui.RpmLabel;
import com.rusefi.ui.RpmModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.config.ConfigUiField;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.util.Arrays;
import java.util.Base64;
import java.util.Random;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.tools.ConsoleTools.startAndConnect;
import static java.lang.Math.*;

/**
 * Date: 20/02/24
 * Aleksey Ershov, (c) 2013-2024
 */
public class KnockPane {
    private static final Logging log = getLogging(KnockPane.class);

    private final KnockCanvas canvas = new KnockCanvas();

    private final JPanel content = new JPanel(new BorderLayout());
    private final AnyCommand command;


    public class KnockKeListener extends KeyAdapter implements ActionListener {

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

    public KnockPane(UIContext uiContext, Node config) {

        uiContext.getLinkManager().getEngineState().registerStringValueAction(Fields.PROTOCOL_KNOCK_SPECTROGRAMM, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                canvas.processValues(value);
                canvas.repaint();
            }
        });

        final JPanel upperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));

        JButton enableButton = new JButton("start");
        enableButton.setMnemonic('s');

        enableButton.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                uiContext.getLinkManager().submit(() -> {
                    //uiContext.getLinkManager().setCompositeLogicEnabled(false); // todo: use big_buffer
                    BinaryProtocol binaryProtocol = uiContext.getLinkManager().getConnector().getBinaryProtocol();
                    binaryProtocol.executeCommand(Fields.TS_KNOCK_SPECTROGRAM_ENABLE, "start knock analyzer");
                });
            }
        });
        upperPanel.add(enableButton);

        JButton disableButton = new JButton("stop");
        disableButton.setMnemonic('s');
        disableButton.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                uiContext.getLinkManager().submit(() -> {
                    BinaryProtocol binaryProtocol = uiContext.getLinkManager().getConnector().getBinaryProtocol();
                    binaryProtocol.executeCommand(Fields.TS_KNOCK_SPECTROGRAM_DISABLE, "stop knock analyzer");
                    //uiContext.getLinkManager().setCompositeLogicEnabled(true); // todo: use big_buffer
                });
            }
        });
        upperPanel.add(disableButton);


        JButton saveImageButton = UiUtils.createSaveImageButton();
        upperPanel.add(saveImageButton);
        saveImageButton.addActionListener(e -> {
            int rpm = RpmModel.getInstance().getValue();
            String fileName = FileLog.getDate() + "_knock_" + rpm + "_spectrogram" + ".png";

            UiUtils.saveImageWithPrompt(fileName, upperPanel, canvas);
        }
        );

        upperPanel.add(new RpmLabel(uiContext,2).getContent());

        command = AnyCommand.createField(uiContext, config, true, false);
        upperPanel.add(command.getContent());

        content.add(upperPanel, BorderLayout.NORTH);

        KnockKeListener l = new KnockKeListener();
        canvas.setFocusable(true);
        canvas.setFocusTraversalKeysEnabled(false);
        canvas.addKeyListener(l);
        canvas.setFocusable(true);
        canvas.setDoubleBuffered(true);
        content.add(canvas, BorderLayout.CENTER);


        final JPanel leftPanel = new KnockScale(this);
        leftPanel.setPreferredSize(new Dimension(150, 0));

        content.add(leftPanel, BorderLayout.WEST);
    }

    public KnockCanvas getCanvas() {
        return canvas;
    }

    public JComponent getPanel() {
        return content;
    }

    public ActionListener getTabSelectedListener() {
        return e -> command.requestFocus();
    }

    public class KnockCanvas extends JComponent implements ComponentListener {

        JComponent dd = this;
        //--------------------------------------

        private BufferedImage bufferedImage;
        private Graphics2D bufferedGraphics;
        static int SPECTROGRAM_X_AXIS_SIZE = 1024;
        float[][] specrtogram;
        float mainFrequency = 0;
        Color[] colorspace;
        Color[] colors;
        float[] amplitudesInColorSpace;

        int spectrogramYAxisSize;

        public double yAxisHz[];

        int currentIndexXAxis = 0;

        private KnockCanvas() {

            SwingUtilities.invokeLater(new Runnable(){
                public void run() {
                    dd.repaint();
                }
            });

            bufferedImage = new BufferedImage(640,480, BufferedImage.TYPE_INT_RGB);
            bufferedGraphics = bufferedImage.createGraphics();
            this.addComponentListener(this);

            //linear-gradient(to right, #000000, #290d1a, #490b32, #670353, #81007b, #a60085, #ca008b, #ef008f, #ff356b, #ff6947, #ff9a22, #ffc700);
            colorspace = new Color[] {
                Color.decode("#000000"),
                Color.decode("#290d1a"),
                Color.decode("#490b32"),
                Color.decode("#670353"),
                Color.decode("#81007b"),
                Color.decode("#a60085"),
                Color.decode("#ca008b"),
                Color.decode("#ef008f"),
                Color.decode("#ff356b"),
                Color.decode("#ff6947"),
                Color.decode("#ff9a22"),
                Color.decode("#ffc700"),
            };

            // this values agreed with firmware sample rate(KNOCK_SAMPLE_RATE) and fft size(512)
            yAxisHz = new double[]{
                0, 427.246, 854.492, 1281.74, 1708.98, 2136.23, 2563.48, 2990.72, 3417.97, 3845.21,
                4272.46, 4699.71, 5126.95, 5554.2, 5981.45, 6408.69, 6835.94, 7263.18, 7690.43, 8117.68,
                8544.92, 8972.17, 9399.41, 9826.66, 10253.9, 10681.2, 11108.4, 11535.6, 11962.9, 12390.1,
                12817.4, 13244.6, 13671.9, 14099.1, 14526.4, 14953.6, 15380.9, 15808.1, 16235.4, 16662.6,
                17089.8, 17517.1, 17944.3, 18371.6, 18798.8, 19226.1, 19653.3, 20080.6, 20507.8, 20935.1,
                21362.3, 21789.6, 22216.8, 22644, 23071.3, 23498.5, 23925.8, 24353, 24780.3, 25207.5,
            };

            spectrogramYAxisSize = yAxisHz.length;
            specrtogram = new float[SPECTROGRAM_X_AXIS_SIZE][spectrogramYAxisSize];
            colors = new Color[spectrogramYAxisSize];
            amplitudesInColorSpace = new float[spectrogramYAxisSize];
        }

        private void processValues(String values) {

            //log.info(values);

            var first_split = values.split("\\*");
            var freq_str = first_split[0];

            mainFrequency = Float.parseFloat(freq_str);

            String compressed = first_split[1];
            char sizec = compressed.charAt(0);
            int size_data = (int)sizec;

            char base64Size = compressed.charAt(1);
            int b64size = (int)base64Size;

            String base64 = compressed.substring(2, b64size + 2);

            byte[] data = Base64.getDecoder().decode(base64);

            assert size_data == compressed.length() - 1;
            assert size_data == yAxisHz.length;

            if(compressed.length() - 1 < spectrogramYAxisSize){
                log.error("data size error: " + compressed.length());
                return;
            }

            for(int i = 0; i < spectrogramYAxisSize; ++i) {

                byte c = data[i];
                int k = c + 128;

                specrtogram[currentIndexXAxis][i] = k;
            }

            int width = bufferedImage.getWidth();
            int height = bufferedImage.getHeight();

            float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;

            float min = Integer.MAX_VALUE;
            float max = 0;
            for(int x = 0; x < SPECTROGRAM_X_AXIS_SIZE; ++x) {
                for(int y = 0; y < spectrogramYAxisSize; ++y) {
                    float value = specrtogram[x][y];
                    if(value < min) {
                        min = value;
                    }

                    if(value > max) {
                        max = value;
                    }
                }
            }

            for(int y = 0; y < spectrogramYAxisSize; ++y)
            {
                float value = specrtogram[currentIndexXAxis][y];
                double lvalue = value;
                double lmin = min;
                double lmax = max;

                double normalized = 0;
                if((lmax-lmin) != 0) {
                    normalized = (lvalue-lmin)/(lmax-lmin);
                }

                if(normalized > 1)
                {
                    normalized = 1.0;
                }

                if(normalized < 0)
                {
                    normalized = 0.0;
                }

                //int color_index = (int)((colorspace.length - 1) * (float)random()); //for test
                int color_index = (int)((colorspace.length-1) * normalized);

                Color color = colorspace[color_index];

                colors[(spectrogramYAxisSize-1) - y] = color;
                amplitudesInColorSpace[y] = ((float)y) / (float) spectrogramYAxisSize;
            }

            LinearGradientPaint lgp = new LinearGradientPaint(
                new Point2D.Float(0, 0),
                new Point2D.Float(0, height),
                amplitudesInColorSpace,
                colors
            );

            bufferedGraphics.setPaint(lgp);

            bufferedGraphics.fillRect((int)(currentIndexXAxis * bx), 0, (int)bx, height);

            //log.info(Arrays.toString(specrtogram[currentIndexXAxis]));

            ++currentIndexXAxis;

            if(currentIndexXAxis >= SPECTROGRAM_X_AXIS_SIZE){
                currentIndexXAxis = 0;
            }
        }

        double lerp(double start, double end, double t) {
            return start * (1 - t) + end * t;
        }

        private static int searchHZ(double[] a, int fromIndex, int toIndex, double key) {
            int low = fromIndex;
            int high = toIndex - 1;

            while (low <= high) {
                int mid = (low + high) >>> 1;
                double midVal = a[mid];

                if (midVal < key)
                    low = mid + 1;
                else if (midVal > key)
                    high = mid - 1;
                else
                    return mid; // key found
            }
            return low;  // key not found.
        }

        int hzToYScreen(double hz, int screen_height) {

            var near_hz_index = searchHZ(yAxisHz, 0, yAxisHz.length - 1, hz);

            int a = near_hz_index-1;
            int b = near_hz_index;

            if(a < 0 || b > yAxisHz.length - 1) {
                // out of bounds
                return -1;
            }

            double a_value = yAxisHz[a];
            double b_value = yAxisHz[b];

            double t = (hz - a_value) / (b_value - a_value);

            double y_step = (double)screen_height / (double)yAxisHz.length;

            double a_screen = (y_step * (a));
            double b_screen = (y_step * (b));

            double y_screen = lerp(a_screen, b_screen,t);

            return screen_height - (int)y_screen;
        }

        float YScreenToHz(int y, int screen_height) {
            return 0;
        }

        @Override
        public void paint(Graphics g) {
            super.paint(g);

            Dimension size = getSize();

            // flip buffers
            g.drawImage(bufferedImage, 0, 0, size.width, size.height,null);

            int width = bufferedImage.getWidth();
            int height = bufferedImage.getHeight();

            float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;

            g.setColor(Color.RED);
            int line = (int)(currentIndexXAxis * bx);
            g.drawLine(line, 0, line, height);


            for(int i = 0; i < yAxisHz.length; ++i) {

                var y= hzToYScreen(yAxisHz[i], height);

                g.setColor(Color.WHITE);
                g.fillRect(0, y, 30, 3);
            }

            Font f = g.getFont();
            g.setFont(new Font(f.getName(), Font.BOLD, g.getFont().getSize() * 10));
            g.setColor(Color.RED);
            g.drawString(Float.valueOf(mainFrequency).toString() + " Hz", width/2,  100);
            g.setFont(f);

            g.setColor(Color.WHITE);
            var yy = hzToYScreen(mainFrequency, height);
            g.fillRect(0, yy, width, 1);

            //for test
            //var yy2 = hzToYScreen(8117.68, height);
            //g.fillRect(0, yy2, width, 1);
        }

        @Override
        public void componentHidden(ComponentEvent e) {
        }
        @Override
        public void componentMoved(ComponentEvent e) {
        }

        @Override
        public void componentResized(ComponentEvent e) {
            bufferedImage = new BufferedImage(getWidth(),getHeight(), BufferedImage.TYPE_INT_RGB);
            bufferedGraphics = bufferedImage.createGraphics();
        }

        @Override
        public void componentShown(ComponentEvent e) {
        }
    }
}
