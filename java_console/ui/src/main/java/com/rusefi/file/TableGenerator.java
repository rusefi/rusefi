package com.rusefi.file;

import com.rusefi.models.XYData;
import com.rusefi.models.XYDataReader;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         3/27/13
 */
public class TableGenerator {
    public static void main(String[] args) throws IOException {

        XYData data = XYDataReader.readFile("in.csv");

        writeAsC(data, "ad_", "advance_map.c");
    }

    public static void writeAsC(XYData data, String prefix, String fileName) {
        try {
            doWrite(data, prefix, fileName);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private static void doWrite(XYData data, String prefix, String fileName) throws IOException {
        List<Double> rpms = new ArrayList<>(data.getXSet());
        BufferedWriter out = new BufferedWriter(new FileWriter(fileName));
        out.write("#include \"thermistors.h\"\n\n\n");

//        writeArray(rpms, out, prefix + "rpm");

        List<Double> engineLoadValues = new ArrayList<>(data.getYAxis(rpms.get(0)).getYs());
//        writeArray(yArray, out, prefix + "maf");

        out.write("static float " + prefix + "table[" + rpms.size() + "][" + engineLoadValues.size() + "] = {\n");

        boolean isFirstEngineLoad = true;
        int indexX = 0;
        for (double engineLoad : engineLoadValues) {
            if (!isFirstEngineLoad)
                out.write(",\n");
            isFirstEngineLoad = false;

            out.write("{");

            int indexY = 0;
            for (double rpm : data.getXSet()) {
                if (indexY == 0)
                    out.write("/*" + indexX + " engineLoad=" + engineLoadValues.get(indexX) + "*/");

                if (indexY > 0)
                    out.write(", ");

                out.write("/*" + indexY + " " + rpms.get(indexY) + "*/" + data.getValue(rpm, engineLoad));
                indexY++;
            }
            out.write("}");
            indexX++;
        }
        out.write("\n};\n");

        out.close();
    }

    private static void writeArray(List<Double> rpms, BufferedWriter out, String title) throws IOException {
        out.write("#define " + title.toUpperCase() + "_COUNT " + rpms.size() + "\n");

        outputDoubles(rpms, out, title);
    }

    private static void outputDoubles(List<Double> values, BufferedWriter out, String title) throws IOException {
        out.write("static float " + title + "_table[] = {");

        for (int i = 0; i < values.size(); i++) {
            if (i > 0)
                out.write(", ");
            out.write("/*" + i + "*/ " + values.get(i));

        }
        out.write("};\n\n");
    }
}
