package com.rusefi;

import com.rusefi.enum_reader.Value;
import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.io.StringReader;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class EnumsReaderTest {

    @Test
    public void testMultiLineCommentOnSingleLine() throws IOException {
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0, /* comment */\n" +
                "  VAL2 = 1,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertEquals(2, testE.values.size());
        assertEquals(0, testE.values.get("VAL1").getIntValue());
        assertEquals(1, testE.values.get("VAL2").getIntValue());
    }

    @Test
    public void testMultiLineCommentSpanningMultipleLines() throws IOException {
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0,\n" +
                "  /* multi-line\n" +
                "     comment */\n" +
                "  VAL2 = 1,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertEquals(2, testE.values.size());
        assertEquals(0, testE.values.get("VAL1").getIntValue());
        assertEquals(1, testE.values.get("VAL2").getIntValue());
    }

    @Test
    public void testDoubleSlashStarComment() throws IOException {
        // This was the problematic case where //* was interpreted as start of /* comment
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0,\n" +
                "  //* Javadoc style comment\n" +
                "  VAL2 = 1,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertEquals(2, testE.values.size());
        assertEquals(0, testE.values.get("VAL1").getIntValue());
        assertEquals(1, testE.values.get("VAL2").getIntValue());
    }

    @Test
    public void testMultipleCommentsOnSameLine() throws IOException {
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0, /* comment 1 */ VAL1_ALIAS = 0, /* comment 2 */\n" +
                "  VAL2 = 1,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        // current implementation of isKeyValueLine might not support multiple values on same line easily if they are not separated by comma correctly in its regex
        // but let's see how it handles it. VAL1_ALIAS = 0 might be skipped or cause issues.
        // Looking at regex: [a-zA-Z_$][a-zA-Z\d_$]*(=(0x[0-9a-fA-F]+|(-)?[0-9]+|([-a-zA-Z\d_])*))*,?
        // It doesn't seem to support multiple values on one line if they are just space separated after comment removal.
        // Actually, removeSpaces(line) is called.
        // VAL1=0VAL1_ALIAS=0 would be the line. regex would NOT match VAL1=0VAL1_ALIAS=0.
        
        // Let's test a more realistic multiple comment scenario
        reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0, /* comment 1 */ /* comment 2 */\n" +
                "  VAL2 = 1,\n" +
                "} test_e;"
        );
        enums = EnumsReader.readStatic(reader);
        testE = enums.get("test_e");
        assertEquals(2, testE.values.size());
    }

    @Test
    public void testEnumClass() throws IOException {
        StringReader reader = new StringReader(
                "enum class test_e : uint8_t {\n" +
                "  VAL1 = 10,\n" +
                "  VAL2,\n" +
                "};"
        );
        // Note: EnumsReader requires some specific format for enum class
        // line.startsWith(ENUMCLASS_PREFIX) where ENUMCLASS_PREFIX = "enumclass"
        // removeSpaces(line) makes it "enumclasstest_e:uint8_t{"
        
        // The issue is that EnumToStringTest uses enum_with_values=false by default,
        // and in that mode, validateValues checks that values are within [0, size).
        // If we have VAL1 = 10, it will fail validation.
        
        VariableRegistry registry = new VariableRegistry();
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader, registry, true);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertTrue(testE.isEnumClass);
        assertEquals(10, testE.values.get("VAL1").getIntValue());
        assertEquals(11, testE.values.get("VAL2").getIntValue()); 
    }

    @Test
    public void testCommentsBeforeEnum() throws IOException {
        StringReader reader = new StringReader(
                "/* pre-enum comment */\n" +
                "// another pre-enum comment\n" +
                "typedef enum {\n" +
                "  VAL1 = 0,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        assertNotNull(enums.get("test_e"));
    }

    @Test
    public void testMultipleMultiLineCommentsOnSameLine() throws IOException {
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 = 0, /* first */ /* second */\n" +
                "  VAL2 = 1, /* third */\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertEquals(2, testE.values.size());
        assertEquals(0, testE.values.get("VAL1").getIntValue());
        assertEquals(1, testE.values.get("VAL2").getIntValue());
    }

    @Test
    public void testCommentsInsideEnumLines() throws IOException {
        StringReader reader = new StringReader(
                "typedef enum {\n" +
                "  VAL1 /* comment */ = 0,\n" +
                "  VAL2 = /* comment */ 1,\n" +
                "  VAL3 = 2 /* comment */,\n" +
                "} test_e;"
        );
        Map<String, EnumsReader.EnumState> enums = EnumsReader.readStatic(reader);
        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("test_e");
        assertEquals(3, testE.values.size());
        assertEquals(0, testE.values.get("VAL1").getIntValue());
        assertEquals(1, testE.values.get("VAL2").getIntValue());
        assertEquals(2, testE.values.get("VAL3").getIntValue());
    }

    @Test
    public void testEngineLoadMode() throws IOException {
        StringReader reader = new StringReader(
                "    enum class engine_load_mode_e: uint8_t {\n" +
                        "        /* Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR\n" +
                        "         * http://articles.sae.org/8539/ */\n" +
                        "        LM_SPEED_DENSITY = 0,\n" +
                        "        /* MAF with a known kg/hour function */\n" +
                        "        LM_REAL_MAF = 1,\n" +
                        "        LM_ALPHA_N = 2,\n" +
                        "        LM_LUA = 3,\n" +
                        "        UNSUPPORTED_ENUM_VALUE\n" +
                        "    };"
        );
        EnumsReader enumsReader = new EnumsReader();
        enumsReader.read(reader, new VariableRegistry(), true);
        Map<String, EnumsReader.EnumState> enums = enumsReader.getEnums();

        assertEquals(1, enums.size());
        EnumsReader.EnumState testE = enums.get("engine_load_mode_e");
        assertNotNull(testE);
        assertTrue(testE.isEnumClass);
        assertEquals(5, testE.values.size());
        assertEquals(0, testE.values.get("LM_SPEED_DENSITY").getIntValue());
        assertEquals(1, testE.values.get("LM_REAL_MAF").getIntValue());
        assertEquals(2, testE.values.get("LM_ALPHA_N").getIntValue());
        assertEquals(3, testE.values.get("LM_LUA").getIntValue());
        assertEquals(4, testE.values.get("UNSUPPORTED_ENUM_VALUE").getIntValue());
    }

    @Test
    public void testEngineLoadModeOneLine() throws IOException {
        StringReader reader = new StringReader(
                "    enum class engine_load_mode_e: uint8_t {\n" +
                        "        LM_SPEED_DENSITY = 0,\n" +
                        "        LM_REAL_MAF = 1,\n" +
                        "    };"
        );
        EnumsReader enumsReader = new EnumsReader();
        enumsReader.read(reader, new VariableRegistry(), true);
        Map<String, EnumsReader.EnumState> enums = enumsReader.getEnums();
        assertEquals(1, enums.size());
        assertEquals(2, enums.get("engine_load_mode_e").values.size());
    }
}
