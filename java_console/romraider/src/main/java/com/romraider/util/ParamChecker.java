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

package com.romraider.util;

import java.util.Collection;
import java.util.Map;

public final class ParamChecker {

    private ParamChecker() {
    }

    public static void checkNotNull(Object param, String paramName) {
        if (param == null) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null");
        }
    }

    public static void checkNotNull(Object... params) {
        for (int i = 0; i < params.length; i++) {
            checkNotNull(params[i], "arg" + i);
        }
    }

    public static void checkNotNullOrEmpty(String param, String paramName) {
        if (isNullOrEmpty(param)) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null or empty");
        }
    }

    public static void checkNotNullOrEmpty(Object[] param, String paramName) {
        if (param == null || param.length == 0) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null or empty");
        }
    }

    public static void checkNotNullOrEmpty(Collection<?> param, String paramName) {
        if (param == null || param.isEmpty()) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null or empty");
        }
    }

    public static void checkNotNullOrEmpty(Map<?, ?> param, String paramName) {
        if (param == null || param.isEmpty()) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null or empty");
        }
    }

    public static void checkGreaterThanZero(int param, String paramName) {
        if (param <= 0) {
            throw new IllegalArgumentException("Parameter " + paramName + " must be > 0");
        }
    }

    public static void checkNotNullOrEmpty(byte[] param, String paramName) {
        if (param == null || param.length == 0) {
            throw new IllegalArgumentException("Parameter " + paramName + " must not be null or empty");
        }
    }

    public static void checkBit(int bit) {
        if (!isValidBit(bit)) {
            throw new IllegalArgumentException("Bit must be between 0 and 7 inclusive.");
        }
    }

    public static boolean isNullOrEmpty(String param) {
        return param == null || param.length() == 0;
    }

    public static boolean isNullOrEmpty(Collection<?> param) {
        return param == null || param.isEmpty();
    }

    public static boolean isNullOrEmpty(Map<?, ?> param) {
        return param == null || param.isEmpty();
    }

    public static boolean isValidBit(int bit) {
        return bit >= 0 && bit <= 7;
    }
}
