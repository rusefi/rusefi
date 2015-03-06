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

package com.romraider.maps;

import static com.romraider.xml.RomAttributeParser.parseByteValue;
import static com.romraider.xml.RomAttributeParser.parseIntegerValue;

import com.romraider.Settings;

public class RomChecksum {

    public static void calculateRomChecksum(byte[] input, int storageAddress, int dataSize) {
        for (int i = storageAddress; i < storageAddress + dataSize; i+=12) {
            byte[] newSum = calculateChecksum(input,
                    (int)parseByteValue(input, 0, i  , 4, true),
                    (int)parseByteValue(input, 0, i+4, 4, true));
            System.arraycopy(newSum, 0, input, i + 8, 4);
        }
    }

    public static int validateRomChecksum(byte[] input, int storageAddress, int dataSize) {
        int result = 0;
        int[] results = new int[dataSize / 12];
        int j = 0;
        for (int i = storageAddress; i < storageAddress + dataSize; i+=12) {
            int startAddr = (int)parseByteValue(input, 0, i  , 4, true);
            int endAddr   = (int)parseByteValue(input, 0, i+4, 4, true);
            int diff      = (int)parseByteValue(input, 0, i+8, 4, true);
            if (j == 0 &&
                    startAddr == 0 &&
                    endAddr   == 0 &&
                    diff      == Settings.CHECK_TOTAL) {
                return result = -1; // -1, all checksums disabled if the first one is disabled
            }
            else {
                results[j] = validateChecksum(input, startAddr, endAddr, diff);
            }
            j++;
        }
        for (j = 0; j < (dataSize / 12); j++) {
            if (results[j] != 0) {
                return j + 1; // position of invalid checksum
            }
        }
        return result; // 0, all checksums are valid
    }

    private static int validateChecksum(byte[] input, int startAddr, int endAddr, int diff) {
        int byteSum = 0;
        for (int i=startAddr; i<endAddr; i+=4) {
            byteSum += (int)parseByteValue(input, 0, i, 4, true);
        }
        int result = (Settings.CHECK_TOTAL - diff - byteSum);
        return result;
    }

    private static byte[] calculateChecksum(byte[] input, int startAddr, int endAddr) {
        int byteSum = 0;
        for (int i=startAddr; i<endAddr; i+=4) {
            byteSum += (int)parseByteValue(input, 0, i, 4, true);
        }
        return parseIntegerValue((Settings.CHECK_TOTAL - byteSum), 0, 4);
    }
}