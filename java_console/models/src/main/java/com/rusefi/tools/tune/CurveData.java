package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Arrays;

public class CurveData {
    private final float[] rawData;

    public CurveData(float[] rawData) {
        this.rawData = rawData;

        int countOfEqualElementsAtTheEnd = 0;
        float lastElement = rawData[rawData.length - 1];
        for (int i = rawData.length - 2; i >= 0; i--) {
            if (rawData[i] != lastElement)
                break;
            countOfEqualElementsAtTheEnd++;
        }
        System.out.println(countOfEqualElementsAtTheEnd + " equal elements at the end of the curve");
    }

    static CurveData processCurve(String msqFileName, String loadSectionName, IniFileModel model, BufferedWriter w) throws IOException {
        ArrayIniField field = (ArrayIniField) model.allIniFields.get(loadSectionName);
        int curveSize = field.getRows();
        BufferedReader r = TS2C.readAndScroll(msqFileName, loadSectionName + "\"");
        float[] curve = new float[curveSize];
        readAxle(curve, r);

        CurveData curveData = new CurveData(curve);


        w.write("static const float hardCoded" + loadSectionName + "[" + curveSize + "] = ");
        w.write(toString(curve));
        w.write(";\r\n\r\n");
        return curveData;
    }

    private static String toString(float[] a) {
        StringBuilder b = new StringBuilder();
        int iMax = a.length - 1;
        b.append('{');
        for (int i = 0; ; i++) {
            b.append(a[i]);
            if (i == iMax)
                return b.append('}').toString();
            b.append(", ");
        }
    }


    private static void readAxle(float[] curve, BufferedReader r) throws IOException {
        int index = 0;

        while (index < curve.length) {
            String line = r.readLine();
            if (line == null)
                throw new IOException("End of file?");
            line = line.trim();
            if (line.isEmpty())
                continue;
            curve[index++] = Float.parseFloat(line);
        }

        System.out.println("Got bins " + Arrays.toString(curve));
    }

    public float[] getRawData() {
        return rawData;
    }
}
