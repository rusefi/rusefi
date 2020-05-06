package com.rusefi;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * Andrey Belomutskiy, (c) 2012-2016
 * 7/20/2016
 */
public class FiringOrderTSLogic {

    private static final String FIRING_ORDER_PREFIX = "FO_";

    public static void main(String[] args) throws IOException {
        invoke("../firmware/controllers/algo/firing_order.h");
    }

    public static String invoke(String fileName) throws IOException {
        State state = new State();

        readFiringOrders(fileName, state);

        StringBuilder sb = new StringBuilder();
        for (int i = 2; i <= 12; i++) {
            String line = processId(i, state);
            sb.append(line).append("\r\n");
        }
        return sb.toString();
    }

    private static void readFiringOrders(String fileName, State state) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        String line;
        while ((line = br.readLine()) != null) {
            int index = line.indexOf(FIRING_ORDER_PREFIX);
            if (index == -1) {
                System.out.println("Skipping [" + line);
                continue;
            }
            line = line.substring(index + FIRING_ORDER_PREFIX.length());
            parseLine(line, state);
        }
    }

    public static void parseLine(String line, State state) {
        line = line.replaceAll("[\\s]*\\,.*", "");
        line = line.replaceAll("[\\s\\,]", "");
        System.out.println("Processing " + line);

        String s[] = line.split("\\=");
        String order[] = s[0].split("_");
        int ordinal = Integer.parseInt(s[1]);

        System.out.println("order " + Arrays.toString(order) + ": " + ordinal);

        state.maxOrdinal = Math.max(ordinal, state.maxOrdinal);
        state.ordinal2order.put(ordinal, order);
    }

    private static String processId(int cylinderId, State state) {

        StringBuilder logic = new StringBuilder();


        for (Map.Entry<Integer, String[]> e : state.ordinal2order.entrySet()) {
            Integer ordinal = e.getKey();
            String[] order = e.getValue();
            if (order.length % 2 != 0) {
                // need even number of cylinders for wasted spark, right?
                continue;
            }
            int halfSize = order.length / 2;

            boolean allowed = false;
            for (int i = 0; i < halfSize; i++) {
                allowed |= order[i].equals(Integer.toString(cylinderId));
            }

            if (allowed) {
                if (logic.length() > 0)
                    logic.append(" || ");

                logic.append("(firingOrder == ").append(ordinal).append(")");

            }
        }
        String result = logic.length() == 0 ? "" : "|| (" + logic + ")";

        //String output = "#define ignitionPin" + cylinderId + "logic" + " " + result + "";
        String output = "\t\tfield = \"Ignition Pin " + cylinderId +
                "\",                       ignitionPins" + cylinderId +
                ", {isIgnitionEnabled == 1 && (ignitionMode != 0 && cylindersCount >= " + cylinderId + ") && (ignitionMode !=2 || twoWireBatchIgnition == 1 " + result + ")}";
        System.out.println(output);
        return output;
    }

    public static class State {
        final Map<Integer, String[]> ordinal2order = new HashMap<>();
        int maxOrdinal;
    }

}
