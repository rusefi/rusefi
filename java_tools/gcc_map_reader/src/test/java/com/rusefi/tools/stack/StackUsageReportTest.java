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

    /**
     * The main blind spot of the direct-call proxy: a virtual or function-pointer call shows up in
     * the GCC callgraph only as an edge to "__indirect_call". The callee's whole subtree (here a
     * 900-byte file-system path) contributes nothing to the estimate, the root is merely marked
     * "indirect". This is how storageWrite() -> SettingStorageBase::store() hides FatFS from the
     * storage manager proxy.
     */
    @Test
    void indirectCalleeSubtreeContributesNothing() throws Exception {
        Map<String, StackUsageReport.Node> nodes = parse(
            "node: { title: \"root\" label: \"root\\nroot.cpp:1:1\\n16 bytes (static)\" }\n"
                + "node: { title: \"virtualImpl\" label: \"virtualImpl\\nimpl.cpp:1:1\\n500 bytes (static)\" }\n"
                + "node: { title: \"f_open\" label: \"f_open\\nff.c:1:1\\n400 bytes (static)\" }\n"
                + "node: { title: \"__indirect_call\" label: \"Indirect Call Placeholder\" shape : ellipse }\n"
                + "edge: { sourcename: \"root\" targetname: \"__indirect_call\" }\n"
                + "edge: { sourcename: \"virtualImpl\" targetname: \"f_open\" }\n");

        StackUsageReport.Analysis result = StackUsageReport.analyze(nodes, "root");
        assertEquals(16, result.stack);
        assertEquals(Collections.singleton("root"), result.indirect);
        assertTrue(result.unknown.isEmpty());
        // the implementation itself is a perfectly analyzable 900-byte path
        assertEquals(900, StackUsageReport.analyze(nodes, "virtualImpl").stack);
    }

    /** A callee with no .su data (libc, builtins, foreign objects) is counted as zero bytes, not as an error. */
    @Test
    void unknownFrameCountsAsZeroEvenWhenItIsTheDeepestCall() throws Exception {
        Map<String, StackUsageReport.Node> nodes = parse(
            "node: { title: \"root\" label: \"root\\nroot.cpp:1:1\\n24 bytes (static)\" }\n"
                + "node: { title: \"sprintf\" label: \"sprintf\" shape : ellipse }\n"
                + "edge: { sourcename: \"root\" targetname: \"sprintf\" }\n");

        StackUsageReport.Analysis result = StackUsageReport.analyze(nodes, "root");
        assertEquals(24, result.stack);
        assertEquals(Collections.singleton("sprintf"), result.unknown);
        assertFalse(result.recursion);
    }

    /**
     * A current proxy above the nominal budget is not an error and gets no dedicated status: an
     * unreviewed root renders as plain NOT REVIEWED, a reviewed one only compares the proxy with
     * its own snapshot. Documents that the report never fails on the proxy alone.
     */
    @Test
    void proxyAboveNominalIsReportOnly() throws Exception {
        Map<String, StackUsageReport.Node> nodes = new TreeMap<>();
        StackUsageReport.Node root = new StackUsageReport.Node("root");
        root.function = "root";
        root.source = "root.cpp:1";
        root.stack = 1500;
        nodes.put("root", root);

        StackUsageReport.Root unreviewed = new StackUsageReport.Root("firmware", "worker", "root", 1024, null);
        StackUsageReport.Graph graph = new StackUsageReport.Graph("firmware", nodes,
            Collections.singletonList(unreviewed), Collections.<String, Integer>emptyMap());
        String report = StackUsageReport.render("test", Collections.singletonList(graph));
        assertTrue(report.contains("| firmware | worker | 1024 | - | - | - | 1500 | NOT REVIEWED: direct graph resolved |"));

        StackUsageReport.Root reviewed = new StackUsageReport.Root("firmware", "worker", "root", 1024,
            new StackUsageReport.ReviewedBaseline(600, 1500, "measured on bench"));
        graph = new StackUsageReport.Graph("firmware", nodes,
            Collections.singletonList(reviewed), Collections.<String, Integer>emptyMap());
        report = StackUsageReport.render("test", Collections.singletonList(graph));
        assertTrue(report.contains("| firmware | worker | 1024 | 600 | measured on bench | 1500 | 1500 | PROXY +0 |"));
        // and the reviewed-budget gate is silent as long as the hand-entered measurement fits
        StackUsageReport.checkReviewedBudgets(Collections.singletonList(graph));
    }

    /** Process and exception budgets come from the linker map and must resolve inside the reviewed-budget gate. */
    @Test
    void reviewedBudgetGateResolvesLinkerStackSizes() throws Exception {
        Path map = write("sizes.map",
            "  0x00000400 __process_stack_size__ = 0x400\n"
                + "  0x00001000 __main_stack_size__ = 0x1000\n");
        Map<String, Integer> sizes = StackUsageReport.parseMapStackSizes(map);

        StackUsageReport.Root exactFit = new StackUsageReport.Root("firmware", "main/process", "main", "process",
            new StackUsageReport.ReviewedBaseline(1024, 0, "boundary"));
        StackUsageReport.checkReviewedBudgets(Collections.singletonList(
            new StackUsageReport.Graph("firmware", Collections.<String, StackUsageReport.Node>emptyMap(),
                Collections.singletonList(exactFit), sizes)));

        StackUsageReport.Root overflow = new StackUsageReport.Root("firmware", "exception/ISR", null, "exception",
            new StackUsageReport.ReviewedBaseline(4097, 0, "nested interrupts"));
        IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.checkReviewedBudgets(Collections.singletonList(
                new StackUsageReport.Graph("firmware", Collections.<String, StackUsageReport.Node>emptyMap(),
                    Collections.singletonList(overflow), sizes))));
        assertTrue(error.getMessage().contains("firmware:exception/ISR uses 4097 bytes"));

        StackUsageReport.Root unlinked = new StackUsageReport.Root("firmware", "main/process", "main", "process",
            new StackUsageReport.ReviewedBaseline(1, 0, "no map"));
        assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.checkReviewedBudgets(Collections.singletonList(
                new StackUsageReport.Graph("firmware", Collections.<String, StackUsageReport.Node>emptyMap(),
                    Collections.singletonList(unlinked), Collections.<String, Integer>emptyMap()))));
    }

    /**
     * Whole pipeline minus the readelf/c++filt subprocesses: callgraph + ELF roots + map sizes +
     * profile + render. The thread's real deep path sits behind a virtual call, so the rendered
     * proxy is the entry frame alone while the hand-measured value is far larger.
     */
    @Test
    void endToEndReportHidesVirtualPathBehindIndirectMarker() throws Exception {
        Map<String, StackUsageReport.Node> nodes = parse(
            "node: { title: \"_ZL20storageManagerThreadPv\" label: \"storageManagerThread\\nstorage.cpp:290:13\\n16 bytes (static)\" }\n"
                + "node: { title: \"_ZN16SettingStorageSD5storeEjPKhj\" label: \"SettingStorageSD::store\\nstorage_sd.cpp:60:15\\n40 bytes (static)\" }\n"
                + "node: { title: \"f_open\" label: \"f_open\\nff.c:3799:9\\n352 bytes (static)\" }\n"
                + "node: { title: \"__indirect_call\" label: \"Indirect Call Placeholder\" shape : ellipse }\n"
                + "edge: { sourcename: \"_ZL20storageManagerThreadPv\" targetname: \"__indirect_call\" }\n"
                + "edge: { sourcename: \"_ZN16SettingStorageSD5storeEjPKhj\" targetname: \"f_open\" }\n");
        // c++filt output is consumed in sorted-symbol order: _ZL.. < _ZN.. < __indirect_call < f_open
        StackUsageReport.applyDemangledSymbols(nodes,
            "storageManagerThread(void*)\nSettingStorageSD::store(unsigned int, unsigned char const*, unsigned int)\n"
                + "Indirect Call Placeholder\nf_open\n");

        Path map = write("e2e.map",
            "Linker script and memory map\n"
                + "  0x00000600 __process_stack_size__ = 0x600\n"
                + "  0x00001000 __main_stack_size__ = 0x1000\n");
        String readelf = "   12: 00000001     1 OBJECT  LOCAL  DEFAULT   11 "
            + "_ZN11stack_usage12explicitRootIXadL_ZL20storageManagerThreadPvEELi1200EEE\n";
        List<StackUsageReport.Root> roots = StackUsageReport.parseElfRoots(map, "firmware", readelf,
            "stack_usage::explicitRoot<&(storageManagerThread(void*)), 1200>\n");
        StackUsageReport.Graph graph = new StackUsageReport.Graph("firmware", nodes, roots,
            StackUsageReport.parseMapStackSizes(map));
        StackUsageReport.applyProfile(Collections.singletonList(graph), StackUsageReport.parseProfile(
            "| Image | Entry | Name | Retained | Proxy | Scenario |\n"
                + "|---|---|---|---:|---:|---|\n"
                + "| firmware | exception/ISR | exception/ISR | - | - | - |\n"
                + "| firmware | storageManagerThread(void*) | storage manager | 1032 | 16 | SD extra-page burn |\n"));

        String report = StackUsageReport.render("e2e", Collections.singletonList(graph));
        assertTrue(report.contains("| firmware | storage manager | 1200 | 1032 | SD extra-page burn | 16 | 16 | "
            + "PROXY +0, PROXY BELOW REVIEWED; partial proxy: 1 indirect |"));
        assertTrue(report.contains("| firmware | exception/ISR | 4096 | - | - | - | - | NOT REVIEWED |"));
        assertTrue(report.contains("| firmware | 352 | f_open | ff.c:3799:9 |"));
        StackUsageReport.checkReviewedBudgets(Collections.singletonList(graph));
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
        assertTrue(report.startsWith("<!-- Generated by com.rusefi.tools.stack.StackUsageReport. "
            + "Do not edit. See java_tools/gcc_map_reader/README.md. -->\n\n"));
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
    void reviewedStackBudgetIsEnforced() {
        StackUsageReport.Root root = new StackUsageReport.Root("firmware", "test", "root", 100,
            new StackUsageReport.ReviewedBaseline(101, 0, "test scenario"));
        StackUsageReport.Graph graph = new StackUsageReport.Graph("firmware",
            Collections.<String, StackUsageReport.Node>emptyMap(), Collections.singletonList(root),
            Collections.<String, Integer>emptyMap());

        IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.checkReviewedBudgets(Collections.singletonList(graph)));
        assertTrue(error.getMessage().contains("firmware:test uses 101 bytes"));
    }

    @Test
    void rootAbsentFromCallgraphIsReported() {
        StackUsageReport.Root root = new StackUsageReport.Root("firmware", "inactive", "missing", 100, null);
        StackUsageReport.Graph graph = new StackUsageReport.Graph("firmware",
            Collections.<String, StackUsageReport.Node>emptyMap(), Collections.singletonList(root),
            Collections.<String, Integer>emptyMap());

        String report = StackUsageReport.render("test", Collections.singletonList(graph));

        assertTrue(report.contains("NOT ANALYZED: entry absent from post-LTO callgraph"));
    }

    @Test
    void linkedStackRootMetadataAndExternalProfile() throws Exception {
        Path map = write("roots.map",
            "Discarded input sections\n"
                + " .rusefi_stack_foreign_root.dead.100\n"
                + "Linker script and memory map\n"
                + ".rusefi_stack_foreign_root.idle.32\n"
                + "                0x00000000 0x1 test.o\n");
        String readelf =
            "   11: 00000002     1 OBJECT  GLOBAL DEFAULT   11 "
                + "_ZN11stack_usage14controllerRootI7CanReadXadL_ZNS1_10ThreadTaskEvEELi1200EEE\n"
                + "   12: 00000001     1 OBJECT  LOCAL  DEFAULT   11 "
                + "_ZN11stack_usage12explicitRootIXadL_ZL12MMCmonThreadPvEELi1200EEE\n"
                + "   13: 00000000     1 OBJECT  GLOBAL DEFAULT   11 "
                + "_ZN11stack_usage11processRootILi0EEE\n"
                + "String dump of section '.rusefi_stack_foreign_root.idle.32':\n"
                + "  [     0]  __idle_thread\n";
        String demangled =
            "stack_usage::processRoot<0>\n"
                + "stack_usage::explicitRoot<&(MMCmonThread(void*)), 1200>\n"
                + "stack_usage::controllerRoot<CanRead, &CanRead::ThreadTask, 1200>\n";

        List<StackUsageReport.Root> roots = StackUsageReport.parseElfRoots(map, "firmware", readelf, demangled);
        assertEquals(5, roots.size());
        List<StackUsageReport.ProfileRoot> profile = StackUsageReport.parseProfile(
            "| Image | Entry | Name | Retained | Proxy | Scenario |\n"
                + "|---|---|---|---:|---:|---|\n"
                + "| firmware | CanRead::ThreadTask | CAN RX | 440 | 1244 | CAN serial receive |\n"
                + "| firmware | exception/ISR | exception/ISR | - | - | - |\n"
                + "| firmware | __idle_thread | idle | - | - | - |\n"
                + "| firmware | main | main/process | - | - | - |\n"
                + "| firmware | MMCmonThread(void*) | SD/MMC | - | - | - |\n");
        StackUsageReport.Graph graph = new StackUsageReport.Graph(
            "firmware", Collections.<String, StackUsageReport.Node>emptyMap(), roots,
            Collections.<String, Integer>emptyMap());
        StackUsageReport.applyProfile(Collections.singletonList(graph), profile);
        roots = graph.roots;

        assertEquals(new StackUsageReport.Root("firmware", "CAN RX", "CanRead::ThreadTask", 1200,
            new StackUsageReport.ReviewedBaseline(440, 1244, "CAN serial receive")), roots.get(0));
        assertEquals(new StackUsageReport.Root("firmware", "exception/ISR", null, "exception", null), roots.get(1));
        assertEquals(new StackUsageReport.Root("firmware", "idle", "__idle_thread", 32, null), roots.get(2));
        assertEquals(new StackUsageReport.Root("firmware", "main/process", "main", "process", null), roots.get(3));
        assertEquals(new StackUsageReport.Root("firmware", "SD/MMC", "MMCmonThread(void*)", 1200, null), roots.get(4));
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
        IllegalArgumentException profileError = assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.applyProfile(Collections.singletonList(graph),
                profile.subList(0, profile.size() - 1)));
        assertTrue(profileError.getMessage().contains("missing=[firmware:MMCmonThread(void*)]"));
    }

    @Test
    void profileFileOverridesClasspathProfile() throws Exception {
        Path profile = write("private-stack-usage.md",
            "| Image | Entry | Name | Retained | Proxy | Scenario |\n"
                + "|---|---|---|---:|---:|---|\n"
                + "| firmware | main | private main | - | - | - |\n");

        List<StackUsageReport.ProfileRoot> roots = StackUsageReport.loadProfile("missing", profile);

        assertEquals(1, roots.size());
        assertEquals("private main", roots.get(0).name);
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
        assertEquals(symbol, StackUsageReport.findSymbol(nodes, "CanWrite::PeriodicTask"));
        StackUsageReport.Node overload = new StackUsageReport.Node("overload");
        overload.function = "CanWrite::PeriodicTask(int)";
        nodes.put(overload.symbol, overload);
        assertThrows(IllegalArgumentException.class,
            () -> StackUsageReport.findSymbol(nodes, "CanWrite::PeriodicTask"));
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
