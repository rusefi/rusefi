package com.rusefi.output;

import com.rusefi.ConfigField;
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
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        FieldIterator iterator = new FieldIterator(structure.cFields);
        for (int i = 0; i < structure.cFields.size(); i++) {
            iterator.start(i);


            append(iterator.cf);


            iterator.end();
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

    private void append(ConfigField cf) {
        if (cf.getName().contains(UNUSED) || cf.getName().contains(ALIGNMENT_FILL_AT))
            return;

        content.append("\tif (strEqualCaseInsensitive(name, \"" + cf.getName() + "\"))\n");
        content.append("\t\treturn engineConfiguration->" + cf.getName() + ";\n");
    }

    public String getContent() {
        return content.toString();
    }
}
