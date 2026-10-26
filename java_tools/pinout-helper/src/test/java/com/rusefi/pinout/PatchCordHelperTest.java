package com.rusefi.pinout;

import org.junit.jupiter.api.Test;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class PatchCordHelperTest {
    @Test
    public void testNormalizeFunction() {
        assertEquals("wbo1 vs", PatchCordHelper.normalizeFunction("WBO1 Vs (Un) (pin 6)"));
        assertEquals("wbo1 vs", PatchCordHelper.normalizeFunction("WBO1 Vs (Un) (OEM 2-8)"));
        assertEquals("wbo2 vs/ip", PatchCordHelper.normalizeFunction("WBO2 Vs/Ip (Vm) (OEM 2-11)"));
        assertEquals("wbo1 heater negative",
                PatchCordHelper.normalizeFunction("WBO1 Heater Negative (pin 3) (and heater +12 could use 18C)"));
        assertEquals("", PatchCordHelper.normalizeFunction(null));
    }

    @Test
    public void testMarkdownFileName() {
        assertEquals("patchcord-BMW-N52-adapter.md", PatchCordHelper.markdownFileName("BMW-N52-adapter"));
        assertEquals("patchcord-VW_1_8T.md", PatchCordHelper.markdownFileName("VW 1.8T"));
    }

    @Test
    public void testSplitByUniqueColor() throws IOException {
        File dir = Files.createTempDirectory("patchcord").toFile();
        File yaml = new File(dir, "conn.yaml");
        Files.write(yaml.toPath(), (
                "pins:\n" +
                "  - pin: 1D\n" +
                "    color: black\n" +
                "  - pin: 2D\n" +
                "    color: black\n" +
                "  - pin: 3D\n" +
                "    color: green\n").getBytes(StandardCharsets.UTF_8));
        ConnectorYaml connector = ConnectorYaml.read(yaml);
        assertNotNull(connector);
        List<PatchCordHelper.Wire> wires = new java.util.ArrayList<>();
        for (ConnectorYaml.PinEntry pin : connector.pins) {
            PatchCordHelper.PinRef ref = new PatchCordHelper.PinRef(connector, pin);
            wires.add(new PatchCordHelper.Wire(ref, ref));
        }

        List<List<PatchCordHelper.Wire>> subSteps = PatchCordHelper.splitByUniqueColor(wires);

        assertEquals(2, subSteps.size());
        assertEquals(2, subSteps.get(0).size());
        assertEquals(1, subSteps.get(1).size());
        assertEquals("black", subSteps.get(1).get(0).color());
    }

    @Test
    public void testOfficialColorAssignmentAndStepSplit() throws IOException {
        assertEquals(Arrays.asList("white", "black", "red", "orange", "yellow",
                "green", "blue", "grey", "brown"), WireColors.officialColors());
        File dir = Files.createTempDirectory("patchcord-colors").toFile();
        File yaml = new File(dir, "conn.yaml");
        Files.write(yaml.toPath(), (
                "pins:\n" +
                "  - {pin: 1D, color: black}\n" +
                "  - {pin: 2D, color: black}\n" +
                "  - {pin: 3D, color: purple}\n" +
                "  - {pin: 4D, color: orange/brown}\n" +
                "  - {pin: 5D, color: purple}\n" +
                "  - {pin: 6D, color: purple}\n" +
                "  - {pin: 7D, color: purple}\n" +
                "  - {pin: 8D, color: purple}\n" +
                "  - {pin: 9D, color: purple}\n" +
                "  - {pin: 10D, color: purple}\n").getBytes(StandardCharsets.UTF_8));
        ConnectorYaml connector = ConnectorYaml.read(yaml);
        List<PatchCordHelper.Wire> wires = new java.util.ArrayList<>();
        for (ConnectorYaml.PinEntry pin : connector.pins) {
            PatchCordHelper.PinRef ref = new PatchCordHelper.PinRef(connector, pin);
            wires.add(new PatchCordHelper.Wire(ref, ref));
        }
        PatchCordHelper.assignOfficialColors(wires);
        assertEquals("black", wires.get(0).color());
        assertEquals("black", wires.get(1).color());
        for (PatchCordHelper.Wire wire : wires) {
            assertTrue(WireColors.isOfficial(wire.color()), wire.color());
        }
        List<List<PatchCordHelper.Wire>> steps = PatchCordHelper.splitByUniqueColor(wires);
        assertEquals(2, steps.size());
        assertEquals(9, steps.get(0).size());
        assertEquals(1, steps.get(1).size());
    }

    @Test
    public void testMatchByFunction() throws IOException {
        File dir = Files.createTempDirectory("patchcord").toFile();
        File ecuYaml = new File(dir, "ecu.yaml");
        Files.write(ecuYaml.toPath(), (
                "pins:\n" +
                "  - pin: 1D\n" +
                "    function: WBO1 Vs (Un) (pin 6)\n" +
                "    type: wbo\n" +
                "    color: black\n" +
                "  - pin: 2D\n" +
                "    function: Only on ECU\n").getBytes(StandardCharsets.UTF_8));
        File adapterYaml = new File(dir, "adapter.yaml");
        Files.write(adapterYaml.toPath(), (
                "pins:\n" +
                "  - pin: 3D\n" +
                "    function: WBO1 Vs (Un) (OEM 2-8)\n" +
                "  - pin: 4D\n" +
                "    function: Only on adapter\n").getBytes(StandardCharsets.UTF_8));

        ConnectorYaml ecu = ConnectorYaml.read(ecuYaml);
        ConnectorYaml adapter = ConnectorYaml.read(adapterYaml);
        assertNotNull(ecu);
        assertNotNull(adapter);
        List<PatchCordHelper.PinRef> ecuPins = new java.util.ArrayList<>();
        ecuPins.add(new PatchCordHelper.PinRef(ecu, ecu.pins.get(0)));
        ecuPins.add(new PatchCordHelper.PinRef(ecu, ecu.pins.get(1)));
        List<PatchCordHelper.PinRef> adapterPins = new java.util.ArrayList<>();
        adapterPins.add(new PatchCordHelper.PinRef(adapter, adapter.pins.get(0)));
        adapterPins.add(new PatchCordHelper.PinRef(adapter, adapter.pins.get(1)));

        List<PatchCordHelper.Wire> wires = PatchCordHelper.matchByFunction(ecuPins, adapterPins);

        assertEquals(1, wires.size());
        assertEquals("1D", wires.get(0).ecu.pin.pin);
        assertEquals("3D", wires.get(0).adapter.pin.pin);
        assertEquals("black", wires.get(0).color());
        assertEquals(1, ecuPins.size());
        assertEquals(1, adapterPins.size());
        assertEquals("2D", ecuPins.get(0).pin.pin);
        assertEquals("4D", adapterPins.get(0).pin.pin);
    }

    @Test
    public void testAdapterColumnHasWireAndMarkerDots() throws IOException {
        File root = Files.createTempDirectory("patchcord").toFile();
        File ecuConnectors = new File(root, "ecu/connectors");
        File adapterConnectors = new File(root, "N52-adapter/connectors");
        assertTrue(ecuConnectors.mkdirs());
        assertTrue(adapterConnectors.mkdirs());
        ImageIO.write(new BufferedImage(20, 20, BufferedImage.TYPE_INT_RGB), "png",
                new File(ecuConnectors, "face.png"));
        ImageIO.write(new BufferedImage(20, 20, BufferedImage.TYPE_INT_RGB), "png",
                new File(adapterConnectors, "face.png"));
        Files.write(new File(ecuConnectors, "26pin.yaml").toPath(), (
                "pins:\n" +
                "  - pin: 17D\n" +
                "    function: Power/Chassis GND ground\n" +
                "    type: gnd\n" +
                "    color: black\n" +
                "info:\n" +
                "  image: {file: face.png}\n" +
                "  pins: [{pin: 17D, x: 10, y: 10}]\n").getBytes(StandardCharsets.UTF_8));
        Files.write(new File(adapterConnectors, "N52-60pin.yaml").toPath(), (
                "pins:\n" +
                "  - pin: 10A\n" +
                "    function: Power/Chassis GND ground (OEM 3-3)\n" +
                "    type: gnd\n" +
                "info:\n" +
                "  image: {file: face.png}\n" +
                "  pins: [{pin: 10A, x: 10, y: 10}]\n").getBytes(StandardCharsets.UTF_8));
        File out = new File(root, "out");

        PatchCordHelper.main(new String[]{new File(root, "ecu").getPath(),
                new File(root, "N52-adapter").getPath(), out.getPath()});

        File md = new File(out, "patchcord-N52-adapter.md");
        assertTrue(md.isFile());
        String text = new String(Files.readAllBytes(md.toPath()), StandardCharsets.UTF_8);
        assertTrue(text.contains("| ![black](dot-black.png) ![paint marker]("
                + PatchCordHelper.PAINT_MARKER_DOT + ") N52-60pin / 10A |"), text);
        assertTrue(new File(out, "dot-black.png").isFile());
        assertTrue(new File(out, PatchCordHelper.PAINT_MARKER_DOT).isFile());
        assertTrue(text.contains("## Complete patchcord diagram"));
        assertTrue(text.contains("| 26pin / 17D | black | N52-60pin / 10A |"));
        assertTrue(new File(out, "overview-ecu-26pin.png").isFile());
        assertTrue(new File(out, "overview-adapter-N52-60pin.png").isFile());
        File html = new File(out, "patchcord-N52-adapter.html");
        assertTrue(html.isFile());
        String interactive = new String(Files.readAllBytes(html.toPath()), StandardCharsets.UTF_8);
        assertTrue(interactive.contains("data:image/png;base64,"));
        assertTrue(interactive.contains("id=\"step-filter\""));
        assertTrue(interactive.contains("id=\"wire-search\""));
        assertTrue(interactive.contains("data-wire=\"w1\" data-step=\"1\""));
        assertTrue(interactive.contains("const byWire = new Map"));
        assertEquals(4, interactive.split("class=\"pin-marker\"", -1).length - 1);
        assertEquals(2, interactive.split("data-overview=\"true\"", -1).length - 1);
        assertEquals(1, interactive.split("class=\"overview-route\"", -1).length - 1);
        assertTrue(interactive.indexOf("<section class=\"total-overview\"")
                > interactive.indexOf("<h2>Wiring list</h2>"));
    }

    @Test
    public void testConnectorPairsRepeatPlugAndKeepOtherWiresFaint() throws IOException {
        File root = Files.createTempDirectory("patchcord-pairs").toFile();
        File ecuConnectors = new File(root, "ecu/connectors");
        File adapterConnectors = new File(root, "adapter/connectors");
        assertTrue(ecuConnectors.mkdirs());
        assertTrue(adapterConnectors.mkdirs());
        BufferedImage face = new BufferedImage(20, 20, BufferedImage.TYPE_INT_RGB);
        ImageIO.write(face, "png", new File(ecuConnectors, "face.png"));
        ImageIO.write(face, "png", new File(adapterConnectors, "face.png"));
        Files.write(new File(ecuConnectors, "ecu-shared.yaml").toPath(), (
                "pins:\n" +
                "  - {pin: 1E, function: Circuit A, type: av, color: green}\n" +
                "  - {pin: 2E, function: Circuit B, type: av, color: blue}\n" +
                "info:\n" +
                "  image: {file: face.png}\n" +
                "  pins: [{pin: 1E, x: 5, y: 5}, {pin: 2E, x: 15, y: 15}]\n")
                .getBytes(StandardCharsets.UTF_8));
        Files.write(new File(ecuConnectors, "ecu-unused.yaml").toPath(), (
                "pins:\n  - {pin: 3E, function: Unmatched}\n")
                .getBytes(StandardCharsets.UTF_8));
        for (String name : Arrays.asList("adapter-a", "adapter-b")) {
            String pin = name.endsWith("a") ? "1A" : "1B";
            String function = name.endsWith("a") ? "Circuit A" : "Circuit B";
            Files.write(new File(adapterConnectors, name + ".yaml").toPath(), (
                    "pins:\n  - {pin: " + pin + ", function: " + function + "}\n" +
                    "info:\n  image: {file: face.png}\n" +
                    "  pins: [{pin: " + pin + ", x: 10, y: 10}]\n")
                    .getBytes(StandardCharsets.UTF_8));
        }
        File out = new File(root, "out");
        PatchCordHelper.main(new String[]{new File(root, "ecu").getPath(),
                new File(root, "adapter").getPath(), out.getPath(), "--html-only=true"});
        assertFalse(new File(out, "patchcord-adapter.md").exists());
        assertEquals(1, out.listFiles().length);
        String html = new String(Files.readAllBytes(new File(out, "patchcord-adapter.html").toPath()),
                StandardCharsets.UTF_8);
        String[] parts = html.split("<section class=\"connector-pair\"", -1);
        assertEquals(3, parts.length);
        assertEquals(8, html.split("class=\"pin-marker\"", -1).length - 1);
        assertEquals(2, html.split("class=\"context-marker\"", -1).length - 1);
        assertEquals(2, html.split("class=\"pair-wire\"", -1).length - 1);
        assertEquals(3, html.split("<h3>ecu-shared", -1).length - 1);
        assertTrue(!html.contains("<h3>ecu-unused"));
        String firstPair = parts[1].split("</section>", 2)[0];
        String secondPair = parts[2].split("</section>", 2)[0];
        assertTrue(firstPair.contains("ECU ecu-shared") && secondPair.contains("ECU ecu-shared"));
        assertTrue(firstPair.contains("Adapter adapter-a") || secondPair.contains("Adapter adapter-a"));
        assertTrue(firstPair.contains("Adapter adapter-b") || secondPair.contains("Adapter adapter-b"));
        assertEquals(2, firstPair.split("class=\"pin-marker\"", -1).length - 1);
        assertEquals(2, secondPair.split("class=\"pin-marker\"", -1).length - 1);
        assertEquals(1, firstPair.split("class=\"context-marker\"", -1).length - 1);
        assertEquals(1, secondPair.split("class=\"context-marker\"", -1).length - 1);
        assertTrue(firstPair.contains("data-wire=\"w1\"") != secondPair.contains("data-wire=\"w1\""));
        assertTrue(firstPair.contains("data-wire=\"w2\"") != secondPair.contains("data-wire=\"w2\""));
        String overview = html.substring(html.indexOf("<section class=\"total-overview\""));
        assertTrue(html.indexOf("<section class=\"total-overview\"") > html.indexOf("<h2>Wiring list</h2>"));
        assertEquals(4, overview.split("data-overview=\"true\"", -1).length - 1);
        assertEquals(2, overview.split("class=\"overview-route\"", -1).length - 1);
        assertEquals(1, overview.split("<h3>ecu-shared", -1).length - 1);
        assertEquals(1, overview.split("<h3>adapter-a", -1).length - 1);
        assertEquals(1, overview.split("<h3>adapter-b", -1).length - 1);
        assertTrue(!overview.contains("<h3>ecu-unused"));
    }

    @Test
    public void testHtmlOnlyOptionRejectsInvalidValue() {
        assertThrows(IllegalArgumentException.class,
                () -> PatchCordHelper.main(new String[]{"ecu", "adapter", "out", "--html-only=maybe"}));
    }

    @Test
    public void testDotFileName() {
        assertEquals("dot-black.png", PatchCordHelper.dotFileName("Black"));
        assertEquals("dot-orange_brown.png", PatchCordHelper.dotFileName("orange/brown"));
        assertEquals("dot-unknown.png", PatchCordHelper.dotFileName(null));
        assertEquals("dot-unknown.png", PatchCordHelper.dotFileName("  "));
    }
}
