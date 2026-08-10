package com.rusefi.pinout;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

/**
 * Generates visual patchcord assembly instructions: which wire goes from which ECU plug pin
 * to which vehicle-adapter plug pin, in phases following the Universal Patchcord Color Scheme
 * wiring order, with connector photos highlighting the pins of each phase.
 * <p>
 * ECU pins and adapter pins are matched by function text (parenthesized suffixes such as
 * "(pin 6)" / "(OEM 2-8)" are ignored); pins without a match on the other side are listed
 * at the end of the generated document for manual review.
 */
public class PatchCordHelper {
    static class PinRef {
        final ConnectorYaml connector;
        final ConnectorYaml.PinEntry pin;

        PinRef(ConnectorYaml connector, ConnectorYaml.PinEntry pin) {
            this.connector = connector;
            this.pin = pin;
        }

        String label() {
            return connector.name + " / " + pin.pin;
        }
    }

    static class Wire {
        final PinRef ecu;
        final PinRef adapter;

        Wire(PinRef ecu, PinRef adapter) {
            this.ecu = ecu;
            this.adapter = adapter;
        }

        // readme: wire colors come from the ECU yaml files
        String color() {
            if (ecu.pin.color != null && !ecu.pin.color.isEmpty()) {
                return ecu.pin.color;
            }
            return adapter.pin.color == null ? "" : adapter.pin.color;
        }
    }

    static class Phase {
        final String title;
        final Set<String> types;

        Phase(String title, String... types) {
            this.title = title;
            this.types = new LinkedHashSet<>(Arrays.asList(types));
        }
    }

    /**
     * Suggested wiring order from Universal-Patchcord-Color-Scheme.md, keyed by pin 'type'
     */
    private static final List<Phase> PHASES = Arrays.asList(
            new Phase("Power and grounds", "12v", "5v", "mr", "gnd", "pgnd", "sgnd", "agnd"),
            new Phase("Electronic throttle and pedal", "etb"),
            new Phase("Communication", "can"),
            new Phase("Sensors", "av", "at", "hall", "vr", "din"),
            new Phase("Wideband oxygen sensor", "wbo"),
            new Phase("Injectors, coils and other numbered channels", "inj", "ign", "ls", "hs"),
            new Phase("Everything else"));

    public static void main(String[] args) throws IOException {
        String ecuFolder = args.length > 0 ? args[0] : "firmware/config/boards/hellen/super-uaefi";
        String breakoutBoard = args.length > 1 ? args[1] : "../private-hardware/BMW-N52-adapter";
        String outputFolder = args.length > 2 ? args[2] : breakoutBoard + "/patchcord";

        List<ConnectorYaml> ecuConnectors = ConnectorYaml.readFolder(new File(ecuFolder, "connectors"));
        List<ConnectorYaml> adapterConnectors = ConnectorYaml.readFolder(new File(breakoutBoard, "connectors"));

        List<PinRef> ecuPins = flatten(ecuConnectors);
        List<PinRef> adapterPins = flatten(adapterConnectors);

        List<Wire> wires = matchByFunction(ecuPins, adapterPins);

        File outputDir = new File(outputFolder);
        if (!outputDir.isDirectory() && !outputDir.mkdirs()) {
            throw new IOException("Failed to create " + outputDir.getAbsolutePath());
        }
        writeInstructions(new File(ecuFolder).getName(), new File(breakoutBoard).getName(),
                wires, ecuPins, adapterPins, outputDir);
        System.out.println("Wrote " + new File(outputDir, "patchcord.md").getAbsolutePath()
                + ": " + wires.size() + " wire(s), "
                + ecuPins.size() + " unmatched ECU pin(s), "
                + adapterPins.size() + " unmatched adapter pin(s)");
    }

    private static List<PinRef> flatten(List<ConnectorYaml> connectors) {
        List<PinRef> result = new ArrayList<>();
        for (ConnectorYaml connector : connectors) {
            for (ConnectorYaml.PinEntry pin : connector.pins) {
                result.add(new PinRef(connector, pin));
            }
        }
        return result;
    }

    /**
     * "WBO1 Vs (Un) (pin 6)" and "WBO1 Vs (Un) (OEM 2-8)" both normalize to "wbo1 vs un"
     */
    static String normalizeFunction(String function) {
        if (function == null) {
            return "";
        }
        String s = function.toLowerCase(Locale.ROOT)
                .replaceAll("\\(pin [^)]*\\)", " ")
                .replaceAll("\\(oem [^)]*\\)", " ")
                .replaceAll("[^a-z0-9+/]+", " ")
                .trim();
        return s;
    }

    /**
     * Pairs ECU pins with adapter pins sharing the same normalized function; matched pins are
     * removed from both input lists, so leftovers are the unmatched ones.
     */
    static List<Wire> matchByFunction(List<PinRef> ecuPins, List<PinRef> adapterPins) {
        Map<String, List<PinRef>> adapterByFunction = new TreeMap<>();
        for (PinRef ref : adapterPins) {
            String key = normalizeFunction(ref.pin.function);
            if (!key.isEmpty()) {
                adapterByFunction.computeIfAbsent(key, k -> new ArrayList<>()).add(ref);
            }
        }
        List<Wire> wires = new ArrayList<>();
        for (PinRef ecu : new ArrayList<>(ecuPins)) {
            String key = normalizeFunction(ecu.pin.function);
            List<PinRef> candidates = adapterByFunction.get(key);
            if (candidates == null || candidates.isEmpty()) {
                continue;
            }
            PinRef adapter = candidates.remove(0);
            wires.add(new Wire(ecu, adapter));
            ecuPins.remove(ecu);
            adapterPins.remove(adapter);
        }
        return wires;
    }

    private static Phase phaseOf(Wire wire) {
        String type = wire.ecu.pin.type != null ? wire.ecu.pin.type : wire.adapter.pin.type;
        String key = type == null ? "" : type.toLowerCase(Locale.ROOT);
        for (Phase phase : PHASES) {
            if (phase.types.contains(key)) {
                return phase;
            }
        }
        return PHASES.get(PHASES.size() - 1);
    }

    private static String fileNameSafe(String s) {
        return s.replaceAll("[^A-Za-z0-9_-]+", "_");
    }

    private static void writeInstructions(String ecuName, String adapterName, List<Wire> wires,
                                          List<PinRef> unmatchedEcu, List<PinRef> unmatchedAdapter,
                                          File outputDir) throws IOException {
        Map<Phase, List<Wire>> byPhase = new LinkedHashMap<>();
        for (Phase phase : PHASES) {
            byPhase.put(phase, new ArrayList<>());
        }
        for (Wire wire : wires) {
            byPhase.get(phaseOf(wire)).add(wire);
        }

        // pins wired in earlier steps stay visible as dimmed circles: visual progress tracking
        Map<ConnectorYaml, Set<String>> done = new LinkedHashMap<>();

        try (PrintWriter md = new PrintWriter(Files.newBufferedWriter(
                new File(outputDir, "patchcord.md").toPath(), StandardCharsets.UTF_8))) {
            md.println("# Patchcord assembly: " + ecuName + " to " + adapterName);
            md.println();
            md.println("Generated by PatchCordHelper - do not edit manually.");
            md.println();
            md.println(wires.size() + " wire(s) matched by pin function. Wiring order follows the");
            md.println("[Universal Patchcord Color Scheme](https://github.com/rusefi/rusefi_documentation/blob/master/Universal-Patchcord-Color-Scheme.md).");
            md.println();
            md.println("On each image: solid circles are wires of the current step (circle color = wire color),");
            md.println("dimmed grey circles are pins already wired in previous steps.");
            md.println();
            md.println("Vehicle adapter side images have orange circles in all four corners: put the paint");
            md.println("marker on that end of each wire, so the two identical plugs of the finished patchcord");
            md.println("can be told apart.");

            int stepIndex = 0;
            for (Map.Entry<Phase, List<Wire>> entry : byPhase.entrySet()) {
                List<Wire> phaseWires = entry.getValue();
                if (phaseWires.isEmpty()) {
                    continue;
                }
                phaseWires.sort(Comparator
                        .comparing((Wire w) -> w.color())
                        .thenComparing(w -> w.ecu.label()));

                List<List<Wire>> subSteps = splitByUniqueColor(phaseWires);
                for (int subStep = 0; subStep < subSteps.size(); subStep++) {
                    List<Wire> stepWires = subSteps.get(subStep);
                    stepIndex++;
                    String title = entry.getKey().title
                            + (subSteps.size() > 1 ? " (" + (subStep + 1) + " of " + subSteps.size() + ")" : "");

                    md.println();
                    md.println("## Step " + stepIndex + ": " + title);
                    md.println();
                    md.println("| Wire color | ECU pin | Adapter pin | Function |");
                    md.println("|------------|---------|-------------|----------|");
                    for (Wire wire : stepWires) {
                        md.println("| " + wire.color() + " | " + wire.ecu.label() + " | " + wire.adapter.label()
                                + " | " + safe(wire.ecu.pin.function) + " |");
                    }

                    writeStepImages(md, stepIndex, "ecu", ecuName, stepWires, true, done, outputDir);
                    writeStepImages(md, stepIndex, "adapter", adapterName, stepWires, false, done, outputDir);
                }
            }

            writeUnmatched(md, "ECU (" + ecuName + ")", unmatchedEcu);
            writeUnmatched(md, "adapter (" + adapterName + ")", unmatchedAdapter);
        }
    }

    /**
     * Within one assembly step every wire must have a distinct color, otherwise two freshly cut
     * wires of the same color cannot be told apart; overflow goes into additional sub-steps.
     */
    static List<List<Wire>> splitByUniqueColor(List<Wire> wires) {
        List<List<Wire>> subSteps = new ArrayList<>();
        List<Set<String>> usedColors = new ArrayList<>();
        for (Wire wire : wires) {
            String color = wire.color().toLowerCase(Locale.ROOT);
            int target = 0;
            while (target < subSteps.size() && usedColors.get(target).contains(color)) {
                target++;
            }
            if (target == subSteps.size()) {
                subSteps.add(new ArrayList<>());
                usedColors.add(new LinkedHashSet<>());
            }
            subSteps.get(target).add(wire);
            usedColors.get(target).add(color);
        }
        return subSteps;
    }

    private static void writeStepImages(PrintWriter md, int stepIndex, String side, String sideName,
                                        List<Wire> stepWires, boolean isEcuSide,
                                        Map<ConnectorYaml, Set<String>> done,
                                        File outputDir) throws IOException {
        Map<ConnectorYaml, Map<String, String>> byConnector = new LinkedHashMap<>();
        for (Wire wire : stepWires) {
            PinRef ref = isEcuSide ? wire.ecu : wire.adapter;
            byConnector.computeIfAbsent(ref.connector, k -> new LinkedHashMap<>())
                    .put(ref.pin.pin, wire.color());
        }
        for (Map.Entry<ConnectorYaml, Map<String, String>> entry : byConnector.entrySet()) {
            ConnectorYaml connector = entry.getKey();
            Set<String> connectorDone = done.computeIfAbsent(connector, k -> new LinkedHashSet<>());
            String imageName = "step-" + stepIndex + "-" + side + "-" + fileNameSafe(connector.name) + ".png";
            if (connector.imageFile != null && connector.imageFile.isFile()) {
                StepImageRenderer.render(connector, entry.getValue(), connectorDone, !isEcuSide, new File(outputDir, imageName));
                md.println();
                md.println(sideName + ", connector " + connector.name + ":");
                md.println();
                md.println("![" + sideName + " " + connector.name + "](" + imageName + ")");
            } else {
                md.println();
                md.println(sideName + ", connector " + connector.name + ": no photo available");
            }
            connectorDone.addAll(entry.getValue().keySet());
        }
    }

    private static void writeUnmatched(PrintWriter md, String sideTitle, List<PinRef> unmatched) {
        if (unmatched.isEmpty()) {
            return;
        }
        md.println();
        md.println("## Unmatched pins on " + sideTitle + " side");
        md.println();
        md.println("No pin with the same function was found on the other side; wire manually or fix the yaml function text.");
        md.println();
        md.println("| Pin | Type | Wire color | Function |");
        md.println("|-----|------|------------|----------|");
        for (PinRef ref : unmatched) {
            md.println("| " + ref.label() + " | " + safe(ref.pin.type) + " | " + safe(ref.pin.color)
                    + " | " + safe(ref.pin.function) + " |");
        }
    }

    private static String safe(String s) {
        return s == null ? "" : s.replace("|", "\\|");
    }
}
