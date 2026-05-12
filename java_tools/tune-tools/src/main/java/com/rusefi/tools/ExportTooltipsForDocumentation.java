package com.rusefi.tools;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.LocalIniFileProvider;

import java.io.FileWriter;
import java.io.IOException;

public class ExportTooltipsForDocumentation {
    public static void main(String[] args) throws IOException {
//        RootHolder.ROOT = "../firmware/";

        IniFileModel ini = IniFileReaderUtil.readIniFile(LocalIniFileProvider.INI_FILE_FOR_SIMULATOR);

        try (FileWriter fw = new FileWriter("all_fields.md")) {

            writeAllFields(fw, ini);
        }
    }

    private static void writeAllFields(FileWriter fw, IniFileModel ini) throws IOException {
        for (DialogModel.Field f : ini.getFieldsInUiOrder().values()) {
//            String fieldName = f.getKey();
//            IniField iniField = ini.allIniFields.get(f.getKey());

            String toolTip = ini.getTooltips().get(f.getKey());

            if (toolTip != null) {

                fw.append("## " + f.getUiName() + "\n");

                fw.append(toolTip);
            }


        }
    }
}
