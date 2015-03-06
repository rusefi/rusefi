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

package com.romraider.logger.ecu.ui.tab;

import com.romraider.logger.ecu.definition.EcuParameter;
import com.romraider.logger.ecu.definition.EcuSwitch;
import com.romraider.logger.ecu.definition.ExternalData;
import javax.swing.JPanel;
import java.util.List;

/**
 * Interface for Logger tabs that have the following:
 * <p/>
 * Control panel where logging information and constraints are set
 * ChartPanel where graph of data is displayed
 */
public interface Tab {
    JPanel getPanel();

    boolean isRecordData();

    void addData(double xData, double yData);

    void setEcuParams(List<EcuParameter> params);

    void setEcuSwitches(List<EcuSwitch> switches);

    void setExternalDatas(List<ExternalData> external);
}
