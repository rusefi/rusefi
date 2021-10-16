package com.rusefi.util;

import java.io.*;
import java.nio.file.Paths;
import java.util.Scanner;
import java.util.regex.Pattern;

/**
 * This file would override file content only of content has changed, disregarding the magic tag line.
 */
public class LazyFile implements Output {
    public static final String LAZY_FILE_TAG = "was generated automatically by rusEFI tool ";
    public static final String LAZY_FILE_TAG_LOWER = LAZY_FILE_TAG.toLowerCase();
    private static boolean isLazyCheckEnabled = true;

    private final String filename;

    private final StringBuffer content = new StringBuffer();
    private final StringBuffer contentWithoutTag = new StringBuffer();

    public LazyFile(String filename) {
        this.filename = filename;
    }

    @Override
    public void write(String line) {
        content.append(line);
        String[] lines = line.split("\\r?\\n");
        for (String subLine : lines) {
            if (!subLine.toLowerCase().contains(LAZY_FILE_TAG_LOWER)) {
                contentWithoutTag.append(subLine);
            }
        }
    }

    @Override
    public void close() throws IOException {
        String fileContent = unifySpaces(readCurrentContent(filename));
        String newContent = unifySpaces(contentWithoutTag.toString());

        if (fileContent.equals(newContent) && LazyFile.isLazyCheckEnabled()) {
            SystemOut.println(getClass().getSimpleName() + ": Not updating " + filename + " since looks to be the same content, new content size=" + contentWithoutTag.length());
            return;
        }
        for (int i = 0; i < Math.min(fileContent.length(), newContent.length()); i++) {
            if (fileContent.charAt(i) != newContent.charAt(i)) {
                SystemOut.println(getClass().getSimpleName() + " " + filename + ": Not same at " + i);
                if (i > 15 && i < fileContent.length() - 6 && i < newContent.length() - 6) {
                    SystemOut.println("file       " + fileContent.substring(i - 15, i + 5));
                    SystemOut.println("newContent " + newContent.substring(i - 15, i + 5));
                }
                break;
            }
        }
        Writer fw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(filename), IoUtils.CHARSET));
        fw.write(content.toString());
        fw.close();
    }

    public static void setLazyFileEnabled(boolean isEnabled) {
        LazyFile.isLazyCheckEnabled = isEnabled;
    }

    public static boolean isLazyCheckEnabled() {
        return LazyFile.isLazyCheckEnabled;
    }

    public static String unifySpaces(String line) {
        line = line.replace("\r", "");
        return line.replaceAll("\n[\n]*", "");
    }

    private String readCurrentContent(String filename) throws IOException {
        if (!new File(filename).exists()) {
            SystemOut.println(filename + " does not exist considering empty current content");
            return "";
        }
        Scanner in = new Scanner(Paths.get(filename), IoUtils.CHARSET.name());
        Pattern pat = Pattern.compile(".*\\R|.+\\z");
        String line;
        StringBuilder sb = new StringBuilder();
        while ((line = in.findWithinHorizon(pat, 0)) != null) {
            if (!line.toLowerCase().contains(LAZY_FILE_TAG_LOWER))
                sb.append(line);
        }
        return sb.toString();
    }
}
