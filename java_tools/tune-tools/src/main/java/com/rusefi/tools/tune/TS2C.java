package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.function.Function;

/**
 * tuner studio project to C data structure converter command line utility
 * <p>
 * 12/27/2014
 * Andrey Belomutskiy, (c) 2012-2016
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class TS2C {
    public static final Function<String, Reader> fileFactory = new Function<String, Reader>() {
        @Override
        public Reader apply(String fileName) {
            try {
                return new FileReader(fileName);
            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }
        }
    };


    public static void writePlainTable(int rows, int columns, StringBuilder sb, ValueSource valueSource) {
        for (int row = 0; row < rows; row++) {
            String line = writePlainTableLine(valueSource, row, columns);
            sb.append(line);
        }
    }

    /**
     * @param fileName       text file to open
     * @param magicStringKey magic string content to scroll to
     * @param factory
     * @return Reader after the magicStringKey line
     */
    static BufferedReader readAndScroll(String fileName, String magicStringKey, Function<String, Reader> factory) throws IOException {

        Reader reader = factory.apply(fileName);
        BufferedReader br = new BufferedReader(reader);
        System.out.println("Reading from " + new File(fileName).getAbsolutePath() + ", scrolling to " + magicStringKey);
        String line;
        while ((line = br.readLine()) != null) {
            if (line.contains(magicStringKey)) {
//                System.out.println("Found " + line);
                break;
            }
        }
        return br;
    }

    private static String writePlainTableLine(ValueSource valueSource, int loadIndex, int loadSize) {
        StringBuilder sb = new StringBuilder("{");

        for (int rpmIndex = 0; rpmIndex < loadSize; rpmIndex++) {
            sb.append(String.format("%3.3f", valueSource.getValue(loadIndex, rpmIndex)) + ",\t");
        }
        sb.append("},\n");

        return sb.toString();
    }

    /*
    private static String writeTableLine(CurveData loadBins, CurveData rpmBins, ValueSource valueSource, int loadIndex) {
        StringBuilder sb = new StringBuilder("{");

        sb.append("/" + "* " + loadIndex + " " + String.format("%3.3f", loadBins.getRawData()[loadIndex]) + "\t*" + "/");
        for (int rpmIndex = 0; rpmIndex < rpmBins.getRawData().length; rpmIndex++) {
            sb.append("/* " + rpmIndex + " " + rpmBins.getRawData()[rpmIndex] + "*" + "/" + String.format("%3.3f", valueSource.getValue(loadIndex, rpmIndex)) + ",\t");
        }
        sb.append("},\n");

        return sb.toString();
    }

    @NotNull
    public static String getCopyMethodBody(String tableReference, IniFileModel model, String tableName) {
        String xRpmBinsName = model.getXBin(tableName);
        String yLoadBinsName = model.getYBin(tableName);

        String x = "\tcopyArray(" + tableReference + "LoadBins, hardCoded" + xRpmBinsName + ");\n" +
            "\tcopyArray(" + tableReference + "RpmBins, hardCoded" + yLoadBinsName + ");\n" +
            "\tcopyTable(" + tableReference + "Table, hardCoded" + tableName + ");\n";
        return x;
    }
*/

    public interface ValueSource {
        float getValue(int rowIndex, int columnIndex);
    }

}
