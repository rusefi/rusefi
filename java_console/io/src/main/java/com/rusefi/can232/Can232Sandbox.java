package com.rusefi.can232;

import com.rusefi.io.IoStream;
import com.rusefi.io.serial.BufferedSerialIoStream;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Can232Sandbox {
    private final static Function<Integer, Long> integerLongFunction = integer -> 0L;

    private static final int MAX_VERSION_LENGTH = 256;

    public static void main(String[] args) throws IOException {
        IoStream stream = BufferedSerialIoStream.openPort("COM20");


        String s = getVersion(stream);
        if (s.length() > 3)
            System.out.println("Looks like legit version [" + s + "]");
        writeCan232Command(stream, "S6"); // 500K
        writeCan232Command(stream, "O"); // open

        while (true) {
            Map<Integer, Long> previousTimestamp = new HashMap<>();

            System.out.println("Requesting All");
//            writeCan232Command(stream, "A"); // get all

            int c = 0;
            while (true) {


                StringBuilder response = readCan232Response(stream);

                if (response.charAt(0) == 'T') {
                    CanPacket p = CanPacket.parse(response.toString());

                    Long previousTime = previousTimestamp.computeIfAbsent(p.getId(), integerLongFunction);
                    long now = System.currentTimeMillis();

                    long period = now - previousTime;

                    System.out.println(new Date() + ": T id=" + p + " " + c++ + " period " + period);

                    previousTimestamp.put(p.getId(), now);
                } else {
                    throw new IllegalStateException(response.toString());
                }

                System.out.println("Got " + response);
            }
        }


    }

    @NotNull
    private static String getVersion(IoStream stream) throws IOException {
        writeCan232Command(stream, "V");

        StringBuilder sb = readCan232Response(stream);
        return sb.toString();
    }

    @NotNull
    private static StringBuilder readCan232Response(IoStream stream) throws IOException {
        StringBuilder sb = new StringBuilder();
        while (sb.length() < MAX_VERSION_LENGTH) {
            char c = (char) stream.getDataBuffer().readByte();
            if (c == '\r') {
                break;
            }
            sb.append(c);
        }
        return sb;
    }

    private static void writeCan232Command(IoStream stream, String command) throws IOException {
        byte[] bytes = (command + "\r").getBytes();
        stream.write(bytes);
        stream.flush();
    }
}
