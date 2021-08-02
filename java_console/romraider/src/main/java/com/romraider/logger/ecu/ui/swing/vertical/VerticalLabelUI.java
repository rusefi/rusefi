/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.logger.ecu.ui.swing.vertical;

import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.plaf.basic.BasicLabelUI;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;

public final class VerticalLabelUI extends BasicLabelUI {
    private static Rectangle PAINT_ICON_R = new Rectangle();
    private static Rectangle PAINT_TEXT_R = new Rectangle();
    private static Rectangle PAINT_VIEW_R = new Rectangle();
    private static Insets PAINT_VIEW_INSETS = new Insets(0, 0, 0, 0);
    protected boolean clockwise;

    static {
        labelUI = new VerticalLabelUI(false);
    }

    public VerticalLabelUI(boolean clockwise) {
        super();
        this.clockwise = clockwise;
    }

    public Dimension getPreferredSize(JComponent c) {
        Dimension dim = super.getPreferredSize(c);
        return new Dimension(dim.height, dim.width);
    }

    public void paint(Graphics graphics, JComponent component) {
        JLabel label = (JLabel) component;
        String text = label.getText();
        Icon icon = (label.isEnabled()) ? label.getIcon() : label.getDisabledIcon();

        if ((icon == null) && (text == null)) return;

        FontMetrics fm = graphics.getFontMetrics();
        PAINT_VIEW_INSETS = component.getInsets(PAINT_VIEW_INSETS);
        PAINT_VIEW_R.x = PAINT_VIEW_INSETS.left;
        PAINT_VIEW_R.y = PAINT_VIEW_INSETS.top;

        // Use inverted height & width
        PAINT_VIEW_R.height = component.getWidth() - (PAINT_VIEW_INSETS.left + PAINT_VIEW_INSETS.right);
        PAINT_VIEW_R.width = component.getHeight() - (PAINT_VIEW_INSETS.top + PAINT_VIEW_INSETS.bottom);

        PAINT_ICON_R.x = PAINT_ICON_R.y = PAINT_ICON_R.width = PAINT_ICON_R.height = 0;
        PAINT_TEXT_R.x = PAINT_TEXT_R.y = PAINT_TEXT_R.width = PAINT_TEXT_R.height = 0;

        String clippedText = layoutCL(label, fm, text, icon, PAINT_VIEW_R, PAINT_ICON_R, PAINT_TEXT_R);
        int textWidth = fm.stringWidth(clippedText);

        Graphics2D g2 = (Graphics2D) graphics;
        AffineTransform tr = g2.getTransform();
        if (clockwise) {
            g2.rotate(Math.PI / 2);
            g2.translate(component.getHeight() / 2 - textWidth / 2, -component.getWidth());
        } else {
            g2.rotate(-Math.PI / 2);
            g2.translate(-component.getHeight() / 2 - textWidth / 2, 0);
        }

        if (icon != null) icon.paintIcon(component, graphics, PAINT_ICON_R.x, PAINT_ICON_R.y);

        if (text != null) {
            int textX = PAINT_TEXT_R.x;
            int textY = PAINT_TEXT_R.y + fm.getAscent();

            if (label.isEnabled()) paintEnabledText(label, graphics, clippedText, textX, textY);
            else paintDisabledText(label, graphics, clippedText, textX, textY);
        }

        g2.setTransform(tr);
    }
}
