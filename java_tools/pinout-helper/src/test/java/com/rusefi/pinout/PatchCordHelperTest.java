package com.rusefi.pinout;

import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
        Files.write(new File(ecuConnectors, "26pin.yaml").toPath(), (
                "pins:\n" +
                "  - pin: 17D\n" +
                "    function: Power/Chassis GND ground\n" +
                "    type: gnd\n" +
                "    color: black\n").getBytes(StandardCharsets.UTF_8));
        Files.write(new File(adapterConnectors, "N52-60pin.yaml").toPath(), (
                "pins:\n" +
                "  - pin: 10A\n" +
                "    function: Power/Chassis GND ground (OEM 3-3)\n" +
                "    type: gnd\n").getBytes(StandardCharsets.UTF_8));
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
    }

    @Test
    public void testDotFileName() {
        assertEquals("dot-black.png", PatchCordHelper.dotFileName("Black"));
        assertEquals("dot-orange_brown.png", PatchCordHelper.dotFileName("orange/brown"));
        assertEquals("dot-unknown.png", PatchCordHelper.dotFileName(null));
        assertEquals("dot-unknown.png", PatchCordHelper.dotFileName("  "));
    }
}
