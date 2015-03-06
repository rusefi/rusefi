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

package com.romraider.logger.ecu.ui.playback;

import com.romraider.logger.ecu.comms.query.Response;
import com.romraider.logger.ecu.comms.query.ResponseImpl;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandler;
import static com.romraider.util.ThreadUtil.sleep;
import java.io.File;
import java.util.List;

//TODO: Finish me.
public final class PlaybackManagerImpl implements PlaybackManager {
    private final List<? extends LoggerData> loggerDatas;
    private final DataUpdateHandler[] dataUpdateHandlers;

    public PlaybackManagerImpl(List<? extends LoggerData> loggerDatas, DataUpdateHandler... dataUpdateHandlers) {
        this.loggerDatas = loggerDatas;
        this.dataUpdateHandlers = dataUpdateHandlers;
    }

    public void load(File file) {
        // TODO: Finish me!
        for (DataUpdateHandler handler : dataUpdateHandlers) {
            handler.registerData(loggerDatas.get(10));
            handler.registerData(loggerDatas.get(20));
            handler.registerData(loggerDatas.get(30));
        }
    }

    public void play() {
        double d = 0.0;
        while (true) {
            for (DataUpdateHandler handler : dataUpdateHandlers) {
                Response response = new ResponseImpl();
                response.setDataValue(loggerDatas.get(10), d);
                response.setDataValue(loggerDatas.get(20), d);
                response.setDataValue(loggerDatas.get(30), d);
                handler.handleDataUpdate(response);
                d += 100.0;
            }
            sleep(100L);
        }
    }

    public void play(int speed) {
        throw new UnsupportedOperationException();
    }

    public void step(int increment) {
        throw new UnsupportedOperationException();
    }

    public void pause() {
        throw new UnsupportedOperationException();
    }

    public void stop() {
        throw new UnsupportedOperationException();
    }

    public void reset() {
        throw new UnsupportedOperationException();
    }
}
