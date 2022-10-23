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
    private static final String TS_FILE_INPUT_NAME = "rusefi.input";
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String TS_CONDITION = "@@if_";
    public static final String SETTING_CONTEXT_HELP_END = "SettingContextHelpEnd";
    public static final String SETTING_CONTEXT_HELP = "SettingContextHelp";

    private final String tsPath;
    private final ReaderState state;
    private int totalTsSize;
    private final TsOutput tsOutput;

    public TSProjectConsumer(String tsPath, ReaderState state) {
        this.tsPath = tsPath;
        tsOutput = new TsOutput(true);
        this.state = state;
    }

    // also known as TS tooltips
    public StringBuilder getSettingContextHelp() {
        return tsOutput.getSettingContextHelp();
    }

    protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
        TsFileContent tsContent = readTsTemplateInputFile(tsPath);
        SystemOut.println("Got " + tsContent.getPrefix().length() + "/" + tsContent.getPostfix().length() + " of " + TS_FILE_INPUT_NAME);

        // File.getPath() would eliminate potential separator at the end of the path
        String fileName = getTsFileOutputName(new File(tsPath).getPath());
        Output tsHeader = new LazyFile(fileName);
        writeContent(fieldsSection, tsContent, tsHeader);
    }

    protected void writeContent(String fieldsSection, TsFileContent tsContent, Output tsHeader) throws IOException {
        tsHeader.write(tsContent.getPrefix());

        tsHeader.write("; " + CONFIG_DEFINITION_START + ToolUtil.EOL);
        tsHeader.write("; this section " + state.getHeader() + ToolUtil.EOL + ToolUtil.EOL);
        tsHeader.write("pageSize            = " + totalTsSize + ToolUtil.EOL);
        tsHeader.write("page = 1" + ToolUtil.EOL);
        tsHeader.write(fieldsSection);
        if (tsOutput.getSettingContextHelp().length() > 0) {
            tsHeader.write("[" + SETTING_CONTEXT_HELP + "]" + ToolUtil.EOL);
            tsHeader.write(tsOutput.getSettingContextHelp().toString() + ToolUtil.EOL + ToolUtil.EOL);
            tsHeader.write("; " + SETTING_CONTEXT_HELP_END + ToolUtil.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ToolUtil.EOL);
        tsHeader.write(tsContent.getPostfix());
        tsHeader.close();
    }

    /**
     * rusefi.input has all the content of the future .ini file with the exception of data page
     * TODO: start generating [outputs] section as well
     */
    private TsFileContent readTsTemplateInputFile(String tsPath) throws IOException {
        String fileName = getTsFileInputName(tsPath);
        BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), CHARSET.name()));

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
                String strValue = state.variableRegistry.get(token);
                boolean value = Boolean.parseBoolean(strValue);
                if (!value)
                    continue; // skipping this line
                line = removeToken(line);
            }

            line = state.variableRegistry.applyVariables(line);

            if (isBeforeStartTag)
                prefix.append(line + ToolUtil.EOL);

            if (isAfterEndTag)
                postfix.append(state.variableRegistry.applyVariables(line) + ToolUtil.EOL);
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
        String token = "";
        while (index < line.length() && !Character.isWhitespace(line.charAt(index))) {
            token += line.charAt(index);
            index++;
        }
        return token;
    }

    private String getTsFileOutputName(String tsPath) {
        return tsPath + File.separator + state.tsFileOutputName;
    }

    public static String getTsFileInputName(String tsPath) {
        return tsPath + File.separator + TS_FILE_INPUT_NAME;
    }

    @Override
    public void endFile() throws IOException {
        writeTunerStudioFile(tsPath, getContent());
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        state.variableRegistry.register(structure.name + "_size", structure.getTotalSize());
        totalTsSize = tsOutput.run(readerState, structure, 0);

        if (state.stack.isEmpty()) {
            state.variableRegistry.register("TOTAL_CONFIG_SIZE", totalTsSize);
        }
    }

    public String getContent() {
        return tsOutput.getContent();
    }
}
