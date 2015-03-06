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

package com.romraider.logger.ecu.ui.handler.file;

import static com.romraider.util.ParamChecker.checkNotNull;
import static java.util.Collections.synchronizedList;
import static java.util.Collections.synchronizedMap;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import com.romraider.logger.ecu.comms.query.Response;
import com.romraider.logger.ecu.definition.ConvertorUpdateListener;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.MessageListener;
import com.romraider.logger.ecu.ui.StatusChangeListener;

public final class FileUpdateHandlerImpl implements FileUpdateHandler, ConvertorUpdateListener {
    private final Map<LoggerData, Integer> loggerDatas = synchronizedMap(new LinkedHashMap<LoggerData, Integer>());
    private final List<StatusChangeListener> listeners = synchronizedList(new ArrayList<StatusChangeListener>());
    private final FileLogger fileLogger;
    private Line currentLine = new Line(loggerDatas.keySet());

    public FileUpdateHandlerImpl(MessageListener messageListener) {
        fileLogger = new FileLoggerImpl(messageListener);
    }

    @Override
    public synchronized void addListener(StatusChangeListener listener) {
        checkNotNull(listener, "listener");
        listeners.add(listener);
    }

    @Override
    public synchronized void registerData(LoggerData loggerData) {
        if (loggerDatas.keySet().contains(loggerData)) {
            loggerDatas.put(loggerData, loggerDatas.get(loggerData) + 1);
        } else {
            loggerDatas.put(loggerData, 1);
            resetLine();
            writeHeaders();
        }
    }

    @Override
    public synchronized void handleDataUpdate(Response response) {
        if (fileLogger.isStarted()) {
            for (LoggerData loggerData : response.getData()) {
                currentLine.updateParamValue(loggerData, loggerData.getSelectedConvertor().format(response.getDataValue(loggerData)));
            }
            if (currentLine.isFull()) {
                fileLogger.writeLine(currentLine.values(), response.getTimestamp());
                resetLine();
            }
        }
    }

    @Override
    public synchronized void deregisterData(LoggerData loggerData) {
        if (loggerDatas.keySet().contains(loggerData) && loggerDatas.get(loggerData) > 1) {
            loggerDatas.put(loggerData, loggerDatas.get(loggerData) - 1);
        } else {
            loggerDatas.remove(loggerData);
            resetLine();
            writeHeaders();
        }
    }

    @Override
    public synchronized void cleanUp() {
        if (fileLogger.isStarted()) {
            fileLogger.stop();
        }
    }

    @Override
    public synchronized void reset() {
    }

    @Override
    public synchronized void notifyConvertorUpdate(LoggerData updatedLoggerData) {
        resetLine();
        writeHeaders();
    }

    @Override
    public synchronized void start() {
        if (!fileLogger.isStarted()) {
            fileLogger.start();
            notifyListeners(true);
            writeHeaders();
        }
    }

    @Override
    public synchronized void stop() {
        if (fileLogger.isStarted()) {
            fileLogger.stop();
            notifyListeners(false);
        }
    }

    private void resetLine() {
        currentLine = new Line(loggerDatas.keySet());
    }

    private void writeHeaders() {
        if (fileLogger.isStarted()) {
            fileLogger.writeHeaders(currentLine.headers());
        }
    }

    private void notifyListeners(boolean loggingToFile) {
        for (StatusChangeListener listener : listeners) {
            if (loggingToFile) {
                listener.loggingData();
            } else {
                listener.readingData();
            }
        }
    }

    private final class Line {
        private final Locale userLocale = Locale.getDefault();
        private static final char COMMA = ',';
        private static final char SEMICOLON = ';';
        private final Set<String> locales = new HashSet<String>(
                Arrays.asList(new String[] {
                        "be_BY","bg_BG","ca_ES","cs_CZ","da_DK","de_AT","de_CH","de_DE","de_LU",
                        "el_CY","el_GR","es_AR","es_BO","es_CL","es_CO","es_EC","es_ES","es_PE",
                        "es_PY","es_UY","es_VE","et_EE","fi_FI","fr_BE","fr_CA","fr_CH","fr_FR",
                        "fr_LU","hr_HR","hu_HU","in_ID","is_IS","it_CH","it_IT","lt_LT","lv_LV",
                        "mk_MK","nl_BE","nl_NL","no_NO","no_NO_NY","pl_PL","pt_BR","pt_PT",
                        "ro_RO","ru_RU","sk_SK","sl_SI","sq_AL","sr_BA","sr_CS","sr_ME","sr_RS",
                        "sv_SE","tr_TR","uk_UA","vi_VN"
                }
                        ));
        private final Map<LoggerData, String> loggerDataValues;
        private final char delimiter;
        {
            if (locales.contains(userLocale.toString())) {
                delimiter = SEMICOLON;
            }
            else {
                delimiter = COMMA;
            }
        }

        public Line(Set<LoggerData> loggerDatas) {
            this.loggerDataValues = new LinkedHashMap<LoggerData, String>();
            for (LoggerData loggerData : loggerDatas) {
                loggerDataValues.put(loggerData, null);
            }
        }

        public synchronized void updateParamValue(LoggerData loggerData, String value) {
            if (loggerDataValues.containsKey(loggerData)) {
                loggerDataValues.put(loggerData, value);
            }
        }

        public synchronized boolean isFull() {
            for (LoggerData loggerData : loggerDataValues.keySet()) {
                if (loggerDataValues.get(loggerData) == null) {
                    return false;
                }
            }
            return true;
        }

        public synchronized String headers() {
            StringBuilder buffer = new StringBuilder();
            for (LoggerData loggerData : loggerDataValues.keySet()) {
                buffer.append(delimiter).append(loggerData.getName()).append(" (")
                .append(loggerData.getSelectedConvertor().getUnits()).append(')');
            }
            return buffer.toString();
        }

        public synchronized String values() {
            StringBuilder buffer = new StringBuilder();
            for (LoggerData loggerData : loggerDataValues.keySet()) {
                String value = loggerDataValues.get(loggerData);
                buffer.append(delimiter).append(value);
            }
            return buffer.toString();
        }
    }
}
