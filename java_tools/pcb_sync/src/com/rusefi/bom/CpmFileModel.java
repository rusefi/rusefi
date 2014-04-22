package com.rusefi.bom;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static com.rusefi.bom.BomBuilder.log;

/**
 * (c) Andrey Belomutskiy
 * 2/22/14
 */
public class CpmFileModel {

    public final Map<String, List<BomComponent>> componentsByKey = new TreeMap<String, List<BomComponent>>(String.CASE_INSENSITIVE_ORDER);

    public int linesCount;
    public int componentsCount;


    public void readCmpFile(List<String> list) throws IOException {
        linesCount = list.size();
        componentsCount = 0;

        for (int lineIndex = 0; lineIndex < list.size(); lineIndex++) {
            String line = list.get(lineIndex);
            if ("BeginCmp".equals(line)) {
                componentsCount++;

                lineIndex += 2; // skipping 'TimeStamp' line

                String a = list.get(lineIndex);
                String reference = parseReferenceLine(a);
                log(lineIndex + ": reference: " + reference);
                lineIndex++;

                String valueLine = list.get(lineIndex);
                String value = parseValueLine(valueLine);
                log(lineIndex + ": value: " + value);
                lineIndex++;

                String packageLine = list.get(lineIndex);

                String packageName = packageLine.split("=")[1].trim();
                packageName = cutLastSymbol(packageName);
                log(lineIndex + ": package: " + packageName);
                lineIndex++;

                String package_value = packageName + "_" + value;
                List<BomComponent> l = componentsByKey.get(package_value);
                if (l == null) {
                    l = new ArrayList<BomComponent>();
                    componentsByKey.put(package_value, l);
                }
                l.add(new BomComponent(reference));
            }
        }
    }

    private static String parseValueLine(String s) {
        String value = s.split("=")[1].trim();
        return cutLastSymbol(value);
    }

    private static String parseReferenceLine(String line) {
        String reference = line.split("=")[1].trim();
        return cutLastSymbol(reference);
    }

    private static String cutLastSymbol(String reference) {
        return reference.substring(0, reference.length() - 1);
    }
}
