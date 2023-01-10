package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;

import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.output.DataLogConsumer.getHumanGaugeName;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class GaugeConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final StringBuilder charArrayWriter = new StringBuilder();

    public GaugeConsumer(String fileName) {
        this.fileName = fileName;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.isStackEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.getTsFields(), "",
                    (state, configField, prefix) -> handle(configField, prefix));
            iterator.loop();
            String content = iterator.getContent();
            charArrayWriter.append(content);
        }

        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(charArrayWriter.toString());
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

        String fullName = prefix + configField.getName();
        return fullName + "Gauge = " + fullName + "," + comment +
                ", " + quote(configField.getUnits()) +
                ", " + min + "," + max +
                ", " + min + "," + max +
                ", " + min + "," + max +
                ", " + digits + "," + digits +
                "\n";
    }

    public String getContent() {
        return charArrayWriter.toString();
    }
}
