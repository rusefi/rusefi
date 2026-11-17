package com.rusefi.ui;

import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

class PinoutPreviewTest {
    @Test
    void cardShowsThePinOnItsConnectorAndWhatItIs() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PinoutPin injector = pin("B6", 100, 100);
            injector.tuneUse = "Injection Output 1";
            PinoutPin other = pin("B5", 200, 100);
            JComponent card = PinoutPreview.create(new PinoutLookup.Match(0, "Connector B",
                new BufferedImage(1000, 400, BufferedImage.TYPE_INT_ARGB), Arrays.asList(other, injector), injector));

            List<String> texts = texts(card);
            assertTrue(texts.containsAll(Arrays.asList("B6", "Connector B", "Function", "Injector output 6",
                "Tune use", "Injection Output 1", "Pigtail", "red", PinoutPreview.CLICK_HINT)), texts.toString());
            PinoutDrawing drawing = drawing(card);
            assertNotNull(drawing, "The connector drawing locates the pin");
            assertEquals("B6", drawing.selectedPin(), "The pin is ringed");
            assertEquals(520, drawing.getPreferredSize().width);
            assertEquals(208, drawing.getPreferredSize().height, "The drawing keeps the image's shape");
        });
    }

    @Test
    void withoutADrawingTheCardHasTheTextOnly() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PinoutPin unplaced = pin("C1", -1, -1);
            JComponent card = PinoutPreview.create(new PinoutLookup.Match(1, "Connector C",
                new BufferedImage(10, 10, BufferedImage.TYPE_INT_ARGB), Arrays.asList(unplaced), unplaced));
            assertNull(drawing(card), "The YAML does not place the pin");
            assertTrue(texts(card).contains("Free"), "An unused pin reads Free");

            PinoutPin placed = pin("C2", 5, 5);
            card = PinoutPreview.create(new PinoutLookup.Match(1, "Connector C", null, Arrays.asList(placed), placed));
            assertNull(drawing(card), "The board has no drawing for this connector");
            assertTrue(texts(card).contains("C2"));
        });
    }

    private static PinoutPin pin(String name, int x, int y) {
        PinoutPin pin = new PinoutPin(name, "Injector output " + name.substring(1), "outputs", "",
            name + " injector output", "red");
        pin.x = x;
        pin.y = y;
        return pin;
    }

    private static PinoutDrawing drawing(Container card) {
        return UiUtils.getAllComponents(card).stream()
            .filter(c -> c instanceof PinoutDrawing).map(c -> (PinoutDrawing) c).findFirst().orElse(null);
    }

    private static List<String> texts(Container card) {
        return UiUtils.getAllComponents(card).stream()
            .filter(c -> c instanceof JLabel).map(c -> ((JLabel) c).getText()).collect(Collectors.toList());
    }
}
