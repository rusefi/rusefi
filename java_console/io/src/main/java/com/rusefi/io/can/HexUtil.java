package com.rusefi.io.can;

public class HexUtil {
    private static final String HEX_STRING = "0123456789ABCDEF";
    public static final byte[] HEX_BYTE_ARRAY = HEX_STRING.getBytes();
    public static final char[] HEX_CHAR_ARRAY = HexUtil.HEX_STRING.toCharArray();

    public static byte[] asBytes(String hex) {
        if (hex.indexOf(' ') >= 0) {
            hex = hex.replaceAll(" ", "");
        }
        if (hex.startsWith("0x")) {
            hex = hex.substring(2);
        }
        return hexToBytes(hex);
    }

    public static String asString(byte[] input) {
        StringBuilder sb = new StringBuilder();
        for (byte b : input) {
            int i = b & 0xff;
            sb.append(HEX_STRING.charAt(i / 16));
            sb.append(HEX_STRING.charAt(i % 16));
        }
        return sb.toString();
    }

    public static byte[] hexToBytes(CharSequence s) {
        return hexToBytes(s, 0);
    }

    private static byte[] hexToBytes(CharSequence s, int off) {
        byte[] bs = new byte[off + (1 + s.length()) / 2];
        hexToBytes(s, bs, off);
        return bs;
    }

    public static void hexToBytes(CharSequence s, byte[] out, int off) throws NumberFormatException, IndexOutOfBoundsException {
        int slen = s.length();
        if ((slen % 2) != 0) {
            byte nibble = (byte) Character.digit(s.charAt(0), 16);
            out[off] = nibble;
            hexToBytes(s.subSequence(1, s.length()), out, off + 1);
            return;
        }
        if (out.length < off + slen / 2) {
            throw new IndexOutOfBoundsException("Output buffer too small for input (" + out.length + "<" + off + slen / 2 + ")");
        }
        // Safe to assume the string is even length
        byte firstNibble, secondNibble;
        for (int i = 0; i < slen; i += 2) {
            firstNibble = (byte) Character.digit(s.charAt(i), 16);
            secondNibble = (byte) Character.digit(s.charAt(i + 1), 16);
            if ((firstNibble < 0) || (secondNibble < 0)) {
                // todo: huh? when does char produce a negative byte?!
                throw new NumberFormatException();
            }
            out[off + i / 2] = (byte) (firstNibble << 4 | secondNibble);
        }
    }
}
