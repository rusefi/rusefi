package com.rusefi.pinout;

import com.rusefi.AssertCompatibility;
import com.rusefi.EnumPair;
import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class PinoutLogicTest {
    @Test
    public void testEnumToTs() {
        Map<String, Value> currentValues = new TreeMap<>();
        currentValues.put("KEY", new Value(null, "3"));
        currentValues.put("KEY4", new Value("NO", "4"));
        EnumsReader.EnumState enumState = new EnumsReader.EnumState(currentValues, "pins", true);

        {
            ArrayList<String> list = new ArrayList<>(Arrays.asList("1", "NO", "10"));
            String result = PinoutLogic.enumToOptionsList("NO", enumState, list).getKeyValueForm();
            AssertCompatibility.assertEquals("0=\"NONE\",2=\"10\",1=\"NO\"", result);

        }

        {
            ArrayList<String> list = new ArrayList<>(Arrays.asList("1", "NO", null, null, null, null, null, "10"));
            String result = PinoutLogic.enumToOptionsList("NO", enumState, list).getKeyValueForm();
            AssertCompatibility.assertEquals("0=\"NONE\",7=\"10\",1=\"NO\"", result);
        }
    }

    @Test
    public void testEnumPaddingToPowerOf2() {
        Map<String, Value> currentValues = new TreeMap<>();
        for (int i = 0; i <= 20; i++) {
            currentValues.put("PIN" + i, new Value(null, String.valueOf(i)));
        }
        EnumsReader.EnumState enumState = new EnumsReader.EnumState(currentValues, "test_enum", true);

        // Test with 17 values - should pad to 32 (next power of 2)
        {
            ArrayList<String> list = new ArrayList<>();
            for (int i = 0; i < 17; i++) {
                list.add(i == 0 ? "NONE" : "PIN" + i);
            }
            EnumPair result = PinoutLogic.enumToOptionsList("NONE", enumState, list);
            String arrayForm = result.getArrayForm();

            String[] values = arrayForm.split(",");
            assertEquals(32, values.length, "17 values should be padded to 32");

            // Verify the last entries are INVALID
            assertEquals("\"INVALID\"", values[31].trim());
            assertEquals("\"INVALID\"", values[20].trim());
        }

        // Test with 5 values - should pad to 8 (next power of 2)
        {
            ArrayList<String> list = new ArrayList<>(Arrays.asList("NONE", "PIN1", "PIN2", "PIN3", "PIN4"));
            EnumPair result = PinoutLogic.enumToOptionsList("NONE", enumState, list);
            String arrayForm = result.getArrayForm();

            String[] values = arrayForm.split(",");
            assertEquals(8, values.length, "5 values should be padded to 8");

            // Verify the last entries are INVALID
            assertEquals("\"INVALID\"", values[7].trim());
            assertEquals("\"INVALID\"", values[5].trim());
        }

        // Test with 33 values - should pad to 64 (next power of 2)
        {
            ArrayList<String> list = new ArrayList<>();
            for (int i = 0; i < 33; i++) {
                list.add(i == 0 ? "NONE" : "PIN" + i);
            }
            EnumPair result = PinoutLogic.enumToOptionsList("NONE", enumState, list);
            String arrayForm = result.getArrayForm();

            String[] values = arrayForm.split(",");
            assertEquals(64, values.length, "33 values should be padded to 64");

            // Verify the last entries are INVALID
            assertEquals("\"INVALID\"", values[63].trim());
            assertEquals("\"INVALID\"", values[40].trim());
        }

        // Test with exact power of 2 (16) - should not pad further
        {
            ArrayList<String> list = new ArrayList<>();
            for (int i = 0; i < 16; i++) {
                list.add(i == 0 ? "NONE" : "PIN" + i);
            }
            EnumPair result = PinoutLogic.enumToOptionsList("NONE", enumState, list);
            String arrayForm = result.getArrayForm();

            String[] values = arrayForm.split(",");
            assertEquals(16, values.length, "16 values should remain at 16 (already power of 2)");
        }
    }
}
