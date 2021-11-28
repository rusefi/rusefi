package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;

import java.io.CharArrayWriter;
import java.io.FileWriter;
import java.io.IOException;

import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class DataLogConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final ReaderState state;
    private final CharArrayWriter tsWriter = new CharArrayWriter();

    public DataLogConsumer(String fileName, ReaderState state) {
        this.fileName = fileName;
        this.state = state;
    }

    @Override
    public void startFile() throws IOException {
        System.out.println("startFile");

    }

    @Override
    public void endFile() throws IOException {
        System.out.println("endFile");

    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {

            FieldIterator iterator = new FieldIterator(structure.tsFields);
            for (int i = 0; i < structure.tsFields.size(); i++) {
                iterator.start(i);

                tsWriter.append(handle(iterator.cf));

                iterator.end();

            }
        }

        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(tsWriter.toCharArray());
            fw.close();
        }
    }

    private String handle(ConfigField configField) {
        if (configField.isBit()) {
            return "";
        }

        if (configField.isArray()) {

            return "";
        }
//        entry = seconds,         @@GAUGE_NAME_UPTIME@@,
//        entry = RPMValue,        @@GAUGE_NAME_RPM@@,

        String typeString;
        String autoscaleSpec = configField.autoscaleSpec();
        if (TypesHelper.isFloat(configField.getType()) || (autoscaleSpec != null && !autoscaleSpec.equals("1, 1"))) {
            typeString = "float,  \"%.3f\"";
        } else {
            typeString = "int,    \"%d\"";
        }

        String comment = state.variableRegistry.applyVariables(configField.getComment());
        if (comment.isEmpty() || comment.charAt(0) != '"')
            comment = quote(comment);

        return "entry = " + configField.getName() + ", " + comment + ", " + typeString + "\n";
    }

    public CharArrayWriter getTsWriter() {
        return tsWriter;
    }
}
