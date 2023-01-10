package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;
import java.util.TreeSet;

import static com.rusefi.VariableRegistry.unquote;
import static org.abego.treelayout.internal.util.java.lang.string.StringUtil.quote;

/**
 * here we generate [Datalog] section of TS .ini file
 * DataLog section in turn references [OutputChannels] entities
 * @see SdCardFieldsConsumer
 */
public class DataLogConsumer implements ConfigurationConsumer {
    // https://github.com/rusefi/web_backend/issues/166
    private static final int MSQ_LENGTH_LIMIT = 34;

    public static final String UNUSED = ConfigStructure.UNUSED_ANYTHING_PREFIX;
    private final String fileName;
    private final StringBuilder tsWriter = new StringBuilder();
    private final TreeSet<String> comments = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    public DataLogConsumer(String fileName) {
        this.fileName = fileName;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.isStackEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.getTsFields(), "",
                    (configField, prefix, prefix2) -> handle(prefix, prefix2));
            iterator.loop();
            String content = iterator.getContent();
            tsWriter.append(content);
        }

        writeStringToFile(fileName, tsWriter);
    }

    private void writeStringToFile(@Nullable String fileName, StringBuilder writer) throws IOException {
        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(writer.toString());
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

        String comment = getHumanGaugeName(prefix, configField);

        if (comments.contains(comment))
            throw new IllegalStateException(comment + " already present in the outputs! " + configField);
        comments.add(comment);
        return "entry = " + prefix + configField.getName() + ", " + comment + ", " + typeString + "\n";
    }

    /**
     * Short human-readable field summary as used for gauge names and log file keys taken from the first line of the comment
     * More detailed technical explanation should be placed in consecutive lines
     */
    @NotNull
    public static String getHumanGaugeName(String prefix, ConfigField configField) {
        String comment = configField.getCommentTemplated();
        comment = getFirstLine(comment);

        if (comment.isEmpty()) {
            /**
             * @see ConfigFieldImpl#getCommentOrName()
             */
            comment = prefix + unquote(configField.getName());
        }
        if (comment.length() > MSQ_LENGTH_LIMIT)
            throw new IllegalStateException("[" + comment + "] is too long for log files at " + comment.length());

        if (comment.charAt(0) != '"')
            comment = quote(comment);
        return comment;
    }

    private static String getFirstLine(String comment) {
        String[] comments = comment == null ? new String[0] : unquote(comment).split("\\\\n");
        comment = (comments.length > 0) ? comments[0] : "";
        return comment;
    }

    public String getContent() {
        return tsWriter.toString();
    }
}
