package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import com.rusefi.VariableRegistry;
import com.rusefi.parse.TypesHelper;
import com.rusefi.util.LazyFile;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.TreeSet;

import static com.rusefi.VariableRegistry.unquote;
import static com.rusefi.ldmp.LiveDataProcessor.needComment;

/**
 * here we generate [Datalog] section of TS .ini file
 * DataLog section in turn references [OutputChannels] entities
 *
 * @see SdCardFieldsContent
 */
public class DataLogConsumer implements ConfigurationConsumer {
    // https://github.com/rusefi/web_backend/issues/166
    private static final int MSQ_LENGTH_LIMIT = 34;

    public static final String UNUSED = UnusedPrefix.UNUSED_ANYTHING_PREFIX;
    private final String fileName;
    private final LazyFile.LazyFileFactory fileFactory;
    private final StringBuilder tsWriter = new StringBuilder();
    private final TreeSet<String> comments = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
    public String[] outputNames = new String[] {""};
    // optional LiveData.yaml 'prefix_names', see getHumanGaugeName
    public String[] prefixNames;

    public DataLogConsumer(String fileName, LazyFile.LazyFileFactory fileFactory) {
        this.fileName = fileName;
        this.fileFactory = fileFactory;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (readerState.isStackEmpty()) {

            for (int i = 0; i < outputNames.length; i++) {
                String temporaryLineComment = needComment(i) ? ";" : "";

                String outputNamePrefix = outputNames.length > 1 ? outputNames[i] : "";
                String humanNamePrefix = getPrefixName(prefixNames, i);

                PerFieldWithStructuresIterator.Strategy strategy = new PerFieldWithStructuresIterator.Strategy() {
                    @Override
                    public String process(ReaderState state, ConfigField configField, String variableNamePrefixForEmptyComment, int currentPosition, PerFieldWithStructuresIterator perFieldWithStructuresIterator) {
                        return handle(configField, variableNamePrefixForEmptyComment, temporaryLineComment, outputNamePrefix, humanNamePrefix);
                    }

                    @Override
                    public boolean skip(ConfigField cf) {
                        return false;
                    }
                };
                PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(readerState, structure.getTsFields(), "",
                        strategy);
                iterator.loop(0);
                String content = iterator.getContent();
                tsWriter.append(content);
            }
        }
    }

    @Override
    public void endFile() throws IOException {
        writeStringToFile(fileName, tsWriter);
    }

    private void writeStringToFile(@Nullable String fileName, StringBuilder writer) throws IOException {
        if (fileName != null) {
            LazyFile fw = fileFactory.create(fileName);
            fw.write(writer);
            fw.close();
        }
    }

    private String handle(ConfigField configField, String variableNamePrefixForEmptyComment, String temporaryLineComment, String outputNamePrefix, String humanNamePrefix) {
        // alignment fill and unused fields are not logged, also keeps them away from the name length check
        if (configField.isUnusedField())
            return "";

        if (configField.isArray()) {

            return "";
        }
//        entry = seconds,         @@GAUGE_NAME_UPTIME@@,
//        entry = RPMValue,        @@GAUGE_NAME_RPM@@,

        String typeString;
        String autoscaleSpec = configField.autoscaleSpec();
        if (TypesHelper.isFloat(configField.getTypeName()) || (autoscaleSpec != null && !autoscaleSpec.equals("1, 1"))) {
            typeString = "float,  \"%.3f\"";
        } else {
            typeString = "int,    \"%d\"";
        }

        String comment = getHumanGaugeName(outputNamePrefix, humanNamePrefix, variableNamePrefixForEmptyComment, configField, "");

        if (comments.contains(comment))
            throw new IllegalStateException(comment + " already present in the outputs! " + configField);
        comments.add(comment);
        return temporaryLineComment + "entry = " + outputNamePrefix + variableNamePrefixForEmptyComment + configField.getName() + ", " + comment + ", " + typeString + "\n";
    }

    /**
     * Short human-readable field summary as used for gauge names and log file keys taken from the first line of the comment
     * More detailed technical explanation should be placed in consecutive lines
     */
    @NotNull
    public static String getHumanGaugeName(String outputNamePrefix, String variableNamePrefixForEmptyComment, ConfigField configField, String variableNameSuffix) {
        return getHumanGaugeName(outputNamePrefix, "", variableNamePrefixForEmptyComment, configField, variableNameSuffix);
    }

    /**
     * @param humanNamePrefix per-instance LiveData.yaml 'prefix_names' entry, empty if none. When present it replaces
     *                        the technical outputNamePrefix and goes before the 'set_prefix' value:
     *                        "WBO1 WBO: Heater duty", or "WBO1: Heater duty" without set_prefix
     */
    @NotNull
    public static String getHumanGaugeName(String outputNamePrefix, String humanNamePrefix, String variableNamePrefixForEmptyComment, ConfigField configField, String variableNameSuffix) {
        String comment = configField.getCommentTemplated();
        comment = getFirstLine(comment);

        if (comment.isEmpty()) {
            /**
             * @see ConfigFieldImpl#getCommentOrName()
             */
            comment = variableNamePrefixForEmptyComment + unquote(configField.getName());
        }
        String logNamePrefix = configField.getLogNamePrefix();
        if (!logNamePrefix.isEmpty()) {
            String fullPrefix = logNamePrefix + ": ";
            // names coming from shared defines like @@GAUGE_NAME_IAC@@ may already carry the prefix
            if (!comment.regionMatches(true, 0, fullPrefix, 0, fullPrefix.length()))
                comment = fullPrefix + comment;
        }
        if (!humanNamePrefix.isEmpty()) {
            comment = humanNamePrefix + (logNamePrefix.isEmpty() ? ": " : " ") + comment;
            outputNamePrefix = "";
        }
        comment = outputNamePrefix + comment + variableNameSuffix;
        if (comment.length() > MSQ_LENGTH_LIMIT)
            throw new IllegalStateException("[" + comment + "] is too long for log files at " + comment.length() + " limit " + MSQ_LENGTH_LIMIT);

        if (comment.charAt(0) != '"')
            comment = VariableRegistry.quote(comment);
        return comment;
    }

    @NotNull
    public static String getPrefixName(String[] prefixNames, int index) {
        return prefixNames == null ? "" : prefixNames[index];
    }

    private static String getFirstLine(String comment) {
        String[] comments = comment == null ? new String[0] : unquote(comment).split("\\\\n");
        // a quoted define followed by more lines, like @@GAUGE_NAME_FUEL_BASE@@\nmore text, leaves quotes on the first line
        comment = (comments.length > 0) ? unquote(comments[0]) : "";
        return comment;
    }

    public String getContent() {
        return tsWriter.toString();
    }
}
