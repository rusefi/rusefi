package com.rusefi.openblt;

import java.util.Arrays;

/** A parsed XCP response: positive (0xFF) or negative (0xFE) acknowledgement. */
public final class XcpResponse {
    private final boolean ok;
    private final byte[] data;
    private final int errorCode;

    private XcpResponse(boolean ok, byte[] data, int errorCode) {
        this.ok = ok;
        this.data = data.clone();
        this.errorCode = errorCode;
    }

    public static XcpResponse ok(byte[] data) {
        return new XcpResponse(true, data, 0);
    }

    public static XcpResponse error(byte[] data) {
        int code = data.length >= 2 ? (data[1] & 0xFF) : -1;
        return new XcpResponse(false, data, code);
    }

    public boolean isOk() {
        return ok;
    }

    /** Raw payload including the PID byte. */
    public byte[] data() {
        return data.clone();
    }

    /** XCP error code for negative responses. */
    public int errorCode() {
        return errorCode;
    }

    @Override
    public String toString() {
        if (ok) {
            return "RES " + hex();
        }
        return "ERR 0x" + String.format("%02X", errorCode) + " " + hex();
    }

    private String hex() {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < data.length; i++) {
            if (i > 0) {
                sb.append(' ');
            }
            sb.append(String.format("%02X", data[i]));
        }
        return sb.append(']').toString();
    }
}
