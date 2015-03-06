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

package com.romraider.logger.ecu.ui.handler.injector;

import com.romraider.logger.ecu.comms.query.Response;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandler;
import com.romraider.logger.ecu.ui.tab.injector.InjectorTab;
import static java.lang.Math.abs;
import static java.lang.System.currentTimeMillis;
import org.apache.log4j.Logger;
import javax.swing.SwingUtilities;
import java.util.Set;

public final class InjectorUpdateHandler implements DataUpdateHandler {
    private static final Logger LOGGER = Logger.getLogger(InjectorUpdateHandler.class);
    private static final String PULSE_WIDTH_16 = "E28";
    private static final String PULSE_WIDTH_32 = "E60";
    private static final String ENGINE_LOAD_16 = "E2";
    private static final String ENGINE_LOAD_32 = "E32";
    private InjectorTab injectorTab;
    private double lastMafv;
    private long lastUpdate;

    public synchronized void registerData(LoggerData loggerData) {
    }

    public synchronized void handleDataUpdate(Response response) {
        if (injectorTab.isRecordData()
                && (containsData(response, PULSE_WIDTH_16, ENGINE_LOAD_16)
                || containsData(response, PULSE_WIDTH_32, ENGINE_LOAD_32))) {
            boolean valid = true;

            // cl/ol check
            if ((containsData(response, "E3") || containsData(response, "E33"))) {
                double clOl = -1;
                if (containsData(response, "E3")) {
                    clOl = (int) findValue(response, "E3");
                    LOGGER.trace("INJ:[CL/OL:E3]:  " + clOl);
                }
                if (containsData(response, "E33")) {
                    clOl = (int) findValue(response, "E33");
                    LOGGER.trace("INJ:[CL/OL:E33]: " + clOl);
                }
                valid = injectorTab.isValidClOl(clOl);
                LOGGER.trace("INJ:[CL/OL]:     " + valid);
            }

            // afr check
            if (valid && containsData(response, "P58")) {
                double afr = findValue(response, "P58");
                LOGGER.trace("INJ:[AFR:P58]: " + afr);
                valid = injectorTab.isValidAfr(afr);
                LOGGER.trace("INJ:[AFR]:     " + valid);
            }

            // rpm check
            if (valid && containsData(response, "P8")) {
                double rpm = findValue(response, "P8");
                LOGGER.trace("INJ:[RPM:P8]: " + rpm);
                valid = injectorTab.isValidRpm(rpm);
                LOGGER.trace("INJ:[RPM]:    " + valid);
            }

            // maf check
            if (valid && containsData(response, "P12")) {
                double maf = findValue(response, "P12");
                LOGGER.trace("INJ:[MAF:P12]: " + maf);
                valid = injectorTab.isValidMaf(maf);
                LOGGER.trace("INJ:[MAF]:     " + valid);
            }

            // intake air temp check
            if (valid && containsData(response, "P11")) {
                double temp = findValue(response, "P11");
                LOGGER.trace("INJ:[IAT:P11]: " + temp);
                valid = injectorTab.isValidIntakeAirTemp(temp);
                LOGGER.trace("INJ:[IAT]:     " + valid);
            }

            // coolant temp check
            if (valid && containsData(response, "P2")) {
                double temp = findValue(response, "P2");
                LOGGER.trace("INJ:[CT:P2]: " + temp);
                valid = injectorTab.isValidCoolantTemp(temp);
                LOGGER.trace("INJ:[CT]:    " + valid);
            }

            // dMAFv/dt check
            if (valid && containsData(response, "P18")) {
                double mafv = findValue(response, "P18");
                long now = currentTimeMillis();
                double mafvChange = abs((mafv - lastMafv) / (now - lastUpdate) * 1000);
                LOGGER.trace("INJ:[dMAFv/dt]: " + mafvChange);
                valid = injectorTab.isValidMafvChange(mafvChange);
                LOGGER.trace("INJ:[dMAFv/dt]: " + valid);
                lastMafv = mafv;
                lastUpdate = now;
            }

            // tip-in throttle check
            if (valid && (containsData(response, "E23") || containsData(response, "E54"))) {
                double tipIn = -1;
                if (containsData(response, "E23")) {
                    tipIn = findValue(response, "E23");
                    LOGGER.trace("INJ:[TIP:E23]: " + tipIn);
                }
                if (containsData(response, "E54")) {
                    tipIn = findValue(response, "E54");
                    LOGGER.trace("INJ:[TIP:E54]: " + tipIn);
                }
                valid = injectorTab.isValidTipInThrottle(tipIn);
                LOGGER.trace("INJ:[TIP]:     " + valid);
            }

            if (valid) {
                final double pulseWidth = containsData(response, PULSE_WIDTH_16) ? findValue(response, PULSE_WIDTH_16) : findValue(response, PULSE_WIDTH_32);
                double load = containsData(response, ENGINE_LOAD_16) ? findValue(response, ENGINE_LOAD_16) : findValue(response, ENGINE_LOAD_32);
                double stoichAfr = injectorTab.getFuelStoichAfr();
                double density = injectorTab.getFuelDensity();
                final double fuelcc = load / 2 / stoichAfr * 1000 / density;
                LOGGER.trace("Injector Data: " + pulseWidth + "ms, " + fuelcc + "cc");
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        injectorTab.addData(pulseWidth, fuelcc);
                    }
                });
            }
        }
    }

    private boolean containsData(Response response, String... ids) {
        Set<LoggerData> datas = response.getData();
        for (String id : ids) {
            boolean found = false;
            for (LoggerData data : datas) {
                if (data.getId().equals(id)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    private double findValue(Response response, String id) {
        for (final LoggerData loggerData : response.getData()) {
            if (id.equals(loggerData.getId())) {
                return response.getDataValue(loggerData);
            }
        }
        throw new IllegalStateException("Expected data item " + id + " not in response.");
    }

    public synchronized void deregisterData(LoggerData loggerData) {
    }

    public synchronized void cleanUp() {
    }

    public synchronized void reset() {
    }

    public void setInjectorTab(InjectorTab injectorTab) {
        this.injectorTab = injectorTab;
    }
}