package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.core.Tuple;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.output.ConfigStructure.ALIGNMENT_FILL_AT;
import static com.rusefi.output.DataLogConsumer.UNUSED;
import static com.rusefi.output.GetOutputValueConsumer.getHashConflicts;
import static com.rusefi.output.GetOutputValueConsumer.wrapSwitchStatement;

@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class GetConfigValueConsumer implements ConfigurationConsumer {
    private static final String CONFIG_ENGINE_CONFIGURATION = "config->engineConfiguration.";
    private static final String ENGINE_CONFIGURATION = "engineConfiguration.";
    static final String FILE_HEADER = "#include \"pch.h\"\n" +
            "#include \"value_lookup.h\"\n";

    private static final String GET_METHOD_HEADER =
            "float getConfigValueByName(const char *name) {\n" +
                    "\t{\n";

    static final String GET_METHOD_FOOTER = "\treturn EFI_ERROR_CODE;\n" + "}\n";
    private static final String SET_METHOD_HEADER = "void setConfigValueByName(const char *name, float value) {\n" +
            "\t{\n" +
            "\t\tplain_get_float_s * known = findFloat(name);\n" +
            "\t\tif (known != nullptr) {\n" +
            "\t\t\t*(float*)hackEngineConfigurationPointer(known->value) = value;\n" +
            "\t\t}\n" +
            "\t}\n" +
            "\n";
    private static final String SET_METHOD_FOOTER = "}\n";
    private final List<Tuple<String>> variables = new ArrayList<>();
    private final String outputFileName;

    public GetConfigValueConsumer(String outputFileName) {
        this.outputFileName = outputFileName;
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
        if (state.stack.isEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.tsFields, "",
                    this::processConfig, ".");
            iterator.loop();
        }
    }

    @Override
    public void endFile() throws IOException {
        writeStringToFile(outputFileName, getContent());
    }

    private String processConfig(ReaderState readerState, ConfigField cf, String prefix) {
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

        variables.add(new Tuple<>(userName, javaName + cf.getName(), cf.getType()));


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

        for (Tuple<String> pair : variables) {

            String cast = TypesHelper.isFloat(pair.third) ? "" : "(int)";


            int hash = HashUtil.hash(pair.first);
            String str = getAssignment(cast, pair.second);
            if (hashConflicts.get(hash).get() == 1) {
                switchBody.append("\t\tcase " + hash + ":\n");
                switchBody.append(str);

            } else {

                setterBody.append(getCompareName(pair.first));
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
