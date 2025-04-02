package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.RootHolder;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class TuneCanToolHelper {
    public static final Set<String> IGNORE_LIST = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
    protected static void initialize(String iniFileName) {
        TuneCanTool.ini = IniFileModelImpl.readIniFile(iniFileName);
/*
        Set<String> allFields = new TreeSet<>();
        allFields.addAll(ini.allIniFields.keySet());
        allFields.removeAll(ini.fieldsInUiOrder.keySet());
        if (!allFields.isEmpty())
            throw new IllegalStateException("What about all these fields not mentioned on the UI: " + allFields);
*/
        RootHolder.ROOT = "../firmware/";
    }

    static void readIgnoreList(String ignoreListFileName) throws IOException {
        List<@NotNull String> lines = Files.readAllLines(Paths.get(ignoreListFileName));
        for(String line : lines)
            IGNORE_LIST.add(line.trim());
    }
}
