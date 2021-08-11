package com.rusefi;

import java.io.*;

public class LogicData2C {
    private final static int NUMBER_OF_CHANNELS = 2;

    public static void main(String[] args) throws IOException {
        System.out.println("This tool reads .csv file produced by Logic software and produces C code based on it");
        if (args.length == 0)
            System.err.println("Input file parameter not specified, using default input file name");
        String inputFileName = args.length == 0 ? "input.csv" : args[0];
        System.out.println("Reading from " + inputFileName);

        BufferedReader br = new BufferedReader(new FileReader(inputFileName));

        BufferedWriter bw = new BufferedWriter(new FileWriter("output.c"));

        int lineCounter = 0;
        String line;

        boolean currentStates[] = new boolean[NUMBER_OF_CHANNELS];

        while ((line = br.readLine()) != null) {
            lineCounter++;
            if (lineCounter == 1) {
                // skipping header line
                continue;
            }
            line = line.replace(" ", "");
            System.out.println("Got [" + line + "]");

            String[] values = line.split(",");
            double timestamp = Double.parseDouble(values[0]);
            boolean channel0 = values[1].equals("0");
            boolean channel1 = values[2].equals("0");

            if (lineCounter > 2) {

                if (currentStates[0] != channel0) {
                    System.out.println(lineCounter + ": Update 0 " + timestamp);
                    printEvent(bw, lineCounter, timestamp, channel0, 0);
                }

                if (currentStates[1] != channel1) {
                    System.out.println(lineCounter + ": Update 1 " + timestamp);
                    printEvent(bw, lineCounter, timestamp, channel0, 1);
                }

            }
            currentStates[0] = channel0;
            currentStates[1] = channel1;
        }


        bw.close();
    }

    private static void printEvent(BufferedWriter bw, int lineCounter, double timestamp, boolean value, int index) throws IOException {
        bw.write("/*line=" + lineCounter + "*/ EVENT(/* timestamp*/" + timestamp + ", /*index*/" + index + ", /*value*/" + value + ");\n");
    }
}
