package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.io.*;
import java.util.List;

public abstract class StreamFile {
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
        if (writer != null) {
            try {
                writeFooter(writer);
                writer.close();
            } catch (IOException e) {
                // ignoring this one
            }
        }
        writer = null;
        if (stream != null) {
            try {
                stream.close();
            } catch (IOException e) {
                // ignoring this one
            }
        }
    }

    protected void createFileWriter(String fileName) throws FileNotFoundException {
        stream = new FileOutputStream(fileName);
        writer = new OutputStreamWriter(stream);
    }

    protected void writeFooter(Writer writer) throws IOException {
    }
}
