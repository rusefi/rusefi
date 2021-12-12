package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.VariableRegistry;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.CharArrayWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.TreeSet;

import static com.rusefi.ConfigField.unquote;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

public class DataLogConsumer extends AbstractConfigurationConsumer {
    public static final String UNUSED = "unused";
    private final String fileName;
    private final ReaderState state;
    private final CharArrayWriter tsWriter = new CharArrayWriter();
    private final TreeSet<String> comments = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    public DataLogConsumer(String fileName, ReaderState state) {
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
            tsWriter.append(content);
        }

        writeStringToFile(fileName, tsWriter);
    }

    private void writeStringToFile(@Nullable String fileName, CharArrayWriter writer) throws IOException {
        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(writer.toCharArray());
            fw.close();
        }
    }

    private String handle(ConfigField configField, String prefix) {
        if (configField.getName().contains(UNUSED))
            return "";

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
            comment = prefix + unquote(configField.getName());

        if (comment.charAt(0) != '"')
            comment = quote(comment);
        return comment;
    }

    public CharArrayWriter getTsWriter() {
        return tsWriter;
    }
}
