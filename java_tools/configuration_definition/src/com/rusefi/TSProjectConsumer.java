package com.rusefi;

import java.io.*;

import static com.rusefi.ConfigDefinition.EOL;

public class TSProjectConsumer implements ConfigurationConsumer {
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String TS_FILE_OUTPUT_NAME = "rusefi.ini";

    private final CharArrayWriter tsWriter;
    private final String tsPath;
    private final ReaderState state;
    private int totalTsSize;

    public TSProjectConsumer(CharArrayWriter tsWriter, String tsPath, ReaderState state) {
        this.tsWriter = tsWriter;
        this.tsPath = tsPath;
        this.state = state;
    }

    private void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
        TsFileContent tsContent = readTsFile(tsPath);
        System.out.println("Got " + tsContent.getPrefix().length() + "/" + tsContent.getPostfix().length() + " of " + ConfigDefinition.TS_FILE_INPUT_NAME);

        BufferedWriter tsHeader = new BufferedWriter(new FileWriter(tsPath + File.separator + TS_FILE_OUTPUT_NAME));
        tsHeader.write(tsContent.getPrefix());

        tsHeader.write("; " + CONFIG_DEFINITION_START + ConfigDefinition.EOL);
        tsHeader.write("; this section " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + ConfigDefinition.EOL);
        tsHeader.write("pageSize            = " + totalTsSize + ConfigDefinition.EOL);
        tsHeader.write("page = 1" + ConfigDefinition.EOL);
        tsHeader.write(fieldsSection);
        if (ConfigDefinition.settingContextHelp.length() > 0) {
            tsHeader.write("[SettingContextHelp]" + ConfigDefinition.EOL);
            tsHeader.write(ConfigDefinition.settingContextHelp.toString() + ConfigDefinition.EOL + ConfigDefinition.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ConfigDefinition.EOL);
        tsHeader.write(tsContent.getPostfix());
        tsHeader.close();
    }

    private static TsFileContent readTsFile(String tsPath) throws IOException {
        BufferedReader r = new BufferedReader(new FileReader(tsPath + File.separator + ConfigDefinition.TS_FILE_INPUT_NAME));

        StringBuilder prefix = new StringBuilder();
        StringBuilder postfix = new StringBuilder();

        boolean isBeforeStartTag = true;
        boolean isAfterEndTag = false;
        String line;
        while ((line = r.readLine()) != null) {
            if (line.contains(CONFIG_DEFINITION_START)) {
                isBeforeStartTag = false;
                continue;
            }
            if (line.contains(CONFIG_DEFINITION_END)) {
                isAfterEndTag = true;
                continue;
            }

            if (isBeforeStartTag)
                prefix.append(line + ConfigDefinition.EOL);

            if (isAfterEndTag)
                postfix.append(VariableRegistry.INSTANCE.applyVariables(line) + ConfigDefinition.EOL);
        }
        r.close();
        return new TsFileContent(prefix.toString(), postfix.toString());
    }

    @Override
    public void startFile() throws IOException {

    }

    @Override
    public void endFile() throws IOException {
        writeTunerStudioFile(tsPath, tsWriter.toString());
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            totalTsSize = structure.writeTunerStudio("", tsWriter, 0);
            tsWriter.write("; total TS size = " + totalTsSize + EOL);
            VariableRegistry.INSTANCE.register("TOTAL_CONFIG_SIZE", totalTsSize);
        }
    }
}
