package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.util.LazyFile;
import com.rusefi.util.Output;
import com.rusefi.util.SystemOut;

import java.io.*;

import static com.rusefi.util.IoUtils.CHARSET;

/**
 * [Constants]
 */
public class TSProjectConsumer implements ConfigurationConsumer {
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String TS_CONDITION = "@@if_";
    public static final String SETTING_CONTEXT_HELP_END = "SettingContextHelpEnd";
    public static final String SETTING_CONTEXT_HELP = "SettingContextHelp";

    private final String tsPath;
    private final ReaderStateImpl state;
    private int totalTsSize;
    private final TsOutput tsOutput;

    public TSProjectConsumer(String tsPath, ReaderStateImpl state) {
        this.tsPath = tsPath;
        tsOutput = new TsOutput(true);
        this.state = state;
    }

    // also known as TS tooltips
    public String getSettingContextHelpForUnitTest() {
        return tsOutput.getSettingContextHelp();
    }

    protected void writeTunerStudioFile(String inputFile, String fieldsSection) throws IOException {
        TsFileContent tsContent = readTsTemplateInputFile(inputFile);
        SystemOut.println("Got " + tsContent.getPrefix().length() + "/" + tsContent.getPostfix().length() + " from " + inputFile);

        // File.getPath() would eliminate potential separator at the end of the path
        String fileName = state.getTsFileOutputName();
        Output tsHeader = new LazyFile(fileName);
        writeContent(fieldsSection, tsContent, tsHeader);
    }

    protected void writeContent(String fieldsSection, TsFileContent tsContent, Output tsHeader) throws IOException {
        tsHeader.write(tsContent.getPrefix());

        tsHeader.write("; " + CONFIG_DEFINITION_START + ToolUtil.EOL);
        tsHeader.write("pageSize            = " + totalTsSize + ToolUtil.EOL);
        tsHeader.write("page = 1" + ToolUtil.EOL);
        tsHeader.write(fieldsSection);
        if (tsOutput.getSettingContextHelp().length() > 0) {
            tsHeader.write("[" + SETTING_CONTEXT_HELP + "]" + ToolUtil.EOL);
            tsHeader.write(tsOutput.getSettingContextHelp() + ToolUtil.EOL + ToolUtil.EOL);
            tsHeader.write("; " + SETTING_CONTEXT_HELP_END + ToolUtil.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ToolUtil.EOL);
        tsHeader.write(tsContent.getPostfix());
        tsHeader.close();
    }

    /**
     * tunerstudio.template.ini has all the content of the future .ini file with the exception of data page
     * TODO: start generating [outputs] section as well
     */
    private TsFileContent readTsTemplateInputFile(String fileName) throws IOException {
        BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), CHARSET));

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

            if (line.contains(TS_CONDITION)) {
                String token = getToken(line);
                String strValue = state.getVariableRegistry().get(token);
                boolean value = Boolean.parseBoolean(strValue);
                if (!value)
                    continue; // skipping this line
                line = removeToken(line);
            }

            line = state.getVariableRegistry().applyVariables(line);

            if (isBeforeStartTag) {
                prefix.append(line);
                prefix.append(ToolUtil.EOL);
            }

            if (isAfterEndTag) {
                postfix.append(state.getVariableRegistry().applyVariables(line));
                postfix.append(ToolUtil.EOL);
            }
        }
        r.close();
        return new TsFileContent(prefix.toString(), postfix.toString());
    }

    public static String removeToken(String line) {
        int index = line.indexOf(TS_CONDITION);
        String token = getToken(line);
        int afterTokenIndex = index + TS_CONDITION.length() + token.length();
        if (afterTokenIndex < line.length())
            afterTokenIndex++; // skipping one whitestace after token
        line = line.substring(0, index) + line.substring(afterTokenIndex);
        return line;
    }

    public static String getToken(String line) {
        int index = line.indexOf(TS_CONDITION) + TS_CONDITION.length();
        StringBuilder token = new StringBuilder();
        while (index < line.length() && !Character.isWhitespace(line.charAt(index))) {
            token.append(line.charAt(index));
            index++;
        }
        return token.toString();
    }

    @Override
    public void endFile() throws IOException {
        writeTunerStudioFile(tsPath, getContent());
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        state.getVariableRegistry().register(structure.getName() + "_size", structure.getTotalSize());
        totalTsSize = tsOutput.run(readerState, structure, 0);

        if (state.isStackEmpty()) {
            state.getVariableRegistry().register("TOTAL_CONFIG_SIZE", totalTsSize);
        }
    }

    public String getContent() {
        return tsOutput.getContent();
    }
}
