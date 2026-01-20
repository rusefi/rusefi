package com.rusefi.output;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.*;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tools.tune.FileLinesHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.util.LazyFileImpl;
import com.rusefi.util.Output;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import java.io.*;
import java.util.List;

import static com.rusefi.ReaderStateImpl.INCLUDE_FILE;
import static com.rusefi.VariableRegistry.unquote;
import static com.rusefi.util.LazyFile.CHARSET;

/**
 * [Constants]
 */
public class TSProjectConsumer implements ConfigurationConsumer {
    private final static Logging log = Logging.getLogging(TSProjectConsumer.class);

    private static final String TS_FILE_INPUT_NAME = "tunerstudio.template.ini";
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String TS_CONDITION = "@@if_";
    public static final String SETTING_CONTEXT_HELP_END = "SettingContextHelpEnd";
    public static final String SETTING_CONTEXT_HELP = "SettingContextHelp";
    public static final String TS_DROP_TEMPLATE_COMMENTS = "ts_drop_template_comments";

    static class TSProjectConsumerState {
        private final ReaderStateImpl state;
        private int totalTsSize;
        final TsOutput tsOutput;

        public TSProjectConsumerState(ReaderStateImpl state, TsOutput tsOutput) {
            this.state = state;
            this.tsOutput = tsOutput;
        }

        public void handleEndStruct(ReaderState readerState, ConfigStructure structure) {
            state.getVariableRegistry().register(structure.getName() + "_size", structure.getTotalSize());
            totalTsSize = tsOutput.run(readerState, structure, 0, "", "");
        }

        public int getTotalSize() {
            return totalTsSize;
        }
    }

    private final TSProjectConsumerState consumerState;

    private boolean dropComments;
    private final ReaderStateImpl state;
    private final String tsPath;

    public TSProjectConsumer(String tsPath, ReaderStateImpl state) {
        this.tsPath = tsPath;
        consumerState = new TSProjectConsumerState(state, new TsOutput(true));
        this.state = state;
    }

    // also known as TS tooltips
    public String getSettingContextHelpForUnitTest() {
        return consumerState.tsOutput.getSettingContextHelp();
    }

    protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
        TsFileContent tsContent = readTsTemplateInputFile(tsPath);
        log.info("Got " + tsContent.getPrefix().length() + "/" + tsContent.getPostfix().length() + " of " + TS_FILE_INPUT_NAME);

        // File.getPath() would eliminate potential separator at the end of the path
        String fileName = getTsFileOutputName(new File(ConfigDefinitionRootOutputFolder.getValue() + tsPath).getPath());
        Output tsHeader = new LazyFileImpl(fileName, ConfigDefinitionRootOutputFolder.getValue() + "generated.patch");
        writeContent(fieldsSection, tsContent, tsHeader);
        try {
            testFreshlyProducedIniFile(fileName);
        } catch (Throwable e) {
            throw new IllegalStateException("While " + fileName, e);
        }
    }

    private void testFreshlyProducedIniFile(String fileName) {
        try {
            IniFileModel ini = IniFileReaderUtil.readIniFile(fileName);
            ConfigurationImage ci = new ConfigurationImage(ini.getMetaInfo().getPageSize(0));
            Msq msq = MsqFactory.valueOf(ci, ini);
            msq.writeXmlFile("quick-self-test.xml");
        } catch (IOException | JAXBException e) {
            throw new RuntimeException(e);
        }
    }

    protected void writeContent(String fieldsSection, TsFileContent tsContent, Output tsHeader) throws IOException {
        tsHeader.write(tsContent.getPrefix());

        tsHeader.write("; " + CONFIG_DEFINITION_START + ToolUtil.EOL);
        if (!dropComments) {
            tsHeader.write("; this section " + state.getHeader() + ToolUtil.EOL + ToolUtil.EOL);
        }
        tsHeader.write("page = 1" + ToolUtil.EOL);
        tsHeader.write(fieldsSection);
        if (consumerState.tsOutput.getSettingContextHelp().length() > 0) {
            tsHeader.write("[" + SETTING_CONTEXT_HELP + "]" + ToolUtil.EOL);
            tsHeader.write(consumerState.tsOutput.getSettingContextHelp() + ToolUtil.EOL + ToolUtil.EOL);
            tsHeader.write("; " + SETTING_CONTEXT_HELP_END + ToolUtil.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ToolUtil.EOL);
        tsHeader.write(tsContent.getPostfix());
        tsHeader.close();
    }

    /**
     * tunerstudio.template.ini has all the content of the future .ini file with the exception of data page
     */
    private TsFileContent readTsTemplateInputFile(String tsPath) throws IOException {
        String fileName = getTsFileInputName(tsPath);
        FileInputStream in = new FileInputStream(fileName);
        return getTsFileContent(in);
    }

    @NotNull
    public TsFileContent getTsFileContent(InputStream in) throws IOException {
        BufferedReader r = new BufferedReader(new InputStreamReader(in, CHARSET));

        StringBuilder prefix = new StringBuilder();
        StringBuilder postfix = new StringBuilder();

        dropComments = Boolean.valueOf(state.getVariableRegistry().get(TS_DROP_TEMPLATE_COMMENTS));

        boolean isBeforeStartTag = true;
        boolean isAfterEndTag = false;
        String line;
        while ((line = r.readLine()) != null) {
            if (line.startsWith(INCLUDE_FILE)) {
                String fileName = line.substring(INCLUDE_FILE.length()).trim();
                fileName = unquote(state.getVariableRegistry().applyVariables(fileName));
                log.info("Including " + fileName);
                List<String> lines = FileLinesHelper.readAllLinesWithRoot(fileName);
                for (String includedLine : lines) {
                    processAndUse(includedLine, isBeforeStartTag, prefix, isAfterEndTag, postfix);
                }
                continue;
            }
            if (line.contains(CONFIG_DEFINITION_START)) {
                isBeforeStartTag = false;
                continue;
            }
            if (line.contains(CONFIG_DEFINITION_END)) {
                isAfterEndTag = true;
                continue;
            }
            processAndUse(line, isBeforeStartTag, prefix, isAfterEndTag, postfix);
        }
        r.close();
        return new TsFileContent(prefix.toString(), postfix.toString());
    }

    private void processAndUse(String line, boolean isBeforeStartTag, StringBuilder prefix, boolean isAfterEndTag, StringBuilder postfix) {
        if (line.trim().startsWith(";") && dropComments) {
            return;
        }

        if (line.contains(TS_CONDITION)) {
            String token = getToken(line);
            String strValue = state.getVariableRegistry().get(token);
            boolean value = Boolean.parseBoolean(strValue);
            if (!value)
                return; // skipping this line
            line = removeToken(line);
        }

        line = state.getVariableRegistry().applyVariables(line) + ToolUtil.EOL;

        if (isBeforeStartTag)
            prefix.append(line);

        if (isAfterEndTag)
            postfix.append(line);
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
        return tsPath + File.separator + state.getTsFileOutputName();
    }

    public static String getTsFileInputName(String tsPath) {
        return tsPath + File.separator + TS_FILE_INPUT_NAME;
    }

    @Override
    public void endFile() throws IOException {
        writeTunerStudioFile(tsPath, getContent());
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) {
        consumerState.handleEndStruct(readerState, structure);
        if (state.isStackEmpty()) {
            state.getVariableRegistry().register("TOTAL_CONFIG_SIZE", consumerState.getTotalSize());
        }
    }

    public String getContent() {
        return consumerState.tsOutput.getContent();
    }
}
