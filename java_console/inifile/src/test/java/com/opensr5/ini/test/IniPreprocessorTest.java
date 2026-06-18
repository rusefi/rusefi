package com.opensr5.ini.test;

import com.rusefi.compatibility.ini.IniPreprocessor;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class IniPreprocessorTest {

    @Test
    public void noDirectives_passesThrough() {
        List<String> input = Arrays.asList("line1", "line2", "line3");
        assertEquals(input, IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void emptyInput_returnsEmpty() {
        assertEquals(Collections.emptyList(), IniPreprocessor.preprocessLines(Collections.emptyList()));
    }

    @Test
    public void simpleIf_keepsIfBranch() {
        List<String> input = Arrays.asList(
                "#if LAMBDA",
                "kept",
                "#endif"
        );
        assertEquals(List.of("kept"), IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void simpleIfElse_keepsIfBranch_dropsElse() {
        List<String> input = Arrays.asList(
                "#if LAMBDA",
                "if-branch",
                "#else",
                "else-branch",
                "#endif"
        );
        assertEquals(List.of("if-branch"), IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void elseWithMultipleLines_allDropped() {
        List<String> input = Arrays.asList(
                "before",
                "#if FOO",
                "if-line1",
                "if-line2",
                "#else",
                "else-line1",
                "else-line2",
                "#endif",
                "after"
        );
        assertEquals(List.of("before", "if-line1", "if-line2", "after"),
                IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void elif_treatedLikeElse_skipped() {
        List<String> input = Arrays.asList(
                "#if FOO",
                "if-branch",
                "#elif BAR",
                "elif-branch",
                "#endif"
        );
        assertEquals(List.of("if-branch"), IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void nestedIf_insideKeepingBranch() {
        List<String> input = Arrays.asList(
                "#if OUTER",
                "outer-if",
                "#if INNER",
                "inner-if",
                "#else",
                "inner-else",
                "#endif",
                "outer-if-after",
                "#else",
                "outer-else",
                "#endif"
        );
        assertEquals(List.of("outer-if", "inner-if", "outer-if-after"),
                IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void nestedIf_insideSkippedBranch_fullySkipped() {
        List<String> input = Arrays.asList(
                "#if OUTER",
                "outer-if",
                "#else",
                "outer-else",
                "#if INNER",
                "inner-if",
                "#else",
                "inner-else",
                "#endif",
                "#endif"
        );
        assertEquals(List.of("outer-if"), IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void multipleTopLevelIfs() {
        List<String> input = Arrays.asList(
                "#if A",
                "a-if",
                "#else",
                "a-else",
                "#endif",
                "middle",
                "#if B",
                "b-if",
                "#else",
                "b-else",
                "#endif"
        );
        assertEquals(List.of("a-if", "middle", "b-if"),
                IniPreprocessor.preprocessLines(input));
    }

    @Test
    public void directiveLinesNotEmitted() {
        List<String> input = Arrays.asList(
                "#if X",
                "content",
                "#endif"
        );
        List<String> result = IniPreprocessor.preprocessLines(input);
        assertFalse(result.stream().anyMatch(l -> l.startsWith("#")));
    }

    @Test
    public void isDirectiveLine_recognizesAllDirectives() {
        assertTrue(IniPreprocessor.isDirectiveLine("#if FOO"));
        assertTrue(IniPreprocessor.isDirectiveLine("#if"));
        assertTrue(IniPreprocessor.isDirectiveLine("#else"));
        assertTrue(IniPreprocessor.isDirectiveLine("#elif BAR"));
        assertTrue(IniPreprocessor.isDirectiveLine("#elif"));
        assertTrue(IniPreprocessor.isDirectiveLine("#endif"));
    }

    @Test
    public void isDirectiveLine_doesNotMatchNonDirectives() {
        assertFalse(IniPreprocessor.isDirectiveLine("someKey = value"));
        assertFalse(IniPreprocessor.isDirectiveLine("; comment"));
        assertFalse(IniPreprocessor.isDirectiveLine("#ifdef X")); // not supported
        assertFalse(IniPreprocessor.isDirectiveLine("#ifndef X")); // not supported
    }

    @Test
    public void isDirectiveType_caseInsensitive() {
        assertTrue(IniPreprocessor.isDirectiveType("directive"));
        assertTrue(IniPreprocessor.isDirectiveType("DIRECTIVE"));
        assertTrue(IniPreprocessor.isDirectiveType("Directive"));
        assertFalse(IniPreprocessor.isDirectiveType("int"));
        assertFalse(IniPreprocessor.isDirectiveType("boolean"));
    }

    @Test
    public void ifWithLeadingWhitespace_stillRecognized() {
        // rawLines already have whitespace; IniPreprocessor trims internally
        List<String> input = Arrays.asList(
                "   #if X",
                "  kept",
                "   #else",
                "  dropped",
                "   #endif"
        );
        assertEquals(List.of("  kept"), IniPreprocessor.preprocessLines(input));
    }
}
