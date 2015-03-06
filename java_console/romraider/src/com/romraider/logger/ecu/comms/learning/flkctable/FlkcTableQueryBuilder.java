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

package com.romraider.logger.ecu.comms.learning.flkctable;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.romraider.logger.ecu.comms.learning.parameter.Parameter;
import com.romraider.logger.ecu.comms.query.EcuQuery;
import com.romraider.logger.ecu.comms.query.EcuQueryImpl;
import com.romraider.logger.ecu.definition.EcuAddress;
import com.romraider.logger.ecu.definition.EcuAddressImpl;
import com.romraider.logger.ecu.definition.EcuData;
import com.romraider.logger.ecu.definition.EcuDataConvertor;
import com.romraider.logger.ecu.definition.EcuParameterImpl;
import com.romraider.logger.ecu.ui.paramlist.ParameterRow;
import com.romraider.util.HexUtil;

/**
 * Build an EcuQuery for each of the cells in the FLKC RAM table. 
 */
public class FlkcTableQueryBuilder {
    private static final Logger LOGGER =
            Logger.getLogger(FlkcTableQueryBuilder.class);

    public FlkcTableQueryBuilder() {
    }

    /**
     * Build an EcuQuery for each cell of the FLKC RAM table. <i>Note this
     * returns an extra null query for column 0 of each row which is later
     * populated with the row header (RPM Ranges) data.</i>
     * @param flkc - a ParameterRow item that helps to identify the
     * ECU bitness and provide a Converter for the raw data.
     * @param flkcAddr - the address in RAM of the start of the table.
     * @param rows - the number of rows in the table.
     * @param columns - the number of columns in the table.
     * @return EcuQueries divided into groups to query each row separately to
     * avoid maxing out the ECU send/receive buffer.
     */
    public final List<List<EcuQuery>> build(
            ParameterRow flkc,
            int flkcAddr,
            int rows,
            int columns) {

        final List<List<EcuQuery>> flkcQueryRows = new ArrayList<List<EcuQuery>>();
        int checksummed = 1;
        int dataSize = 1;
        if (Parameter.fromValue(flkc.getLoggerData().getId()) == Parameter.E41 ||
            Parameter.fromValue(flkc.getLoggerData().getId()) == Parameter.E173) {
            checksummed = 2;
            dataSize = 4;
        }
        LOGGER.debug(
                String.format(
                        "FLKC Data format rows:%d col:%d checksummed:%d " +
                        "dataSize:%d FLKC:%s",
                        rows, columns, checksummed, dataSize,
                        flkc.getLoggerData().getId()));

        int i = 0;
        for (int j = 0; j < rows; j++) {
            final List<EcuQuery> flkcQueryCols = new ArrayList<EcuQuery>();
            flkcQueryCols.add(null);
            for (int k = 0; k < columns; k++) {
                String id = "flkc-r" + j + "c" + k;
                final String addrStr =
                        HexUtil.intToHexString(
                                flkcAddr + (i * dataSize * checksummed));
                LOGGER.debug(
                        String.format(
                                "FLKC Data row:%d col:%d addr:%s",
                                j, k, addrStr));
                final EcuAddress ea = new EcuAddressImpl(addrStr, dataSize, -1);
                final EcuParameterImpl epi =
                    new EcuParameterImpl(id, addrStr, id, ea,
                        new EcuDataConvertor[] {
                            flkc.getLoggerData().getSelectedConvertor()
                        }
                    );
                flkcQueryCols.add(new EcuQueryImpl((EcuData) epi));
                i++;
            }
            flkcQueryRows.add(flkcQueryCols);
        }
        return flkcQueryRows;
    }
}
