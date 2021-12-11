package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.CharArrayWriter;
import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.output.DataLogConsumer.getComment;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class GaugeConsumer extends AbstractConfigurationConsumer {
    private final String fileName;
    private final ReaderState state;
    private final CharArrayWriter charArrayWriter = new CharArrayWriter();

    public GaugeConsumer(String fileName, ReaderState state) {
        this.fileName = fileName;
        this.state = state;
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(structure.tsFields, "",
                    this::handle);
            iterator.loop();
            String content = iterator.sb.toString();
            charArrayWriter.append(content);
        }

        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(charArrayWriter.toCharArray());
            fw.close();
        }
    }

    private String handle(ConfigField configField, String prefix) {
        String comment = getComment("", configField, state.variableRegistry);
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

    public CharArrayWriter getTsWriter() {
        return charArrayWriter;
    }
}
