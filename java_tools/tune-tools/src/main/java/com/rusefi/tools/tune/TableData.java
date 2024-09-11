package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.Arrays;
import java.util.function.Function;

public class TableData implements CannableEntity {
    private final int rows;
    private final int columns;
    public final float[][] floats;
    private final String tableName;

    public TableData(int rows, int columns, float[][] floats, String tableName) {
        this.rows = rows;
        this.columns = columns;
        this.floats = floats;
        this.tableName = tableName;
    }

    @Nullable
    public static TableData readTable(String msqFileName, String tableName, IniFileModel model) throws IOException {
        IniField iniField = model.getAllIniFields().get(tableName);
        if (!(iniField instanceof ArrayIniField)) {
            // this could happen if older tune is not compatible with newer .ini
            return null;
        }
        ArrayIniField field = (ArrayIniField) iniField;

        int rows = field.getRows();

        int columns = field.getCols();
        return readTable(msqFileName, tableName, rows, TS2C.fileFactory, columns);

    }

    public static TableData readTable(String msqFileName, String tableName, int rows, Function<String, Reader> factory, int columns) throws IOException {
        float[][] table = new float[rows][];
        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            table[rowIndex] = new float[columns];
        }

        try (BufferedReader reader = TS2C.readAndScroll(msqFileName, tableName, factory)) {
            readTable(table, reader, rows, columns);
        } catch (IllegalStateException e) {
            // we read potentially old tune using current IniFileModel, curve dimension might not match
            System.err.println("[IllegalStateException] while " + tableName);
        }
        return new TableData(rows, columns, table, tableName);
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
//            System.out.println("Got line " + rowIndex + ": " + Arrays.toString(table[rowIndex]));
            rowIndex++;
        }
    }

    public String getCTable() {
        StringBuilder output = new StringBuilder();
        TS2C.writePlainTable(rows, columns, output, (loadIndex, rpmIndex) -> floats[loadIndex][rpmIndex]);
        return output.toString();
    }

    private String getCannedMethod(String methodNamePrefix) {
        return methodNamePrefix + "canned" + tableName + "()";
    }

    private String getCannedName() {
        return "hardCoded" + tableName;
    }

    @NotNull
    public String getCsourceCode() {
        return "static const float " +
            getCannedName() + "[" + rows + "][" + columns + "] = {\n" +
            getCTable() +
            "};\n";
    }

    @Override
    public String getCsourceMethod(String reference, String methodNamePrefix, String name) {
        String scale = "";
        if (tableName.equals("lambdaTable"))
            scale = ", 1.0 / 14.7";

        return "static void " + getCannedMethod(methodNamePrefix) + " {\n"
            + "\t" + getCsourceCode() +
            "\tcopyTable(" + reference + tableName + ", " + getCannedName() + scale + ");\n" +
            "}\n\n";
    }


    @Override
    public String getName() {
        return tableName;
    }

    @Override
    public String getCinvokeMethod(String methodNamePrefix) {
        return "\t" + getCannedMethod(methodNamePrefix) + ";\n";
    }
}
