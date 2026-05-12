package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;

public class ProtocolCommand {

    private final byte[] bytes;

    public ProtocolCommand(byte[] bytes) {
        this.bytes = bytes;
    }

    public byte[] getBytes() {
        return bytes;
    }

    public static ProtocolCommand parse(String meta) {
        // todo: support placeholders

        List<Byte> result = new ArrayList<>();

        for (int i = 0; i < meta.length(); i++) {
            char c = meta.charAt(i);
            if (c == '\\') {
                char next = meta.charAt(i + 1);
                if (next != 'x')
                    throw new IllegalArgumentException("Unexpected symbol: " + next);
                String hex = meta.substring(i + 2, i + 4);
                result.add(Byte.parseByte(hex, 16));
                i += 3;
            } else {
                result.add((byte) c);
            }
        }

        byte[] bytes = new byte[result.size()];
        for (int i = 0; i < result.size(); i++)
            bytes[i] = result.get(i);

        return new ProtocolCommand(bytes);

    }
}
