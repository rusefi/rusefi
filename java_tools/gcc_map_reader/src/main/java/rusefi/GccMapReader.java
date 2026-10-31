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
    // Match the columns before the object path, which may itself contain spaces or "0x".
    private static final Pattern COLUMNS_PATTERN = Pattern.compile(
            "^\\s*0x[0-9a-fA-F]+\\s+0x([0-9a-fA-F]+)(?:\\s+(.*))?$");
    private static final String[] REGIONS = {"bss", "ram4", "text", "data", "rodata"};
    private static final Pattern SECTION_PATTERN = Pattern.compile(
            "^\\s*\\.(bss|ram4|text|data|rodata)\\.(\\S+)(?:\\s+(.*))?$");
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
            if (!isUsefulData) {
                continue;
            }

            Matcher section = SECTION_PATTERN.matcher(line);
            // Ignore linker-script wildcards, assignments, and symbol-address rows.
            if (!section.matches() || !region.equals(section.group(1))) {
                continue;
            }

            String suffix = section.group(2);
            String columns = section.group(3);
            if (columns == null || columns.trim().isEmpty()) {
                if (i + 1 == lines.size()) {
                    throw new IllegalStateException("Missing address and size after line " + (i + 1) + ": " + line);
                }
                suffix = line;
                line = lines.get(++i);
                columns = line;
            }

            Matcher values = COLUMNS_PATTERN.matcher(columns);
            if (!values.matches()) {
                throw new IllegalStateException("Invalid address or size at line " + (i + 1) + ": " + line);
            }

            int size;
            try {
                size = Integer.parseInt(values.group(1), 16);
            } catch (NumberFormatException e) {
                throw new IllegalStateException("Invalid size at line " + (i + 1) + ": " + line, e);
            }

            String path = values.group(2) == null ? "" : values.group(2);
            result.add(new Record(size, path + "@" + suffix, region));
        }
        return result;
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
