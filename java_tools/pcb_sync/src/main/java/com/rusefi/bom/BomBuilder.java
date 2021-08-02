package com.rusefi.bom;

import com.rusefi.pcb.ModuleNode;
import com.rusefi.pcb.PcbNode;
import com.rusefi.util.FileUtils;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

/**
 * (c) Andrey Belomutskiy
 * 2/2/14
 */
public class BomBuilder {
    private static final String DELIMITER = ",";

    private static final CpmFileModel allComponents = new CpmFileModel();
    private static final String IGNORE_TAG = "ignore";
    private static String cmpFileName;
    private static String bomDictionaryName;
    private static Map<String, BomRecord> bomDictionary;
    private static Set<String> ignoreList = new TreeSet<String>(String.CASE_INSENSITIVE_ORDER);

    private static boolean printQtyInFull = true;
    private static boolean printPadCount = false;
    private static boolean printReference = false;
    private static boolean printUserComment = false;
    private static PcbNode pcb;

    public static void main(String[] args) throws IOException {
        if (args.length < 3) {
            System.out.println("bom_builder FILE_NAME.CMP PCB.kicad_pcb COMPONENTS_DICTIONARY.CSV OUTPUT_FILE.CSV");
            return;
        }
        cmpFileName = args[0];
        String pcbFileName = args[1];
        bomDictionaryName = args[2];
        String outputFileName = args[3];

        pcb = PcbNode.readFromFile(pcbFileName);

        for (int i = 4; i < args.length; i++) {
            String option = args[i].trim();
            if (option.equalsIgnoreCase("printUserComment")) {
                printUserComment = true;
            } else if (option.equalsIgnoreCase("printreference")) {
                printReference = true;
            } else if (option.equalsIgnoreCase("printpadcount")) {
                printPadCount = true;
            } else if (option.equalsIgnoreCase("skipqtyinfull")) {
                printQtyInFull = false;
            }
        }


        allComponents.readCmpFile(FileUtils.readFileToList(cmpFileName));

        bomDictionary = readBomDictionary(FileUtils.readFileToList(bomDictionaryName));

        writeCompactPartList(outputFileName + "_compact.csv", bomDictionary);
        writeFullPartList(outputFileName + "_full.csv", bomDictionary);
    }

    private static void writeMissingElements(Map<String, BomRecord> bomDictionary, BufferedWriter bw, Map<String, List<BomComponent>> componentsByKey) throws IOException {
        for (Iterator<Map.Entry<String, List<BomComponent>>> i = componentsByKey.entrySet().iterator(); i.hasNext(); ) {
            Map.Entry<String, List<BomComponent>> e = i.next();
            String key = e.getKey();

            if (ignoreList.contains(key))
                continue;

            BomRecord bomRecord = bomDictionary.get(key);
            if (bomRecord == null) {
                bw.write("### no BOM record for " + key + ": " + e.getValue() + "\r\n");
                i.remove();
            }
        }
        bw.write("\r\n\n\n\n");
    }

    private static void writeFullPartList(String outputFileName, Map<String, BomRecord> bomDictionary) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName));

        Map<String, List<BomComponent>> componentsByKey = new TreeMap<String, List<BomComponent>>(allComponents.componentsByKey);

        log("Writing full parts list to " + outputFileName);
        writeCommonHeader(bw);
        writeMissingElements(bomDictionary, bw, componentsByKey);

        StringBuilder notMounted = new StringBuilder();

        for (Map.Entry<String, List<BomComponent>> e : componentsByKey.entrySet()) {
            // for instance, SM0805_47pF
            String componentName = e.getKey();

            List<BomComponent> list = e.getValue();

            if (ignoreList.contains(componentName))
                continue;

            BomRecord bomRecord = bomDictionary.get(componentName);
            if (bomRecord == null)
                throw new NullPointerException();




            for (BomComponent c : list) {
                if (!findModule(c.getReference())) {
                    bw.write("Not mounted: " + c.getReference() + "\r\n");
                    continue;
                }
                writeLine(bw, bomRecord, 1, c.getReference() + ": ", c.getReference());
            }
        }
        bw.close();
    }

    private static boolean findModule(String reference) {
        for (PcbNode node : pcb.iterate("module")) {
            if (node instanceof ModuleNode) {
                ModuleNode mn = (ModuleNode) node;
                if (mn.getReference().equalsIgnoreCase(reference))
                    return true;
            }
        }
        return false;
    }

    private static void writeCompactPartList(String outputFileName, Map<String, BomRecord> bomDictionary) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName));
        Map<String, List<BomComponent>> componentsByKey = new TreeMap<String, List<BomComponent>>(allComponents.componentsByKey);

        log("Writing compact parts list to " + outputFileName);
        writeCommonHeader(bw);
        writeMissingElements(bomDictionary, bw, componentsByKey);

        for (Map.Entry<String, List<BomComponent>> e : componentsByKey.entrySet()) {
            // for instance, SM0805_47pF
            String componentName = e.getKey();

            if (ignoreList.contains(componentName))
                continue;

            List<BomComponent> list = e.getValue();
            log(list.size() + " items of " + componentName);

            BomRecord bomRecord = bomDictionary.get(componentName);
            if (bomRecord == null)
                throw new NullPointerException();
            writeLine(bw, bomRecord, list.size(), "", "");
        }
        bw.close();
    }

    private static void writeCommonHeader(BufferedWriter bw) throws IOException {
        bw.write("### this file was generated by rusEfi bom_builder on " + new Date() + "\r\n");
        bw.write("### " + allComponents.componentsCount + " components in " + allComponents.linesCount + " lines of " + cmpFileName + "\r\n");
        bw.write("### " + allComponents.componentsByKey.size() + " kinds of components in input file\r\n");
        bw.write("### " + bomDictionary.size() + " entries in BOM dictionary " + bomDictionaryName + "\r\n");
        bw.write("### " + ignoreList.size() + " entries in ignore list\r\n");
    }

    private static void writeLine(BufferedWriter bw, BomRecord bomRecord, int quantity, String prefix, String reference) throws IOException {
        bw.write(quantity + DELIMITER +
                        bomRecord.getStorePart() + DELIMITER +
                        prefix + bomRecord.getCustomerRef() + DELIMITER +
                        (printReference ? reference + DELIMITER : "") +
                        (printUserComment ? bomRecord.getUserComment() + DELIMITER : "") +
                        (printPadCount ? bomRecord.getPadCount() + DELIMITER : "") +
                        "\r\n"
        );
    }

    private static Map<String, BomRecord> readBomDictionary(List<String> strings) {
        Map<String, BomRecord> result = new TreeMap<String, BomRecord>(String.CASE_INSENSITIVE_ORDER);
        for (String line : strings) {
            line = line.trim();
            if (line.isEmpty())
                continue;

            if (line.startsWith("#")) {
                log("Skipping comment: " + line);
                continue;
            }

            String[] tokens = line.split(",");

            if (tokens.length < 2) {
                log("Unexpected line: " + line + " Expected at least two tokens but " + tokens.length);
                System.exit(-1);
            }

            String ref = tokens[0];
            String mfgPart = tokens[1];

            if (mfgPart.equalsIgnoreCase(IGNORE_TAG)) {
                log("Ignoring entry: " + ref);
                ignoreList.add(ref);
                continue;
            }

            if (tokens.length != 6) {
                log("Unexpected line: [" + line + "] Expected 6 tokens but " + tokens.length);
                System.exit(-1);
            }

            String storePart = tokens[2];
            String componentName = tokens[3];
            int padCount = Integer.parseInt(tokens[4]);
            String customerRef = tokens[5];

            result.put(ref, new BomRecord(mfgPart, storePart, customerRef, padCount, componentName));

            log("BOM key: " + ref);
            log("mfgPartNo: " + mfgPart);
            log("storePartNo: " + storePart);
        }

        log("Got " + result.size() + " entries in BOM dictionary");
        log("Got " + ignoreList.size() + " entries in ignore list");
        return result;
    }

    protected static void log(String msg) {
        System.out.println(msg);
    }
}
