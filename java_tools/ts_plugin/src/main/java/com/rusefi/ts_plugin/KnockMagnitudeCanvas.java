package com.rusefi.ts_plugin;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import static org.apache.commons.math3.util.Precision.round;


public class KnockMagnitudeCanvas extends JComponent implements ComponentListener {

    JComponent dd = this;
    //--------------------------------------

    private BufferedImage bufferedImage;
    private Graphics2D bufferedGraphics;

    public double yAxisHz[];
    int yAxisFequencyStart = -1;
    float yAxisFequencyStep = -1;


    int[] xPoints = new int[66]; // + 2 last points for polygon
    int[] yPoints = new int[66]; // + 2 last points for polygon

    public KnockMagnitudeCanvas() {

        SwingUtilities.invokeLater(new Runnable(){
            public void run() {
                dd.repaint();
            }
        });

        bufferedImage = new BufferedImage(640,480, BufferedImage.TYPE_INT_RGB);
        bufferedGraphics = bufferedImage.createGraphics();
        this.addComponentListener(this);

        yAxisHz = new double[64]; // protocol size
    }

    public void setFrequencyStart(int start) {
        boolean needSetup = this.yAxisFequencyStart < 0;

        this.yAxisFequencyStart = start;

        if(needSetup) {
            setupFrequencyyAxis();
        }
    }

    public void setFrequencyStep(float step) {

        boolean needSetup = this.yAxisFequencyStep < 0;

        this.yAxisFequencyStep = step;

        if(needSetup) {
            setupFrequencyyAxis();
        }
    }

    public void setupFrequencyyAxis() {

        if(this.yAxisFequencyStep < 0 || this.yAxisFequencyStart < 0) {
            return;
        }

        for(var i = 0; i < 64; ++i) {
            float y = (float)this.yAxisFequencyStart + (this.yAxisFequencyStep * (float)i);
            this.yAxisHz[i] = y;
        }
    }

    private int searchPeak(int[] yPoints) {

        int max = 256;
        int index = -1;

        for(int i = 0; i < 64; ++i) {
            int value = yPoints[i];

            if(value < max) {
                max = value;
                index = i;
            }
        }

        return index;
    }

    public void processValues(float magnitudes[]) {

        int width = bufferedImage.getWidth();
        int height = bufferedImage.getHeight();

        bufferedGraphics.clearRect(0,0, width, height);

        float bx = (float)width / (float)64;

        for(int i = 0; i < 64; ++i) {

            xPoints[i] = (int)(bx * i);

            float normalized = magnitudes[i]/255.f;
            float y = height * normalized;

            yPoints[i] = (int)(height - y);
        }
        //last point 2 always in corners
        xPoints[64] = xPoints[63];
        yPoints[64] = height;
        xPoints[65] = 0;
        yPoints[65] = height;

        bufferedGraphics.setColor(Color.lightGray);
        bufferedGraphics.drawPolygon(xPoints, yPoints, 66);
        bufferedGraphics.setColor(Color.gray);
        bufferedGraphics.fillPolygon(xPoints, yPoints, 66);

        int index = searchPeak(yPoints);
        int xLine = xPoints[index];

        bufferedGraphics.setColor(Color.yellow);
        bufferedGraphics.drawLine(xLine, 0, xLine, height);

        double hz = this.yAxisHz[index];
        bufferedGraphics.drawString(Double.valueOf(round(hz, 1)).toString(), xLine,  10);

        Font defaultFont = bufferedGraphics.getFont();
        Font font = new Font(null, Font.PLAIN, 14);
        AffineTransform rotate90 = new AffineTransform();
        rotate90.rotate(-Math.PI / 2);
        Font rotatedFont = font.deriveFont(rotate90);
        bufferedGraphics.setFont(rotatedFont);

        for(int i = 0; i < 64; ++i) {
            int x = (int) (bx * i);
            double hzZxis = this.yAxisHz[i];
            bufferedGraphics.setColor(Color.white);
            bufferedGraphics.drawLine(x, height, x, height - 10);
            bufferedGraphics.drawString(Double.valueOf(round(hzZxis, 1)).toString(), x,  height - 20);
        }
        bufferedGraphics.setFont(defaultFont);

        this.repaint();
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);

        Dimension size = getSize();

        // flip buffers
        g.drawImage(bufferedImage, 0, 0, size.width, size.height,null);
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
