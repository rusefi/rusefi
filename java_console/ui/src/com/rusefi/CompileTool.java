package com.rusefi;

import com.fathzer.soft.javaluator.DoubleEvaluator;

import java.io.*;
import java.util.List;

import static org.junit.Assert.assertEquals;

/**
 * http://rusefi.com/wiki/index.php?title=Manual:Flexible_Logic
 * <p/>
 * (c) Andrey Belomutskiy 2013-2017
 * 1/19/2017
 */
public class CompileTool {
    public static void run(List<String> args) throws IOException {
        System.out.println("Params " + args);

        if (args.size() != 2) {
            System.out.println("Please specify input file and output file name");
            return;
        }

        String inputFileName = args.get(0);
        System.out.println("Reading from " + inputFileName);
        String outputFileName = args.get(1);
        System.out.println("Writing to " + outputFileName);

        BufferedReader br = new BufferedReader(new FileReader(inputFileName));

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName))) {
            bw.write("// this https://en.wikipedia.org/wiki/Reverse_Polish_notation is generated automatically\r\n");
            bw.write("// from " + inputFileName + "\r\n");
            bw.write("// on " + FileLog.getDate() + "\r\n//\r\n");

            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
//            line = line.replaceAll("\\s+", " ");

                handleOneFsioLine(line, bw);
            }
        }
        System.out.println("Done!");
    }

    private static void handleOneFsioLine(String line, BufferedWriter bw) throws IOException {
        if (line.isEmpty())
            return;
        if (line.charAt(0) == '#') {
            // forwarding comment into the output
            bw.write("//" + line.substring(1) + "\r\n");
            return;
        }

        int indexOfEquals = line.indexOf('=');

        if (indexOfEquals == -1) {
            System.err.println("Unexpected line: " + line);
            System.exit(-1);
        }

        String name = line.substring(0, indexOfEquals).trim();

        String expression = line.substring(indexOfEquals + 1).trim();

        DoubleEvaluator evaluator = new DoubleEvaluator();
        evaluator.evaluate(expression.toLowerCase());

        String rpn = evaluator.getRusEfi();

        bw.write("\n// Human-readable: " + expression + "\r\n");
        bw.write("#define " + name + " \"" + rpn + "\"\r\n");
    }
}
