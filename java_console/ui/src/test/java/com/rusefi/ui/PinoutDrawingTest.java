package com.rusefi.ui;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.imageio.ImageIO;
import javax.swing.AbstractButton;
import javax.swing.JToggleButton;
import javax.swing.SwingUtilities;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The Pinout tab's connector drawing, connector switch and legend. */
class PinoutDrawingTest {
    @TempDir
    Path tempDir;

    private static PinoutPin pin(String name, String type, int x, int y) {
        PinoutPin pin = new PinoutPin(name, name + " function", type, "", "", "");
        pin.x = x;
        pin.y = y;
        return pin;
    }

    /** Two rows of pins 100 image pixels apart on a 1000 x 400 image. */
    private static PinoutDrawing drawing(int width, int height) {
        PinoutDrawing drawing = new PinoutDrawing();
        List<PinoutPin> pins = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            pins.add(pin("A" + (i + 1), "av", 300 + 100 * i, 150));
            pins.add(pin("B" + (i + 1), "gnd", 300 + 100 * i, 250));
        }
        PinoutPin unplaced = new PinoutPin("On-board MAP", "MAP", "av", "", "", "");
        pins.add(unplaced);
        drawing.setData(new BufferedImage(1000, 400, BufferedImage.TYPE_INT_RGB), pins);
        drawing.setSize(width, height);
        return drawing;
    }

    @Test
    void markersFollowThePinSpacingWithinLimits() {
        PinoutDrawing small = drawing(532, 232);
        PinoutDrawing large = drawing(1032, 432);
        // scale 0.5 and 1.0: a third of the 100 px spacing
        assertEquals(17, small.markerRadius());
        assertEquals(24, large.markerRadius(), "capped");
        assertEquals(9, drawing(232, 112).markerRadius(), "never too small to read");
    }

    @Test
    void aClickSelectsThePinUnderIt() {
        PinoutDrawing drawing = drawing(1032, 432);
        List<String> clicked = new ArrayList<>();
        drawing.setPinClickListener(clicked::add);
        int x = drawing.screenX(pin("", "", 500, 250));
        int y = drawing.screenY(pin("", "", 500, 250));
        assertEquals("B3", drawing.pinAt(x + 10, y - 10), "anywhere on the marker");
        assertNull(drawing.pinAt(x + 50, y), "between markers");

        drawing.dispatchEvent(new MouseEvent(drawing, MouseEvent.MOUSE_CLICKED, 0, 0, x, y, 1, false));
        assertEquals(Arrays.asList("B3"), clicked);
        assertEquals("B3", drawing.selectedPin());

        drawing.dispatchEvent(new MouseEvent(drawing, MouseEvent.MOUSE_MOVED, 0, 0, x, y, 0, false));
        assertEquals("B3", drawing.hoveredPin());
        drawing.dispatchEvent(new MouseEvent(drawing, MouseEvent.MOUSE_EXITED, 0, 0, -1, -1, 0, false));
        assertNull(drawing.hoveredPin());
    }

    @Test
    void connectorsAreASwitchAndTheLegendListsWhatIsOnTheConnector() throws Exception {
        Path archive = archive();
        SwingUtilities.invokeAndWait(() -> {
            PinoutPane pane = new PinoutPane(new UIContext(), false);
            pane.buildConnectorTabs(Arrays.asList("b.yaml", "a.yaml"), archive.toFile());
            List<JToggleButton> switches = toolbarToggles(pane);
            assertEquals(Arrays.asList("Connector A  ·  2", "Connector B  ·  1", "Type", "Pigtail", "Tune use"),
                    texts(switches), "connectors sorted by name, with pin counts, then Color by");
            assertTrue(switches.get(0).isSelected());
            assertEquals(0, pane.selectedConnector());

            PinoutConnectorPage a = page(pane, 0);
            assertEquals(Arrays.asList("Power ground  1", "Analog input  1"), texts(chips(a)),
                    "only the types on this connector, with counts, in family order");
            assertEquals("0 used  ·  1 free  ·  1 fixed", a.summary.getText());

            switches.get(4).doClick();
            assertEquals(Arrays.asList("Free  1", "Fixed function  1"), texts(chips(a)));

            pane.highlightByEnumValue("B1 Low Side output");
            assertEquals(1, pane.selectedConnector(), "a pin on another connector switches to it");
            assertTrue(switches.get(1).isSelected());
            assertEquals("B1", page(pane, 1).selected().name);
        });
    }

    @Test
    void legendChipsAndTheFilterNarrowTheTableAndFadeTheDrawing() throws Exception {
        Path archive = archive();
        SwingUtilities.invokeAndWait(() -> {
            PinoutPane pane = new PinoutPane(new UIContext(), false);
            pane.buildConnectorTabs(Arrays.asList("a.yaml", "b.yaml"), archive.toFile());
            PinoutConnectorPage a = page(pane, 0);
            assertEquals(2, a.table.getRowCount());

            chips(a).get(1).doClick();
            assertEquals(1, a.table.getRowCount(), "only analog inputs");
            assertEquals("A1", a.table.getValueAt(0, 0));
            assertTrue(chips(a).get(1).isSelected(), "the chosen chip stays marked");
            assertTrue(a.isShown(a.find("A1")));
            assertTrue(!a.isShown(a.find("A2")), "the drawing fades the rest");
            assertEquals("1 of 2 pins shown", a.summary.getText());
            chips(a).get(1).doClick();
            assertEquals(2, a.table.getRowCount(), "chosen twice: all again");

            a.filterButtons.get(PinoutPin.Use.FIXED).doClick();
            assertEquals(1, a.table.getRowCount());
            assertEquals("A2", a.table.getValueAt(0, 0));
            assertEquals("Fixed  1", a.filterButtons.get(PinoutPin.Use.FIXED).getText());
            assertEquals("All  2", a.allButton.getText());
            a.allButton.doClick();
            assertEquals(2, a.table.getRowCount());
        });
    }

    @Test
    void thePinCardFollowsTheSelectionAndOpensTheSettingInTuning() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<PinoutPin> opened = new ArrayList<>();
            PinoutConnectorPage page = new PinoutConnectorPage("Connector A", null, pins(),
                    PinoutDrawing.ColorMode.TYPE,
                    pin -> pin.use() == PinoutPin.Use.USED ? new String[]{"fanPin", "fanSettings"} : null,
                    assignment -> opened.add(null));
            PinoutConnectorPage.PinCard card = page.card;
            assertNull(card.pin(), "nothing selected yet");

            page.select(page.find("A1"));
            assertEquals("Pin A1", card.name.getText());
            assertEquals("Fan output", card.useValue.getText());
            assertTrue(card.openButton.isVisible());
            assertEquals("Analog input", card.type.getText());
            assertEquals("", card.typeNote.getText());
            assertEquals("A1 AV1", card.tsName.getText());
            assertEquals(0, page.table.getSelectedRow(), "the table selects the same pin");
            card.openButton.doClick();
            assertEquals(1, opened.size());

            page.table.setRowSelectionInterval(1, 1);
            assertEquals("Pin A2", card.name.getText());
            assertEquals("Fixed function", card.useValue.getText());
            assertTrue(!card.openButton.isVisible());
            assertEquals("Power ground", card.type.getText());
            assertEquals("from its function", card.typeNote.getText());
            assertEquals("A2", page.drawing.selectedPin(), "the drawing selects the same pin");

            page.select(page.find("A3"));
            assertEquals("Free", card.useValue.getText());
            assertTrue(card.useHint.getText().contains("\"A3 AV3\""), "says which choice in Tuning uses it");

            // a tune edit: the table, the summary and the open card follow
            page.setTuneUse(java.util.Collections.singletonMap("A3 AV3", "Boost output"));
            assertEquals("Boost output", card.useValue.getText());
            assertEquals("Boost output", page.table.getValueAt(2, PinoutConnectorPage.TUNE_USE_COLUMN));
            assertEquals("Free", PinoutConnectorPage.useText(page.find("A1")), "A1 no longer used");
        });
    }

    @Test
    void searchCoversEveryConnector() throws Exception {
        Path archive = archive();
        SwingUtilities.invokeAndWait(() -> {
            PinoutPane pane = new PinoutPane(new UIContext(), false);
            pane.buildConnectorTabs(Arrays.asList("a.yaml", "b.yaml"), archive.toFile());
            pane.search.setText("low side");
            assertEquals(Arrays.asList("Connector A  ·  none", "Connector B  ·  1 found"),
                    texts(toolbarToggles(pane)).subList(0, 2));
            assertEquals(1, pane.selectedConnector(), "moves to the connector with a match");
            pane.search.postActionEvent();
            assertEquals("B1", page(pane, 1).selected().name, "Enter selects the first match");

            pane.search.setText("ground");
            assertEquals(0, pane.selectedConnector());
            assertEquals(1, page(pane, 0).table.getRowCount());
            pane.search.setText("");
            assertEquals(Arrays.asList("Connector A  ·  2", "Connector B  ·  1"),
                    texts(toolbarToggles(pane)).subList(0, 2));
            assertEquals(2, page(pane, 0).table.getRowCount());
        });
    }

    /** A1 analog input used by the tune, A2 a ground (type from its function), A3 free. */
    private static List<PinoutPin> pins() {
        PinoutPin a1 = new PinoutPin("A1", "Analog input 1", "av", "analog_inputs", "A1 AV1", "blue");
        a1.tuneUse = "Fan output";
        PinoutPin a2 = new PinoutPin("A2", "Ground", "", "", "", "black");
        PinoutPin a3 = new PinoutPin("A3", "Analog input 3", "av", "", "A3 AV3", "");
        return new ArrayList<>(Arrays.asList(a1, a2, a3));
    }

    private static PinoutConnectorPage page(PinoutPane pane, int index) {
        return components(pane.getContent(), PinoutConnectorPage.class).get(index);
    }

    private static List<JToggleButton> chips(PinoutConnectorPage page) {
        return components(page.legend, JToggleButton.class);
    }

    /** The connector switch, then Color by. */
    private static List<JToggleButton> toolbarToggles(PinoutPane pane) {
        BorderLayout layout = (BorderLayout) pane.getContent().getLayout();
        return components(layout.getLayoutComponent(BorderLayout.NORTH), JToggleButton.class);
    }

    private static List<String> texts(List<? extends AbstractButton> buttons) {
        List<String> texts = new ArrayList<>();
        for (AbstractButton button : buttons) {
            texts.add(button.getText());
        }
        return texts;
    }

    private Path archive() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        ByteArrayOutputStream png = new ByteArrayOutputStream();
        ImageIO.write(new BufferedImage(200, 100, BufferedImage.TYPE_INT_RGB), "png", png);
        String a = "pins:\n"
                + "  - pin: A1\n    type: av\n    function: Analog input 1\n    ts_name: ___ AV1\n"
                + "  - pin: A2\n    type: gnd\n    function: Ground\n"
                + "info:\n  name: connector A\n  image:\n    file: a.png\n"
                + "  pins:\n    - pin: A1\n      x: 50\n      y: 50\n    - pin: A2\n      x: 150\n      y: 50\n";
        String b = "pins:\n  - pin: B1\n    type: ls\n    ts_name: ___ Low Side output\n"
                + "info:\n  name: connector B\n";
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(archive))) {
            zip.putNextEntry(new ZipEntry("a.yaml"));
            zip.write(a.getBytes(StandardCharsets.UTF_8));
            zip.putNextEntry(new ZipEntry("a.png"));
            zip.write(png.toByteArray());
            zip.putNextEntry(new ZipEntry("b.yaml"));
            zip.write(b.getBytes(StandardCharsets.UTF_8));
            zip.closeEntry();
        }
        return archive;
    }

    private static <T extends Component> List<T> components(Component root, Class<T> type) {
        List<T> result = new ArrayList<>();
        if (type.isInstance(root)) {
            result.add(type.cast(root));
        }
        if (root instanceof Container) {
            for (Component child : ((Container) root).getComponents()) {
                result.addAll(components(child, type));
            }
        }
        return result;
    }
}
