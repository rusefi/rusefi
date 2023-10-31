package com.rusefi.test.newParse;

import com.rusefi.RusefiParseErrorStrategy;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;

import static com.rusefi.test.newParse.NewParseHelper.parse;


public class ParseDefinitionsTest {
    @Test
    public void basicIntegerDefinition() throws IOException {
        ParseState state = parse("#define foo 123");

        Assert.assertNull(state.findDefinition("notFoo"));

        Definition def = state.findDefinition("foo");
        Assert.assertTrue(def.isNumeric());
        Assert.assertEquals(123, def.asDouble(), 1e-5);

        Definition defHex = state.findDefinition("foo_16_hex");
        Assert.assertFalse(defHex.isNumeric());
        Assert.assertEquals("\\x00\\x7b", defHex.value);
    }

    @Test
    public void basicDoubleDefinition() throws IOException {
        ParseState state = parse("#define foo 123.5");

        Definition def = state.findDefinition("foo");

        Assert.assertTrue(def.isNumeric());
        Assert.assertEquals(123.5, def.asDouble(), 1e-5);
    }

    @Test
    public void basicStringDefinition() throws IOException {
        ParseState state = parse("#define foo \"fooValue\"");

        Definition def = state.findDefinition("foo");

        Assert.assertFalse(def.isNumeric());
        Assert.assertTrue(def.value instanceof String);
        Assert.assertEquals("\"fooValue\"", def.value);
    }

    @Test
    public void definitionWithNumexprValue() throws IOException {
        ParseState state = parse("#define foo 5 + 8 * 2 + 3");

        Definition def = state.findDefinition("foo");

        Assert.assertTrue(def.isNumeric());
        Assert.assertEquals(24, def.asDouble(), 1e-5);
    }

    @Test
    public void definitionWithAnotherDefinition() throws IOException {
        ParseState state = parse(
            "#define val1 20\n" +
            "#define val2 val1 + 1\n" +
            "#define val3 3 * @@val2@@");

        Definition def1 = state.findDefinition("val1");
        Assert.assertTrue(def1.isNumeric());
        Assert.assertEquals(20, def1.asDouble(), 1e-5);

        Definition def2 = state.findDefinition("val2");
        Assert.assertTrue(def2.isNumeric());
        Assert.assertEquals(21, def2.asDouble(), 1e-5);

        Definition def3 = state.findDefinition("val3");
        Assert.assertTrue(def3.isNumeric());
        Assert.assertEquals(63, def3.asDouble(), 1e-5);
    }


    @Test(expected = RuntimeException.class)
    public void definitionEvalMissingDefinition() throws IOException {
        parse(
            "#define val1 20\n" +
            "#define val2 val1 + 1\n" +
            "#define val3 3 * valgggggg"
        );
    }

    @Test(expected = RuntimeException.class)
    public void definitionEvalNonNumeric() throws IOException {
        parse(
                "#define val1 \"foo\"\n" +
                        "#define val2 val1 + 1"
        );
    }

    @Test
    public void definitionOverwritePolicyReplace() throws IOException {
        ParseState state = new ParseState();
        state.setDefinitionPolicy(Definition.OverwritePolicy.Replace);
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 20");

        Definition def = state.findDefinition("val");
        Assert.assertEquals(20, def.asDouble(), 1e-5);

        // Now parse another definition with the same name
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 40");

        // Should get back the new definition, not the old one
        Definition def2 = state.findDefinition("val");
        Assert.assertNotSame(def, def2);
        Assert.assertEquals(40, def2.asDouble(), 1e-5);
    }

    @Test
    public void definitionOverwritePolicyIgnoreNew() throws IOException {
        ParseState state = new ParseState();
        state.setDefinitionPolicy(Definition.OverwritePolicy.IgnoreNew);
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 20");

        Definition def = state.findDefinition("val");
        Assert.assertEquals(20, def.asDouble(), 1e-5);

        // Now parse another definition with the same name
        state.setDefinitionPolicy(Definition.OverwritePolicy.IgnoreNew);
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 40");

        // New one ignored, still returns the same old one.
        Assert.assertEquals(def, state.findDefinition("val"));
        Assert.assertEquals(20, def.asDouble(), 1e-5);
    }

    @Test(expected = IllegalStateException.class)
    public void definitionOverwritePolicyNotAllowed() throws IOException {
        ParseState state = new ParseState();
        state.setDefinitionPolicy(Definition.OverwritePolicy.NotAllowed);
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 20");

        Definition def = state.findDefinition("val");
        Assert.assertEquals(20, def.asDouble(), 1e-5);

        // Now parse another definition with the same name, this should throw
        RusefiParseErrorStrategy.parseDefinitionString(state.getListener(), "#define val 40");
    }
}
