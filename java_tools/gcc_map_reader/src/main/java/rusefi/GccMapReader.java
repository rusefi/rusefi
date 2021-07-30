package rusefi;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * This is an utility to print the information from GCC linked .map file
 *
 * @author Andrey Belomutskiy
 * 10/16/13
 */
public class GccMapReader {
    private static final Pattern MULTI_LINE_PATTERN = Pattern.compile(".*0x(\\S*)(.*)");
    private static final String[] REGIONS = {"bss", "text", "data", "rodata"};
    private static final Pattern SINGLE_LINE_PATTERN = Pattern.compile(".*\\.(bss|text|data|rodata)\\.(\\S*).*0x.*0x(\\S*)(.*)");
    static final String START_OF_DATA_TAG = "Linker script and memory map";

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("file name parameter expected");
            System.exit(-1);
        }
        String fileName = args[0];
        BufferedReader fr = new BufferedReader(new FileReader(fileName));

        String line;
        List<String> lines = new ArrayList<>();

        while ((line = fr.readLine()) != null)
            lines.add(line);

        debug("Got " + lines.size() + " lines");

        for (String region : REGIONS)
            processAndPrint(lines, region);
    }

    private static void processAndPrint(List<String> lines, String region) {
        List<Record> records = process(lines, region);

        records.sort(Comparator.reverseOrder());

        int totalSize = 0;
        for (Record record : records) {
            System.out.println(record);
            totalSize += record.size;
        }

        System.out.println(region + " Total size: " + totalSize);
    }

    static List<Record> process(List<String> lines, String region) {

        List<Record> result = new ArrayList<>();
        boolean isUsefulData = false;
        for (int i = 0; i < lines.size(); i++) {
            String line = lines.get(i);
            if (line.contains(START_OF_DATA_TAG)) {
                isUsefulData = true;
            }
            if (!isUsefulData)
                continue;
            if (!line.contains("." + region + "."))
                continue;
            debug("Got: " + line);

            Matcher m1 = SINGLE_LINE_PATTERN.matcher(line);

            if (m1.matches()) {
                parseSingleLine(result, line, m1, i);
            } else {
                i = parseMultiLine(lines, result, i, line, region);
            }
        }
        return result;
    }

    private static int parseMultiLine(List<String> lines, List<Record> result, int lineIndex, String line, String region) {
        debug("Multi-line " + line);
        String suffix = line;
        line = lines.get(++lineIndex);

        Matcher m2 = MULTI_LINE_PATTERN.matcher(line);

        if (!m2.matches()) {
            debug("Returning into consideration: " + line);
            return lineIndex - 1;
        }

        String sizeString = m2.group(1);
        String prefix = m2.group(2);

        debug("Next line " + line);

        String name = prefix + "@" + suffix;

        if (line.contains("ALIGN")) {
            System.out.println("TODO: better handle " + line);
            return lineIndex;
        }

        int size;
        try {
            size = Integer.parseInt(sizeString, 16);
        } catch (NumberFormatException e) {
            throw new IllegalStateException("While parsing @ " + lineIndex);
        }

        debug("Name " + name);
        debug("size " + size);

        result.add(new Record(size, name, region));
        return lineIndex;
    }

    private static void parseSingleLine(List<Record> result, String line, Matcher m1, int lineIndex) {
        debug("Single-line " + line);

        int i = 1;
        String region = m1.group(i++);
        String suffix = m1.group(i++);
        String sizeString = m1.group(i++);
        //noinspection UnusedAssignment
        String prefix = m1.group(i++);

        String name = prefix + "@" + suffix;

        int size;
        try {
            size = Integer.parseInt(sizeString, 16);
        } catch (NumberFormatException e) {
            throw new IllegalStateException("While parsing @ " + lineIndex);
        }

        debug("Name " + name);
        debug("size " + size);

        result.add(new Record(size, name, region));
    }

    @SuppressWarnings("unused")
    private static void debug(String s) {
//        System.out.println(s);
    }

    static class Record implements Comparable<Record> {
        private final int size;
        private final String name;
        private String region;

        Record(int size, String name, String region) {
            this.size = size;
            this.name = name;
            this.region = region;
        }

        @Override
        public int compareTo(Record o) {
            int d = size - o.size;
            if (d != 0)
                return d;
            return name.compareTo(o.name);
        }

        @Override
        public String toString() {
            return "Record{" +
                    "size=" + size +
                    ", name='" + name + '\'' +
                    ", region='" + region + '\'' +
                    '}';
        }

        int getSize() {
            return size;
        }
    }
}
