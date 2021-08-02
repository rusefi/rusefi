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

package com.romraider.logger.ecu.ui.swing.layout;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Insets;

public final class BetterFlowLayout extends FlowLayout {
    private static final long serialVersionUID = -6784712723817241270L;

    public BetterFlowLayout() {
        super();
    }

    public BetterFlowLayout(int align) {
        super(align);
    }

    public BetterFlowLayout(int align, int hgap, int vgap) {
        super(align, hgap, vgap);
    }

    @Override
    public Dimension preferredLayoutSize(Container target) {
        return betterPreferredSize(target);
    }

    @Override
    public Dimension minimumLayoutSize(Container target) {
        return betterPreferredSize(target);
    }

    public Dimension betterPreferredSize(Container target) {
        synchronized (target.getTreeLock()) {
            Insets insets = target.getInsets();
            int maxwidth = target.getWidth() - (insets.left + insets.right + getHgap() * 2);
            int nmembers = target.getComponentCount();
            int x = 0, y = insets.top + getVgap();
            int rowh = 0;
            for (int i = 0; i < nmembers; i++) {
                Component m = target.getComponent(i);
                if (m.isVisible()) {
                    Dimension d = m.getPreferredSize();
                    m.setSize(d.width, d.height);
                    if ((x == 0) || ((x + d.width) <= maxwidth)) {
                        if (x > 0) x += getHgap();
                        x += d.width;
                        rowh = Math.max(rowh, d.height);
                    } else {
                        x = d.width;
                        y += getVgap() + rowh;
                        rowh = d.height;
                    }
                }
            }
            return new Dimension(maxwidth, y + rowh + getVgap());
        }
    }
}