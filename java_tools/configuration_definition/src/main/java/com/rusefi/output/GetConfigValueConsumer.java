package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.output.variables.VariableRecord;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.output.ConfigStructureImpl.ALIGNMENT_FILL_AT;
import static com.rusefi.output.DataLogConsumer.UNUSED;
import static com.rusefi.output.GetOutputValueConsumer.getHashConflicts;
import static com.rusefi.output.GetOutputValueConsumer.wrapSwitchStatement;

/**
 * Here we generate C++ code for https://github.com/rusefi/rusefi/wiki/Lua-Scripting#getcalibrationname
 * @see GetOutputValueConsumer
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class GetConfigValueConsumer implements ConfigurationConsumer {
    private static final String CONFIG_ENGINE_CONFIGURATION = "config->engineConfiguration.";
    private static final String ENGINE_CONFIGURATION = "engineConfiguration.";
    static final String FILE_HEADER = "#include \"pch.h\"\n" +
            "#include \"value_lookup.h\"\n";

    private static final String GET_METHOD_HEADER =
            "float getConfigValueByName(const char *name) {\n";

    static final String GET_METHOD_FOOTER = "\treturn EFI_ERROR_CODE;\n" + "}\n";
    private static final String SET_METHOD_HEADER = "void setConfigValueByName(const char *name, float value) {\n";
    private static final String SET_METHOD_FOOTER = "}\n";
    private final List<VariableRecord> variables = new ArrayList<>();
    private final String outputFileName;
    private final String mdOutputFileName;

    private final StringBuilder mdContent = new StringBuilder();

    public GetConfigValueConsumer() {
        this(null, null);
    }

    public GetConfigValueConsumer(String outputFileName, String mdOutputFileName) {
        this.outputFileName = outputFileName;
        this.mdOutputFileName = mdOutputFileName;
    }

    public static void writeStringToFile(@Nullable String fileName, String content) throws IOException {
        if (fileName != null) {
            FileWriter fw = new FileWriter(fileName);
            fw.write(content);
            fw.close();
        }
    }

    @Override
    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        if (state.isStackEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.getTsFields(), "",
                    (readerState, cf, prefix) -> processConfig(cf, prefix), ".");
            iterator.loop();
        }
    }

    @Override
    public void endFile() throws IOException {
        writeStringToFile(outputFileName, getContent());
        writeStringToFile(mdOutputFileName, getMdContent());
    }

    private String processConfig(ConfigField cf, String prefix) {
        if (cf.getName().contains(UNUSED) || cf.getName().contains(ALIGNMENT_FILL_AT))
            return "";

        if (cf.isArray() || cf.isFromIterate() || cf.isDirective())
            return "";
        if (!TypesHelper.isPrimitive(cf.getType()) && !TypesHelper.isBoolean(cf.getType())) {
            return "";
        }

        String userName = prefix + cf.getName();
        if (userName.startsWith(ENGINE_CONFIGURATION))
            userName = userName.substring(ENGINE_CONFIGURATION.length());

        String javaName = "config->" + prefix;
        if (javaName.startsWith(CONFIG_ENGINE_CONFIGURATION))
            javaName = "engineConfiguration->" + javaName.substring(CONFIG_ENGINE_CONFIGURATION.length());

        variables.add(new VariableRecord(userName, javaName + cf.getName(), cf.getType(), null));

        mdContent.append("### " + userName + "\n");
        mdContent.append(cf.getComment() + "\n\n");


        return "";
    }

    @NotNull
    private String getAssignment(String cast, String value) {
        return "\t{\n" + "\t\t" + value + " = " + cast +
                "value;\n" +
                "\t\treturn;\n\t}\n";
    }

    @NotNull
    static String getCompareName(String userName) {
        return "\tif (strEqualCaseInsensitive(name, \"" + userName + "\"))\n";
    }

    public String getHeaderAndGetter() {
        return FILE_HEADER +
                getCompleteGetterBody();
    }

    public String getMdContent() {
        return mdContent.toString();
    }

    @NotNull
    public String getCompleteGetterBody() {
        StringBuilder switchBody = new StringBuilder();

        StringBuilder getterBody = GetOutputValueConsumer.getGetters(switchBody, variables);

        String fullSwitch = wrapSwitchStatement(switchBody);

        return GET_METHOD_HEADER +
                fullSwitch +
                getterBody + GET_METHOD_FOOTER;
    }

    public String getSetterBody() {
        StringBuilder switchBody = new StringBuilder();

        StringBuilder setterBody = new StringBuilder();
        HashMap<Integer, AtomicInteger> hashConflicts = getHashConflicts(variables);

        for (VariableRecord pair : variables) {

            String cast = TypesHelper.isFloat(pair.type) ? "" : "(int)";


            int hash = HashUtil.hash(pair.getUserName());
            String str = getAssignment(cast, pair.getFullName());
            if (hashConflicts.get(hash).get() == 1) {
                switchBody.append("\t\tcase " + hash + ":\n");
                switchBody.append(str);

            } else {

                setterBody.append(getCompareName(pair.getUserName()));
                setterBody.append(str);
            }
        }

        String fullSwitch = wrapSwitchStatement(switchBody);

        return fullSwitch + setterBody;
    }

    public String getContent() {
        return getHeaderAndGetter()
                +
                SET_METHOD_HEADER + getSetterBody() + SET_METHOD_FOOTER
                ;
    }
}
