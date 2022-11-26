package com.rusefi.stream;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class LogicdataOutputStream {
    private final OutputStream outputStream;

    public LogicdataOutputStream(OutputStream outputStream) {
        this.outputStream = outputStream;
    }

    public void writeDouble(double value) throws IOException {
        if (value == 0.0) {
            writeByte(0);
        } else {
            writeByte(8);
            // doubles are saved little-endian, sorry Java :)
            ByteBuffer bb = ByteBuffer.allocate(8);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            bb.putDouble(value);
            bb.rewind();
            for (int i = 0; i < 8; i++) {
                writeByte(bb.get());
            }
        }
    }

    // This is the main secret of this format! :)
    public void writeVarLength(long value) throws IOException {
        if (value < 0 || value > 0xFFFFFFFFL) {
            writeAs(value, 8);
        } else if (value == 0) {
            writeByte(0);
        } else if (value <= 0xff) {
            writeAs(value, 1);
        } else if (value <= 0xffff) {
            writeAs(value, 2);
        } else if (value <= 0xffffff) {
            writeAs(value, 3);
        } else {
            writeAs(value, 4);
        }
    }

    private void writeAs(long value, int numBytes) throws IOException {
        if (value == 0) {
            writeByte(0);
        } else {
            writeByte(numBytes);
            for (int i = 0; i < numBytes; i++) {
                writeByte((byte) ((value >> (i * 8)) & 0xff));
            }
        }
    }

    public void close() throws IOException {
        outputStream.close();
    }

    public void flush() throws IOException {
        outputStream.flush();
    }

    public void writeByte(int i) throws IOException {
        outputStream.write(i);
    }

    void writeString(String str) throws IOException {
        writeVarLength(str.length());
        for (char c : str.toCharArray()) {
            writeByte(c);
        }
    }
}
