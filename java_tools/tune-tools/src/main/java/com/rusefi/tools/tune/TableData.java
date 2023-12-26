package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.Arrays;

public class TableData {
    @NotNull
    public static float[][] readTable(String msqFileName, String tableName, IniFileModel model) throws IOException {
        ArrayIniField field = (ArrayIniField) model.allIniFields.get(tableName);

        if (field.getRows() != field.getCols())
            throw new UnsupportedOperationException("Not square table not supported yet");
        // todo: replace with loadCount & rpmCount
        int size = field.getRows();

        float[][] table = new float[size][];
        for (int i = 0; i < size; i++) {
            table[i] = new float[size];
        }

        BufferedReader reader = TS2C.readAndScroll(msqFileName, tableName);
        readTable(table, reader, size);
        return table;
    }

    private static void readTable(float[][] table, BufferedReader r, int size) throws IOException {
        int index = 0;

        while (index < size) {
            String line = r.readLine();
            if (line == null)
                throw new IOException("End of file?");
            line = line.trim();
            if (line.isEmpty())
                continue;

            String[] values = line.split("\\s");
            if (values.length != size)
                throw new IllegalStateException("Expected " + size + " but got " + Arrays.toString(values) + ". Unexpected line: " + line);

            for (int i = 0; i < size; i++) {
                String str = values[i];
                try {
                    table[index][i] = Float.parseFloat(str);
                } catch (NumberFormatException e) {
                    throw new IllegalStateException("While reading " + str, e);
                }
            }
            System.out.println("Got line " + index + ": " + Arrays.toString(table[index]));
            index++;
        }
    }
}
