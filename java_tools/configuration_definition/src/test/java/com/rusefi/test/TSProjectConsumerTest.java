package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.VariableRegistry;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.ConfigurationConsumer;
import com.rusefi.output.DuplicateFieldNameException;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.PlainTsProjectConsumer;
import com.rusefi.output.TSProjectConsumer;
import com.rusefi.output.TsOutput;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.StringBufferInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.TreeSet;

import static com.rusefi.AssertCompatibility.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class TSProjectConsumerTest {
    private static final String smallContent = "hello = \";\"\n" +
        "world;comment\n" +
        ";comment2\n" +
        "end\n";

    @Test
    public void getTsCondition() {
        assertEquals("ts", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts\r\n"));
        assertEquals("ts_show_hip9011", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts_show_hip9011"));

        assertEquals("1", TSProjectConsumer.removeToken("1@@if_ts"));
        assertEquals("12", TSProjectConsumer.removeToken("1@@if_ts 2"));
        assertEquals("H2\r\n", TSProjectConsumer.removeToken("H@@if_ts 2\r\n"));
    }

    @Test
    public void oddFireMenuConditionEmitsMenuWhenEnabled() throws IOException {
        String generated = oddFireMenuResult("true", oddFireMenuTemplateLine());
        assertEquals("before\nsubMenu = ignitionCylExtra, \"Cylinder offsets\"\nafter\n",
                generated.replaceAll("[\\t ]+", " ").replace("\n ", "\n"));
    }

    @Test
    public void oddFireMenuConditionWorksWithAndWithoutZeroPlaceholder() throws IOException {
        for (String placeholder : new String[]{"", ", 0"}) {
            String menu = "subMenu = ignitionCylExtra, \"Cylinder offsets\"" + placeholder;
            String template = menu + "@@if_ts_show_odd_fire";
            // The generator preserves the optional placeholder; the flag gates either form.
            assertEquals("before\n" + menu + "\nafter\n", oddFireMenuResult("true", template));
            assertEquals("before\nafter\n", oddFireMenuResult("false", template));
        }
    }

    @Test
    public void oddFireMenuConditionDropsOnlyMenuWhenDisabled() throws IOException {
        assertEquals("before\nafter\n", oddFireMenuResult("false", oddFireMenuTemplateLine()));
    }

    @Test
    public void misspelledOddFireConditionIsRejected() throws IOException {
        String line = oddFireMenuTemplateLine().replace("ts_show_odd_fire", "ts_show_odd_fier");
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> oddFireMenuResult("true", line));
        assertTrue(e.getMessage().contains("tunerstudio.template.ini:2:"), e.getMessage());
        assertTrue(e.getMessage().contains("Unknown condition [ts_show_odd_fier]"), e.getMessage());
    }

    @Test
    public void singleAtOddFireConditionIsRejected() throws IOException {
        String line = oddFireMenuTemplateLine().replace("@@if_", "@if_");
        assertTrue(line.contains("@if_ts_show_odd_fire"));
        for (String value : new String[]{"true", "false"}) {
            IllegalStateException e = assertThrows(IllegalStateException.class, () -> oddFireMenuResult(value, line));
            assertTrue(e.getMessage().contains("tunerstudio.template.ini:2:"), e.getMessage());
            assertTrue(e.getMessage().contains("Malformed condition marker"), e.getMessage());
        }
    }

    @Test
    public void conditionValuesMustBeBoolean() throws IOException {
        String line = oddFireMenuTemplateLine();
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> oddFireMenuResult("fasle", line));
        assertTrue(e.getMessage().contains("must be true or false, got [fasle]"), e.getMessage());
        assertEquals("before\nafter\n", oddFireMenuResult("FALSE", line));
        assertEquals(oddFireMenuResult("true", line), oddFireMenuResult("TRUE", line));
    }

    @Test
    public void malformedConditionsAreRejectedEvenWhenDisabled() {
        for (String marker : new String[]{"@@@if_ts_show_odd_fire", "@@if_", "@@if_ts_show_odd_fire,",
                "@@if_ts_show_odd_fire@@", "@@if_ts_show_odd_fire @@if_typo"}) {
            assertThrows(IllegalStateException.class, () -> oddFireMenuResult("false", "menu" + marker));
        }
    }

    @Test
    public void includedConditionErrorReportsIncludedFileAndLine(@TempDir Path directory) throws IOException {
        Path included = directory.resolve("board_menu.ini");
        Files.write(included, ("; comment\n" + oddFireMenuTemplateLine().replace("@@if_", "@if_")).getBytes(StandardCharsets.UTF_8));
        TSProjectConsumer consumer = new TestTSProjectConsumer(new ReaderStateImpl());
        String template = "include_file \"" + included + "\"\n";
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> consumer.getTsFileContent(
                new ByteArrayInputStream(template.getBytes(StandardCharsets.UTF_8))));
        assertTrue(e.getMessage().contains(included + ":2:"), e.getMessage());
    }

    @Test
    public void substitutedFragmentCannotLeakTemplateMarkers() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().put("BOARD_MENU", "; comment\nsubMenu = offsets, \"Offsets\", 0@if_typo\n");
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> consumer.getTsFileContent(
                new ByteArrayInputStream("@@BOARD_MENU@@\n".getBytes(StandardCharsets.UTF_8))));
        assertTrue(e.getMessage().contains("Unresolved or malformed template marker"), e.getMessage());
    }

    @Test
    public void conditionMayImmediatelyFollowSubstitution() throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().put("LABEL", "\"Offsets\"");
        state.getVariableRegistry().put("FLAG", "true");
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        String template = "subMenu = offsets, @@LABEL@@@@if_FLAG\n";
        assertEquals("subMenu = offsets, \"Offsets\"\n", consumer.getTsFileContent(
                new ByteArrayInputStream(template.getBytes(StandardCharsets.UTF_8))).getPrefix());
    }

    @Test
    public void optionalConditionDefaultsPreserveBoardOverrides() {
        VariableRegistry defaults = ConfigDefinitionTest.readRealConfig();
        assertEquals("false", defaults.get("show_default_engine_type"));
        assertEquals("false", defaults.get("ts_show_vvt_frequency"));
        VariableRegistry proteus = new VariableRegistry();
        proteus.readPrependValues(ConfigDefinitionTest.FIRMWARE + "/config/boards/proteus/prepend.txt", true);
        proteus.readPrependValues(ConfigDefinitionTest.FIRMWARE + "/integration/rusefi_config.txt", true);
        assertEquals("true", proteus.get("ts_show_vvt_frequency"));
    }

    private static String oddFireMenuTemplateLine() throws IOException {
        return Files.readAllLines(Path.of(ConfigDefinitionTest.FIRMWARE, "tunerstudio", "top_level_menu.ini"))
                .stream()
                .filter(line -> line.trim().startsWith("subMenu = ignitionCylExtra,"))
                .findFirst()
                .orElseThrow(() -> new AssertionError("Missing Cylinder offsets menu in real template"));
    }

    private static String oddFireMenuResult(String flagValue, String line) throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().put("ts_show_odd_fire", flagValue);
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        String template = "before\n" + line + "\nafter\n";
        TsFileContent result = consumer.getTsFileContent(new ByteArrayInputStream(template.getBytes(StandardCharsets.UTF_8)));
        assertEquals("", result.getPostfix());
        return result.getPrefix();
    }

    @Test
    public void malformedTsConditionTokenIsParseError() {
        // real-world bug shape: a comma glued to the @@if_ token made the registry lookup miss,
        // so the condition was silently false and the whole line vanished from the .ini
        String line = "panel = vvtPidDialog2,\tEast@@if_ts_show_exhaust_vvt, { vvtMode1 != @@vvt_mode_e_VVT_INACTIVE@@ }";
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> TSProjectConsumer.getToken(line));
        assertTrue(e.getMessage().contains("ts_show_exhaust_vvt,"), e.getMessage());
        assertThrows(IllegalStateException.class, () -> TSProjectConsumer.removeToken(line));
        // empty token is just as malformed
        assertThrows(IllegalStateException.class, () -> TSProjectConsumer.getToken("hello @@if_ world"));
    }

    @Test
    public void conditionalField() {
        String test = "struct pid_s\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "#if LAMBDA\n" +
                "\tint periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "#else\n" +
                "\tint periodMs2;PID dTime;\"ms2\",      1,      0,       0, 3000,      0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#else\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#endif\n" +
                "\tint16_t autoscale periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);


        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();

        state.readBufferedReader(test, javaFieldsConsumer, consumer, tsProjectConsumer);

        assertEquals("#if LAMBDA\n" +
                "periodMs2 = scalar, S32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "#else\n" +
                "periodMs2 = scalar, S32, 0, \"ms2\", 1, 0, 0, 3000, 0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#else\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#endif\n" +
                "periodMs = scalar, S16, 20, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "; total TS size = 24\n", tsProjectConsumer.getContent());

        assertEquals("\tpublic static final Field PERIODMS2 = Field.create(\"PERIODMS2\", 0, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field AFRTABLE = Field.create(\"AFRTABLE\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 20, FieldType.INT16).setScale(0.1).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_22 = Field.create(\"ALIGNMENTFILL_AT_22\", 22, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());


        assertEquals("// start of pid_s\n" +
            "struct pid_s {\n" +
            "\t/**\n" +
            "\t * PID dTime\n" +
            "\t * units: ms\n" +
            "\t * offset 0\n" +
            "\t */\n" +
            "\tint periodMs2 = (int)0;\n" +
            "\t/**\n" +
            "\t * offset 4\n" +
            "\t */\n" +
            "\tafr_table_t afrTable;\n" +
            "\t/**\n" +
            "\t * PID dTime\n" +
            "\t * units: ms\n" +
            "\t * offset 20\n" +
            "\t */\n" +
            "\tscaled_channel<int16_t, 10, 1> periodMs = (int16_t)0;\n" +
            "\t/**\n" +
            "\t * need 4 byte alignment\n" +
            "\t * units: units\n" +
            "\t * offset 22\n" +
            "\t */\n" +
            "\tuint8_t alignmentFill_at_22[2] = {};\n" +
            "};\n" +
            "static_assert(sizeof(pid_s) == 24);\n" +
            "\n", consumer.getContent());
    }

    @Test
    public void cppCornerCaseEmptyStruct() {
        String test = "struct pid_s\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);


        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();

        state.readBufferedReader(test, javaFieldsConsumer, consumer, tsProjectConsumer);

        assertEquals("; total TS size = 0\n", tsProjectConsumer.getContent());

        assertEquals("", javaFieldsConsumer.getContent());


        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 1);\n" +
                "\n", consumer.getContent());
    }

    @Test
    public void testReaderKeepComments() throws IOException {
        TSProjectConsumer consumer = new TestTSProjectConsumer(new ReaderStateImpl());
        TsFileContent content = consumer.getTsFileContent(new StringBufferInputStream(smallContent));
        assertEquals(smallContent, content.getPrefix());
        assertEquals("", content.getPostfix());
    }

    @Test
    public void testReaderDropComments() throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.getVariableRegistry().put(TSProjectConsumer.TS_DROP_TEMPLATE_COMMENTS, "true");
        TsFileContent content = consumer.getTsFileContent(new StringBufferInputStream(smallContent));
        assertEquals("hello = \";\"\n" +
            "world;comment\n" +
            "end\n", content.getPrefix());
        assertEquals("", content.getPostfix());
    }

    @Test
    public void testDuplicateFieldInConstants() {
        String test = "struct pid_s\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        assertThrows(DuplicateFieldNameException.class, () -> {
            state.readBufferedReader(test, tsProjectConsumer);
        });
    }

    @Test
    public void testDuplicateFieldInConstantsWithDirective() {
        String test = "struct pid_s\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "#if DEBUG\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "#endif\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        // This should NOT throw exception because we allow re-definitions if there is a directive in between
        state.readBufferedReader(test, tsProjectConsumer);
    }

    @Test
    public void wueAliasOffsetsArePublishedForPageOne() {
        String test = "struct config\n" +
                "    float[2] cltFuelCorr;warmup correction;\"ratio\", 1, 0, 0, 5, 2\n" +
                "    uint8_t[2] lambdaTable;target;\"afr\", 1, 0, 0, 25, 1\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, consumer);

        assertEquals("0", state.getVariableRegistry().get("TS_PAGE_1_OFFSET_cltFuelCorr"));
        assertEquals("8", state.getVariableRegistry().get("TS_PAGE_1_OFFSET_lambdaTable"));
    }

    @Test
    public void wueAliasesStayInConstantsPageOneBeforeGeneratedSettingContextHelp() throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        state.setDefinitionInputFile("wue-alias-fixture.txt");
        VariableRegistry registry = state.getVariableRegistry();
        registry.put("TS_PAGE_FIELD_BLOCKS", "page = 2\npage2Field = scalar, U08, 0, \"\", 1, 0, 0, 1, 0\n"
                + "page = 5\npage5Field = scalar, U08, 0, \"\", 1, 0, 0, 1, 0\n");

        String definition = "#define FUEL_RPM_COUNT 4\n"
                + "#define FUEL_LOAD_COUNT 3\n"
                + "#define PACK_MULT_AFR_CFG 10\n"
                + "#define CLT_FUEL_CURVE_SIZE 2\n"
                + "struct config\n"
                + "    uint32_t precedingField;nonzero offset;\"\", 1, 0, 0, 1, 0\n"
                + "    float[CLT_FUEL_CURVE_SIZE] cltFuelCorr;canonical ratio;\"ratio\", 1, 0, 0, 5, 2\n"
                + "    uint8_t[FUEL_LOAD_COUNT x FUEL_RPM_COUNT] lambdaTable;canonical target;\"ratio\", 1, 0, 0, 25, 1\n"
                + "end_struct\n";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(definition, consumer);

        TsFileContent content = consumer.getTsFileContent(new ByteArrayInputStream(
                wueAliasTemplateRegion().getBytes(StandardCharsets.UTF_8)));
        String generated = consumer.writeContentForTest(consumer.getContent(), content);

        assertTrue(generated.startsWith("[Constants]\npage = 2\n"));
        assertTrue(generated.contains("page = 5\npage5Field"));
        assertTrue(generated.contains("page = 1\nwueAnalyzeTargetTable = array, U08, 12, [4x3], \"afr\", {1/10}, 0, 0, 25, 1, noMsqSave"));
        assertTrue(generated.contains("wueAnalyzeCltFuelCorr = array, F32, 4, [2], \"%\", 100, 0, 0, 500, 0, noMsqSave"));
        assertTrue(generated.contains("cltFuelCorr = array, F32, 4, [2], \"ratio\", 1, 0, 0, 5, 2"));
        assertTrue(generated.contains("lambdaTable = array, U08, 12, [4x3], \"ratio\", 1, 0, 0, 25, 1"));

        int aliasIndex = generated.indexOf("wueAnalyzeTargetTable =");
        int generatedConfigStartIndex = generated.indexOf("; CONFIG_DEFINITION_START");
        int settingContextHelpIndex = generated.indexOf("[SettingContextHelp]");
        assertTrue(aliasIndex > generated.indexOf("page = 5"));
        assertTrue(aliasIndex < generatedConfigStartIndex,
                "WUE aliases must select page 1 before the generated constants block");
        assertTrue(settingContextHelpIndex > aliasIndex,
                "WUE aliases must remain in [Constants], before generated [SettingContextHelp]");
    }

    private static String wueAliasTemplateRegion() throws IOException {
        String template = Files.readString(Path.of(ConfigDefinitionTest.FIRMWARE, "tunerstudio", "tunerstudio.template.ini"));
        int begin = template.indexOf("@@TS_PAGE_FIELD_BLOCKS@@");
        int end = template.indexOf("; Continue SettingContextHelp");
        assertTrue(begin >= 0 && end > begin, "Missing WUE alias template region");
        return "[Constants]\n" + template.substring(begin, end);
    }

    // --- [tag:ts_page_table] generator page-table emission + @@if_block (issue #9699) ---

    // registry seeded as if all five TS pages were generated (sizes, contents, command chars)
    private static ReaderStateImpl stateWithFivePages() {
        ReaderStateImpl state = new ReaderStateImpl();
        VariableRegistry reg = state.getVariableRegistry();
        reg.put("TS_READ_COMMAND_char", "R");
        reg.put("TS_BURN_COMMAND_char", "B");
        reg.put("TS_CHUNK_WRITE_COMMAND_char", "C");
        reg.put("TS_CRC_CHECK_COMMAND_char", "k");
        reg.put("persistent_config_s_size", "1000");
        reg.put("PAGE_SIZE_2", "256");  reg.put("PAGE_CONTENT_2", "c2");
        reg.put("PAGE_SIZE_3", "2048"); reg.put("PAGE_CONTENT_3", "c3");
        reg.put("PAGE_SIZE_4", "1268"); reg.put("PAGE_CONTENT_4", "c4");
        reg.put("PAGE_SIZE_5", "8000"); reg.put("PAGE_CONTENT_5", "c5");
        return state;
    }

    @Test
    public void tsPageTableAllEnabled() {
        ReaderStateImpl state = stateWithFivePages();
        VariableRegistry reg = state.getVariableRegistry();
        new TestTSProjectConsumer(state).registerTsPagesBlockForTest();

        assertEquals("5", reg.get("TS_PAGE_COUNT"));
        assertEquals("\"\\x00\\x00\", \"\\x00\\x01\", \"\\x00\\x02\", \"\\x00\\x03\", \"\\x00\\x04\"",
                reg.get("TS_PAGE_IDENTIFIERS"));
        assertEquals("1000, 256, 2048, 1268, 8000", reg.get("TS_PAGE_SIZES"));
        // page 1 (settings) and 4/5 burn; scatter (2) and ltft (3) are not burnable
        assertEquals("\"B%2i\", \"\", \"\", \"B%2i\", \"B%2i\"", reg.get("TS_PAGE_BURN_COMMANDS"));
        assertEquals("page = 2\n\tc2\npage = 3\n\tc3\npage = 4\n\tc4\npage = 5\n\tc5\n",
                reg.get("TS_PAGE_FIELD_BLOCKS"));
        assertEquals("  triggeredPageRefresh = 1, { triggerPageRefreshFlag }\n" +
                        "  triggeredPageRefresh = 3, { ltftPageRefreshFlag }\n" +
                        "  triggeredPageRefresh = 4, { triggerPageRefreshFlag }\n",
                reg.get("TS_PAGE_REFRESH_TRIGGERS"));
    }

    @Test
    public void tsPageTableLtftDisabledRenumbers() {
        ReaderStateImpl state = stateWithFivePages();
        VariableRegistry reg = state.getVariableRegistry();
        reg.put("EFI_LTFT_CONTROL", "FALSE");
        new TestTSProjectConsumer(state).registerTsPagesBlockForTest();

        assertEquals("4", reg.get("TS_PAGE_COUNT"));
        // LTFT identifier \x00\x02 dropped - firmware identifiers are fixed, so a gap is correct
        assertEquals("\"\\x00\\x00\", \"\\x00\\x01\", \"\\x00\\x03\", \"\\x00\\x04\"",
                reg.get("TS_PAGE_IDENTIFIERS"));
        assertEquals("1000, 256, 1268, 8000", reg.get("TS_PAGE_SIZES"));
        // page=N ordinals renumber: second_tables 4->3, lua 5->4
        assertEquals("page = 2\n\tc2\npage = 3\n\tc4\npage = 4\n\tc5\n", reg.get("TS_PAGE_FIELD_BLOCKS"));
        // ltft refresh gone, second_tables refresh renumbered 4->3
        assertEquals("  triggeredPageRefresh = 1, { triggerPageRefreshFlag }\n" +
                        "  triggeredPageRefresh = 3, { triggerPageRefreshFlag }\n",
                reg.get("TS_PAGE_REFRESH_TRIGGERS"));
        // published for @@if_block guards of LTFT's scattered references (editors, menus, tooltips)
        assertEquals("false", reg.get("LTFT_PAGE_ENABLED"));
    }

    @Test
    public void tsPageTableLuaDisabledTrailingDrop() {
        ReaderStateImpl state = stateWithFivePages();
        VariableRegistry reg = state.getVariableRegistry();
        reg.put("EFI_LUA", "FALSE");
        new TestTSProjectConsumer(state).registerTsPagesBlockForTest();

        assertEquals("4", reg.get("TS_PAGE_COUNT"));
        // Lua is the last page - dropping it needs no renumbering and leaves no gap
        assertEquals("\"\\x00\\x00\", \"\\x00\\x01\", \"\\x00\\x02\", \"\\x00\\x03\"",
                reg.get("TS_PAGE_IDENTIFIERS"));
        assertEquals("1000, 256, 2048, 1268", reg.get("TS_PAGE_SIZES"));
        assertEquals("page = 2\n\tc2\npage = 3\n\tc3\npage = 4\n\tc4\n", reg.get("TS_PAGE_FIELD_BLOCKS"));
    }

    @Test
    public void tsPageTableZeroSizeDropped() {
        ReaderStateImpl state = stateWithFivePages();
        VariableRegistry reg = state.getVariableRegistry();
        reg.put("PAGE_SIZE_5", "0"); // e.g. a board with LUA_SCRIPT_SIZE 0
        new TestTSProjectConsumer(state).registerTsPagesBlockForTest();

        assertEquals("4", reg.get("TS_PAGE_COUNT"));
        assertEquals("1000, 256, 2048, 1268", reg.get("TS_PAGE_SIZES"));
    }

    private static String ifBlockResult(String flagValue, String content) throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        if (flagValue != null)
            state.getVariableRegistry().put("MY_FLAG", flagValue);
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        return consumer.getTsFileContent(new StringBufferInputStream(content)).getPrefix();
    }

    @Test
    public void ifBlockKeptWhenTrue() throws IOException {
        String content = "before\n@@if_block MY_FLAG@@\ninside1\ninside2\n@@endif_block\nafter\n";
        assertEquals("before\ninside1\ninside2\nafter\n", ifBlockResult("true", content));
    }

    @Test
    public void ifBlockDroppedWhenFalse() throws IOException {
        String content = "before\n@@if_block MY_FLAG@@\ninside1\ninside2\n@@endif_block\nafter\n";
        assertEquals("before\nafter\n", ifBlockResult("false", content));
    }

    @Test
    public void ifBlockRejectedWhenTokenMissing() {
        String content = "before\n@@if_block MISSING@@\ninside\n@@endif_block\nafter\n";
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> ifBlockResult(null, content));
        assertTrue(e.getMessage().contains("Unknown condition [MISSING]"), e.getMessage());
        assertTrue(e.getMessage().contains("tunerstudio.template.ini:2:"), e.getMessage());
    }

    @Test
    public void malformedBlocksAreRejectedEvenWhenDisabled() {
        for (String content : new String[]{
                "@@if_block MY_FLAG@@\ninside\n",
                "@@endif_block\n",
                "@@if_block MY_FLAG@@\n@@if_block MY_FLAG@@\n@@endif_block\n",
                "@@if_block MY_FLAG@@\n@if_typo\n@@endif_block\n",
                "@@if_block MY_FLAG@@\ninside@@if_UNKNOWN\n@@endif_block\n"}) {
            assertThrows(IllegalStateException.class, () -> ifBlockResult("false", content));
        }
    }

    @Test
    public void ifBlockStartTagWithoutTrailingMarker() throws IOException {
        // the closing "@@" on the start line is optional
        String content = "a\n@@if_block MY_FLAG\nx\n@@endif_block\nb\n";
        assertEquals("a\nb\n", ifBlockResult("false", content));
    }

    @Test
    public void testDuplicateAcrossPages() {
        TreeSet<String> usedNames = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

        String page1 = "struct page1\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";

        String page2 = "struct page2\n" +
                "    int field1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";

        ReaderStateImpl state1 = new ReaderStateImpl();
        // Use isConstantsSection = false to enforce uniqueness check
        PlainTsProjectConsumer consumer1 = new PlainTsProjectConsumer(state1, usedNames) {
            @Override
            public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
                // Manually trigger run with isConstantsSection = false
                TsOutput tsOutput = new TsOutput(false, usedNames);
                tsOutput.run(readerState, structure, 0, "", "");
            }
        };
        state1.readBufferedReader(page1, consumer1);

        ReaderStateImpl state2 = new ReaderStateImpl();
        PlainTsProjectConsumer consumer2 = new PlainTsProjectConsumer(state2, usedNames) {
            @Override
            public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
                TsOutput tsOutput = new TsOutput(false, usedNames);
                tsOutput.run(readerState, structure, 0, "", "");
            }
        };

        assertThrows(DuplicateFieldNameException.class, () -> {
            state2.readBufferedReader(page2, consumer2);
        });
    }
}
