package com.rusefi;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RenameComponents {
    private static final Map<String, String> replaceMap = new HashMap<String, String>();

    public static void main(String[] args) throws IOException {
        if (args.length != 3) {
            System.out.println("Three paramters expected:");
            System.out.println("  replace_replace_file source_folder destination_folder");
            return;
        }
        String requestFileName = args[0];
        File file = new File(requestFileName);
        if (!file.exists() || !file.isFile())
            throw new IllegalArgumentException("No replace_request_file");
        String sourceDir = args[1];
        assertFolder(sourceDir);
        String destinationDir = args[2];
        assertFolder(destinationDir);

        readReplaceRequest(file);
        for (String fileName : new File(sourceDir).list())
            processFile(sourceDir, destinationDir, fileName);
    }

    private static void processFile(String sourceDir, String destinationDir, String fileName) throws IOException {
        String newFileName = destinationDir + File.separator + fileName;
        BufferedWriter bw = new BufferedWriter(new FileWriter(newFileName));

        String sourceFile = sourceDir + File.separator + fileName;
        BufferedReader br = new BufferedReader(new FileReader(sourceFile));

        System.out.println("Making " + newFileName + " from " + sourceFile);

        String line;
        while ((line = br.readLine()) != null) {
            line = processLine(line);
            bw.append(line + "\r\n");
        }

        bw.close();
    }

    private static String processLine(String line) {
        for (Map.Entry<String, String> e : replaceMap.entrySet()) {
            String from = e.getKey();
            String to = e.getValue();

            line = Pattern.compile(" " + from + "$").matcher(
                    line).replaceAll(Matcher.quoteReplacement(" " + to));

            line = processLine(line, " " + from + ";", " " + to + ";");
            line = processLine(line, " " + from + ")", " " + to + ")");
            line = processLine(line, " " + from + " ", " " + to + " ");
            line = processLine(line, " " + from + "-", " " + to + "-");
            line = processLine(line, "\"" + from + "\"", "\"" + to + "\"");
        }
        return line;
    }

    private static String processLine(String line, String s, String s1) {
        return line.replace(s, s1);
    }

    private static void readReplaceRequest(File file) throws IOException {
        String line;
        BufferedReader br = new BufferedReader(new FileReader(file));
        int lineNumber = 0;

        while ((line = br.readLine()) != null) {
            lineNumber++;
            line = line.trim();
            if (line.isEmpty())
                continue;
            String[] s = line.split("\\;");
            if (s.length != 2)
                throw new IllegalArgumentException("Invalid replace request: " + line);

            String from = s[0];
            String to = s[1];


            if (replaceMap.containsKey(from) || replaceMap.containsValue(from))
                throw new IllegalArgumentException("Name already mentioned: " + from + " @" + lineNumber);

            if (replaceMap.containsKey(to) || replaceMap.containsValue(to))
                throw new IllegalArgumentException("Name already mentioned: " + to + " @" + lineNumber);

            replaceMap.put(from, to);

            System.out.println("Replacing [" + from + "] by [" + to + "]");
        }
    }

    private static void assertFolder(String dir) {
        File d = new File(dir);
        if (!d.exists() || !d.isDirectory())
            throw new IllegalArgumentException("No such folder: " + dir);
    }
}
