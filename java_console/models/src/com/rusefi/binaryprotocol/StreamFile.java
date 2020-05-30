package com.rusefi.binaryprotocol;

import com.rusefi.composite.CompositeEvent;

import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

public abstract class StreamFile {
    protected FileWriter writer;

    public StreamFile() {
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                close();
            }
        }));
    }

    abstract void append(List<CompositeEvent> events);

    public synchronized void close() {
        if (writer != null) {
            try {
                writeFooter(writer);
                writer.close();
            } catch (IOException e) {
                // ignoring this one
            }
        }
        writer = null;
    }

    protected void writeFooter(FileWriter writer) throws IOException {
    }
}
