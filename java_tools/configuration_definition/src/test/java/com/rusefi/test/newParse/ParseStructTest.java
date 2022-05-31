package com.rusefi.test.newParse;

import com.rusefi.RusefiParseErrorStrategy;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.*;
import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;
import java.lang.reflect.Array;
import java.util.List;

import static com.rusefi.test.newParse.NewParseHelper.parse;

public class ParseStructTest {
    @Test
    public void structEmpty() throws IOException {
        ParseState state = parse(
                "struct myStruct\n\n" +
                "end_struct\n"
        );

        // Parsed exactly one struct
        Assert.assertEquals(1, state.getStructs().size());

        Struct myStruct = state.getStructs().get(0);

        // It's a real struct!
        Assert.assertNotNull(myStruct);

        // Check that it was parsed correctly
        Assert.assertEquals("myStruct", myStruct.name);
        Assert.assertEquals(0, myStruct.fields.size());
        Assert.assertEquals(false, myStruct.noPrefix);

        Assert.assertEquals(state.getStructs().get(0), state.getLastStruct());
    }

    @Test
    public void structEmptyNoPrefix() throws IOException {
        ParseState state = parse(
                "struct_no_prefix myStruct\n\n" +
                        "end_struct\n"
        );

        Struct myStruct = state.getStructs().get(0);

        // Check that this one is marked as no prefix
        Assert.assertEquals(true, myStruct.noPrefix);
    }

    @Test
    public void structMultiple() throws IOException {
        ParseState state = parse(
                "struct myStruct1\n\n" +
                        "end_struct\n" +
                        "struct myStruct2\n\n" +
                        "end_struct\n"
        );

        // Now there are two structs!
        Assert.assertEquals(2, state.getStructs().size());

        Assert.assertEquals("myStruct1", state.getStructs().get(0).name);
        Assert.assertEquals("myStruct2", state.getStructs().get(1).name);

        // Check that get last struct returns the last one
        Assert.assertEquals(state.getStructs().get(1), state.getLastStruct());
    }

    @Test
    public void structNested() throws IOException {
        ParseState state = parse(
                "struct myStructOuter\n" +
                        "struct myStructInner\n\n" +
                        "end_struct\n" +
                        "end_struct\n"
        );

        // Now there are two structs!
        Assert.assertEquals(2, state.getStructs().size());

        Assert.assertEquals("myStructInner", state.getStructs().get(0).name);
        Assert.assertEquals("myStructOuter", state.getStructs().get(1).name);

        // Check that get last struct returns the outer one
        Assert.assertEquals(state.getStructs().get(1), state.getLastStruct());
    }

    @Test
    public void structContainsStruct() throws IOException {
        ParseState state = parse(
                "struct myStruct\n\n" +
                        "end_struct\n" +
                        "struct myStruct2\n" +
                        "myStruct foo\n" +
                        "end_struct"
        );

        Assert.assertEquals(2, state.getStructs().size());

        Struct inner = state.getStructs().get(0);
        Assert.assertEquals(0, inner.fields.size());

        // Check that the outer contains a struct
        Struct outer = state.getStructs().get(1);
        Assert.assertEquals(1, outer.fields.size());
        Assert.assertTrue(outer.fields.get(0) instanceof StructField);
    }

    @Test(expected = RuntimeException.class)
    public void structMissingStruct() throws IOException {
        parse(
            "struct myStruct2\n" +
            "myStruct foo\n" +
            "end_struct"
        );
    }

    private static Field parseSingleField(String fieldLine) throws IOException {
        ParseState state = parse(
                "struct myStruct\n" +
                        fieldLine + "\n" +
                        "end_struct\n"
        );

        List<Field> fields = state.getLastStruct().fields;

        Assert.assertEquals(1, fields.size());

        return fields.get(0);
    }

    @Test
    public void scalarOptions() throws IOException {
        Field f = parseSingleField("int foo;comment;\"units\", 1, 2, 3, 4, 5");
        ScalarField sf = (ScalarField) f;

        FieldOptions opt = sf.options;

        Assert.assertEquals("comment", opt.comment);
        Assert.assertEquals("\"units\"", opt.units);
        Assert.assertEquals(1, opt.scale, 0.001f);
        Assert.assertEquals(2, opt.offset, 0.001f);
        Assert.assertEquals(3, opt.min, 0.001f);
        Assert.assertEquals(4, opt.max, 0.001f);
        Assert.assertEquals(5, opt.digits);
    }

    private static void checkType(String inputType, int size, String cType, String tsType) throws IOException {
        ScalarField sf = (ScalarField) parseSingleField(inputType + " foo");

        Assert.assertEquals("foo", sf.name);
        Assert.assertEquals(false, sf.autoscale);
        Assert.assertEquals(size, sf.type.size);

        Assert.assertEquals(cType, sf.type.cType);
        Assert.assertEquals(tsType, sf.type.tsType);
    }

    @Test
    public void typeMappings() throws IOException {
        checkType("int", 4, "int", "S32");

        checkType("int8_t", 1, "int8_t", "S08");
        checkType("int16_t", 2, "int16_t", "S16");
        checkType("int32_t", 4, "int32_t", "S32");
        checkType("uint8_t", 1, "uint8_t", "U08");
        checkType("uint16_t", 2, "uint16_t", "U16");
        checkType("uint32_t", 4, "uint32_t", "U32");

        checkType("float", 4, "float", "F32");
    }

    @Test
    public void arraySingleDimension() throws IOException {
        Field f = parseSingleField("uint8_t[10] testBins");

        Assert.assertTrue(f instanceof ArrayField);
        ArrayField af = (ArrayField)f;

        Assert.assertEquals(1, af.length.length);
        Assert.assertEquals(10, af.length[0]);

        PrototypeField prototype = af.prototype;

        Assert.assertTrue(prototype instanceof ScalarField);
    }

    @Test
    public void arrayMultiDimension() throws IOException {
        Field f = parseSingleField("uint8_t[5 x 8] testBins");

        Assert.assertTrue(f instanceof ArrayField);
        ArrayField af = (ArrayField)f;

        Assert.assertEquals(2, af.length.length);
        Assert.assertEquals(5, af.length[0]);
        Assert.assertEquals(8, af.length[1]);
    }

    @Test
    public void arrayIterate() throws IOException {
        Field f = parseSingleField("uint8_t[10 iterate] testBins");

        Assert.assertTrue(f instanceof ArrayField);
        ArrayField af = (ArrayField)f;

        Assert.assertEquals(true, af.iterate);
    }

    @Test(expected = IllegalStateException.class)
    public void checkMultiDimensionArrayIterateThrows() throws IOException {
        parseSingleField("uint8_t[5 x 8 iterate] testBins");
    }

    @Test
    public void arrayOfStructs() throws IOException {
        ParseState state = parse(
                "struct myStruct\n\n" +
                        "end_struct\n" +
                        "struct myStruct2\n" +
                        "myStruct[5 iterate] foo\n" +
                        "end_struct"
        );

        Assert.assertEquals(2, state.getStructs().size());

        // Inner should be empty
        Struct inner = state.getStructs().get(0);
        Assert.assertEquals(0, inner.fields.size());

        // Check that the outer contains an array of structs
        Struct outer = state.getStructs().get(1);
        Assert.assertEquals(1, outer.fields.size());

        ArrayField af = (ArrayField)outer.fields.get(0);

        // Check length and that it contains a struct
        Assert.assertEquals(1, af.length.length);
        Assert.assertEquals(5, af.length[0]);
        Assert.assertTrue(af.prototype instanceof StructField);
    }
}
