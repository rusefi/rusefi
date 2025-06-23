package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.RootHolder;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class TuneCanToolHelper {
    public static final Set<String> IGNORE_LIST = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
    private static final Set<String> WHITE_LIST = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    protected static void initialize(String iniFileName) {
        TuneCanTool.ini = IniFileModelImpl.readIniFile(iniFileName);
    }

    static void readIgnoreList(String ignoreListFileName) throws IOException {
        List<String> trimmed = readAndTrim(ignoreListFileName);
        IGNORE_LIST.addAll(trimmed);
    }

    static void readWhiteList(String ignoreListFileName) throws IOException {
        List<String> trimmed = readAndTrim(ignoreListFileName);
        WHITE_LIST.addAll(trimmed);
    }

    private static @NotNull List<String> readAndTrim(String ignoreListFileName) throws IOException {
        List<String> trimmed = new ArrayList<>();
        List<@NotNull String> lines = Files.readAllLines(Paths.get(ignoreListFileName));
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
