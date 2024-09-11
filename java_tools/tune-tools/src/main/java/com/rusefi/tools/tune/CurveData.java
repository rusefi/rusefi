package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class CurveData implements CannableEntity {
    private final String curveName;
    private final float[] rawData;

    public CurveData(String curveName, float[] rawData) {
        this.curveName = curveName;
        this.rawData = rawData;

        int countOfEqualElementsAtTheEnd = 0;
        float lastElement = rawData[rawData.length - 1];
        for (int i = rawData.length - 2; i >= 0; i--) {
            if (rawData[i] != lastElement)
                break;
            countOfEqualElementsAtTheEnd++;
        }
//        System.out.println(countOfEqualElementsAtTheEnd + " equal elements at the end of the curve");
    }

    static CurveData processCurve(String msqFileName, String curveName, IniFileModel model, BufferedWriter w) throws IOException {
        CurveData curveData = valueOf(msqFileName, curveName, model);

        w.write(curveData.getCsourceCode());

        return curveData;
    }

    @Nullable
    public static CurveData valueOf(String msqFileName, String curveName, IniFileModel model) throws IOException {
        IniField iniField = model.getAllIniFields().get(curveName);
        if (!(iniField instanceof ArrayIniField))
            return null;
        ArrayIniField field = (ArrayIniField) iniField;
        int curveSize = field.getRows();
        float[] curveValues = new float[curveSize];
        try (BufferedReader r = TS2C.readAndScroll(msqFileName, curveName + "\"", TS2C.fileFactory)) {
            readAxle(curveValues, r);
        } catch (NumberFormatException e) {
            // we read potentially old tune using current IniFileModel, curve dimension might not match
            System.err.println("[NumberFormatException] while " + curveName);
        }

        return new CurveData(curveName, curveValues);
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

//        System.out.println("Got bins " + Arrays.toString(curve));
    }

    @NotNull
    public String getCsourceCode() {
        return "static const float " +
                getCannedName() + "[" + rawData.length + "] = " +
                toString(rawData) +
                ";\n";
    }

    @NotNull
    private String getCannedName() {
        return "hardCoded" + curveName;
    }

    public float[] getRawData() {
        return rawData;
    }

    @Override
    public String getCsourceMethod(String reference, String methodNamePrefix, String name) {
        return "static void " + getCannedMethod(methodNamePrefix) + " {\n"
                + "\t" + getCsourceCode() +
                "\tcopyArray(" + reference + name + ", " + getCannedName() + ");\n" +
                "}\n\n";
    }

    @NotNull
    private String getCannedMethod(String methodNamePrefix) {
        return methodNamePrefix + "canned" + curveName + "()";
    }

    @Override
    public String getCinvokeMethod(String methodNamePrefix) {
        return "\t" + getCannedMethod(methodNamePrefix) + ";\n";
    }

    @Override
    public String getName() {
        return curveName;
    }
}
