package com.rusefi.ui.knock;


import javax.swing.*;
import java.awt.*;

public class KnockScale extends JPanel {

    double yAxisHz[];

    KnockPane knockPane;
    public KnockScale(KnockPane pane) {
        super(new FlowLayout());

        knockPane = pane;
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2 = (Graphics2D) g;

        Dimension size = getSize();

        Font f = g.getFont();
        g.setFont(new Font(f.getFontName(), Font.BOLD, 22));

        for(int i = 0; i < knockPane.getCanvas().yAxisHz.length; ++i) {
            int dy = size.height / knockPane.getCanvas().yAxisHz.length;
            double hz = knockPane.getCanvas().yAxisHz[i];

            int y = knockPane.getCanvas().hzToYScreen(hz, size.height);

            g.setColor(Color.BLACK);

            g.drawString(Integer.valueOf((int)hz).toString() + " Hz", getWidth() - 130, y + 6);

            g.setColor(Color.BLACK);
            g2.fillRect(getWidth() - 30, y, getWidth(), 3);
        }
    }
}
