package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileReader;
import org.jetbrains.annotations.NotNull;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

public class TuneCanToolHelper {
    public static final Set<String> IGNORE_LIST = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
    private static final Set<String> WHITE_LIST = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    protected static void initialize(String iniFileName) {
        try {
            TuneCanTool.ini = IniFileReader.readIniFile(iniFileName);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    static void readIgnoreList(String ignoreListFileName) throws IOException {
        List<String> trimmed = readAndTrim(ignoreListFileName);
        IGNORE_LIST.addAll(trimmed);
    }

    static void readWhiteList(String ignoreListFileName) throws IOException {
        List<String> trimmed = readAndTrim(ignoreListFileName);
        WHITE_LIST.addAll(trimmed);
    }

    private static @NotNull List<String> readAndTrim(String fileName) throws IOException {
        if (fileName.trim().isEmpty())
            return Collections.emptyList();
        List<String> trimmed = new ArrayList<>();
        List<@NotNull String> lines = Files.readAllLines(Paths.get(fileName));
        for (String line : lines)
            trimmed.add(line.trim());
        return trimmed;
    }

    public static boolean accept(String name) {
        if (!WHITE_LIST.isEmpty()) {
            return WHITE_LIST.contains(name);
        }
        return !IGNORE_LIST.contains(name);
    }
}
