/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2014 RomRaider.com
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

public final class HexUtil {

    private HexUtil() {
    }

    public static String asHex(byte b) {
        return asHex(new byte[]{b});
    }

    public static String asHex(byte[] in) {
        return bytesToHex(in).toUpperCase();
    }

    public static byte[] asBytes(String hex) {
        if (hex.indexOf(' ') >= 0) {
            hex = hex.replaceAll(" ", "");
        }
        if (hex.startsWith("0x")) {
            hex = hex.substring(2);
        }
        return hexToBytes(hex);
    }

    public static String bytesToHex(byte[] bs, int off, int length) {
        StringBuffer sb = new StringBuffer(length * 2);
        bytesToHexAppend(bs, off, length, sb);
        return sb.toString();
    }

    public static void bytesToHexAppend(byte[] bs, int off, int length, StringBuffer sb) {
        sb.ensureCapacity(sb.length() + length * 2);
        for (int i = off; (i < (off + length)) && (i < bs.length); i++) {
            sb.append(Character.forDigit((bs[i] >>> 4) & 0xf, 16));
            sb.append(Character.forDigit(bs[i] & 0xf, 16));
        }
    }

    public static String bytesToHex(byte[] bs) {
        return bytesToHex(bs, 0, bs.length);
    }

    public static byte[] hexToBytes(String s) {
        return hexToBytes(s, 0);
    }

    public static byte[] hexToBytes(String s, int off) {
        byte[] bs = new byte[off + (1 + s.length()) / 2];
        hexToBytes(s, bs, off);
        return bs;
    }

    public static void hexToBytes(String s, byte[] out, int off) throws NumberFormatException, IndexOutOfBoundsException {
        int slen = s.length();
        if ((slen % 2) != 0) {
            s = '0' + s;
        }
        if (out.length < off + slen / 2) {
            throw new IndexOutOfBoundsException("Output buffer too small for input (" + out.length + "<" + off + slen / 2 + ")");
        }
        // Safe to assume the string is even length
        byte b1, b2;
        for (int i = 0; i < slen; i += 2) {
            b1 = (byte) Character.digit(s.charAt(i), 16);
            b2 = (byte) Character.digit(s.charAt(i + 1), 16);
            if ((b1 < 0) || (b2 < 0)) {
                throw new NumberFormatException();
            }
            out[off + i / 2] = (byte) (b1 << 4 | b2);
        }
    }

    public static int hexToInt(String input) {
        if (input.length() > 2 && input.substring(0, 2).equalsIgnoreCase("0x")) {
            return Integer.parseInt(input.substring(2), 16);
        } else {
            return Integer.parseInt(input, 16);
        }
    }

    public static String intToHexString(int input) {
        return "0x" + Integer.toHexString(input).toUpperCase();
    }
}
