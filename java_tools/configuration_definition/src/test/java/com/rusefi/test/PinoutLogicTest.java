package com.rusefi.test;

import com.rusefi.EnumsReader;
import com.rusefi.PinoutLogic;
import com.rusefi.enum_reader.Value;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.Assert.assertEquals;

public class PinoutLogicTest {
    @Test
    public void testEnumToTs() {
        Map<String, Value> currentValues = new TreeMap<>();
        currentValues.put("KEY", new Value(null, "3"));
        currentValues.put("KEY4", new Value("NO", "4"));
        EnumsReader.EnumState enumState = new EnumsReader.EnumState(currentValues, "pins", true);

        {
            ArrayList<String> list = new ArrayList<>(Arrays.asList("1", "NO", "10"));
            String result = PinoutLogic.enumToOptionsList("NO", enumState, list).getShorterForm();
            assertEquals("0=\"NONE\",2=\"10\",1=\"NO\"", result);

        }

        {
            ArrayList<String> list = new ArrayList<>(Arrays.asList("1", "NO", null, null, null, null, null, "10"));
            String result = PinoutLogic.enumToOptionsList("NO", enumState, list).getShorterForm();
            assertEquals("0=\"NONE\",7=\"10\",1=\"NO\"", result);
        }
    }
}
