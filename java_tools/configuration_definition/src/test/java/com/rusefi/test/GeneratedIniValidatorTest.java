package com.rusefi.test;

import com.rusefi.output.GeneratedIniValidator;
import com.rusefi.ReaderStateImpl;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

public class GeneratedIniValidatorTest {
    private static final String DEFINITIONS = "[Constants]\n"
            + "setting = scalar, U08, 0, \"\", 1, 0, 0, 100, 0\n"
            + "bins = array, U08, 1, [2], \"\", 1, 0, 0, 100, 0\n"
            + "[PcVariables]\nlabels = bits, U08, [0:1], \"Off\", \"On\"\n"
            + "[OutputChannels]\nrpm = scalar, U16, 0, \"rpm\", 1, 0\n";

    private static void validate(Path directory, String content) throws IOException {
        Path ini = directory.resolve("references.ini");
        Files.write(ini, content.getBytes(StandardCharsets.UTF_8));
        GeneratedIniValidator.validate(ini);
    }

    @Test
    public void warmupFiltersReferenceAvailableChannels(@TempDir Path directory) throws IOException {
        String filters = Files.readAllLines(Path.of(ConfigDefinitionTest.FIRMWARE, "tunerstudio/WueAnalyze.ini"))
                .stream().filter(line -> line.trim().startsWith("filter ="))
                .collect(Collectors.joining("\n"));
        String channels = "[OutputChannels]\n";
        for (String name : new String[]{"engine", "actualLastInjection", "TPSValue", "RPMValue"}) {
            channels += name + " = scalar, U16, 0, \"\", 1, 0\n";
        }
        String ini = channels + "[WueAnalyze]\n" + filters;
        validate(directory, ini);
    }

    @Test
    public void alphaXBoardControlsReferenceDeclaredConstants(@TempDir Path directory) throws IOException {
        Path board = Path.of(ConfigDefinitionTest.FIRMWARE, "config/boards/hellen/alphax-8chan-revA");
        ReaderStateImpl state = new ReaderStateImpl();
        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader("struct config\n" + Files.readString(board.resolve("board_config.txt"))
                + "\nend_struct\n", consumer);
        String constants = consumer.getContent();
        // These five existing controls must keep their original bit positions.
        String[] existing = {"boardUseTempPullUp", "boardUse2stepPullDown", "boardUseD2PullDown",
                "boardUseD3PullDown", "boardUseTachPullUp"};
        for (int i = 0; i < existing.length; i++) {
            assertTrue(constants.contains(existing[i] + " = bits, U32, 0, [" + i + ":" + i + "]"), constants);
        }
        String ini = "[Constants]\n" + constants + "[UserDefined]\n" + Files.readString(board.resolve("board_options.ini"));
        validate(directory, ini);
    }

    @Test
    public void disabledMafDoesNotReferenceRemovedGauge(@TempDir Path directory) throws IOException {
        String template = Files.readString(Path.of(ConfigDefinitionTest.FIRMWARE, "tunerstudio/tunerstudio.template.ini"));
        int start = template.indexOf("\tcurve = mafDecodingCurve,");
        String curve = template.substring(start, template.indexOf("\tcurve =", start + 1));
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().put("ts_show_maf", "false");
        String generated = new TestTSProjectConsumer(state).getTsFileContent(
                new ByteArrayInputStream(curve.getBytes(StandardCharsets.UTF_8))).getPrefix();
        String ini = DEFINITIONS + "rawMaf = { rpm }\n[CurveEditor]\n" + generated
                .replace("mafDecodingBins", "bins").replace("mafDecoding\n", "bins\n");
        validate(directory, ini);
    }

    @Test
    public void undefinedConstantReferencesAreRejected(@TempDir Path directory) {
        for (String usage : new String[]{
                "[UserDefined]\ndialog = test\nfield = \"Setting\", missing\n",
                "[UserDefined]\ndialog = test\nfield = missing\n",
                "[UserDefined]\ndialog = test\nfield = \"Setting\", missing[0]\n",
                "[UserDefined]\ndialog = test\nfield = Label, missing\n",
                "[CurveEditor]\ncurve = test\nxBins = missing, rpm\n",
                "[TableEditor]\ntable = test, map\nzBins = missing\n",
                "[ConstantsExtensions]\nreadOnly = missing\n",
                "[SettingContextHelp]\nmissing = \"Help\"\n",
                "[UserDefined]\ndialog = test\nsettingOption = \"Preset\", missing=1\n"}) {
            IllegalStateException e = assertThrows(IllegalStateException.class,
                    () -> validate(directory, DEFINITIONS + usage));
            assertTrue(e.getMessage().contains("Undefined constant [missing]"), e.getMessage());
            assertTrue(e.getMessage().contains("references.ini:"), e.getMessage());
        }
    }

    @Test
    public void undefinedDataReferencesAreRejected(@TempDir Path directory) {
        for (String usage : new String[]{
                "[GaugeConfigurations]\ngauge = missing, \"RPM\", \"rpm\", 0, 9000\n",
                "[Datalog]\nentry = missing, \"RPM\", float, \"%.1f\"\n",
                "[UserDefined]\nliveGraph = graph\ngraphLine = missing\n",
                "[CurveEditor]\ncurve = test\nxBins = bins, missing\n",
                "[PcVariables]\nsnapshot = continuousChannelValue, missing\n",
                "[OutputChannels]\ncomputed = { max(rpm, missing) }\n",
                "[OutputChannels]\nfield = { missing }\n",
                "[Constants]\nresizable = array, U08, 0, [{setting}x{missing}], \"\", 1, 0, 0, 100, 0\n",
                "[FrontPage]\nindicator = missing, \"Off\", \"On\"\n",
                "[UserDefined]\nindicatorPanel = states\nindicator = missing, \"Off\", \"On\"\n",
                "[CurveEditor]\ncurve = test\nshowXYDataPlot = true, rpm, missing\n",
                "[UserDefined]\ndialog = test\nfield = \"Setting\", setting, { missing > 0 }\n"}) {
            IllegalStateException e = assertThrows(IllegalStateException.class,
                    () -> validate(directory, DEFINITIONS + usage));
            assertTrue(e.getMessage().contains("[missing]"), e.getMessage());
        }
    }

    @Test
    public void missingAndForwardPanelsAreRejected(@TempDir Path directory) {
        for (String suffix : new String[]{"", "dialog = child\n"}) {
            IllegalStateException e = assertThrows(IllegalStateException.class,
                    () -> validate(directory, "[UserDefined]\ndialog = parent\npanel = child\n" + suffix));
            assertTrue(e.getMessage().contains("references.ini:3:"), e.getMessage());
            assertTrue(e.getMessage().contains("[child]"), e.getMessage());
        }
        assertThrows(IllegalStateException.class,
                () -> validate(directory, "[UserDefined]\ndialog = parent\npanel = parent\n"));
    }

    @Test
    public void undefinedGaugesMenusAndMacrosAreRejected(@TempDir Path directory) {
        for (String usage : new String[]{
                "[UserDefined]\ndialog = test\ngauge = missing\n",
                "[CurveEditor]\ncurve = test\ngauge = missing\n",
                "[FrontPage]\ngauge1 = missing\n",
                "[Tuning]\ngauge2 = missing\n",
                "[Menu]\nsubMenu = missing, \"Missing\"\n",
                "[KeyActions]\nshowPanel = \"Ctrl+a\", missing\n",
                "[Constants]\noptions = bits, U08, 0, [0:1], $missing\n"}) {
            IllegalStateException e = assertThrows(IllegalStateException.class,
                    () -> validate(directory, DEFINITIONS + usage));
            assertTrue(e.getMessage().contains("[missing]"), e.getMessage());
        }
    }

    @Test
    public void reportsAllUndefinedReferencesWithTheirLines(@TempDir Path directory) {
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> validate(directory,
                "[UserDefined]\ndialog = test\nfield = \"Setting\", missingSetting\npanel = missingPanel\n"));
        assertTrue(e.getMessage().contains("references.ini:3: Undefined constant [missingSetting]"), e.getMessage());
        assertTrue(e.getMessage().contains("references.ini:4: Undefined panel [missingPanel]"), e.getMessage());
    }

    @Test
    public void priorPanelsAndTableAliasesAreAccepted(@TempDir Path directory) throws IOException {
        validate(directory, DEFINITIONS + "[Menu]\nsubMenu = parent, \"Parent\"\n"
                + "[UserDefined]\nindicatorPanel = indicators\nindicator = { rpm > 0 }, \"Off\", \"On\"\n"
                + "readoutPanel = readouts\nreadout = rpm\n"
                + "dialog = child\nfield = \"Setting\", setting\n"
                + "dialog = parent\npanel = child\npanel = indicators\npanel = readouts\npanel = map\n"
                + "[TableEditor]\ntable = table, map, \"Table\"\nxBins = bins, rpm\nyBins = bins\nzBins = bins\n");
    }

    @Test
    public void whitespaceSeparatedArgumentsAndGaugeCategoriesAreAccepted(@TempDir Path directory) throws IOException {
        validate(directory, DEFINITIONS + "[GaugeConfigurations]\ngaugeCategory = Fuel math\n"
                + "rpmGauge = rpm, \"RPM\", \"rpm\", 0, 9000\n"
                + "[UserDefined]\ndialog = test\nfield = \"Setting\" setting, { rpm > 0 }\n"
                + "field = UnquotedLabel, setting\nfield = bins[0]\n"
                + "field = \"Example stringValue(missing) is literal text\", setting\n"
                + "field = \"Array element\", bins[setting], { rpm > 0 }\n"
                + "gauge = rpmGauge\nhelp = about, \"About\"\n"
                + "[Menu]\nsubMenu = test \"Test\"\nsubMenu = about, \"Help\"\n");
    }

    @Test
    public void analyzerTargetsAreTablesNotChannels(@TempDir Path directory) throws IOException {
        String ini = DEFINITIONS + "[TableEditor]\ntable = target, target3d, \"Target\"\nzBins = bins\n"
                + "[VeAnalyze]\nlambdaTargetTables = target, afrTSCustom\n";
        validate(directory, ini);
        assertThrows(IllegalStateException.class, () -> validate(directory, ini.replace("target, afrTSCustom", "rpm, afrTSCustom")));
    }

    @Test
    public void forwardDataReferencesAndExpressionLiteralsAreAccepted(@TempDir Path directory) throws IOException {
        validate(directory, "[OutputChannels]\ncomputed = { max(rpm, 1e3) + 0xFF + timeNow }\n"
                + DEFINITIONS + "[UserDefined]\ndialog = test\n"
                + "indicator = { rpm > setting }, { Status: bitStringValue(labels, setting) }, \"Not defined is only text\"\n"
                + "field = \"A literal {not_a_reference}; text\", setting\n"
                + "[Constants]\nresizable = array, U08, 0, [{setting}x{setting}], \"\", 1, 0, 0, 100, 0\n");
    }

    @Test
    public void stringFunctionReferencesAreChecked(@TempDir Path directory) {
        assertThrows(IllegalStateException.class, () -> validate(directory, DEFINITIONS
                + "[UserDefined]\ndialog = test\nfield = { Label: stringValue(missing) }\n"));
        assertThrows(IllegalStateException.class, () -> validate(directory, DEFINITIONS
                + "[UserDefined]\ndialog = test\nindicator = { rpm > 0 }, { Status: bitStringValue(labels, missing) }, \"On\"\n"));
        assertThrows(IllegalStateException.class, () -> validate(directory, DEFINITIONS
                + "[UserDefined]\ndialog = test\nfield = \"$stringValue(missing)\", setting\n"));
    }

    @Test
    public void loggerFieldsStayWithinTheirLogger(@TempDir Path directory) throws IOException {
        String logger = "[LoggerDefinition]\nloggerDef = logger, \"Logger\", composite\n"
                + "recordField = refTime, \"Ref time\", 0, 32, 1, \"ms\"\n"
                + "calcField = time, \"Time\", \"ms\", { refTime }\n";
        validate(directory, logger);
        assertThrows(IllegalStateException.class, () -> validate(directory, logger
                + "loggerDef = second, \"Second\", composite\ncalcField = time, \"Time\", \"ms\", { refTime }\n"));
        assertThrows(IllegalStateException.class, () -> validate(directory, logger
                + "[OutputChannels]\nchannel = { refTime }\n"));
    }

    @Test
    public void generatedMarkersAreRejectedWithFileAndLine(@TempDir Path directory) throws IOException {
        Path ini = directory.resolve("generated.ini");
        for (String marker : new String[]{"@@if_FLAG", "@if_FLAG", "@@endif_block", "@endif_block",
                "@@UNEXPANDED@@", "@@UNTERMINATED", "@#UNEXPANDED#@", "@OFFSET@"}) {
            Files.write(ini, ("[Menu]\nsubMenu = offsets, \"Offsets\", 0" + marker + "\n").getBytes(StandardCharsets.UTF_8));
            IllegalStateException e = assertThrows(IllegalStateException.class, () -> GeneratedIniValidator.validate(ini));
            assertTrue(e.getMessage().contains(ini + ":2:"), e.getMessage());
        }
    }

    @Test
    public void commentsAndZeroPlaceholdersAreAllowed() {
        GeneratedIniValidator.validateLine("subMenu = offsets, \"Offsets\", 0");
        GeneratedIniValidator.validateLine("subMenu = offsets, \"Offsets\", 0, { enabled }");
        GeneratedIniValidator.validateLine("; Template documentation: @@if_FLAG");
        GeneratedIniValidator.validateLine("field = \"Literal ; semicolon\" ; @@comment");
        GeneratedIniValidator.validateLine("field = \"Escaped \\\" quote\" ; @@comment");
        assertThrows(IllegalStateException.class,
                () -> GeneratedIniValidator.validateLine("field = \"Literal ; @@UNEXPANDED\""));
    }

    @Test
    public void commandRequiresExistingFiles(@TempDir Path directory) {
        assertThrows(IllegalArgumentException.class, () -> GeneratedIniValidator.main(new String[0]));
        assertThrows(IOException.class, () -> GeneratedIniValidator.main(new String[]{directory.resolve("missing.ini").toString()}));
    }
}
