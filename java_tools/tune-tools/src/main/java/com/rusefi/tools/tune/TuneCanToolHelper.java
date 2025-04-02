package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.RootHolder;

public class TuneCanToolHelper {
    protected static void initialize(String iniFileName) {
        TuneCanTool.ini = IniFileModelImpl.readIniFile(iniFileName);
        if (TuneCanTool.ini == null)
            throw new IllegalStateException("Not found " + iniFileName);
/*
        Set<String> allFields = new TreeSet<>();
        allFields.addAll(ini.allIniFields.keySet());
        allFields.removeAll(ini.fieldsInUiOrder.keySet());
        if (!allFields.isEmpty())
            throw new IllegalStateException("What about all these fields not mentioned on the UI: " + allFields);
*/
        RootHolder.ROOT = "../firmware/";
    }
}
