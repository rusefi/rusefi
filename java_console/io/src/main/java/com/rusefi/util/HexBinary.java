package com.rusefi.util;

import com.rusefi.io.can.HexUtil;

import java.util.List;

public class HexBinary {
    public static String printHexBinary(byte[] data) {
        if (data == null)
            return "(null)";
        char[] hexCode = HexUtil.HEX_CHAR_ARRAY;

        StringBuilder r = new StringBuilder(data.length * 2);
        for (byte b : data) {
            r.append(hexCode[(b >> 4) & 0xF]);
            r.append(hexCode[(b & 0xF)]);
            r.append(' ');
        }
        return r.toString();
    }

    public static String printHexBinary(List<Byte> data) {
        if (data == null)
            return "(null)";
        char[] hexCode = HexUtil.HEX_CHAR_ARRAY;

        StringBuilder r = new StringBuilder(data.size() * 2);
        for (byte b : data) {
            r.append(hexCode[(b >> 4) & 0xF]);
            r.append(hexCode[(b & 0xF)]);
            r.append(' ');
        }
        return r.toString();
    }

    public static String printByteArray(byte[] data) {
        StringBuilder sb = new StringBuilder();
        for (byte b : data) {
            if (Character.isJavaIdentifierPart(b)) {
                sb.append((char) b);
            } else {
                sb.append(' ');
            }
        }
        return printHexBinary(data) + sb;
    }
}
