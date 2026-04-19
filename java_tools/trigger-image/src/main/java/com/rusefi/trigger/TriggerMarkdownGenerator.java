package com.rusefi.trigger;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;
import java.util.stream.Collectors;

public class TriggerMarkdownGenerator {
    public static void generate(String workingFolder, String outputFile) throws IOException {
        List<TriggerWheelInfo> wheels = new ArrayList<>();
        TriggerWheelInfo.readWheels(workingFolder, wheels::add);

        // Group by size/tooth count (number of signals)
        Map<Integer, List<TriggerWheelInfo>> groupedByCount = wheels.stream()
                .collect(Collectors.groupingBy(w -> w.getSignals().size()));

        // Sort sizes in ascending order
        List<Integer> sortedCounts = new ArrayList<>(groupedByCount.keySet());
        Collections.sort(sortedCounts);

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            writer.println("# Trigger Wheels Report");
            writer.println();
            writer.println("Total triggers: " + wheels.size());
            writer.println();

            for (int count : sortedCounts) {
                writer.println("## tooth count " + (count / 2));
                List<TriggerWheelInfo> group = groupedByCount.get(count);
                // Sort by name within the group
                group.sort(Comparator.comparing(TriggerWheelInfo::getTriggerName));
                for (TriggerWheelInfo info : group) {
                    String name = info.getTriggerName();
                    if (name.startsWith("TT_")) {
                        name = name.substring(3);
                    }
                    String lowerName = name.toLowerCase();
                    if (lowerName.contains("custom") || lowerName.contains("unused") || lowerName.contains("dev")) {
                        continue;
                    }
                    writer.println("- [" + name + "](#" + name + ")");
                }
                writer.println();
            }
        }
    }

    public static void main(String[] args) throws IOException {
        String workingFolder = args.length > 0 ? args[0] : TriggerWheelInfo.DEFAULT_WORK_FOLDER;
        String outputFile = args.length > 1 ? args[1] : "triggers.md";
        generate(workingFolder, outputFile);
    }
}
