package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.output.ConfigStructure.ALIGNMENT_FILL_AT;
import static com.rusefi.output.DataLogConsumer.UNUSED;

@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class GetConfigValueConsumer extends AbstractConfigurationConsumer {
    private final StringBuilder content = new StringBuilder();
    private final String outputFIleName;

    public GetConfigValueConsumer(String outputFIleName) {
        System.out.println("Hello " + getClass() + " " + outputFIleName);
        this.outputFIleName = outputFIleName;
    }

    private void writeStringToFile(@Nullable String fileName, String content) throws IOException {
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
                    this::process, ".");
            iterator.loop();
        }
    }

    @Override
    public void startFile() {
        content.append("#include \"pch.h\"\n");
        content.append("float getConfigValueByName(const char *name) {\n");
    }

    @Override
    public void endFile() throws IOException {
        content.append("\treturn EFI_ERROR_CODE;\n");
        content.append("}\n");
        writeStringToFile(outputFIleName, content.toString());
    }

    private String process(ReaderState readerState, ConfigField cf, String prefix) {
        if (cf.getName().contains(UNUSED) || cf.getName().contains(ALIGNMENT_FILL_AT))
            return "";

        if (cf.isArray() || cf.isFromIterate() || cf.isDirective())
            return "";
        if (!TypesHelper.isPrimitive(cf.getType())) {
            return "";
        }

        content.append("\tif (strEqualCaseInsensitive(name, \"" + prefix + cf.getName() + "\"))\n");
        content.append("\t\treturn config->" + prefix + cf.getName() + ";\n");
        return "";
    }

    public String getContent() {
        return content.toString();
    }
}
