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

package com.romraider.util;

import java.nio.ByteBuffer;
import java.util.List;

public final class ByteUtil {

    private ByteUtil() {
        throw new UnsupportedOperationException();
    }

    public static int asUnsignedInt(byte b) {
        return asUnsignedInt(new byte[]{b});
    }

    public static int asSignedInt(byte[] bytes) {
        int i = 0;
        for (int j = 0; j < bytes.length; j++) {
            if (j == 0) {
                i |= bytes[j];
            }
            else {
                i <<= 8;
                i |= bytes[j] & 0xFF;
            }
        }
        return i;
    }

    public static int asUnsignedInt(byte[] bytes) {
        int i = 0;
        for (int j = 0; j < bytes.length; j++) {
            if (j > 0) {
                i <<= 8;
            }
            i |= bytes[j] & 0xFF;
        }
        return i;
    }

    public static byte asByte(int i) {
        return Integer.valueOf(i).byteValue();
    }

    public static float asFloat(byte[] b, int offset, int length) {
        final ByteBuffer buf = ByteBuffer.wrap(b, offset, length);
        return buf.getFloat();
    }

    public static int asInt(byte b) {
        return Byte.valueOf(b).intValue();
    }

    public static boolean matchOnes(byte b, int mask) {
        return (b & mask) == mask;
    }

    public static boolean matchZeroes(byte b, int mask) {
        return (b & mask) == 0;
    }

    public static void byteListToBytes(List<Byte> buffer, byte[] response) {
        for (int i = 0; i < buffer.size(); i++) {
            response[i] = buffer.get(i);
        }
    }

    public static int indexOfBytes(byte[] bytes, byte[] pattern) {
        int[] failure = computeFailure(pattern);
        int j = 0;
        for (int i = 0; i < bytes.length; i++) {
            while (j > 0 && pattern[j] != bytes[i]) {
                j = failure[j - 1];
            }
            if (pattern[j] == bytes[i]) {
                j++;
            }
            if (j == pattern.length) {
                return i - pattern.length + 1;
            }
        }
        return -1;
    }

    private static int[] computeFailure(byte[] pattern) {
        int[] failure = new int[pattern.length];
        int j = 0;
        for (int i = 1; i < pattern.length; i++) {
            while (j>0 && pattern[j] != pattern[i]) {
                j = failure[j - 1];
            }
            if (pattern[j] == pattern[i]) {
                j++;
            }
            failure[i] = j;
        }
        return failure;
    }
}
