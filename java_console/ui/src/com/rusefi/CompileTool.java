package com.rusefi;

import com.fathzer.soft.javaluator.DoubleEvaluator;

import java.io.*;
import java.util.List;

/**
 * http://rusefi.com/wiki/index.php?title=Manual:Flexible_Logic
 * <p/>
 * (c) Andrey Belomutskiy 2013-2018
 * 1/19/2017
 */
public class CompileTool {
    private static String NEWLINE = "\n";

    public static int run(List<String> args) throws IOException {
        System.out.println("Params " + args);

        if (args.size() != 2) {
            System.out.println("Please specify input file and output file name");
            return -1;
        }

        String inputFileName = args.get(0);
        System.out.println("Reading from " + inputFileName);
        String outputFileName = args.get(1);
        System.out.println("Writing to " + outputFileName);

        BufferedReader br = new BufferedReader(new FileReader(inputFileName));

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName))) {
            bw.write("// this https://en.wikipedia.org/wiki/Reverse_Polish_notation is generated automatically" + NEWLINE);
            bw.write("// from " + inputFileName + NEWLINE);
            bw.write("// on " + FileLog.getDate() + NEWLINE + "//" + NEWLINE);

            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
//            line = line.replaceAll("\\s+", " ");
                bw.write(handleOneFsioLine(line));

            }
        }
        System.out.println("Done!");
        return 0;
    }

    public static String handleOneFsioLine(String line) throws IOException {
        if (line.isEmpty())
            return "";
        StringBuilder result = new StringBuilder();
        if (line.charAt(0) == '#') {
            // forwarding comment into the output
            result.append("//" + line.substring(1) + NEWLINE);
            return result.toString();
        }


        int indexOfEquals = line.indexOf('=');

        if (indexOfEquals == -1) {
            System.err.println("Unexpected line: " + line);
            System.exit(-1);
        }

        String name = line.substring(0, indexOfEquals).trim();

        String expression = line.substring(indexOfEquals + 1).trim();

        String rpn;
        try {
            rpn = DoubleEvaluator.process(expression).getPosftfixExpression();

        } catch (Throwable e) {
            throw new IllegalStateException("For " + expression, e);
        }
        result.append(NEWLINE + "// Human-readable: " + expression + NEWLINE);
        result.append("#define " + name + " \"" + rpn + "\"" + NEWLINE);
        return result.toString();
    }
}
