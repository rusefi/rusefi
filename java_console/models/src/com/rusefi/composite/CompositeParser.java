package com.rusefi.composite;

import com.opensr5.ini.field.EnumIniField;
import com.rusefi.binaryprotocol.VcdStreamFile;
import com.rusefi.config.generated.Fields;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class CompositeParser {

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

    public static void writeVCD(List<CompositeEvent> events, Writer writer, Date date) throws IOException {
        VcdStreamFile.writeHeader(writer, date);
        VcdStreamFile.appendEvents(events, writer);
    }

    public static void writeVCD(List<CompositeEvent> events, FileWriter fileWriter) throws IOException {
        writeVCD(events, fileWriter, new Date());
    }
}
