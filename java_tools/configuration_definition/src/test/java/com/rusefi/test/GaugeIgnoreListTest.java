package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.output.GaugeIgnoreList;
import com.rusefi.output.TSProjectConsumer;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.*;
import java.nio.file.Path;

import static com.rusefi.AssertCompatibility.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class GaugeIgnoreListTest {


    @Test
    public void exactMatchIgnored() {
        GaugeIgnoreList list = listOf("knockSpectrum16");
        assertTrue(list.shouldIgnore("knockSpectrum16"));
    }

    @Test
    public void exactMatchNotIgnored() {
        GaugeIgnoreList list = listOf("knockSpectrum16");
        assertFalse(list.shouldIgnore("knockSpectrum1"));
        assertFalse(list.shouldIgnore("knockLevel"));
    }

    @Test
    public void suffixWildcard() {
        GaugeIgnoreList list = listOf("knockSpectrum*");
        assertTrue(list.shouldIgnore("knockSpectrum1"));
        assertTrue(list.shouldIgnore("knockSpectrum16"));
        assertFalse(list.shouldIgnore("knockLevel"));
    }

    @Test
    public void prefixWildcard() {
        GaugeIgnoreList list = listOf("*Gauge");
        assertTrue(list.shouldIgnore("knockLevelGauge"));
        assertTrue(list.shouldIgnore("RPMGauge"));
        assertFalse(list.shouldIgnore("RPMGaugeExtra"));
    }

    @Test
    public void midWildcard() {
        GaugeIgnoreList list = listOf("alternator*Gauge");
        assertTrue(list.shouldIgnore("alternatorStatus_iTermGauge"));
        assertTrue(list.shouldIgnore("alternatorFooGauge"));
        assertFalse(list.shouldIgnore("alternatorStatus_iTerm"));
        assertFalse(list.shouldIgnore("boostStatus_iTermGauge"));
    }

    @Test
    public void emptyListIgnoresNothing() {
        GaugeIgnoreList list = new GaugeIgnoreList();
        assertFalse(list.shouldIgnore("anything"));
        assertTrue(list.isEmpty());
    }

    // name@category pattern matching

    @Test
    public void exactNameAndExactCategoryMatches() {
        GaugeIgnoreList list = listOf("alternatorStatus_iTermGauge@Alternator PID");
        assertTrue(list.shouldIgnore("alternatorStatus_iTermGauge", "Alternator PID"));
    }

    @Test
    public void exactNameWrongCategoryDoesNotMatch() {
        GaugeIgnoreList list = listOf("alternatorStatus_iTermGauge@Alternator PID");
        assertFalse(list.shouldIgnore("alternatorStatus_iTermGauge", "Boost PID"));
        assertFalse(list.shouldIgnore("alternatorStatus_iTermGauge", ""));
    }

    @Test
    public void wildcardNameAndExactCategoryMatchesAll() {
        GaugeIgnoreList list = listOf("*@Alternator PID");
        assertTrue(list.shouldIgnore("alternatorStatus_iTermGauge", "Alternator PID"));
        assertTrue(list.shouldIgnore("alternatorStatus_outputGauge", "Alternator PID"));
        assertFalse(list.shouldIgnore("boostStatus_iTermGauge", "Boost PID"));
    }

    @Test
    public void wildcardCategoryMatchesAnyCategory() {
        GaugeIgnoreList list = listOf("specialGauge@*");
        assertTrue(list.shouldIgnore("specialGauge", "Knock"));
        assertTrue(list.shouldIgnore("specialGauge", "Alternator PID"));
        assertFalse(list.shouldIgnore("otherGauge", "Knock"));
    }

    @Test
    public void wildcardBothMatchesEverything() {
        GaugeIgnoreList list = listOf("*@*");
        assertTrue(list.shouldIgnore("anything", "any category"));
        assertTrue(list.shouldIgnore("knockLevel", "Knock"));
    }

    @Test
    public void nameOnlyPatternIgnoresCategoryCompletely() {
        // A plain name pattern (no @) should match regardless of what category is active
        GaugeIgnoreList list = listOf("knockSpectrum1");
        assertTrue(list.shouldIgnore("knockSpectrum1", "Knock"));
        assertTrue(list.shouldIgnore("knockSpectrum1", "Some Other Category"));
        assertTrue(list.shouldIgnore("knockSpectrum1", ""));
    }

    // gauge name / category line extraction

    @Test
    public void extractGaugeNameFromDefinitionLine() {
        assertEquals("knockSpectrum16",
                GaugeIgnoreList.extractGaugeName("\tknockSpectrum16 = m_knockSpectrum16, \"knockSpectrum16\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0"));
    }

    @Test
    public void extractGaugeNameNoLeadingWhitespace() {
        assertEquals("RPMGauge",
                GaugeIgnoreList.extractGaugeName("RPMGauge = RPMValue, \"RPM\", \"RPM\", 0, 10000, 0, 0, 8000, 9000, 0, 0"));
    }

    @Test
    public void extractGaugeNameReturnsNullForCategory() {
        assertNull(GaugeIgnoreList.extractGaugeName("gaugeCategory = Knock"));
    }

    @Test
    public void extractGaugeNameReturnsNullForComment() {
        assertNull(GaugeIgnoreList.extractGaugeName("; this is a comment"));
    }

    @Test
    public void extractGaugeNameReturnsNullForBlankLine() {
        assertNull(GaugeIgnoreList.extractGaugeName(""));
        assertNull(GaugeIgnoreList.extractGaugeName("   "));
    }

    @Test
    public void extractCategoryNameSimple() {
        assertEquals("Knock", GaugeIgnoreList.extractCategoryName("gaugeCategory = Knock"));
    }

    @Test
    public void extractCategoryNameWithSpaces() {
        assertEquals("Alternator PID", GaugeIgnoreList.extractCategoryName("gaugeCategory = Alternator PID"));
    }

    @Test
    public void extractCategoryNameWithLeadingWhitespace() {
        assertEquals("VVT", GaugeIgnoreList.extractCategoryName("\tgaugeCategory = VVT"));
    }

    @Test
    public void extractCategoryNameReturnsNullForGaugeLine() {
        assertNull(GaugeIgnoreList.extractCategoryName("\tknockLevel = m_knockLevel, \"Knock\", \"dBv\", 0, 0, 0, 0, 0, 0, 0, 0"));
    }

    //  filterGauges: single-line

    @Test
    public void filterSingleLineMatchReturnsNull() {
        GaugeIgnoreList list = listOf("knockSpectrum16");
        assertNull(list.filterGauges("\tknockSpectrum16 = m_knockSpectrum16, \"knockSpectrum16\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0"));
    }

    @Test
    public void filterSingleLineNoMatchReturnsUnchanged() {
        GaugeIgnoreList list = listOf("knockSpectrum16");
        String line = "\tknockLevelGauge = m_knockLevel, \"Knock level\", \"dBv\", -60, 10, -60, -60, 10, 10, 0, 0";
        assertEquals(line, list.filterGauges(line));
    }

    @Test
    public void filterSingleLineCategoryLineIsUnchangedForNameOnlyPattern() {
        // A bare name pattern (no @) should never remove a category header line
        GaugeIgnoreList list = listOf("*");
        String line = "gaugeCategory = Knock";
        assertEquals(line, list.filterGauges(line));
    }

    @Test
    public void filterSingleLineCategoryLineRemovedByCategoryPattern() {
        GaugeIgnoreList list = listOf("*@Alternator PID");
        assertNull(list.filterGauges("gaugeCategory = Alternator PID"));
    }

    @Test
    public void filterSingleLineCategoryLineKeptWhenOnlySpecificGaugesAreIgnored() {
        // specificGauge@X hides one gauge but not the whole category header
        GaugeIgnoreList list = listOf("alternatorStatus_iTermGauge@Alternator PID");
        String line = "gaugeCategory = Alternator PID";
        assertEquals(line, list.filterGauges(line));
    }

    @Test
    public void filterSingleLineCategoryPatternTracksState() {
        // Category pattern: only ignore gauges when inside "Alternator PID"
        GaugeIgnoreList list = listOf("*@Alternator PID");

        // Feed a category declaration first (single-line path, as in include_file processing)
        list.filterGauges("gaugeCategory = Alternator PID");

        // Gauge inside the matching category → filtered
        assertNull(list.filterGauges("\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"\", \"\", 0, 0, 0, 0, 0, 0, 0, 0"));

        // Switch to a different category
        list.filterGauges("gaugeCategory = Boost PID");

        // Same gauge name, but now in a different category → NOT filtered
        assertNotNull(list.filterGauges("\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"\", \"\", 0, 0, 0, 0, 0, 0, 0, 0"));
    }

    //  filterGauges: multi-line

    @Test
    public void filterMultiLineRemovesMatchingGauges() {
        GaugeIgnoreList list = listOf("knockSpectrum*");
        String input = "gaugeCategory = Knock\n" +
                "\tknockLevelGauge = m_knockLevel, \"Knock level\", \"dBv\", -60, 10, 0, 0, 0, 0, 0, 0\n" +
                "\tknockSpectrum1 = m_knockSpectrum1, \"Knock Spectrum1\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0\n" +
                "\tknockSpectrum2 = m_knockSpectrum2, \"knockSpectrum2\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0";

        String result = list.filterGauges(input);

        assertNotNull(result);
        assertTrue(result.contains("knockLevelGauge"));
        assertTrue(result.contains("gaugeCategory = Knock"));
        assertFalse(result.contains("knockSpectrum1"));
        assertFalse(result.contains("knockSpectrum2"));
    }

    @Test
    public void filterMultiLineKeepsNonMatchingGauges() {
        GaugeIgnoreList list = listOf("alternatorStatus_*");
        String input = "gaugeCategory = Alternator PID\n" +
                "\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"Alternator PID iTerm\", \"\", -10000.0, 10000.0, -10000.0, 10000.0, -10000.0, 10000.0, 3, 3\n" +
                "gaugeCategory = Boost PID\n" +
                "\tboostStatus_iTermGauge = boostStatus_iTerm, \"Boost PID iTerm\", \"\", -10000.0, 10000.0, -10000.0, 10000.0, -10000.0, 10000.0, 3, 3";

        String result = list.filterGauges(input);

        assertNotNull(result);
        assertFalse(result.contains("alternatorStatus_iTermGauge"));
        assertTrue(result.contains("boostStatus_iTermGauge"));
        // Both category headers are preserved (category lines are never stripped)
        assertTrue(result.contains("gaugeCategory = Alternator PID"));
        assertTrue(result.contains("gaugeCategory = Boost PID"));
    }

    @Test
    public void filterMultiLineCategoryPatternHidesWholeCategoryIncludingHeader() {
        GaugeIgnoreList list = listOf("*@Alternator PID");
        String input = "gaugeCategory = Alternator PID\n" +
                "\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"iTerm\", \"\", 0, 0, 0, 0, 0, 0, 3, 3\n" +
                "\talternatorStatus_outputGauge = alternatorStatus_output, \"output\", \"\", 0, 0, 0, 0, 0, 0, 3, 3\n" +
                "gaugeCategory = Boost PID\n" +
                "\tboostStatus_iTermGauge = boostStatus_iTerm, \"Boost iTerm\", \"\", 0, 0, 0, 0, 0, 0, 3, 3";

        String result = list.filterGauges(input);

        assertNotNull(result);
        assertFalse(result.contains("alternatorStatus_iTermGauge"));
        assertFalse(result.contains("alternatorStatus_outputGauge"));
        // Category header is also removed when *@Category pattern is used
        assertFalse(result.contains("gaugeCategory = Alternator PID"));
        // Unrelated category and its gauges survive
        assertTrue(result.contains("gaugeCategory = Boost PID"));
        assertTrue(result.contains("boostStatus_iTermGauge"));
    }

    @Test
    public void filterMultiLineCategoryHeaderKeptWhenOnlySpecificGaugesIgnored() {
        // specificGauge@X hides one gauge but not the category header
        GaugeIgnoreList list = listOf("alternatorStatus_iTermGauge@Alternator PID");
        String input = "gaugeCategory = Alternator PID\n" +
                "\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"iTerm\", \"\", 0, 0, 0, 0, 0, 0, 3, 3\n" +
                "\talternatorStatus_outputGauge = alternatorStatus_output, \"output\", \"\", 0, 0, 0, 0, 0, 0, 3, 3";

        String result = list.filterGauges(input);

        assertFalse(result.contains("alternatorStatus_iTermGauge"));
        assertTrue(result.contains("alternatorStatus_outputGauge"));
        assertTrue(result.contains("gaugeCategory = Alternator PID"));
    }

    @Test
    public void filterMultiLineCategoryPatternDoesNotAffectOtherCategories() {
        GaugeIgnoreList list = listOf("*@Knock");
        String input = "gaugeCategory = Knock\n" +
                "\tknockLevelGauge = m_knockLevel, \"Knock level\", \"dBv\", 0, 0, 0, 0, 0, 0, 0, 0\n" +
                "gaugeCategory = Trigger\n" +
                "\ttriggerErrorsCounterGauge = totalTriggerErrorCounter, \"Trigger errors\", \"count\", 0, 0, 0, 0, 0, 0, 0, 0";

        String result = list.filterGauges(input);

        assertFalse(result.contains("knockLevelGauge"));
        assertTrue(result.contains("triggerErrorsCounterGauge"));
    }

    @Test
    public void emptyListFilterIsPassthrough() {
        GaugeIgnoreList list = new GaugeIgnoreList();
        String input = "gaugeCategory = Knock\n" +
                "\tknockSpectrum1 = m_knockSpectrum1, \"Knock Spectrum1\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0";
        assertEquals(input, list.filterGauges(input));
    }

    //  file loading

    @Test
    public void loadNonExistentFileReturnsEmpty() throws Exception {
        GaugeIgnoreList list = GaugeIgnoreList.load("/nonexistent/path/ignore_gauges.txt");
        assertTrue(list.isEmpty());
    }

    @Test
    public void loadFileWithCommentsAndBlanks(@TempDir Path tempDir) throws Exception {
        File f = tempDir.resolve("ignore_gauges.txt").toFile();
        try (PrintWriter w = new PrintWriter(f)) {
            w.println("; this is a comment");
            w.println("# also a comment");
            w.println("");
            w.println("knockSpectrum*");
            w.println("  "); // blank after trim
            w.println("*@Alternator PID");
        }
        GaugeIgnoreList list = GaugeIgnoreList.load(f.getAbsolutePath());
        assertFalse(list.isEmpty());
        assertTrue(list.shouldIgnore("knockSpectrum5", "Knock"));
        assertTrue(list.shouldIgnore("alternatorStatus_output", "Alternator PID"));
        assertFalse(list.shouldIgnore("knockLevelGauge", "Knock"));
        assertFalse(list.shouldIgnore("boostStatus_iTerm", "Boost PID"));
    }

    //  TSProjectConsumer integration

    @Test
    public void tsConsumerFiltersGaugeFromIncludedContent() throws IOException {
        GaugeIgnoreList list = listOf("badGauge");
        ReaderStateImpl state = new ReaderStateImpl();
        // Simulate content processed one line at a time (as with include_file)
        String template =
                "gaugeCategory = Test\n" +
                "\tgoodGauge = someChannel, \"Good Gauge\", \"\", 0, 100, 0, 0, 100, 100, 0, 0\n" +
                "\tbadGauge = otherChannel, \"Bad Gauge\", \"\", 0, 100, 0, 0, 100, 100, 0, 0\n" +
                "\tanotherGoodGauge = moreChannel, \"Another\", \"\", 0, 100, 0, 0, 100, 100, 0, 0\n";

        TSProjectConsumer consumer = new TSProjectConsumer(null, state, list) {
            @Override public void endFile() { }
        };
        TsFileContent content = consumer.getTsFileContent(new ByteArrayInputStream(template.getBytes()));

        String result = content.getPrefix();
        assertTrue(result.contains("goodGauge"), "goodGauge should be present");
        assertTrue(result.contains("anotherGoodGauge"), "anotherGoodGauge should be present");
        assertFalse(result.contains("badGauge"), "badGauge should be filtered out");
    }

    @Test
    public void tsConsumerFiltersGaugesFromVariableSubstitution() throws IOException {
        GaugeIgnoreList list = listOf("knockSpectrum*");
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_BLOCK",
                "gaugeCategory = Knock\n" +
                "\tknockLevelGauge = m_knockLevel, \"Knock level\", \"dBv\", -60, 10, 0, 0, 0, 0, 0, 0\n" +
                "\tknockSpectrum1 = m_knockSpectrum1, \"Knock Spectrum1\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0\n" +
                "\tknockSpectrum2 = m_knockSpectrum2, \"knockSpectrum2\", \"Volume\", 0, 0, 0, 0, 0, 256, 0, 0");

        TSProjectConsumer consumer = new TSProjectConsumer(null, state, list) {
            @Override public void endFile() { }
        };
        TsFileContent content = consumer.getTsFileContent(
                new ByteArrayInputStream("@@GAUGE_BLOCK@@\n".getBytes()));

        String result = content.getPrefix();
        assertTrue(result.contains("knockLevelGauge"), "knockLevelGauge should survive");
        assertFalse(result.contains("knockSpectrum1"), "knockSpectrum1 should be filtered");
        assertFalse(result.contains("knockSpectrum2"), "knockSpectrum2 should be filtered");
    }

    @Test
    public void tsConsumerFiltersByCategoryPatternFromVariableSubstitution() throws IOException {
        GaugeIgnoreList list = listOf("*@Alternator PID");
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_BLOCK",
                "gaugeCategory = Alternator PID\n" +
                "\talternatorStatus_iTermGauge = alternatorStatus_iTerm, \"iTerm\", \"\", 0, 0, 0, 0, 0, 0, 3, 3\n" +
                "gaugeCategory = Boost PID\n" +
                "\tboostStatus_iTermGauge = boostStatus_iTerm, \"Boost iTerm\", \"\", 0, 0, 0, 0, 0, 0, 3, 3");

        TSProjectConsumer consumer = new TSProjectConsumer(null, state, list) {
            @Override public void endFile() { }
        };
        TsFileContent content = consumer.getTsFileContent(
                new ByteArrayInputStream("@@GAUGE_BLOCK@@\n".getBytes()));

        String result = content.getPrefix();
        assertFalse(result.contains("alternatorStatus_iTermGauge"), "alternator gauge should be hidden");
        assertFalse(result.contains("gaugeCategory = Alternator PID"), "alternator category header should be hidden");
        assertTrue(result.contains("boostStatus_iTermGauge"), "boost gauge should survive");
        assertTrue(result.contains("gaugeCategory = Boost PID"), "boost category header should survive");
    }

    //  multi-file accumulation

    @Test
    public void addPatternsFromAccumulatesAcrossFiles(@TempDir Path tempDir) throws Exception {
        File globalFile = tempDir.resolve("global_ignore_gauges.txt").toFile();
        File boardFile = tempDir.resolve("ignore_gauges.txt").toFile();

        try (PrintWriter w = new PrintWriter(globalFile)) {
            w.println("knockSpectrum*");
        }
        try (PrintWriter w = new PrintWriter(boardFile)) {
            w.println("*@Alternator PID");
        }

        GaugeIgnoreList list = new GaugeIgnoreList();
        list.addPatternsFrom(globalFile.getAbsolutePath());
        list.addPatternsFrom(boardFile.getAbsolutePath());

        assertTrue(list.shouldIgnore("knockSpectrum5", "Knock"));
        assertTrue(list.shouldIgnore("alternatorStatus_output", "Alternator PID"));
        assertFalse(list.shouldIgnore("knockLevelGauge", "Knock"));
        assertFalse(list.shouldIgnore("boostStatus_iTerm", "Boost PID"));
    }

    @Test
    public void addPatternsFromNonExistentFileIsNoop() throws Exception {
        GaugeIgnoreList list = new GaugeIgnoreList();
        list.addPatternsFrom("/nonexistent/global_ignore_gauges.txt");
        assertTrue(list.isEmpty());
    }

    //  helpers

    private static GaugeIgnoreList listOf(String... patterns) {
        return GaugeIgnoreList.forPatterns(patterns);
    }
}
