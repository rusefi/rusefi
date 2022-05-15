package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.output.ConfigStructure.ALIGNMENT_FILL_AT;
import static com.rusefi.output.DataLogConsumer.UNUSED;
import static com.rusefi.output.JavaSensorsConsumer.quote;

@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class GetConfigValueConsumer implements ConfigurationConsumer {
    private static final String CONFIG_ENGINE_CONFIGURATION = "config->engineConfiguration.";
    private static final String ENGINE_CONFIGURATION = "engineConfiguration.";
    static final String FILE_HEADER = "#include \"pch.h\"\n" +
            "#include \"value_lookup.h\"\n";
    private static final String FIND_METHOD =
            "plain_get_float_s * findFloat(const char *name) {\n" +
            "\tplain_get_float_s *currentF = &getF_plain[0];\n" +
            "\twhile (currentF < getF_plain + efi::size(getF_plain)) {\n" +
            "\t\tif (strEqualCaseInsensitive(name, currentF->token)) {\n" +
            "\t\t\treturn currentF;\n" +
            "\t\t}\n" +
            "\t\tcurrentF++;\n" +
            "\t}\n" +
            "\treturn nullptr;\n" +
            "}\n";

    private static final String GET_METHOD_HEADER =
            "float getConfigValueByName(const char *name) {\n"            +
            "\t{\n" +
            "\t\tplain_get_float_s * known = findFloat(name);\n" +
            "\t\tif (known != nullptr) {\n" +
            "\t\t\treturn *(float*)hackEngineConfigurationPointer(known->value);\n" +
            "\t\t}\n" +
            "\t}\n"
            ;

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
    private final StringBuilder getterBody = new StringBuilder();
    private final StringBuilder setterBody = new StringBuilder();
    private final StringBuilder allFloatAddresses = new StringBuilder(
            "static plain_get_float_s getF_plain[] = {\n");
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


        if (TypesHelper.isFloat(cf.getType())) {
            allFloatAddresses.append("\t{" + quote(userName) + ", &engineConfiguration->" + userName + "},\n");
        } else {
            getterBody.append(getCompareName(userName));
            getterBody.append("\t\treturn " + javaName + cf.getName() + ";\n");

            setterBody.append(getCompareName(userName));
            String str = getAssignment(cf, javaName, "(int)");
            setterBody.append(str);
        }


        return "";
    }

    @NotNull
    private String getAssignment(ConfigField cf, String javaName, String cast) {
        return "\t{\n" + "\t\t" + javaName + cf.getName() + " = " + cast +
                "value;\n" +
                "\t\treturn;\n\t}\n";
    }

    @NotNull
    static String getCompareName(String userName) {
        return "\tif (strEqualCaseInsensitive(name, \"" + userName + "\"))\n";
    }

    public String getHeaderAndGetter() {
        return FILE_HEADER +
                getFloatsSections() +
                FIND_METHOD +
                getComleteGetterBody();
    }

    @NotNull
    public String getComleteGetterBody() {
        return GET_METHOD_HEADER + getterBody + GET_METHOD_FOOTER;
    }

    @NotNull
    public String getFloatsSections() {
        return allFloatAddresses + "};\n\n";
    }

    public String getSetterBody() {
        return setterBody.toString();
    }

    public String getContent() {
        return getHeaderAndGetter()
                +
                SET_METHOD_HEADER + setterBody + SET_METHOD_FOOTER
                ;
    }
}
