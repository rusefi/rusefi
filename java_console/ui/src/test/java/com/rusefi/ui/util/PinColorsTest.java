package com.rusefi.ui.util;

import com.rusefi.ui.util.PinColors.PinType;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PinColorsTest {
    /** Every type the boards' connector YAMLs use has a colour; before, 9 of uaefi's 13 fell back to one blue. */
    @Test
    void everyTypeTheBoardsUseHasAType() {
        String[] used = {"ls", "inj", "ign", "av", "gnd", "agnd", "wbo", "5v", "can", "at", "etb", "hall", "12v",
                "vr", "din", "hs", "gp_low", "12V", "[gp_low, gp_high]", "gp_pp", "gp_high", "usb", "TPS", "5V",
                "vign", "sqnd", "mr", "VR", "MIL", "pgnd", "sgnd"};
        for (String type : used) {
            assertNotNull(PinType.of(type), type);
        }
        assertEquals(PinType.LOW_SIDE, PinType.of("[gp_low, gp_high]"), "a list counts as its first entry");
        assertNull(PinType.of("??"));
        assertNull(PinType.of(""));
    }

    @Test
    void typesHaveDistinctColours() {
        Set<Integer> colours = new HashSet<>();
        for (PinType type : PinType.values()) {
            assertTrue(colours.add(type.color().getRGB()), type + " repeats a colour");
        }
    }

    @Test
    void lightFillsGetDarkText() {
        assertTrue(PinColors.isLight(PinType.SWITCH.color()));
        assertFalse(PinColors.isLight(PinType.SWITCH.textColor()));
        assertEquals(java.awt.Color.WHITE, PinType.GROUND.textColor());
    }

    @Test
    void theYamlTypeWins() {
        PinColors.Resolved resolved = PinColors.resolve("hall", "VR1+", "event_inputs");
        assertEquals(PinType.HALL, resolved.type);
        assertFalse(resolved.isInferred());
    }

    /** Pins of uaefi connectors A and C without a type. */
    @Test
    void aMissingTypeIsInferredFromFunctionThenClass() {
        assertInferred(PinType.SENSOR_GROUND, "function", "Sensor ground", "");
        assertInferred(PinType.GROUND, "function", "Power Ground / VR Shield", "");
        assertInferred(PinType.VR, "function", "VR1- Discrete for low count wheel", "");
        assertInferred(PinType.VR, "function", "VR2-/HALL", "event_inputs");
        assertInferred(PinType.HALL, "function", "VSS or HALL input 3 (4.7K PU)", "event_inputs, sent_inputs");
        assertInferred(PinType.SWITCH, "class", "Button input 3 PullDown 10K by default. 12v tolerant",
                "switch_inputs, event_inputs");
        // power words come after the class: this is an analog input measuring the battery
        assertInferred(PinType.ANALOG, "class", "Ignition Switch / Battery Voltage Analog Input", "analog_inputs");
        assertInferred(PinType.POWER, "function", "+12V From Main Relay", "");
        assertInferred(PinType.SUPPLY_5V, "function", "Sensor +5v Power Feed", "");
        assertEquals(PinType.UNTYPED, PinColors.resolve("", "EGT positive input", "").type);
        assertFalse(PinColors.resolve("", "EGT positive input", "").isInferred());
    }

    private static void assertInferred(PinType expected, String from, String function, String classes) {
        PinColors.Resolved resolved = PinColors.resolve("", function, classes);
        assertEquals(expected, resolved.type, function);
        assertEquals(from, resolved.inferredFrom, function);
    }

    @Test
    void pigtailColours() {
        assertEquals(2, PinColors.pigtailToColors("orange/brown").length);
        assertEquals(1, PinColors.pigtailToColors("grey").length);
        assertNull(PinColors.pigtailToColors(""));
        assertTrue(new HashSet<>(Arrays.asList("red", "white")).stream()
                .allMatch(c -> PinColors.pigtailTokenToColor(c) != null));
    }
}
