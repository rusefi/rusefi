package com.rusefi;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

/**
 * Dirty hack of using java instead of python
 */
public class RandomToolHondaKPacketAnalyzer {
    private static final Map<Integer, Integer> headerToLength = new TreeMap<>();
    private static final Map<Integer, String> comments = new HashMap<>();
    private static final Map<Integer, Set<String>> packets = new TreeMap<>();
    private static boolean individualMode = false;
    private static int fileCounter;


    public static void main(String[] args) throws IOException {
        register((char) 1, 4, "request 1");
        register('A' /* 65 */, 4, "request 65");
        // Quit?
        register('Q', 4, "BCM QUIT");

        register((char) 0, 5, "status 0");
        register('@' /* 64 */, 5, "status 64");

        register((char) 2, 7, "response 2");
        register('B' /* 66 */, 7, "response 66");
        register((char) 130, 7, "response 130 rare");


        String folder = "C:\\stuff\\rusefi_documentation\\OEM-Docs\\Honda\\E24-SEFMJ-white-civic-si";
        for (String file : new File(folder).list((dir, name) -> {
            return
                    //                    name.contains("1-key-on-car-off.csv")
//                handle("C:\\stuff\\rusefi_documentation\\OEM-Docs\\Honda\\E24-SEFMJ-white-civic-si\\2-key-removed-30-seconds.csv");
//                        name.contains("3-door-open-wakes-bus-up.csv")
//                                            name.contains("4-idling.csv")
//                    name.contains("5-stop-and-restart.csv")
//                name.contains("6-high-rpm.csv")
                    //name.contains("7-ac-on-off.csv")
                    //name.contains("9-re") && name.endsWith(".csv")
//                    name.contains("less-cold") && name.endsWith(".csv")
//                    name.contains("12-w") && name.endsWith(".csv")
            name.contains("29-") && name.endsWith(".csv")
//                    name.contains("-ac-") && name.endsWith(".csv")

//            !name.startsWith("__") && name.endsWith(".csv")
                    ;
        })) {
            fileCounter++;
            if (individualMode)
                clearState();
            handle(folder + File.separator + file);
            if (individualMode)
                printPacketPayloads();
        }

        if (!individualMode) {
            System.out.println("Total " + fileCounter + " files");
            printPacketPayloads();
        }

    }

    private static void clearState() {
        packets.clear();
        pairs.clear();
    }

    private static void printPacketPayloads() {
        System.out.println();
        System.out.println();
        System.out.println();
        for (int header : packets.keySet()) {
            Set<String> payloads = packets.get(header);
            System.out.println(comments.get(header) + ": Header " + dualSid(header, "/") + ": packet of length " + (headerToLength.get(header) - 1) + ". Total " + payloads.size() + " payload variations");
            System.out.println(payloads);
            System.out.println();
        }
    }

    public static String dualSid(int sid, String separator) {
        char c = (char) sid;
        return String.format("%d%s0x%x%s%s", sid, separator, sid, separator, "" + c);
    }

    private static final Map<String, AtomicInteger> pairs = new TreeMap<>();

    private static void handle(String fileName) throws IOException {
        System.out.println("Handling " + fileName);
        List<String> list = Files.lines(new File(fileName).toPath()).collect(Collectors.toList());

        Map<Integer, AtomicInteger> perHeaderCounter = new TreeMap<>();

        Integer previousHeader = null;


        int total = 0;
        for (int i = 1; i < list.size(); i++) {
            String s = list.get(i).trim();
            if (s.isEmpty())
                continue;
            total++;

            String payload = getPayoad(s);

            int header = decode(payload);

//            System.out.println("Looking at " + payload + " " + header);

            if (previousHeader != null) {
                if (header == 130) {
                    System.out.println("rare thing 130 " + s);
                    System.out.println("with " + list.get(i + 1).trim());
                    System.out.println("with " + list.get(i + 2).trim());
                }
                if (previousHeader == 0 && header == 65)
                    System.out.println("??? interesting 65/2/" + s);
                if (previousHeader == 65 && header == 2)
                    System.out.println("*** interesting 65/2/" + s);
                if (previousHeader == 66 && header == 1)
                    System.out.println("    interesting 66/1/" + s);
                pairs.computeIfAbsent(previousHeader + " followed by " + header, s1 -> new AtomicInteger()).incrementAndGet();
                pairs.computeIfAbsent(header + " after " + previousHeader, s1 -> new AtomicInteger()).incrementAndGet();
            }

            if (headerToLength.containsKey(header)) {

                AtomicInteger counter = perHeaderCounter.computeIfAbsent(header, integer -> new AtomicInteger());
                counter.incrementAndGet();

                int length = headerToLength.get(header);
//                System.out.println("Header " + header + " has len " + length);
                i = consume(header, list, i, length);


                previousHeader = header;

                continue;

            }
            throw new UnsupportedOperationException("Unexpected starts [" + payload + "] header=" + header + " " + list.subList(i, i + 5));
        }
        System.out.println("Distribution in " + fileName + ": total=" + total + ": " + xx(perHeaderCounter));

        for (Map.Entry<String, AtomicInteger> e : pairs.entrySet()) {
            System.out.println(e);
        }
    }

    private static String xx(Map<Integer, AtomicInteger> perHeaderCounter) {
        Iterator<Map.Entry<Integer, AtomicInteger>> i = perHeaderCounter.entrySet().iterator();
        if (!i.hasNext())
            return "{}";

        StringBuilder sb = new StringBuilder();
        sb.append('{');
        for (; ; ) {
            Map.Entry<Integer, AtomicInteger> e = i.next();
            Integer key = e.getKey();
            AtomicInteger value = e.getValue();
            sb.append(dualSid(key, "/"));
            sb.append('=');
            sb.append(value);
            if (!i.hasNext())
                return sb.append('}').toString();
            sb.append(',').append(' ');
        }
    }

    private static String getPayoad(String s) {
        return s.split(",")[1];
    }

    private static void register(char a, int len, String comment) {
        int header = a;
        comments.put(header, comment);
        System.out.println("Registering chat [" + a + "] code " + header + " as len " + len);
        headerToLength.put(header, len);
    }

    /**
     * @return next packet start index
     */
    private static int consume(int header, List<String> list, int start, int count) {
        if (list.size() < start + count)
            return list.size() - 1;
        StringBuilder packet = new StringBuilder();
        for (int i = start; i < start + count - 1; i++) {
            int payload = decode(getPayoad(list.get(i)));
            packet.append(String.format("0x%x ", payload));
        }

        Set<String> set = packets.computeIfAbsent(header, integer -> new TreeSet<>());
        set.add(packet.toString());

//        System.out.println("Consumed " + count + " at " + start + " " + packet);
        return start + count - 1;
    }

    private static int decode(String x) {
        if (x.startsWith("'"))
            return Integer.parseInt(x.substring(1, x.length() - 1));
        return Character.valueOf(x.charAt(0));
    }
}
