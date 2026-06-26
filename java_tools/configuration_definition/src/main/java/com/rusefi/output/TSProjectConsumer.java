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

import jakarta.xml.bind.JAXBException;
import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

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
    // multi-line variant of @@if_<token>: drops every line between the markers when the token's
    // registry boolean is false, so guarding a whole block does not need a directive per line.
    // Like the per-line @@if_, an unregistered/typo'd token parses as false (block dropped); no
    // nesting support.
    private static final String TS_IF_BLOCK_START = "@@if_block";
    private static final String TS_IF_BLOCK_END = "@@endif_block";
    private static final String TEMPLATE_TAG = "@@";
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
    private final GaugeIgnoreList ignoreList;

    public TSProjectConsumer(String tsPath, ReaderStateImpl state) {
        this(tsPath, state, new TreeSet<>(String.CASE_INSENSITIVE_ORDER));
    }

    public TSProjectConsumer(String tsPath, ReaderStateImpl state, GaugeIgnoreList ignoreList) {
        this(tsPath, state, ignoreList, new TreeSet<>(String.CASE_INSENSITIVE_ORDER));
    }

    public TSProjectConsumer(String tsPath, ReaderStateImpl state, TreeSet<String> usedNames) {
        this(tsPath, state, new GaugeIgnoreList(), usedNames);
    }

    public TSProjectConsumer(String tsPath, ReaderStateImpl state, GaugeIgnoreList ignoreList, TreeSet<String> usedNames) {
        this.tsPath = tsPath;
        consumerState = new TSProjectConsumerState(state, new TsOutput(true, usedNames));
        this.state = state;
        this.ignoreList = ignoreList;
    }

    // also known as TS tooltips
    public String getSettingContextHelpForUnitTest() {
        return consumerState.tsOutput.getSettingContextHelp();
    }

    // [tag:ts_page_table] Page identifiers must match the TS_PAGE_* constants in console/binary/tunerstudio.h.
    // One row per TS page, in wire order. Columns:
    //   identifier  - pageIdentifier wire bytes (TS_PAGE_* in tunerstudio.h)
    //   sizeVar     - registry variable holding the page size
    //   contentVar  - registry variable holding the page's field definitions (null for page 1,
    //                 whose fields are emitted directly by writeContent)
    //   burnable    - whether the page is burned to flash (burnCommand entry)
    //   refreshFlag - [EventTriggers] triggeredPageRefresh flag for the page, or null
    //   guardFlag   - feature flag (read from a board prepend) that drops the page when FALSE
    //   enabledFlag - boolean the generator publishes so @@if[_block] guards can drop this page's
    //                 scattered references (tooltips, table editors), or null
    // A page is dropped when its guardFlag is FALSE or its size is zero; nPages, every parallel
    // command list, the page=N field blocks and the triggeredPageRefresh ordinals are then all
    // rebuilt from the surviving pages so they stay consistent. See issue #9699.
    // A guarded page is dropped from the .ini when its guardFlag is FALSE in the registry (read
    // from a board prepend). The guardFlag must mirror the board's compile flag. IMPORTANT: a page
    // that has a reference surface (tooltips, [TableEditor], menus, triggeredPageRefresh that name
    // its fields) also needs an enabledFlag and those references wrapped in @@if_block <enabledFlag>,
    // exactly as LTFT does - otherwise the drop leaves dangling references. LTFT has such a surface
    // (enabledFlag = LTFT_PAGE_ENABLED); Lua does not - its only page-5 entity is the luaScript
    // field, which is dropped with the field block, so it needs no enabledFlag. The size==0 rule is
    // generic but only safe for a genuinely empty page (no fields, hence nothing to reference).
    // NOTE: EFI_LUA gates the .ini only; the firmware still serves page 5 (it is not gated on
    // EFI_LUA), which is the safe direction - TS simply never requests the dropped page.
    private static final TsPage[] TS_PAGES = {
        new TsPage("\\x00\\x00", "persistent_config_s_size", null,            true,  "triggerPageRefreshFlag", null,               null),
        new TsPage("\\x00\\x01", "PAGE_SIZE_2",              "PAGE_CONTENT_2", false, null,                     null,               null),
        new TsPage("\\x00\\x02", "PAGE_SIZE_3",              "PAGE_CONTENT_3", false, "ltftPageRefreshFlag",    "EFI_LTFT_CONTROL", "LTFT_PAGE_ENABLED"),
        new TsPage("\\x00\\x03", "PAGE_SIZE_4",              "PAGE_CONTENT_4", true,  "triggerPageRefreshFlag", null,               null),
        new TsPage("\\x00\\x04", "PAGE_SIZE_5",              "PAGE_CONTENT_5", true,  null,                     "EFI_LUA",          null),
    };

    private static class TsPage {
        final String identifier;
        final String sizeVar;
        final String contentVar;
        final boolean burnable;
        final String refreshFlag;
        final String guardFlag;
        final String enabledFlag;

        TsPage(String identifier, String sizeVar, String contentVar, boolean burnable,
               String refreshFlag, String guardFlag, String enabledFlag) {
            this.identifier = identifier;
            this.sizeVar = sizeVar;
            this.contentVar = contentVar;
            this.burnable = burnable;
            this.refreshFlag = refreshFlag;
            this.guardFlag = guardFlag;
            this.enabledFlag = enabledFlag;
        }

        boolean isEnabled(VariableRegistry registry) {
            if (guardFlag != null) {
                String flag = registry.get(guardFlag);
                if (flag != null && flag.trim().equalsIgnoreCase("false"))
                    return false;
            }
            String size = registry.get(sizeVar);
            if (size == null)
                return false; // page size never computed - page is not present
            try {
                return Integer.parseInt(size.trim()) > 0;
            } catch (NumberFormatException e) {
                return true; // non-numeric size expression - keep the page
            }
        }
    }

    /**
     * Emits everything in the .ini that depends on the set of enabled pages: the [Constants]
     * page table (nPages + the parallel pageIdentifier / pageSize / read / burn / write / crc
     * lists), the page=N field blocks, the [EventTriggers] triggeredPageRefresh ordinals, and a
     * boolean enabled-flag per guarded page (consumed by @@if[_block] guards of that page's
     * tooltips and table editors). Building it all from one ordered pass keeps the page ordinals
     * and the parallel lists consistent automatically when a page is added or compiled out,
     * fixing issue #9699.
     */
    protected void registerTsPagesBlock() {
        VariableRegistry registry = state.getVariableRegistry();
        if (registry.get("TS_PAGE_COUNT") != null)
            return; // already registered (defensive against double invocation)

        String read = registry.get("TS_READ_COMMAND_char");
        String burn = registry.get("TS_BURN_COMMAND_char");
        String write = registry.get("TS_CHUNK_WRITE_COMMAND_char");
        String crc = registry.get("TS_CRC_CHECK_COMMAND_char");

        List<String> ids = new ArrayList<>();
        List<String> sizes = new ArrayList<>();
        List<String> readCmds = new ArrayList<>();
        List<String> burnCmds = new ArrayList<>();
        List<String> writeCmds = new ArrayList<>();
        List<String> crcCmds = new ArrayList<>();
        StringBuilder fieldBlocks = new StringBuilder();
        StringBuilder refreshTriggers = new StringBuilder();

        int ordinal = 0;
        for (TsPage page : TS_PAGES) {
            boolean enabled = page.isEnabled(registry);
            // publish the enabled flag so @@if[_block] can drop this page's scattered references
            if (page.enabledFlag != null)
                registry.register(page.enabledFlag, Boolean.toString(enabled));
            if (!enabled)
                continue;
            ordinal++;
            ids.add("\"" + page.identifier + "\"");
            sizes.add(registry.get(page.sizeVar));
            readCmds.add("\"" + read + "%2i%2o%2c\"");
            burnCmds.add(page.burnable ? "\"" + burn + "%2i\"" : "\"\"");
            writeCmds.add("\"" + write + "%2i%2o%2c%v\"");
            crcCmds.add("\"" + crc + "%2i%2o%2c\"");
            // page 1's fields are emitted by writeContent; pages 2+ come from PAGE_CONTENT_N
            if (page.contentVar != null)
                fieldBlocks.append("page = ").append(ordinal).append("\n\t@@")
                           .append(page.contentVar).append("@@\n");
            if (page.refreshFlag != null)
                refreshTriggers.append("  triggeredPageRefresh = ").append(ordinal)
                               .append(", { ").append(page.refreshFlag).append(" }\n");
        }

        registry.register("TS_PAGE_COUNT", Integer.toString(ids.size()));
        registry.register("TS_PAGE_IDENTIFIERS", String.join(", ", ids));
        registry.register("TS_PAGE_SIZES", String.join(", ", sizes));
        registry.register("TS_PAGE_READ_COMMANDS", String.join(", ", readCmds));
        registry.register("TS_PAGE_BURN_COMMANDS", String.join(", ", burnCmds));
        registry.register("TS_PAGE_VALUE_WRITE", String.join(", ", writeCmds));
        registry.register("TS_PAGE_CHUNK_WRITE", String.join(", ", writeCmds));
        registry.register("TS_PAGE_CRC_CHECK", String.join(", ", crcCmds));
        registry.register("TS_PAGE_FIELD_BLOCKS", fieldBlocks.toString());
        registry.register("TS_PAGE_REFRESH_TRIGGERS", refreshTriggers.toString());
    }

    protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
        registerTsPagesBlock();
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
        skippingBlock = false;
        String line;
        while ((line = r.readLine()) != null) {
            if (isConditionalBlockSkip(line))
                continue;
            if (line.startsWith(INCLUDE_FILE)) {
                String fileName = line.substring(INCLUDE_FILE.length()).trim();
                fileName = unquote(state.getVariableRegistry().applyVariables(fileName));
                log.info("Including " + fileName);
                List<String> lines = FileLinesHelper.readAllLinesWithRoot(fileName);
                for (String includedLine : lines) {
                    if (isConditionalBlockSkip(includedLine))
                        continue;
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

    private boolean skippingBlock;

    /**
     * Handles the multi-line conditional: @@if_block &lt;token&gt; ... @@endif_block. Returns true
     * when the line is a block marker (consume it) or falls inside a region whose token is false
     * (skip it). Works for both template and included-file lines. No nesting.
     */
    private boolean isConditionalBlockSkip(String line) {
        String trimmed = line.trim();
        if (trimmed.startsWith(TS_IF_BLOCK_START)) {
            String token = trimmed.substring(TS_IF_BLOCK_START.length()).trim();
            if (token.endsWith(TEMPLATE_TAG))
                token = token.substring(0, token.length() - TEMPLATE_TAG.length()).trim();
            skippingBlock = !Boolean.parseBoolean(state.getVariableRegistry().get(token));
            return true;
        }
        if (trimmed.startsWith(TS_IF_BLOCK_END)) {
            skippingBlock = false;
            return true;
        }
        return skippingBlock;
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

        line = state.getVariableRegistry().applyVariables(line);

        line = ignoreList.filterGauges(line);
        if (line == null)
            return;

        line = line + ToolUtil.EOL;

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

