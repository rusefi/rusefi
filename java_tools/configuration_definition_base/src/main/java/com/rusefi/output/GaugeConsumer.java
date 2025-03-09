package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import com.rusefi.VariableRegistry;
import com.rusefi.util.LazyFile;

import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map;

import static com.rusefi.output.DataLogConsumer.getHumanGaugeName;

public class GaugeConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final LazyFile.LazyFileFactory fileFactory;
    private final LinkedHashMap<String, StringBuilder> byCategory = new LinkedHashMap<>();
    public String[] outputNames = new String[]{""};

    public GaugeConsumer(String fileName, LazyFile.LazyFileFactory fileFactory) {
        this.fileName = fileName;
        this.fileFactory = fileFactory;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.isStackEmpty()) {
            for (int i = 0; i < outputNames.length; i++) {

                String variableNamePrefix = outputNames.length > 1 ? outputNames[i] : "";

                PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.getTsFields(), variableNamePrefix,
                        (state, configField, prefix, currentPosition, perFieldWithStructuresIterator) -> handle(configField, prefix, ""));
                iterator.loop(0);
            }
        }
    }

    @Override
    public void endFile() throws IOException {
        if (fileName != null) {
            LazyFile fw = fileFactory.create(fileName);
            fw.write(getContent());
            fw.close();
        }
    }

    private String handle(ConfigField configField, String prefix, String variableNameSuffix) {
        String comment = getHumanGaugeName("", "", configField, variableNameSuffix);
        comment = ConfigFieldImpl.unquote(comment);
        if (!prefix.isEmpty()) {
            comment = prefix + " " + comment;
        }
        comment = VariableRegistry.quote(comment);


        double min = configField.getMin();
        double max = configField.getMax();
        int digits = configField.getDigits();
        String category = configField.getCategory();
        if (category == null)
            return "";

        StringBuilder sb = byCategory.computeIfAbsent(category, s -> new StringBuilder());

        String fullName = prefix + configField.getName();
        String gaugeEntry = fullName + variableNameSuffix + "Gauge = " + fullName + variableNameSuffix + "," + comment +
                ", " + VariableRegistry.quote(configField.getUnits()) +
                ", " + min + "," + max +
                ", " + min + "," + max +
                ", " + min + "," + max +
                ", " + digits + "," + digits +
                "\n";
        sb.append(gaugeEntry);

        return "";
    }

    public String getContent() {
        StringBuilder sb = new StringBuilder();

        for (Map.Entry<String, StringBuilder> e : byCategory.entrySet()) {
            sb.append("\t").append("gaugeCategory = ").append(e.getKey()).append("\n");
            sb.append(e.getValue());
        }

        return sb.toString();
    }
}
