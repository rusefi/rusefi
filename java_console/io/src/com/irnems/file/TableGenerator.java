package com.irnems.file;

import com.irnems.models.XYData;
import com.irnems.models.XYDataReader;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

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
        List<Double> rpms = new ArrayList<Double>(data.getXSet());
        BufferedWriter out = new BufferedWriter(new FileWriter(fileName));
        out.write("#include \"thermistors.h\"\n\n\n");

        writeArray(rpms, out, prefix + "rpm");

        Set<Double> mafs = data.getYAxis(rpms.get(0)).getYs();
        ArrayList<Double> yArray = new ArrayList<Double>(mafs);
        writeArray(yArray, out, prefix + "maf");

        out.write("static float " + prefix + "table[" + rpms.size() + "][" + mafs.size() + "] = {\n");

        boolean isFirstX = true;
        int indexX = 0;
        for (double x : data.getXSet()) {
            if (!isFirstX)
                out.write(",\n");
            isFirstX = false;

            out.write("{");

            int indexY = 0;
            for (double y : mafs) {
                if (indexY == 0)
                    out.write("/*" + indexX + " rpm=" + rpms.get(indexX) + "*/");

                if (indexY > 0)
                    out.write(", ");

                out.write("/*" + indexY + " " + yArray.get(indexY) + "*/" + data.getValue(x, y));
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
