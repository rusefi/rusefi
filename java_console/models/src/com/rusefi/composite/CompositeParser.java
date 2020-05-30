package com.rusefi.composite;

import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.generated.Fields;

import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;
import java.io.Writer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class CompositeParser {

    public static final String TAG_PRIMARY = "t";
    public static final String TAG_SECONDARY = "s";
    public static final String TAG_TRG = "r";
    public static final String TAG_SYNC = "y";

    public static List<CompositeEvent> parse(byte[] response) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(response);
        byteBuffer.order(ByteOrder.BIG_ENDIAN);
        System.out.println("Got composite " + response.length);
        int ptr = 1;

        List<CompositeEvent> events = new ArrayList<>();

        while (ptr + Fields.COMPOSITE_PACKET_SIZE <= response.length) {

            int timestamp = byteBuffer.getInt(ptr);
            byte flags = byteBuffer.get(ptr + 4);
            System.out.println(timestamp + " " + flags);

            boolean primaryTrigger = EnumIniField.getBit(flags, 0);
            boolean secondaryTrigger = EnumIniField.getBit(flags, 1);
            boolean trg = EnumIniField.getBit(flags, 2);
            boolean sync = EnumIniField.getBit(flags, 3);

            ptr += Fields.COMPOSITE_PACKET_SIZE;

            events.add(new CompositeEvent(timestamp, primaryTrigger, secondaryTrigger, trg, sync));
        }
        return events;
    }

    /**
     * https://en.wikipedia.org/wiki/Value_change_dump
     */
    public static void writeVCD(List<CompositeEvent> events, Writer writer, Date date) throws IOException {
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

        for (CompositeEvent event : events) {
            writer.write("#" + event.getTimestamp() + "\n");
            writer.write(event.isPrimaryTriggerAsInt() + TAG_PRIMARY + "\n");
            writer.write(event.isSecondaryTriggerAsInt() + TAG_SECONDARY + "\n");
            writer.write(event.isTrgAsInt() + TAG_TRG + "\n");
            writer.write(event.isSyncAsInt() + TAG_SYNC + "\n");

        }
        writer.flush();
    }

    public static void writeVCD(List<CompositeEvent> events, FileWriter fileWriter) throws IOException {
           writeVCD(events, fileWriter, new Date());
    }
}
