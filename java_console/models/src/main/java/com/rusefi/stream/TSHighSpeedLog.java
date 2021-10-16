package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;
import com.rusefi.rusEFIVersion;

import java.io.*;
import java.util.List;

public class TSHighSpeedLog extends StreamFile {
    private final String fileName;
    private int prevTime = 0;

    public TSHighSpeedLog(String fileName) {
        this.fileName = fileName;
    }

    private static void writeHeader(Writer writer) throws IOException {
        writer.write("#Firmware: console" + rusEFIVersion.CONSOLE_VERSION + " firmware " + rusEFIVersion.firmwareVersion.get() + "\n");
        writer.write("PriLevel,SecLevel,Trigger,Sync,Time,ToothTime,coil,inj\n" +
                "Flag,Flag,Flag,Flag,ms,ms,Flag,Flag\n");
    }

    @Override
    public void append(List<CompositeEvent> events) {
        try {
            if (writer == null) {
                createFileWriter(fileName);
                writeHeader(writer);
            }
            for (CompositeEvent event : events) {
                writer.write(event.isPrimaryTriggerAsInt() + "," + event.isSecondaryTriggerAsInt() + "," + event.isTrgAsInt() + "," + event.isSyncAsInt() + ",");
                int delta = event.getTimestamp() - prevTime;
                writer.write(event.getTimestamp() / 1000.0 + "," + delta / 1000.0);

                writer.write("," + event.isCoil() + "," + event.isInjector());

                writer.write("\n");
                prevTime = event.getTimestamp();
            }
            writer.flush();

        } catch (IOException e) {
            // ignoring IO exceptions
        }
    }

    @Override
    protected void writeFooter() throws IOException {
        if (writer != null)
            writer.write("MARK 028\n");
    }
}
