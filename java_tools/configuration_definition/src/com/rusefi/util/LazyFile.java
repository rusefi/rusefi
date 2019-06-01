package com.rusefi.util;

import com.rusefi.ConfigDefinition;

import java.io.*;
import java.nio.file.Paths;
import java.util.Scanner;
import java.util.regex.Pattern;

/**
 * This file would override file content only of content has changed, disregarding the magic tag line.
 */
public class LazyFile {
    private static final String PROPERTY_NAME = "rusefi.generator.lazyfile.enabled";
    private static boolean ENABLED = Boolean.getBoolean(PROPERTY_NAME);

    static {
        System.out.println(PROPERTY_NAME + "=" + ENABLED);
    }

    private String filename;

    private final StringBuffer content = new StringBuffer();
    private final StringBuffer contentWithoutTag = new StringBuffer();

    public LazyFile(String filename) {
        this.filename = filename;
    }

    public void write(String line) {
        content.append(line);
        if (!line.contains(ConfigDefinition.GENERATED_AUTOMATICALLY_TAG))
            contentWithoutTag.append(line);
    }

    public void close() throws IOException {
        String fileContent = unifySpaces(readCurrentContent(filename));
        String newContent = unifySpaces(contentWithoutTag.toString().trim());
        if (fileContent.equals(newContent)) {
            System.out.println(getClass().getSimpleName() + ": Not updating " + filename + " since looks to be the same content");
            return;
        }
        for (int i = 0; i < Math.min(fileContent.length(), newContent.length()); i++) {
            if (fileContent.charAt(i) != newContent.charAt(i)) {
                System.out.println(getClass().getSimpleName() + " " + filename + ": Not same at " + i);
                break;
            }
        }
        FileWriter fw = new FileWriter(filename);
        fw.write(content.toString());
        fw.close();
    }

    private static String unifySpaces(String line) {
        return line.replaceAll("\n[\n]*", "\n").trim();
    }

    private String readCurrentContent(String filename) throws IOException {
        if (!new File(filename).exists())
            return "";
        BufferedReader br = new BufferedReader(new FileReader(filename));
        Scanner in = new Scanner(Paths.get(filename), "UTF-8");
        String line;
        Pattern pat = Pattern.compile(".*\\R|.+\\z");
        StringBuffer sb = new StringBuffer();
        while ((line = in.findWithinHorizon(pat, 0)) != null) {
            if (!line.contains(ConfigDefinition.GENERATED_AUTOMATICALLY_TAG))
                sb.append(line);
        }
        return sb.toString();
    }
}
