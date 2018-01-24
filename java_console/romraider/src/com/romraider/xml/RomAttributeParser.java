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

// Parses attributes from ROM XML

package com.romraider.xml;

import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.romraider.Settings;

public final class RomAttributeParser {

    private RomAttributeParser() {
    }

    public static int parseEndian(String input) {
        if (input.equalsIgnoreCase("big") || input.equalsIgnoreCase(String.valueOf(Settings.ENDIAN_BIG))) {
            return Settings.ENDIAN_BIG;
        }
        else if (input.equalsIgnoreCase("little") || input.equalsIgnoreCase(String.valueOf(Settings.ENDIAN_LITTLE))) {
            return Settings.ENDIAN_LITTLE;
        }
        else {
            return Settings.ENDIAN_LITTLE;
        }
    }

    public static int parseHexString(String input) {
        if (input.equals("0")) {
            return 0;
        }
        else if (input.length() > 2 && input.substring(0, 2).equalsIgnoreCase("0x")) {
            return Integer.parseInt(input.substring(2), 16);
        }
        else {
            return Integer.parseInt(input, 16);
        }
    }

    public static int parseStorageType(String input) {
        if (input.equalsIgnoreCase("float")) {
            return Settings.STORAGE_TYPE_FLOAT;
        }
        else if (input.startsWith("uint")) {
            return Integer.parseInt(input.substring(4)) / 8;
        }
        else if (input.startsWith("int")) {
            return Integer.parseInt(input.substring(3)) / 8;
        }
        else {
            return Integer.parseInt(input);
        }
    }

    public static boolean parseStorageDataSign(String input) {
        if (input.startsWith("int")) {
            return true;
        }
        else {
            return false;
        }
    }

    public static int parseScaleType(String input) {
        if (input.equalsIgnoreCase("inverse")) {
            return Settings.INVERSE;
        }
        else {
            return Settings.LINEAR;
        }
    }

    public static int parseTableType(String input) {
        if (input.equalsIgnoreCase("3D") || input.equalsIgnoreCase(String.valueOf(Settings.TABLE_3D))) {
            return Settings.TABLE_3D;
        }
        else if (input.equalsIgnoreCase("2D") || input.equalsIgnoreCase(String.valueOf(Settings.TABLE_2D))) {
            return Settings.TABLE_2D;
        }
        else if (input.equalsIgnoreCase("X Axis") || input.equalsIgnoreCase("Static X Axis") || input.equalsIgnoreCase(String.valueOf(Settings.TABLE_X_AXIS))) {
            return Settings.TABLE_X_AXIS;
        }
        else if (input.equalsIgnoreCase("Y Axis") || input.equalsIgnoreCase("Static Y Axis") || input.equalsIgnoreCase(String.valueOf(Settings.TABLE_Y_AXIS))) {
            return Settings.TABLE_Y_AXIS;
        }
        else {
            return Settings.TABLE_1D;
        }
    }

    public static long parseByteValue(byte[] input, int endian, int address, int length, boolean signed) throws ArrayIndexOutOfBoundsException, IndexOutOfBoundsException {
        try {
            long output = 0L;
            ByteBuffer bb = ByteBuffer.wrap(input, address, length);
            if (endian == Settings.ENDIAN_LITTLE) {
                bb.order(ByteOrder.LITTLE_ENDIAN);
            }
            switch (length) {
            case 1:
                output = bb.get();
                break;
            case 2:
                output = bb.getShort();
                break;
            case 4:
                output = bb.getInt();
                break;
            }
            if (!signed) {
                switch (length) {
                case 1:
                    output = output & 0xff;
                    break;
                case 2:
                    output = output & 0xffff;
                    break;
                case 4:
                    output = output & 0xffffffffL;
                    break;
                }
            }
            return output;
        } catch (IndexOutOfBoundsException ex) {
            throw ex;
        }
    }

    public static byte[] parseIntegerValue(int input, int endian, int length) {
        try {
            ByteBuffer bb = ByteBuffer.allocate(length);
            if (endian == Settings.ENDIAN_LITTLE) {
                bb.order(ByteOrder.LITTLE_ENDIAN);
            }
            switch (length) {
            case 1:
                bb.put((byte) input);
                break;
            case 2:
                bb.putShort((short) input);
                break;
            case 4:
                bb.putInt(input);
                break;
            }
            return bb.array();
        }
        catch (BufferOverflowException ex) {
            throw new BufferOverflowException();
        }
    }

    public static int parseFileSize(String input) throws NumberFormatException {
        try {
            return Integer.parseInt(input);
        } catch (NumberFormatException ex) {
            if (input.substring(input.length() - 2).equalsIgnoreCase("kb")) {
                return Integer.parseInt(input.substring(0, input.length() - 2)) * 1024;
            }
            else if (input.substring(input.length() - 2).equalsIgnoreCase("mb")) {
                return Integer.parseInt(input.substring(0, input.length() - 2)) * 1024 * 1024;
            }
            throw new NumberFormatException("While parsing [" + input + "]");
        }
    }

    public static byte[] floatToByte(float input, int endian) {
        byte[] output = new byte[4];
        ByteBuffer bb = ByteBuffer.wrap(output, 0, 4);
        if (endian == Settings.ENDIAN_LITTLE) {
            bb.order(ByteOrder.BIG_ENDIAN);
        } else {
            bb.order(ByteOrder.LITTLE_ENDIAN);
        }
        bb.putFloat(input);
        return bb.array();
    }

    public static float byteToFloat(byte[] input, int endian) {
        ByteBuffer bb = ByteBuffer.wrap(input, 0, 4);
        if (endian == Settings.ENDIAN_LITTLE) {
            bb.order(ByteOrder.BIG_ENDIAN);
        } else {
            bb.order(ByteOrder.LITTLE_ENDIAN);
        }
        return bb.getFloat();
    }
}