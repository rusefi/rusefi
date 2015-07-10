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

package com.romraider.util.proxy;

import static com.romraider.util.ParamChecker.checkNotNull;
import static java.lang.System.currentTimeMillis;
import org.apache.log4j.Logger;
import static org.apache.log4j.Logger.getLogger;
import java.lang.reflect.Method;

public final class TimerWrapper implements Wrapper {
    private static final Logger LOGGER = getLogger(TimerWrapper.class);
    private final Object delegate;

    public TimerWrapper(Object delegate) {
        checkNotNull(delegate);
        this.delegate = delegate;
    }

    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        long start = currentTimeMillis();
        try {
            return method.invoke(delegate, args);
        } finally {
            long time = currentTimeMillis() - start;
            log(method, time);
        }
    }

    private void log(Method method, long time) {
        String c = delegate.getClass().getSimpleName();
        String m = method.getName();
        LOGGER.error("[TIMER] - " + c + "." + m + ": " + time + "ms");
    }
}
