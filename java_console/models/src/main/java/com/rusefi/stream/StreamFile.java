package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.io.*;
import java.util.List;

public abstract class StreamFile {
    // todo: always write into Writer since it has better API, it's insane to have to references into same stream
    protected OutputStream stream;
    protected Writer writer;

    public StreamFile() {
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                close();
            }
        }));
    }

    public abstract void append(List<CompositeEvent> events);

    public synchronized void close() {
        try {
            writeFooter();
        } catch (IOException e) {
            return;
        }
        if (writer != null) {
            try {
                writer.close();
            } catch (IOException e) {
                // ignoring this one
            }
            writer = null;
        }
        if (stream != null) {
            try {
                stream.close();
            } catch (IOException e) {
                // ignoring this one
            }
            stream = null;
        }
    }

    protected void createFileWriter(String fileName) throws FileNotFoundException {
        stream = new FileOutputStream(fileName);
        writer = new OutputStreamWriter(stream);
    }

    protected void writeFooter() throws IOException {
    }
}
