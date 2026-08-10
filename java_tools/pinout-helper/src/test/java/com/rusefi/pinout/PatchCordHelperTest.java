package com.rusefi.pinout;

import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class PatchCordHelperTest {
    @Test
    public void testNormalizeFunction() {
        assertEquals("wbo1 vs un", PatchCordHelper.normalizeFunction("WBO1 Vs (Un) (pin 6)"));
        assertEquals("wbo1 vs un", PatchCordHelper.normalizeFunction("WBO1 Vs (Un) (OEM 2-8)"));
        assertEquals("wbo2 vs/ip vm", PatchCordHelper.normalizeFunction("WBO2 Vs/Ip (Vm) (OEM 2-11)"));
        assertEquals("", PatchCordHelper.normalizeFunction(null));
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
}
