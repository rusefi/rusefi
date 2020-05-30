package com.rusefi.binaryprotocol;

import com.rusefi.FileLog;
import com.rusefi.composite.CompositeEvent;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.Date;
import java.util.List;

/**
 * https://en.wikipedia.org/wiki/Value_change_dump
 *
 * @see FileLog#WIKI_URL
 */
public class VcdStreamFile extends StreamFile {
    private static final String TAG_PRIMARY = "t";
    private static final String TAG_SECONDARY = "s";
    private static final String TAG_TRG = "r";
    private static final String TAG_SYNC = "y";

    private static void writeHeader(Writer writer, Date date) throws IOException {
        writer.write("$date\n");
        writer.write("\t" + date + "\n");
        writer.write("$end\n" +
                "$version\n" +
                "        1.0\n" +
                "$end\n" +
                "$timescale\n" +
                "        1ps\n" +
                "$end\n" +
                "$scope module test $end\n" +
                "$var wire 1 " + TAG_PRIMARY + " PRI_TRG $end\n" +
                "$var wire 1 " + TAG_SECONDARY + " SEC_TRG $end\n" +
                "$var wire 1 " + TAG_TRG + " TRG $end\n" +
                "$var wire 1 " + TAG_SYNC + " SYNC $end\n" +
                "$upscope $end\n" +
                "$enddefinitions $end\n" +
                "$dumpvars\n");
    }

    public static void appendEvents(List<CompositeEvent> events, Writer writer) throws IOException {
        for (CompositeEvent event : events) {
            writer.write("#" + event.getTimestamp() + "\n");
            writer.write(event.isPrimaryTriggerAsInt() + TAG_PRIMARY + "\n");
            writer.write(event.isSecondaryTriggerAsInt() + TAG_SECONDARY + "\n");
            writer.write(event.isTrgAsInt() + TAG_TRG + "\n");
            writer.write(event.isSyncAsInt() + TAG_SYNC + "\n");

        }
        writer.flush();
    }

    public static void writeVCD(List<CompositeEvent> events, Writer writer, Date date) throws IOException {
        writeHeader(writer, date);
        appendEvents(events, writer);
    }

    public static void writeVCD(List<CompositeEvent> events, FileWriter fileWriter) throws IOException {
        writeVCD(events, fileWriter, new Date());
    }

    @Override
    public void append(List<CompositeEvent> events) {
        try {
            if (writer == null) {
                String fileName = FileLog.DIR + "rusEFI_trigger_log_" + FileLog.getDate() + ".vcd";
                writer = new FileWriter(fileName);
                writeHeader(writer, new Date());
            }
            appendEvents(events, writer);
        } catch (IOException e) {
            // ignoring this one
        }
    }
}
