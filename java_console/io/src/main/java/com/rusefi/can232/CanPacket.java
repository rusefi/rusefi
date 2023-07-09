package com.rusefi.can232;

import org.jetbrains.annotations.NotNull;

import java.util.Arrays;

public class CanPacket {
    private final int id;
    private final byte[] payload;

    public CanPacket(int id, byte[] payload) {
        this.id = id;
        this.payload = payload;
    }

    @NotNull
    static CanPacket parse(String string) {
        String extId = string.substring(1, 9);
        String len = string.substring(9, 10);
        String payloadString = string.substring(10);

        int id = Integer.parseInt(extId, 16);
        byte[] payload = new byte[Integer.parseInt(len)];

        CanPacket p = new CanPacket(id, payload);
        return p;
    }

    public int getId() {
        return id;
    }

    public byte[] getPayload() {
        return payload;
    }

    @Override
    public String toString() {
        return "CanPacket{" +
                "id=" + String.format("%x", id) +
                ", payload=" + Arrays.toString(payload) +
                '}';
    }
}
