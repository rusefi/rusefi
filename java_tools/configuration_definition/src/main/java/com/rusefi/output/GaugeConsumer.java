package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.CharArrayWriter;
import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.output.DataLogConsumer.getComment;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class GaugeConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final StringBuilder charArrayWriter = new StringBuilder();

    public GaugeConsumer(String fileName) {
        this.fileName = fileName;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.stack.isEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.tsFields, "",
                    (state, configField, prefix) -> handle(readerState, configField, prefix));
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

    private String handle(ReaderState readerState, ConfigField configField, String prefix) {
        String comment = getComment("", configField, readerState.variableRegistry);
        comment = ConfigField.unquote(comment);
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
