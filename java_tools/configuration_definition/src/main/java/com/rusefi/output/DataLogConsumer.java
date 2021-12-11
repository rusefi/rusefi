package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.VariableRegistry;
import org.jetbrains.annotations.NotNull;

import java.io.CharArrayWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.TreeSet;

import static com.rusefi.ConfigField.unquote;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class DataLogConsumer implements ConfigurationConsumer {
    private final String fileName;
    private final ReaderState state;
    private final CharArrayWriter tsWriter = new CharArrayWriter();
    private final TreeSet<String> comments = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

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
            DataLogFieldIterator iterator = new DataLogFieldIterator(structure.tsFields, "");
            iterator.loop();
            String content = iterator.sb.toString();
            tsWriter.append(content);
        }

        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(tsWriter.toCharArray());
            fw.close();
        }
    }

    class DataLogFieldIterator extends FieldIterator {
        private final String prefix;
        StringBuilder sb = new StringBuilder();

        public DataLogFieldIterator(List<ConfigField> fields, String prefix) {
            super(fields);
            this.prefix = prefix;
        }

        @Override
        public void end() {
            String content = handle(cf, prefix);
            sb.append(content);
            super.end();
        }
    }

    private String handle(ConfigField configField, String prefix) {
        if (configField.getName().contains("unused"))
            return "";

        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            DataLogFieldIterator fieldIterator = new DataLogFieldIterator(cs.tsFields, extraPrefix);
            fieldIterator.loop();
            return fieldIterator.sb.toString();
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

        String comment = getComment(prefix, configField, state.variableRegistry);

        if (comments.contains(comment))
            throw new IllegalStateException(comment + " already present in the outputs!");
        comments.add(comment);
        return "entry = " + prefix + configField.getName() + ", " + comment + ", " + typeString + "\n";
    }

    @NotNull
    public static String getComment(String prefix, ConfigField configField, VariableRegistry variableRegistry) {
        String comment = variableRegistry.applyVariables(configField.getComment());
        String[] comments = comment == null ? new String[0] : comment.split("\\\\n");
        comment = (comments.length > 0) ? comments[0] : "";

        if (comment.isEmpty())
            comment =  prefix + unquote(configField.getName());

        if (comment.charAt(0) != '"')
            comment = quote(comment);
        return comment;
    }

    public CharArrayWriter getTsWriter() {
        return tsWriter;
    }
}
