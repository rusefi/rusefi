package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.image.BufferedImage;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class PinoutLookupTest {
    @Test
    void pinFieldsFollowTheNamingConvention() {
        assertTrue(PinoutLookup.isPinField("fuelPumpPin"));
        assertTrue(PinoutLookup.isPinField("injectionPins1"));
        assertTrue(PinoutLookup.isPinField("ignitionPins12"));
        assertTrue(PinoutLookup.isPinField("MAINRELAYPIN"), "Case does not matter");
        assertFalse(PinoutLookup.isPinField("fuelPumpPinMode"));
        assertFalse(PinoutLookup.isPinField("cylindersCount"));
        assertFalse(PinoutLookup.isPinField(null));
    }

    @Test
    void pinChoiceDropsQuotesAndNoPinValues() {
        assertEquals("B6 injector output 1", PinoutLookup.pinChoice("\"B6 injector output 1\""));
        assertEquals("B6 injector output 1", PinoutLookup.pinChoice("  B6 injector output 1 "));
        assertNull(PinoutLookup.pinChoice("NONE"));
        assertNull(PinoutLookup.pinChoice("\"INVALID\""));
        assertNull(PinoutLookup.pinChoice(" "));
        assertNull(PinoutLookup.pinChoice(null));
    }

    @Test
    void findReturnsTheConnectorItsDrawingAndThePin() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            BufferedImage drawing = new BufferedImage(4, 4, BufferedImage.TYPE_INT_ARGB);
            PinoutPin injector = pin("B6", "B6 injector output 1");
            PinoutPin fan = pin("C3", "C3 low side 1");
            List<PinoutConnectorPage> connectors = Arrays.asList(
                page("Connector B", drawing, Arrays.asList(pin("B1", "B1 injector output 6"), injector)),
                page("Connector C", null, Collections.singletonList(fan)));

            PinoutLookup.Match match = PinoutLookup.find(connectors, "\"B6 injector output 1\"");
            assertNotNull(match);
            assertEquals(0, match.connector);
            assertEquals("Connector B", match.connectorTitle);
            assertSame(drawing, match.image);
            assertSame(injector, match.pin);

            match = PinoutLookup.find(connectors, "C3 low side 1");
            assertEquals(1, match.connector);
            assertNull(match.image, "A connector without a drawing still finds its pins");
            assertSame(fan, match.pin);

            assertNull(PinoutLookup.find(connectors, "NONE"));
            assertNull(PinoutLookup.find(connectors, "D1 not on this board"));
        });
    }

    private static PinoutPin pin(String name, String tsName) {
        return new PinoutPin(name, tsName, "outputs", "", tsName, "");
    }

    private static PinoutConnectorPage page(String title, BufferedImage image, List<PinoutPin> pins) {
        return new PinoutConnectorPage(title, image, pins, PinoutDrawing.ColorMode.TYPE, pin -> null, assignment -> { });
    }
}
