package com.rusefi.test.newParse;

import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;

import static com.rusefi.test.newParse.NewParseHelper.parseToOutputChannels;
import static com.rusefi.test.newParse.NewParseHelper.parseToTs;
import static org.junit.Assert.assertEquals;

public class LayoutTest {
    @Test
    public void singleField() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int8_t xyz;comment;\"units\", 1, 2, 3, 4, 5\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 4\n" +
                "page = 1\n" +
                "xyz = scalar, S08, 0, \"units\", 1, 2, 3, 4, 5\n" +
                "; unused 3 bytes at offset 1\n" +
                "; total TS size = 4\n" +
                "[SettingContextHelp]\n" +
                "\txyz = \"comment\"\n", parseToTs(input));

        Assert.assertEquals(
                "xyz = scalar, S08, 0, \"units\", 1, 2\n" +
                        "; total TS size = 4\n"
                , parseToOutputChannels(input));
    }

    @Test
    public void twoFieldsSameSize() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int32_t abc;;\"\", 1, 2, 3, 4, 5\n" +
                "int32_t xyz;;\"\", 6, 7, 8, 9, 10\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 8\n" +
                        "page = 1\n" +
                        "abc = scalar, S32, 0, \"\", 1, 2, 3, 4, 5\n" +
                        "xyz = scalar, S32, 4, \"\", 6, 7, 8, 9, 10\n" +
                        "; total TS size = 8\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "abc = scalar, S32, 0, \"\", 1, 2\n" +
                "xyz = scalar, S32, 4, \"\", 6, 7\n" +
                "; total TS size = 8\n", parseToOutputChannels(input));
    }

    @Test
    public void twoFieldsBigThenSmall() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int32_t abc;;\"\", 1, 2, 3, 4, 5\n" +
                "int8_t xyz;;\"\", 6, 7, 8, 9, 10\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 8\n" +
                        "page = 1\n" +
                        "abc = scalar, S32, 0, \"\", 1, 2, 3, 4, 5\n" +
                        "xyz = scalar, S08, 4, \"\", 6, 7, 8, 9, 10\n" +
                        "; unused 3 bytes at offset 5\n" +
                        "; total TS size = 8\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "abc = scalar, S32, 0, \"\", 1, 2\n" +
                "xyz = scalar, S08, 4, \"\", 6, 7\n" +
                "; total TS size = 8\n", parseToOutputChannels(input));
    }

    @Test
    public void twoFieldsSmallThenBig() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int8_t abc;;\"\", 1, 2, 3, 4, 5\n" +
                "int32_t xyz;;\"\", 6, 7, 8, 9, 10\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 8\n" +
                        "page = 1\n" +
                        "abc = scalar, S08, 0, \"\", 1, 2, 3, 4, 5\n" +
                        "; unused 3 bytes at offset 1\n" +
                        "xyz = scalar, S32, 4, \"\", 6, 7, 8, 9, 10\n" +
                        "; total TS size = 8\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "abc = scalar, S08, 0, \"\", 1, 2\n" +
                "xyz = scalar, S32, 4, \"\", 6, 7\n" +
                "; total TS size = 8\n", parseToOutputChannels(input));
    }

    @Test
    public void threeFieldsSmallInMisalignment() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int16_t abc\n" +
                "int8_t def\n" +
                "int32_t xyz\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 8\n" +
                        "page = 1\n" +
                        "abc = scalar, S16, 0, \"\", 1, 0, 0, 0, 0\n" +
                        "def = scalar, S08, 2, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 1 bytes at offset 3\n" +
                        "xyz = scalar, S32, 4, \"\", 1, 0, 0, 0, 0\n" +
                        "; total TS size = 8\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "abc = scalar, S16, 0, \"\", 1, 0\n" +
                "def = scalar, S08, 2, \"\", 1, 0\n" +
                "xyz = scalar, S32, 4, \"\", 1, 0\n" +
                "; total TS size = 8\n", parseToOutputChannels(input));
    }

    @Test
    public void twoFieldsArrayThenBig() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "int8_t[10] abc;;\"\", 1, 2, 3, 4, 5\n" +
                "int32_t xyz;;\"\", 6, 7, 8, 9, 10\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 16\n" +
                        "page = 1\n" +
                        "abc = array, S08, 0, [10], \"\", 1, 2, 3, 4, 5\n" +
                        "; unused 2 bytes at offset 10\n" +
                        "xyz = scalar, S32, 12, \"\", 6, 7, 8, 9, 10\n" +
                        "; total TS size = 16\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "abc1 = scalar, S08, 0, \"\", 1, 2\n" +
                "abc2 = scalar, S08, 1, \"\", 1, 2\n" +
                "abc3 = scalar, S08, 2, \"\", 1, 2\n" +
                "abc4 = scalar, S08, 3, \"\", 1, 2\n" +
                "abc5 = scalar, S08, 4, \"\", 1, 2\n" +
                "abc6 = scalar, S08, 5, \"\", 1, 2\n" +
                "abc7 = scalar, S08, 6, \"\", 1, 2\n" +
                "abc8 = scalar, S08, 7, \"\", 1, 2\n" +
                "abc9 = scalar, S08, 8, \"\", 1, 2\n" +
                "abc10 = scalar, S08, 9, \"\", 1, 2\n" +
                "xyz = scalar, S32, 12, \"\", 6, 7\n" +
                "; total TS size = 16\n", parseToOutputChannels(input));
    }

    @Test
    public void singleFieldMultiDimArray() throws IOException {
        String ts = parseToTs("struct_no_prefix myStruct\n" +
                "int8_t[10 x 5] abc;;\"\", 1, 2, 3, 4, 5\n" +
                "end_struct");

        Assert.assertEquals(
                "pageSize            = 52\n" +
                        "page = 1\n" +
                        "abc = array, S08, 0, [10x5], \"\", 1, 2, 3, 4, 5\n" +
                        "; unused 2 bytes at offset 50\n" +
                        "; total TS size = 52\n" +
                        "[SettingContextHelp]\n", ts);
    }

    @Test
    public void structs() throws IOException {
        String ts = parseToTs(
                "struct s1\n" +
                        "int32_t abc\n" +
                        "uint8_t def\n" +
                        "end_struct\n" +
                "struct_no_prefix rootStruct\n" +
                        "s1 a\n" +
                        "s1 b\n" +
                "end_struct");

        Assert.assertEquals(
                "pageSize            = 16\n" +
                        "page = 1\n" +
                        "a_abc = scalar, S32, 0, \"\", 1, 0, 0, 0, 0\n" +
                        "a_def = scalar, U08, 4, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 3 bytes at offset 5\n" +
                        "b_abc = scalar, S32, 8, \"\", 1, 0, 0, 0, 0\n" +
                        "b_def = scalar, U08, 12, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 3 bytes at offset 13\n" +
                        "; total TS size = 16\n" +
                        "[SettingContextHelp]\n", ts);
    }

    @Test
    public void arrayIterate() throws IOException {
        String input = "struct_no_prefix rootStruct\n" +
                        "int32_t[4 iterate] myArr\n" +
                        "end_struct";

        Assert.assertEquals(
                "pageSize            = 16\n" +
                        "page = 1\n" +
                        "myArr1 = scalar, S32, 0, \"\", 1, 0, 0, 0, 0\n" +
                        "myArr2 = scalar, S32, 4, \"\", 1, 0, 0, 0, 0\n" +
                        "myArr3 = scalar, S32, 8, \"\", 1, 0, 0, 0, 0\n" +
                        "myArr4 = scalar, S32, 12, \"\", 1, 0, 0, 0, 0\n" +
                        "; total TS size = 16\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "myArr1 = scalar, S32, 0, \"\", 1, 0\n" +
                "myArr2 = scalar, S32, 4, \"\", 1, 0\n" +
                "myArr3 = scalar, S32, 8, \"\", 1, 0\n" +
                "myArr4 = scalar, S32, 12, \"\", 1, 0\n" +
                "; total TS size = 16\n", parseToOutputChannels(input));
    }

    @Test
    public void arrayZeroLength() throws IOException {
        String ts = parseToTs(
                "struct_no_prefix rootStruct\n" +
                        "int32_t[0] myArr\n" +
                        "end_struct");

        Assert.assertEquals(
                "pageSize            = 0\n" +
                        "page = 1\n" +
                        "; total TS size = 0\n" +
                        "[SettingContextHelp]\n", ts);
    }

    @Test
    public void arrayOfStructsIterate() throws IOException {
        String ts = parseToTs(
                "struct s1\n" +
                "uint8_t var1\n" +
                "uint16_t var2\n" +
                "end_struct\n" +
                "struct_no_prefix rootStruct\n" +
                "s1[4 iterate] arr\n" +
                "end_struct");

        Assert.assertEquals(
                "pageSize            = 16\n" +
                        "page = 1\n" +
                        "arr1_var1 = scalar, U08, 0, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 1 bytes at offset 1\n" +
                        "arr1_var2 = scalar, U16, 2, \"\", 1, 0, 0, 0, 0\n" +
                        "arr2_var1 = scalar, U08, 4, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 1 bytes at offset 5\n" +
                        "arr2_var2 = scalar, U16, 6, \"\", 1, 0, 0, 0, 0\n" +
                        "arr3_var1 = scalar, U08, 8, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 1 bytes at offset 9\n" +
                        "arr3_var2 = scalar, U16, 10, \"\", 1, 0, 0, 0, 0\n" +
                        "arr4_var1 = scalar, U08, 12, \"\", 1, 0, 0, 0, 0\n" +
                        "; unused 1 bytes at offset 13\n" +
                        "arr4_var2 = scalar, U16, 14, \"\", 1, 0, 0, 0, 0\n" +
                        "; total TS size = 16\n" +
                        "[SettingContextHelp]\n", ts);
    }

    @Test
    public void bits() throws IOException {
        String input = "struct_no_prefix myStruct\n" +
                "bit first\n" +
                "bit second\n" +
                "bit withOpt,\"a\",\"b\"\n" +
                "end_struct";

        Assert.assertEquals(
                "pageSize            = 4\n" +
                        "page = 1\n" +
                        "first = bits, U32, 0, [0:0], \"false\", \"true\"\n" +
                        "second = bits, U32, 0, [1:1], \"false\", \"true\"\n" +
                        "withOpt = bits, U32, 0, [2:2], \"b\", \"a\"\n" +
                        "; total TS size = 4\n" +
                        "[SettingContextHelp]\n", parseToTs(input));

        Assert.assertEquals(
                "first = bits, U32, 0, [0:0]\n" +
                        "second = bits, U32, 0, [1:1]\n" +
                        "withOpt = bits, U32, 0, [2:2]\n" +
                        "; total TS size = 4\n"
                , parseToOutputChannels(input));
    }

    @Test
    public void bigOutputChannels() throws IOException {
        String input = "struct_no_prefix total\n" +
                "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "uint8_t afr_typet;@@GAUGE_NAME_FUEL_WALL_CORRECTION@@;\"ms\",      1,      0,       0, 3000,      0\n" +
                "bit isForcedInduction;isForcedInduction\\nDoes the vehicle have a turbo or supercharger?\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "angle_t m_requested_pump;Computed requested pump \n" +
                "float tCharge;speed density\n" +
                "end_struct\n";

        assertEquals("afr_type = scalar, F32, 0, \"ms\", 1, 0\n" +
                "afr_typet = scalar, U08, 4, \"ms\", 1, 0\n" +
                "isForcedInduction = bits, U32, 8, [0:0]\n" +
                "enableFan1WithAc = bits, U32, 8, [1:1]\n" +
                "m_requested_pump = scalar, F32, 12, \"\", 1, 0\n" +
                "tCharge = scalar, F32, 16, \"\", 1, 0\n" +
                "; total TS size = 20\n", parseToOutputChannels(input));
    }
}
