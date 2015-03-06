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

package com.romraider.swing;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

public class JProgressPane extends JPanel implements PropertyChangeListener{

    private static final long serialVersionUID = -6827936662738014543L;
    JLabel label = new JLabel();
    JProgressBar progressBar = new JProgressBar(JProgressBar.HORIZONTAL, 0, 100);
    String status = "ready";
    int percent = 0;

    public JProgressPane() {

        this.setPreferredSize(new Dimension(500, 18));
        this.setLayout(new BorderLayout(1, 2));
        label.setHorizontalAlignment(JLabel.CENTER);
        label.setText(" Ready...");
        label.setFont(new Font("Tahoma", Font.PLAIN, 11));
        label.setHorizontalAlignment(JLabel.LEFT);
        progressBar.setMinimumSize(new Dimension(200, 50));
        progressBar.setValue(0);

        this.add(progressBar, BorderLayout.WEST);
        this.add(label, BorderLayout.CENTER);

    }

    public void update(String status, int percent) {
        label.setText(" " + status);
        progressBar.setValue(percent);
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public JProgressBar getProgressBar() {
        return this.progressBar;
    }

    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        if("progress" == evt.getPropertyName()) {
            int progress = (Integer) evt.getNewValue();
            progressBar.setValue(progress);
            label.setText(" " + status);
        }
        else{
            ;// do nothing
        }
    }
}