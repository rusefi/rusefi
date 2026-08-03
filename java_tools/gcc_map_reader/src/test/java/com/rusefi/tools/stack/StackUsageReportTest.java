package com.rusefi.tools.stack;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.InputStream;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class StackUsageReportTest {
    @TempDir
    Path temporaryDirectory;

    @Test
    void directEstimateAndPartialCalls() throws Exception {
        Map<String, StackUsageReport.Node> nodes = parse(
            "node: { title: \"root\" label: \"root\\nroot.cpp:1:1\\n16 bytes (static)\" }\n"
                + "node: { title: \"small\" label: \"small\\nroot.cpp:2:1\\n8 bytes (static)\" }\n"
                + "node: { title: \"deep\" label: \"deep\\nroot.cpp:3:1\\n32 bytes (static)\" }\n"
                + "node: { title: \"external\" label: \"external\" shape : ellipse }\n"
                + "node: { title: \"__indirect_call\" label: \"Indirect Call Placeholder\" shape : ellipse }\n"
                + "edge: { sourcename: \"root\" targetname: \"small\" }\n"
                + "edge: { sourcename: \"root\" targetname: \"deep\" }\n"
                + "edge: { sourcename: \"small\" targetname: \"external\" }\n"
                + "edge: { sourcename: \"root\" targetname: \"__indirect_call\" }\n");

        StackUsageReport.Analysis result = StackUsageReport.analyze(nodes, "root");
        assertEquals(48, result.stack);
        assertEquals(Collections.singleton("external"), result.unknown);
        assertEquals(Collections.singleton("root"), result.indirect);
        assertEquals("root.cpp:1:1", nodes.get("root").source);
    }

    @Test
    void recursionAndDynamicStackArePartial() throws Exception {
        Map<String, StackUsageReport.Node> nodes = parse(
            "node: { title: \"a\" label: \"a\\na.c:1:1\\n8 bytes (static)\" }\n"
                + "node: { title: \"b\" label: \"b\\nb.c:1:1\\n12 bytes (dynamic)\" }\n"
                + "node: { title: \"deep\" label: \"deep\\ndeep.c:1:1\\n100 bytes (static)\" }\n"
                + "edge: { sourcename: \"a\" targetname: \"b\" }\n"
                + "edge: { sourcename: \"b\" targetname: \"a\" }\n"
                + "edge: { sourcename: \"b\" targetname: \"deep\" }\n");

        StackUsageReport.Analysis result = StackUsageReport.analyze(nodes, "a");
        assertEquals(120, result.stack);
        assertTrue(result.recursion);
        assertEquals(Collections.singleton("b"), result.dynamic);
    }

    @Test
    void mapSizesAndSummaryRendering() throws Exception {
        Path map = write("sizes.map",
            "  0x00000064 __process_stack_size__ = 0x64\n"
                + "  0x00000100 __main_stack_size__ = 0x100\n");
        Map<String, Integer> sizes = StackUsageReport.parseMapStackSizes(map);

        Map<String, StackUsageReport.Node> nodes = new TreeMap<>();
        StackUsageReport.Node root = new StackUsageReport.Node("root");
        root.function = "root";
        root.source = "root.cpp:1";
        root.stack = 96;
        root.indirectCalls = 1;
        nodes.put("root", root);
        StackUsageReport.Node helper = new StackUsageReport.Node("helper");
        helper.function = "helper";
        helper.source = "helper.cpp:2";
        helper.stack = 32;
        nodes.put("helper", helper);

        List<StackUsageReport.Root> roots = Arrays.asList(
            new StackUsageReport.Root("firmware", "main/process", "root", "process",
                new StackUsageReport.ReviewedBaseline(120, 96, "test scenario")),
            new StackUsageReport.Root("firmware", "exception/ISR", null, "exception", null));
        List<StackUsageReport.Graph> graphs = Collections.singletonList(
            new StackUsageReport.Graph("firmware", nodes, roots, sizes));

        String report = StackUsageReport.render("test", graphs);
        assertTrue(report.contains("| firmware | main/process | 100 | 120 | test scenario | 96 | 96 | "
            + "PROXY +0, PROXY BELOW REVIEWED, SCENARIO OVER NOMINAL +20; partial proxy: 1 indirect |"));
        assertTrue(report.contains("| firmware | exception/ISR | 256 | - | - | - | - | NOT REVIEWED |"));

        root.stack = 100;
        report = StackUsageReport.render("test", graphs);
        assertTrue(report.contains("REVIEW PROXY +4, PROXY BELOW REVIEWED, SCENARIO OVER NOMINAL +20"));
        assertTrue(report.contains("## Largest Post-LTO Frames"));
        assertFalse(report.contains(" -> "));

    }

    @Test
    void linkedStackRootMetadata() throws Exception {
        Path map = write("roots.map",
            "Discarded input sections\n"
                + " .rusefi_stack_root.dead.100\n"
                + "Linker script and memory map\n"
                + ".rusefi_stack_root.CAN_RX.3*400\n"
                + " .rusefi_stack_root.CAN_RX.3*400\n"
                + ".rusefi_stack_root.idle.32\n"
                + "                0x00000000 0x1 test.o\n"
                + ".rusefi_stack_reviewed_roots.CAN_RX\n"
                + "                0x00000000 0x1 test.o\n");
        String readelf =
            "String dump of section '.rusefi_stack_root.CAN_RX.3*400':\n"
                + "  [     0]  CanRead::ThreadTask()\n"
                + "  [    16]  440|1244|CAN serial receive\n"
                + "String dump of section '.rusefi_stack_root.idle.32':\n"
                + "  [     0]  __idle_thread\n";

        List<StackUsageReport.Root> roots = StackUsageReport.parseMapRoots(map, "firmware", readelf);
        assertEquals(3, roots.size());
        assertEquals(new StackUsageReport.Root("firmware", "CAN RX", "CanRead::ThreadTask()", 1200,
            new StackUsageReport.ReviewedBaseline(440, 1244, "CAN serial receive")), roots.get(0));
        assertEquals(new StackUsageReport.Root("firmware", "exception/ISR", null, "exception", null), roots.get(1));
        assertEquals(new StackUsageReport.Root("firmware", "idle", "__idle_thread", 32, null), roots.get(2));
        assertEquals("SD/MMC", StackUsageReport.rootName("SD__MMC"));
        assertEquals(BigInteger.valueOf(448), StackUsageReport.parseBudget("(3*128)+64"));
        assertEquals(BigInteger.valueOf(256), StackUsageReport.parseBudget("0x100"));
        assertEquals(new BigInteger("4294967296"), StackUsageReport.parseBudget("1<<32"));
        assertEquals(new BigInteger("4294967297"), StackUsageReport.parseBudget("65536*65536+1"));
        assertEquals(new BigInteger("4294967296"),
            new StackUsageReport.Root("firmware", "large", "root", StackUsageReport.parseBudget("1<<32"), null).budget);
        IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.parseBudget("unknown"));
        assertTrue(error.getMessage().contains("invalid stack budget"));
        for (String malformed : Arrays.asList("010", "_1", "1_", "1__0", "0_x10")) {
            assertThrows(IllegalArgumentException.class, () -> StackUsageReport.parseBudget(malformed));
        }
        assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.parseReviewedBaseline("440|missing scenario"));

        Path wrongManifest = write("wrong_manifest.map",
            new String(Files.readAllBytes(map), StandardCharsets.UTF_8)
                .replace(".rusefi_stack_reviewed_roots.CAN_RX", ".rusefi_stack_reviewed_roots.idle"));
        IllegalArgumentException manifestError = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.parseMapRoots(wrongManifest, "firmware", readelf));
        assertTrue(manifestError.getMessage().contains("missing=[idle], extra=[CAN RX]"));

        Path missingManifest = write("missing_manifest.map",
            new String(Files.readAllBytes(map), StandardCharsets.UTF_8)
                .replace(".rusefi_stack_reviewed_roots.CAN_RX", ".ignored"));
        IllegalArgumentException missingManifestError = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.parseMapRoots(missingManifest, "firmware", readelf));
        assertTrue(missingManifestError.getMessage().contains("missing reviewed stack root manifest"));
    }

    @Test
    void demanglingAndMissingInputs() {
        String symbol = "/tmp/cc.o:_ZN8CanWrite12PeriodicTaskEx.lto_priv.0";
        Map<String, StackUsageReport.Node> nodes = new TreeMap<>();
        nodes.put(symbol, new StackUsageReport.Node(symbol));
        StackUsageReport.applyDemangledSymbols(nodes,
            "CanWrite::PeriodicTask(long long) [clone .lto_priv.0]\n");

        assertEquals("CanWrite::PeriodicTask(long long)", nodes.get(symbol).function);
        assertEquals("lwip_thread", StackUsageReport.canonicalizeFunction("lwip_thread.lto_priv.0"));
        assertEquals(symbol, StackUsageReport.findSymbol(nodes, "CanWrite::PeriodicTask(long long)"));
        assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.parseCallgraphs(Collections.<Path>emptyList()));
        assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.findSymbol(nodes, "missing"));
    }

    @Test
    void subprocessDrainsInputOutputAndErrorConcurrently() throws Exception {
        String java = Paths.get(System.getProperty("java.home"), "bin", "java").toString();
        if (Files.exists(Paths.get(java + ".exe"))) {
            java += ".exe";
        }
        char[] characters = new char[300_000];
        Arrays.fill(characters, 'i');
        String output = StackUsageReport.run(Arrays.asList(
            java, "-cp", System.getProperty("java.class.path"), PipeFlood.class.getName()),
            new String(characters));
        assertTrue(output.endsWith("DONE 300000"));
    }

    private Map<String, StackUsageReport.Node> parse(String text) throws Exception {
        return StackUsageReport.parseCallgraphs(Collections.singletonList(write("test.ci", text)));
    }

    private Path write(String name, String text) throws Exception {
        Path path = temporaryDirectory.resolve(name);
        Files.write(path, text.getBytes(StandardCharsets.UTF_8));
        return path;
    }

    public static class PipeFlood {
        public static void main(String[] args) throws Exception {
            byte[] output = new byte[300_000];
            Arrays.fill(output, (byte) 'o');
            System.out.write(output);
            System.out.flush();
            System.err.write(output);
            System.err.flush();

            int count = 0;
            byte[] input = new byte[4096];
            InputStream stream = System.in;
            int read;
            while ((read = stream.read(input)) >= 0) {
                count += read;
            }
            System.out.print("\nDONE " + count);
        }
    }
}
