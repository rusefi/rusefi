package com.rusefi.output;

import com.opensr5.ini.field.IniField;
import com.rusefi.*;
import com.rusefi.util.LazyFile;
import com.rusefi.util.Output;
import com.rusefi.util.SystemOut;

import java.io.*;

import static com.rusefi.util.IoUtils.CHARSET;
import static com.rusefi.ConfigDefinition.EOL;

public class TSProjectConsumer implements ConfigurationConsumer {
    private static final String TS_FILE_INPUT_NAME = "rusefi.input";
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String TS_CONDITION = "@@if_";
    public static final String SETTING_CONTEXT_HELP_END = "SettingContextHelpEnd";
    public static final String SETTING_CONTEXT_HELP = "SettingContextHelp";
    public static String TS_FILE_OUTPUT_NAME = "rusefi.ini";
    private final StringBuilder settingContextHelp = new StringBuilder();

    private final CharArrayWriter tsWriter;
    private final String tsPath;
    private final ReaderState state;
    private int totalTsSize;

    public TSProjectConsumer(CharArrayWriter tsWriter, String tsPath, ReaderState state) {
        this.tsWriter = tsWriter;
        this.tsPath = tsPath;
        this.state = state;
    }

    private int writeTunerStudio(ConfigField configField, String prefix, Writer tsHeader, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        String nameWithPrefix = prefix + configField.getName();

        if (configField.isDirective() && configField.getComment() != null) {
            tsHeader.write(configField.getComment());
            tsHeader.write(EOL);
            return tsPosition;
        }

        if (configField.getComment() != null && configField.getComment().startsWith(ConfigField.TS_COMMENT_TAG + "")) {
            settingContextHelp.append("\t" + nameWithPrefix + " = \"" + configField.getCommentContent() + "\"" + EOL);
        }
        state.variableRegistry.register(nameWithPrefix + "_offset", tsPosition);

        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            return writeTunerStudio(cs, prefix + extraPrefix, tsHeader, tsPosition);
        }

        if (configField.isBit()) {
            tsHeader.write(nameWithPrefix + " = bits, U32,");
            tsHeader.write(" " + tsPosition + ", [");
            tsHeader.write(bitIndex + ":" + bitIndex);
            tsHeader.write("], \"" + configField.getFalseName() + "\", \"" + configField.getTrueName() + "\"");
            tsHeader.write(EOL);

            tsPosition += configField.getSize(next);
            return tsPosition;
        }

        if (configField.getState().tsCustomLine.containsKey(configField.getType())) {
            String bits = configField.getState().tsCustomLine.get(configField.getType());
            if (!bits.startsWith("bits")) {
                bits = handleTsInfo(bits, 5);
            }

            bits = bits.replaceAll("@OFFSET@", "" + tsPosition);
            tsHeader.write(nameWithPrefix + " = " + bits);

            tsPosition += configField.getState().tsCustomSize.get(configField.getType());
        } else if (configField.getTsInfo() == null) {
            throw new IllegalArgumentException("Need TS info for " + configField.getName() + " at "+ prefix);
        } else if (configField.getArraySize() == 0) {
            // write nothing for empty array
            // TS does not like those
        } else if (configField.getArraySize() != 1) {
            tsHeader.write(nameWithPrefix + " = array, ");
            tsHeader.write(TypesHelper.convertToTs(configField.getType()) + ",");
            tsHeader.write(" " + tsPosition + ",");
            tsHeader.write(" [" + configField.getArraySize() + "],");
            tsHeader.write(" " + handleTsInfo(configField.getTsInfo(), 1));

            tsPosition += configField.getArraySize() * configField.getElementSize();
        } else {
            tsHeader.write(nameWithPrefix + " = scalar, ");
            tsHeader.write(TypesHelper.convertToTs(configField.getType()) + ",");
            tsHeader.write(" " + tsPosition + ",");
            tsHeader.write(" " + handleTsInfo(configField.getTsInfo(), 1));
            tsPosition += configField.getArraySize() * configField.getElementSize();
        }
        tsHeader.write(EOL);
        return tsPosition;
    }

    private static String handleTsInfo(String tsInfo, int mutliplierIndex) {
        try {
            String[] fields = tsInfo.split("\\,");
            if (fields.length > mutliplierIndex) {
                /**
                 * Evaluate static math on .ini layer to simplify rusEFI java and rusEFI PHP project consumers
                 * https://github.com/rusefi/web_backend/issues/97
                 */
                double val = IniField.parseDouble(fields[mutliplierIndex]);

                if (val == 0) {
                    fields[mutliplierIndex] = " 0";
                } else if (val == 1) {
                    fields[mutliplierIndex] = " 1";
                } else {
                    fields[mutliplierIndex] = " " + val;
                }
            }
            StringBuilder sb = new StringBuilder();
            for (String f : fields) {
                if (sb.length() > 0) {
                    sb.append(",");
                }
                sb.append(f);
            }
            return sb.toString();
        } catch (Throwable e) {
            throw new IllegalStateException("While parsing " + tsInfo, e);
        }
    }

    private int writeTunerStudio(ConfigStructure configStructure, String prefix, Writer tsHeader, int tsPosition) throws IOException {
        BitState bitState = new BitState();
        ConfigField prev = ConfigField.VOID;
        int prevTsPosition = tsPosition;
        for (int i = 0; i < configStructure.tsFields.size(); i++) {
            ConfigField next = i == configStructure.tsFields.size() - 1 ? ConfigField.VOID : configStructure.tsFields.get(i + 1);
            ConfigField cf = configStructure.tsFields.get(i);

            // if duplicate names, use previous position
            if (cf.getName().equals(prev.getName())) {
                tsPosition = prevTsPosition;
            }

            // Update 'prev' state needed for duplicate names recognition
            if (!cf.isDirective()) {
                prevTsPosition = tsPosition;
                prev = cf;
            }

            tsPosition = writeTunerStudio(cf, prefix, tsHeader, tsPosition, next, bitState.get());

            bitState.incrementBitIndex(cf, next);
        }
        return tsPosition;
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

        tsHeader.write("; " + CONFIG_DEFINITION_START + ConfigDefinition.EOL);
        tsHeader.write("; this section " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + ConfigDefinition.EOL);
        tsHeader.write("pageSize            = " + totalTsSize + ConfigDefinition.EOL);
        tsHeader.write("page = 1" + ConfigDefinition.EOL);
        tsHeader.write(fieldsSection);
        if (settingContextHelp.length() > 0) {
            tsHeader.write("[" + SETTING_CONTEXT_HELP + "]" + ConfigDefinition.EOL);
            tsHeader.write(settingContextHelp.toString() + ConfigDefinition.EOL + ConfigDefinition.EOL);
            tsHeader.write("; " + SETTING_CONTEXT_HELP_END + ConfigDefinition.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ConfigDefinition.EOL);
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
                prefix.append(line + ConfigDefinition.EOL);

            if (isAfterEndTag)
                postfix.append(state.variableRegistry.applyVariables(line) + ConfigDefinition.EOL);
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

    public static String getTsFileOutputName(String tsPath) {
        return tsPath + File.separator + TS_FILE_OUTPUT_NAME;
    }

    public static String getTsFileInputName(String tsPath) {
        return tsPath + File.separator + TS_FILE_INPUT_NAME;
    }

    @Override
    public void startFile() {
    }

    @Override
    public void endFile() throws IOException {
        writeTunerStudioFile(tsPath, tsWriter.toString());
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        state.variableRegistry.register(structure.name + "_size", structure.getTotalSize());
        if (state.stack.isEmpty()) {
            totalTsSize = writeTunerStudio(structure, "", tsWriter, 0);
            tsWriter.write("; total TS size = " + totalTsSize + EOL);
            state.variableRegistry.register("TOTAL_CONFIG_SIZE", totalTsSize);
        }
    }
}
