package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * This command line utility is part of rusEfi unknown trigger discovery
 * https://rusefi.com/wiki/index.php?title=Manual:Software:Trigger
 *
 * 2/15/2015
 */
public class AverageAnglesUtil {

    public static final String KEY = Fields.PROTOCOL_ANALOG_CHART;
    public static final String ANALOG_CHART = KEY + ",";

    private static int currentRpm = -1;

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.out.println("One parameter expected: log_file_name.csv");
            System.exit(-1);
        }
        String fileName = args[0];
        runUtil(fileName, Logger.CONSOLE);
    }

    public static String runUtil(String fileName, Logger logger) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        String line;

        AverageAngles aa = new AverageAngles();

        while ((line = br.readLine()) != null) {
            int index = line.indexOf(Logger.END_OF_TIMESTAND_TAG);
            if (index == -1)
                continue;
            line = line.substring(index + Logger.END_OF_TIMESTAND_TAG.length());

            if (line.startsWith("time")) {
                String[] f = line.split(",");
                if (f.length > 3 && f[2].equals("rpm")) {

                    currentRpm = Integer.parseInt(f[3]);
                    logger.info("New rpm " + currentRpm);
                }
                continue;
            }

            if (!line.startsWith(ANALOG_CHART))
                continue;
            line = line.substring(ANALOG_CHART.length());

            String p[] = line.split(",");
            line = p[0];

            aa.add(currentRpm, line);
            logger.info("Got " + line);
        }

        String report = aa.getReport();
        System.out.println(report);
        return report;
    }
}