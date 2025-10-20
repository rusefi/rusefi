package com.rusefi.ts_plugin.knock;

import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;

import static org.apache.commons.math3.util.Precision.round;

public class KnockCanvas {

    private final JComponent component = new JComponent() {
        @Override
        public void paint(Graphics g) {
            super.paint(g);

            Dimension size = component.getSize();

            int width = bufferedImage.getWidth();
            int height = bufferedImage.getHeight();
            float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;

            int offset = (int)(currentIndexXAxis * bx);

            // flip buffers
            g.drawImage(bufferedImage,
                0, 0, size.width - offset, size.height,
                offset, 0, size.width, size.height,
                null);
            g.drawImage(bufferedImage, size.width - offset, 0, size.width, size.height,null);

            //debug helper
            /*g.setColor(Color.RED);
            int line = (int)(currentIndexXAxis * bx);
            g.drawLine(line, 0, line, height);*/

            Font f = g.getFont();
            g.setFont(new Font(f.getName(), Font.BOLD, g.getFont().getSize() - 4));

            g.setColor(Color.YELLOW);
            for(int i = 0; i < yAxisHz.length / 8; ++i) {

                int y = hzToYScreen(yAxisHz[i * 8], height);

                g.setColor(Color.orange);
                g.fillRect(0, y, 30, 1);

                double hz = yAxisHz[i * 8];
                g.drawString(Double.valueOf(round(hz, 1)).toString(), 35,  y);
            }

            float mouseFrequency = (float) YScreenToHz(mouse_y, height);

            int mouseSpecX = canvasXToSpectrogramSpace(mouse_x, false);
            int mouseSpecY = canvasYToSpectrogramSpace(mouse_y);

            float mouseAmplitude = spectrogram[mouseSpecX][mouseSpecY];

            //Font f = g.getFont();
            g.setFont(new Font(f.getName(), Font.BOLD, g.getFont().getSize()));

            g.setColor(Color.YELLOW);
            int currentX = width / 4;
            g.drawString("[current]", currentX,  10);
            g.drawString(Float.valueOf(currentFrequency).toString() + " Hz", currentX,  30);
            g.drawString(Float.valueOf(currentAmplitude).toString() + " Amp", currentX,  50);

            g.setColor(Color.RED);
            g.drawString("[peak]", currentX * 2,  10);
            g.drawString(Float.valueOf(peakFrequency).toString() + " Hz", currentX * 2,  30);
            g.drawString(Float.valueOf(peakAmplitude).toString() + " Amp", currentX * 2,  50);

            g.setColor(Color.ORANGE);
            g.drawString("[mouse]", currentX * 3,  10);
            g.drawString(Float.valueOf(mouseFrequency).toString() + " Hz", currentX * 3,  30);
            g.drawString(Float.valueOf(mouseAmplitude).toString() + " Amp", currentX * 3,  50);


            g.setFont(new Font(f.getName(), Font.BOLD, g.getFont().getSize() * 5));
            g.setColor(Color.WHITE);
            if (isCombined) {
                g.drawString("Combined", 10,  30);
            } else {
                g.drawString(Integer.valueOf(number).toString(), 10,  30);
            }

            g.setFont(f);

            g.setColor(Color.RED);
            //g.fillOval(spectrogramSpaceToCanvasX(peakX)-5, spectrogramSpaceToCanvasY(peakY)-5, 10, 10);
            int peakYCanvasSpace = spectrogramSpaceToCanvasY(peakY);
            g.drawPolygon(new int[]{width, width-5, width}, new int[]{peakYCanvasSpace-3, peakYCanvasSpace, peakYCanvasSpace+3}, 3);

            g.setColor(Color.YELLOW);
            int currentYCanvasSpace = hzToYScreen(currentFrequency, height);
            g.drawPolygon(new int[]{width, width-5, width}, new int[]{currentYCanvasSpace-3, currentYCanvasSpace, currentYCanvasSpace+3}, 3);

            g.setColor(Color.DARK_GRAY);
            g.fillRect(0, mouse_y, width, 1);

            g.dispose();
        }
    };
    private final boolean isCombined;

    private BufferedImage bufferedImage;
    private Graphics2D bufferedGraphics;

    private static final int SPECTROGRAM_X_AXIS_SIZE_BASE = 1024 * 4;

    private final int SPECTROGRAM_X_AXIS_SIZE;
    private final float[][] spectrogram;
    private final Color[] colorspace;

    private final int spectrogramYAxisSize;

    private int currentIndexXAxis = 0;

    private final int number;

    public double[] yAxisHz;
    private int yAxisFrequencyStart = -1;
    private float yAxisFrequencyStep = -1;

    private float currentFrequency = 0;
    private float currentAmplitude = 0;

    private int peakY = 0;
    private float peakFrequency = 0;
    private float peakAmplitude = 0;
    private int peakAmplitudeCol = 0;

    private int mouse_x = 0;
    private int mouse_y = 0;

    public KnockCanvas(int number, int divider, boolean isCombined) {
        this.isCombined = isCombined;

        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayoutAndRepaint(component));

        this.number = number;
        SPECTROGRAM_X_AXIS_SIZE = SPECTROGRAM_X_AXIS_SIZE_BASE / divider;

        bufferedImage = new BufferedImage(640,480, BufferedImage.TYPE_INT_RGB);
        bufferedGraphics = bufferedImage.createGraphics();
        ComponentListener componentListener = new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                bufferedImage = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
                bufferedGraphics = bufferedImage.createGraphics();
            }
        };
        component.addComponentListener(componentListener);

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

        yAxisHz = new double[64]; // protocol size

        spectrogramYAxisSize = yAxisHz.length;
        spectrogram = new float[SPECTROGRAM_X_AXIS_SIZE][spectrogramYAxisSize];
    }

    public void setFrequencyStart(int start) {
        boolean needSetup = this.yAxisFrequencyStart < 0;

        this.yAxisFrequencyStart = start;

        if(needSetup) {
            setupFrequencyYAxis();
        }
    }


    public JComponent getComponent() {
        return component;
    }

    public void setFrequencyStep(float step) {

        boolean needSetup = this.yAxisFrequencyStep < 0;

        this.yAxisFrequencyStep = step;

        if(needSetup) {
            setupFrequencyYAxis();
        }
    }

    public void setupFrequencyYAxis() {

        if(this.yAxisFrequencyStep < 0 || this.yAxisFrequencyStart < 0) {
            return;
        }

        for (int i = 0; i < 64; ++i) {
            float y = (float)this.yAxisFrequencyStart + (this.yAxisFrequencyStep * (float)i);
            this.yAxisHz[i] = y;
        }
    }

    public void processValues(float[] values) {

        for (int i = 0; i < spectrogramYAxisSize; ++i) {
            spectrogram[currentIndexXAxis][i] = values[i];
        }

        int width = bufferedImage.getWidth();
        int height = bufferedImage.getHeight();

        float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;

        int maxYIndex = 0;
        float min = Integer.MAX_VALUE;
        float max = 0;
        float minCurrent = min;
        float maxCurrent = max;
        int maxX = 0;
        int maxY = 0;
        for(int x = 0; x < SPECTROGRAM_X_AXIS_SIZE; ++x) {
            for(int y = 0; y < spectrogramYAxisSize; ++y) {
                float value = spectrogram[x][y];
                if(value < min) {
                    min = value;
                }

                if(value > max) {
                    max = value;
                    maxX = x;
                    maxY = y;
                }


                if(x == currentIndexXAxis && value < minCurrent) {
                    minCurrent = value;
                }

                if(x == currentIndexXAxis && value > maxCurrent) {
                    maxCurrent = value;
                    maxYIndex = y;
                }
            }
        }

        currentAmplitude = maxCurrent;
        currentFrequency = (float)this.yAxisHz[maxYIndex];

        if(peakAmplitudeCol == currentIndexXAxis) {
            peakFrequency = 0;
            peakAmplitude = 0;
            peakY = 0;
        }

        if(currentAmplitude > peakAmplitude) {
            peakFrequency = currentFrequency;
            peakAmplitude = currentAmplitude;
            peakY = maxY;
            peakAmplitudeCol = currentIndexXAxis;
        }

        for(int y = 0; y < spectrogramYAxisSize; ++y)
        {
            float value = spectrogram[currentIndexXAxis][y];
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

            //simple draw
            bufferedGraphics.setColor(color);
            int yr = height - (height / spectrogramYAxisSize) * y;
            int xr = (int)(currentIndexXAxis * bx);
            int widthr = ((int)bx) + 1;
            int heightr = (height / spectrogramYAxisSize);
            bufferedGraphics.fillRect(xr, yr, widthr, heightr);
        }

        ++currentIndexXAxis;

        if(currentIndexXAxis >= SPECTROGRAM_X_AXIS_SIZE){
            currentIndexXAxis = 0;
        }
    }

    public void resetPeak() {
        peakFrequency = 0;
        peakAmplitude = 0;
        peakY = 0;
    }

    private double lerp(double start, double end, double t) {
        return start * (1 - t) + end * t;
    }

    private int searchHZ(double[] a, int fromIndex, int toIndex, double key) {
        int low = fromIndex;
        int high = toIndex - 1;

        // bisect
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

    private int hzToYScreen(double hz, int screen_height) {

        int near_hz_index = searchHZ(yAxisHz, 0, yAxisHz.length - 1, hz);

        int a = near_hz_index-1;
        int b = near_hz_index;

        if(a < 0) {
            a = 0;
        }

        if(b > yAxisHz.length - 1) {
            b = yAxisHz.length - 1;
        }

        // interpolate value between two y axis coordinates
        double a_value = yAxisHz[a];
        double b_value = yAxisHz[b];

        double t = a == b ? 0 : (hz - a_value) / (b_value - a_value);

        double y_step = (double)screen_height / (double)(yAxisHz.length-1);

        double a_screen = (y_step * (a));
        double b_screen = (y_step * (b));

        double y_screen = lerp(a_screen, b_screen,t);

        return screen_height - (int)y_screen;
    }

    private double YScreenToHz(int screenY, int screen_height) {

        if(screenY < 0 || screenY > screen_height) {
            return 0;
        }

        double relative = ((double)screen_height - (double)screenY) / ((double)screen_height);
        double spectrogramRelative = (double)(spectrogramYAxisSize) * relative;

        int spectrogramY = (int)spectrogramRelative;
        if(spectrogramY < 0) {
            spectrogramY = 0;
        }
        if(spectrogramY >= yAxisHz.length) {
            spectrogramY = yAxisHz.length - 1;
        }

        int spectrogramY0 = spectrogramY + 1;

        if(spectrogramY0 < 0) {
            spectrogramY0 = spectrogramY;
        }
        if(spectrogramY0 >= yAxisHz.length) {
            spectrogramY0 = yAxisHz.length - 1;
        }

        //double t = (spectrogramRelative - spectrogramY0) / (spectrogramY - spectrogramY0);

        double a_hz = yAxisHz[spectrogramY];
        double b_hz = yAxisHz[spectrogramY0];

        int a_screen = hzToYScreen(a_hz, screen_height);
        int b_screen = hzToYScreen(b_hz, screen_height);

        double t = a_screen == b_screen ? 0 : ((double)screenY - (double)b_screen) / ((double)a_screen - (double)b_screen);

        double hz = lerp(a_hz, b_hz, 1.0 - t);

        return hz;
    }

/* dead code?
    int spectrogramSpaceToCanvasX(int x) {
        int width = bufferedImage.getWidth();
        float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;
        return (int)((float)x * bx);
    }
*/

    private int spectrogramSpaceToCanvasY(int y) {
        int height = bufferedImage.getHeight();
        float by = (float)height / (float)spectrogramYAxisSize;
        return (int)(((float)spectrogramYAxisSize - 1 - (float)y) * by);
    }

    private int canvasXToSpectrogramSpace(int x, boolean reverseOffset) {
        int width = bufferedImage.getWidth();
        float bx = (float)width / (float)SPECTROGRAM_X_AXIS_SIZE;

        int offset = (int)(currentIndexXAxis * bx);

        int imageX;

        if(reverseOffset) {
            if(offset < x) {
                imageX = x + offset;
                if(imageX > width) {
                    imageX -= width;
                }
            }
            else {
                imageX = x - (width - offset);
                if(imageX < 0) {
                    imageX = Math.abs(imageX);
                }
            }
        } else {
            if(offset < x) {
                imageX = x - offset;
            }
            else {
                imageX = x + (width - offset);
            }
        }

        int i = (int) ((float) imageX / bx);
        // we shall be in [0, width) range
        return Integer.min(width - 1, i);
    }

    private int canvasYToSpectrogramSpace(int y) {
        int height = bufferedImage.getHeight();
        float by = (float)height / (float)spectrogramYAxisSize;
        return spectrogramYAxisSize - 1 - (int)((float)y / by);
    }
/* some unfinished feature?
    void setupFrequencyByClick(int x, int y) {

    }
*/
    float[] getCurrentMouseMagnitudes() {
        int spectrogramSpaceX = this.canvasXToSpectrogramSpace(this.mouse_x, true);
        return spectrogram[spectrogramSpaceX];
    }

    public void setMousePosition(int x, int y) {
        mouse_x = x;
        mouse_y = y;
    }
}
