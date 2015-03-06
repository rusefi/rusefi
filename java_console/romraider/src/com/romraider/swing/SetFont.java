/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

package com.romraider.swing;

import java.awt.Font;

import javax.swing.JComponent;

/**
 * Methods of this class can be used to change the style and/or size of a
 * JComponent within the same font family.
 */
public class SetFont {
    public SetFont() {}

    /**
     * Set the style of JComponent to plain.
     * @param component - item to change.
     */
    public static final void plain(JComponent component) {
        component.setFont(
                new Font(
                        component.getFont().getFamily(),
                        Font.PLAIN,
                        component.getFont().getSize()
                )
        );
    }

    /**
     * Set the style of JComponent to plain using new size.
     * @param component - item to change.
     * @param size - the new font size.
     */
    public static final void plain(JComponent component, int size) {
        component.setFont(
                new Font(
                        component.getFont().getFamily(),
                        Font.PLAIN,
                        size
                )
        );
    }

    /**
     * Set the style of JComponent to bold.
     * @param component - item to change.
     */
    public static final void bold(JComponent component) {
        component.setFont(
                new Font(
                        component.getFont().getFamily(),
                        Font.BOLD,
                        component.getFont().getSize()
                )
        );
    }

    /**
     * Set the style of JComponent to bold using new size.
     * @param component - item to change.
     * @param size - the new font size.
     */
    public static final void bold(JComponent component, int size) {
        component.setFont(
                new Font(
                        component.getFont().getFamily(),
                        Font.BOLD,
                        size
                )
        );
    }
}
