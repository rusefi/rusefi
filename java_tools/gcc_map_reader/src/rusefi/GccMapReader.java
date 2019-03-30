package rusefi;

import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
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
    private static final Pattern SINGLE_LINE_PATTERN = Pattern.compile(".*\\.bss\\.(\\S*).*0x.*0x(\\S*)(.*)");

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

        List<Record> records = process(lines);

        Collections.sort(records, Comparator.reverseOrder());

        int totalSize = 0;
        for (Record record : records) {
            System.out.println(record);
            totalSize += record.size;
        }

        System.out.println("Total size: " + totalSize);
    }

    static List<Record> process(List<String> lines) {

        List<Record> result = new ArrayList<Record>();
        for (int i = 0; i < lines.size(); i++) {
            String line = lines.get(i);
            if (!line.contains(".bss."))
                continue;
            debug("Got: " + line);

            Matcher m1 = SINGLE_LINE_PATTERN.matcher(line);

            if (m1.matches()) {
                parseSingleLine(result, line, m1, i);
            } else {
                i = parseMultiLine(lines, result, i, line);
            }
        }
        return result;
    }

    private static int parseMultiLine(List<String> lines, List<Record> result, int lineIndex, String line) {
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

        result.add(new Record(size, name));
        return lineIndex;
    }

    private static void parseSingleLine(List<Record> result, String line, Matcher m1, int lineIndex) {
        debug("Single-line " + line);

        String suffix = m1.group(1);
        String sizeString = m1.group(2);
        String prefix = m1.group(3);

        String name = prefix + "@" + suffix;

        int size;
        try {
            size = Integer.parseInt(sizeString, 16);
        } catch (NumberFormatException e) {
            throw new IllegalStateException("While parsing @ " + lineIndex);
        }

        debug("Name " + name);
        debug("size " + size);

        result.add(new Record(size, name));
    }

    private static void debug(String s) {
//        System.out.println(s);
    }

    static class Record implements Comparable<Record> {
        private final int size;
        private final String name;

        public Record(int size, String name) {
            this.size = size;
            this.name = name;
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
                    '}';
        }

        public int getSize() {
            return size;
        }

        public String getName() {
            return name;
        }
    }
}
