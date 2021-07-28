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

package com.romraider.net;

import javax.swing.JLabel;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class URL extends JLabel implements MouseListener {

    private static final long serialVersionUID = 8972482185527165793L;
    String url = "";

    public URL(String url) {
        super(url);
        this.url = url;
        this.setFont(new Font("Arial", Font.PLAIN, 12));
        this.addMouseListener(this);
    }

    public void paint(Graphics g) {
        super.paint(g);
        Font f = getFont();
        FontMetrics fm = getFontMetrics(f);
        int x1 = 0;
        int y1 = fm.getHeight() + 3;
        int x2 = fm.stringWidth(getText());
        if (getText().length() > 0) {
            g.drawLine(x1, y1, x2, y1);
        }
    }

    public void mouseClicked(MouseEvent e) {
        BrowserControl.displayURL(url);
    }

    public void mousePressed(MouseEvent e) {
    }

    public void mouseReleased(MouseEvent e) {
    }

    public void mouseEntered(MouseEvent e) {
    }

    public void mouseExited(MouseEvent e) {
    }
}