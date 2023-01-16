package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;

import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map;

import static com.rusefi.output.DataLogConsumer.getHumanGaugeName;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class GaugeConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final LinkedHashMap<String, StringBuilder> byCategory = new LinkedHashMap<>();

    public GaugeConsumer(String fileName) {
        this.fileName = fileName;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.isStackEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.getTsFields(), "",
                    (state, configField, prefix) -> handle(configField, prefix));
            iterator.loop();
        }
    }

    @Override
    public void endFile() throws IOException {
        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(getContent());
            fw.close();
        }
    }

    private String handle(ConfigField configField, String prefix) {
        String comment = getHumanGaugeName("", configField);
        comment = ConfigFieldImpl.unquote(comment);
        if (!prefix.isEmpty()) {
            comment = prefix + " " + comment;
        }
        comment = quote(comment);


        double min = configField.getMin();
        double max = configField.getMax();
        int digits = configField.getDigits();
        String category = configField.getCategory();
        if (category == null)
            return "";

        StringBuilder sb = byCategory.computeIfAbsent(category, s -> new StringBuilder());

        String fullName = prefix + configField.getName();
        String gaugeEntry = fullName + "Gauge = " + fullName + "," + comment +
                ", " + quote(configField.getUnits()) +
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
