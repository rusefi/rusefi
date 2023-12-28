package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.Arrays;
import java.util.function.Function;

public class TableData {
    @NotNull
    public static float[][] readTable(String msqFileName, String tableName, IniFileModel model) throws IOException {
        ArrayIniField field = (ArrayIniField) model.allIniFields.get(tableName);

        if (field.getRows() != field.getCols())
            throw new UnsupportedOperationException("Not square table not supported yet");
        // todo: replace with loadCount & rpmCount
        int rows = field.getRows();

        return readTable(msqFileName, tableName, rows, TS2C.fileFactory, field.getCols());
    }

    @NotNull
    public static float[][] readTable(String msqFileName, String tableName, int rows, Function<String, Reader> factory, int columns) throws IOException {
        float[][] table = new float[rows][];
        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            table[rowIndex] = new float[columns];
        }

        BufferedReader reader = TS2C.readAndScroll(msqFileName, tableName, factory);
        readTable(table, reader, rows, columns);
        return table;
    }

    private static void readTable(float[][] table, BufferedReader r, int rows, int columns) throws IOException {
        int rowIndex = 0;

        while (rowIndex < rows) {
            String line = r.readLine();
            if (line == null)
                throw new IOException("End of file?");
            line = line.trim();
            if (line.isEmpty())
                continue;

            String[] values = line.split("\\s");
            if (values.length != columns)
                throw new IllegalStateException("Expected " + columns + " but got " + values.length + " content = " + Arrays.toString(values) + ". Unexpected line: " + line);

            for (int column = 0; column < columns; column++) {
                String str = values[column];
                try {
                    table[rowIndex][column] = Float.parseFloat(str);
                } catch (NumberFormatException e) {
                    throw new IllegalStateException("While reading " + str, e);
                }
            }
            System.out.println("Got line " + rowIndex + ": " + Arrays.toString(table[rowIndex]));
            rowIndex++;
        }
    }
}
