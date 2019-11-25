package com.rusefi;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class PerfTraceTool {

    private static final String EOL = "\r\n";

    public static void readPerfTrace(String inputFileName, String outputFileName, String topClassLine, String stringType) throws IOException {
        List<String> enumNames = readEnums(inputFileName);
        System.out.println("Got enums: " + enumNames);


        writeClass(outputFileName, enumNames, topClassLine, stringType);
    }

    private static void writeClass(String outputFileName, List<String> enumNames, String topClassLine, String stringType) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputFileName));


        writer.write(topClassLine + EOL + EOL);

        writer.write("public class EnumNames {" + EOL);
        writer.write("\t" +
                stringType +
                "[] TypeNames = {" + EOL);

        for (String enumValue : enumNames)
            writer.write("\t\"" + enumValue + "\"," + EOL);


        writer.write("\t};" + EOL);
        writer.write("}" + EOL);
        writer.close();

        System.out.println("Done writing to " + outputFileName);
    }

    private static List<String> readEnums(String inputFileName) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(inputFileName));

        boolean weAreInBusiness = false;
        List<String> result = new ArrayList<>();

        String line;
        while ((line = br.readLine()) != null) {
            if (line.contains("enum_start_tag")) {
                weAreInBusiness = true;
                continue;
            }

            if (line.contains("enum_end_tag")) {
                // we are done here
                break;
            }

            if (!weAreInBusiness)
                continue;

            line = line.trim().replaceAll("\\s", "");

            if (line.endsWith(",")) {
                result.add(line.substring(0, line.length() - 1));
            }


        }
        return result;
    }
}
