package com.rusefi.io.can;

public class HexUtil {
    public static byte[] asBytes(String hex) {
        if (hex.indexOf(' ') >= 0) {
            hex = hex.replaceAll(" ", "");
        }
        if (hex.startsWith("0x")) {
            hex = hex.substring(2);
        }
        return hexToBytes(hex);
    }

    private static byte[] hexToBytes(String s) {
        return hexToBytes(s, 0);
    }

    private static byte[] hexToBytes(String s, int off) {
        byte[] bs = new byte[off + (1 + s.length()) / 2];
        hexToBytes(s, bs, off);
        return bs;
    }

    private static void hexToBytes(String s, byte[] out, int off) throws NumberFormatException, IndexOutOfBoundsException {
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
}
