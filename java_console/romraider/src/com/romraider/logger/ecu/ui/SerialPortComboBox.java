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

package com.romraider.logger.ecu.ui;

import static com.romraider.util.ParamChecker.checkNotNull;

import java.util.Set;
import java.util.TreeSet;

import javax.swing.JComboBox;

import com.romraider.io.serial.port.SerialPortRefreshListener;
import com.romraider.util.SettingsManager;

public final class SerialPortComboBox extends JComboBox implements SerialPortRefreshListener {
    private static final long serialVersionUID = 5693976713268676676L;

    public SerialPortComboBox() {
    }

    @Override
    public synchronized void refreshPortList(Set<String> ports, String defaultSelectedPort) {
        checkNotNull(ports);
        boolean changeDetected = ports.isEmpty() || ports.size() != getItemCount();
        if (!changeDetected) {
            for (int i = 0; i < getItemCount(); i++) {
                String port = (String) getItemAt(i);
                if (!ports.contains(port)) {
                    changeDetected = true;
                    break;
                }
            }
            if (!changeDetected) {
                Set<String> comboPorts = new TreeSet<String>();
                for (int i = 0; i < getItemCount(); i++) {
                    comboPorts.add((String) getItemAt(i));
                }
                for (String port : ports) {
                    if (!comboPorts.contains(port)) {
                        changeDetected = true;
                        break;
                    }
                }
            }
        }
        if (changeDetected) {
            String selectedPort = (String) getSelectedItem();
            if (selectedPort == null) {
                selectedPort = defaultSelectedPort;
            }
            removeAllItems();
            if (!ports.isEmpty()) {
                for (String port : ports) {
                    addItem(port);
                }
                if (selectedPort != null && ports.contains(selectedPort)) {
                    setSelectedItem(selectedPort);
                    SettingsManager.getSettings().setLoggerPort(selectedPort);
                } else {
                    setSelectedIndex(0);
                    SettingsManager.getSettings().setLoggerPort((String) getItemAt(0));
                }
            }
        }
    }

    @Override
    public void setSelectedItem(Object object) {
        if (contains(object)) {
            super.setSelectedItem(object);
        } else {
            if (getItemCount() >= 1) {
                setSelectedIndex(0);
            }
        }
    }

    private boolean contains(Object object) {
        for (int i = 0; i < getItemCount(); i++) {
            if (getItemAt(i) != null && getItemAt(i).equals(object)) {
                return true;
            }
        }
        return false;
    }
}
