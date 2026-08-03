package com.rusefi.tools.stack;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Summarizes GCC post-LTO callgraph stack usage for selected firmware profiles. */
public class StackUsageReport {
    private static final String MAP_MARKER = "Linker script and memory map";
    private static final Pattern NODE_RE = Pattern.compile("^node: \\{ title: \"([^\"]+)\" label: \"([^\"]+)\"");
    private static final Pattern EDGE_RE = Pattern.compile("^edge: \\{ sourcename: \"([^\"]+)\" targetname: \"([^\"]+)\"");
    private static final Pattern STACK_RE = Pattern.compile("\\\\n(\\d+) bytes \\(([^)]+)\\)");
    private static final Pattern CLONE_RE = Pattern.compile("(?: \\[clone [^]]+]|\\.(?:lto_priv|isra|constprop|part)\\.\\d+|\\.cold)+$");
    private static final Pattern MAP_STACK_RE = Pattern.compile(
        "^\\s+0x([0-9a-fA-F]+)\\s+__(process|main)_stack_size__\\s*=", Pattern.MULTILINE);
    private static final Pattern STACK_ROOT_RE = Pattern.compile(
        "^\\s*(\\.rusefi_stack_root\\.([A-Za-z0-9_]+)\\.(\\S+))\\s*$", Pattern.MULTILINE);
    private static final Pattern STACK_REVIEWED_ROOTS_RE = Pattern.compile(
        "^\\s*\\.rusefi_stack_reviewed_roots\\.([A-Za-z0-9_.]+)\\s*$", Pattern.MULTILINE);
    private static final Pattern STRING_DUMP_HEADER_RE = Pattern.compile("^String dump of section '([^']+)':$");
    private static final Pattern STRING_DUMP_VALUE_RE = Pattern.compile("^\\s*\\[\\s*[0-9a-fA-F]+]\\s+(.*)$");

    public static void main(String[] args) throws Exception {
        Options options = Options.parse(args);
        if (options.help) {
            System.out.println(Options.USAGE);
            return;
        }
        String report = generate(options);
        System.out.println(report);
        if (options.output != null) {
            Files.write(options.output, (report + "\n").getBytes(StandardCharsets.UTF_8));
        }
    }

    private static String generate(Options options) throws IOException, InterruptedException {
        Path firmwareMap = findOne(options.firmwareDir, "*.map");
        Path firmwareElf = findOne(options.firmwareDir, "*.elf");
        Path bootloaderMap = findOne(options.bootloaderDir, "*" + options.profile + "*.map");
        Path bootloaderElf = findOne(options.bootloaderDir, "*" + options.profile + "*.elf");

        List<Graph> graphs = new ArrayList<>();
        graphs.add(new Graph(
            "firmware",
            parseCallgraphs(find(options.firmwareDir, "*.ci")),
            parseMapRoots(firmwareMap, firmwareElf, "firmware", options.readelf),
            parseMapStackSizes(firmwareMap)));
        graphs.add(new Graph(
            "bootloader",
            parseCallgraphs(find(options.bootloaderDir, "*" + options.profile + "*.ci")),
            parseMapRoots(bootloaderMap, bootloaderElf, "bootloader", options.readelf),
            parseMapStackSizes(bootloaderMap)));

        for (Graph graph : graphs) {
            demangleSymbols(graph.nodes, options.cxxfilt);
        }
        return render(options.profile, graphs);
    }

    static Map<String, Node> parseCallgraphs(List<Path> paths) throws IOException {
        if (paths.isEmpty()) {
            throw new IllegalArgumentException("no GCC .ci files found");
        }

        Map<String, Node> nodes = new LinkedHashMap<>();
        List<String[]> edges = new ArrayList<>();
        for (Path path : paths) {
            for (String line : Files.readAllLines(path, StandardCharsets.UTF_8)) {
                Matcher nodeMatch = NODE_RE.matcher(line);
                Matcher edgeMatch = EDGE_RE.matcher(line);
                if (nodeMatch.find()) {
                    String symbol = nodeMatch.group(1);
                    String label = nodeMatch.group(2);
                    Node node = getNode(nodes, symbol);
                    String[] parts = label.split("\\\\n", -1);
                    node.label = parts[0];
                    node.source = parts.length > 1 ? parts[1] : "";
                    Matcher stackMatch = STACK_RE.matcher(label);
                    if (stackMatch.find()) {
                        node.stack = Integer.parseInt(stackMatch.group(1));
                        node.qualifier = stackMatch.group(2);
                    }
                } else if (edgeMatch.find()) {
                    edges.add(new String[] {edgeMatch.group(1), edgeMatch.group(2)});
                }
            }
        }

        for (String[] edge : edges) {
            Node caller = getNode(nodes, edge[0]);
            if ("__indirect_call".equals(edge[1])) {
                caller.indirectCalls++;
            } else {
                caller.callees.add(edge[1]);
                getNode(nodes, edge[1]);
            }
        }
        return nodes;
    }

    static Map<String, Integer> parseMapStackSizes(Path path) throws IOException {
        Matcher matcher = MAP_STACK_RE.matcher(read(path));
        Map<String, Integer> sizes = new HashMap<>();
        while (matcher.find()) {
            sizes.put("process".equals(matcher.group(2)) ? "process" : "exception",
                Integer.parseInt(matcher.group(1), 16));
        }
        if (sizes.size() != 2 || !sizes.containsKey("process") || !sizes.containsKey("exception")) {
            throw new IllegalArgumentException("expected process and exception stack sizes in " + path);
        }
        return sizes;
    }

    static Object parseBudget(String expression) {
        if ("process".equals(expression) || "exception".equals(expression)) {
            return expression;
        }
        try {
            BigInteger result = new BudgetParser(expression).parse();
            if (result.signum() <= 0) {
                throw new IllegalArgumentException("stack budget must be positive, got " + result);
            }
            return result;
        } catch (RuntimeException e) {
            throw new IllegalArgumentException("invalid stack budget expression '" + expression + "'", e);
        }
    }

    static String rootName(String identifier) {
        return identifier.replace("__", "\0").replace('_', ' ').replace("\0", "/");
    }

    static Map<String, List<String>> parseRootStringDump(String text) {
        String current = null;
        Map<String, List<String>> values = new TreeMap<>();
        for (String line : splitLines(text)) {
            Matcher header = STRING_DUMP_HEADER_RE.matcher(line);
            Matcher value = STRING_DUMP_VALUE_RE.matcher(line);
            if (header.matches()) {
                current = header.group(1);
                values.put(current, new ArrayList<String>());
            } else if (current != null && value.matches()) {
                values.get(current).add(value.group(1));
            }
        }
        return values;
    }

    static List<Root> parseMapRoots(Path path, String image, String readelfOutput) throws IOException {
        return parseMapRoots(path, image, parseRootSections(path), readelfOutput);
    }

    private static List<Root> parseMapRoots(Path path, Path elf, String image, String readelf)
        throws IOException, InterruptedException {
        Map<String, SectionRoot> sections = parseRootSections(path);
        List<String> command = new ArrayList<>();
        command.add(readelf);
        for (String section : sections.keySet()) {
            command.add("-p");
            command.add(section);
        }
        command.add(elf.toString());
        return parseMapRoots(path, image, sections, run(command, null));
    }

    private static Map<String, SectionRoot> parseRootSections(Path path) throws IOException {
        String text = read(path);
        int marker = text.indexOf(MAP_MARKER);
        if (marker < 0) {
            throw new IllegalArgumentException("missing linked memory map in " + path);
        }

        Map<String, SectionRoot> sections = new TreeMap<>();
        Matcher matcher = STACK_ROOT_RE.matcher(text.substring(marker + MAP_MARKER.length()));
        while (matcher.find()) {
            sections.put(matcher.group(1), new SectionRoot(matcher.group(2), matcher.group(3)));
        }
        if (sections.isEmpty()) {
            throw new IllegalArgumentException("no linked stack root metadata in " + path);
        }
        return sections;
    }

    private static List<Root> parseMapRoots(
        Path path, String image, Map<String, SectionRoot> sections, String readelfOutput) {
        Map<String, List<String>> metadata = parseRootStringDump(readelfOutput);
        if (!metadata.keySet().equals(sections.keySet())) {
            Set<String> missing = new TreeSet<>(sections.keySet());
            missing.removeAll(metadata.keySet());
            Set<String> extra = new TreeSet<>(metadata.keySet());
            extra.removeAll(sections.keySet());
            throw new IllegalArgumentException(
                "stack root ELF/map mismatch: missing=" + missing + ", extra=" + extra);
        }

        Map<String, Root> roots = new HashMap<>();
        for (Map.Entry<String, SectionRoot> entry : sections.entrySet()) {
            SectionRoot section = entry.getValue();
            List<String> values = metadata.get(entry.getKey());
            if (values.isEmpty() || values.size() > 2 || values.get(0).isEmpty()) {
                throw new IllegalArgumentException(
                    "stack root section '" + entry.getKey() + "' has invalid metadata " + values);
            }
            String name = rootName(section.identifier);
            ReviewedBaseline reviewed = values.size() == 2 ? parseReviewedBaseline(values.get(1)) : null;
            Root root = new Root(image, name, canonicalizeFunction(values.get(0)),
                parseBudget(section.budget), reviewed);
            Root previous = roots.put(name, root);
            if (previous != null && !previous.equals(root)) {
                throw new IllegalArgumentException("conflicting stack root '" + name + "' in " + path);
            }
        }
        Set<String> expectedReviewed = parseExpectedReviewedRoots(path);
        if (expectedReviewed == null && "firmware".equals(image)) {
            throw new IllegalArgumentException("missing reviewed stack root manifest in " + path);
        }
        if (expectedReviewed != null) {
            Set<String> actualReviewed = new TreeSet<>();
            for (Root root : roots.values()) {
                if (root.reviewed != null) {
                    actualReviewed.add(root.name);
                }
            }
            if (!actualReviewed.equals(expectedReviewed)) {
                Set<String> missing = new TreeSet<>(expectedReviewed);
                missing.removeAll(actualReviewed);
                Set<String> extra = new TreeSet<>(actualReviewed);
                extra.removeAll(expectedReviewed);
                throw new IllegalArgumentException(
                    "reviewed stack root mismatch in " + path + ": missing=" + missing + ", extra=" + extra);
            }
        }
        roots.put("exception/ISR", new Root(image, "exception/ISR", null, "exception", null));

        List<Root> result = new ArrayList<>(roots.values());
        result.sort(Comparator.comparing(root -> root.name, String.CASE_INSENSITIVE_ORDER));
        return result;
    }

    private static Set<String> parseExpectedReviewedRoots(Path path) {
        try {
            String text = read(path);
            int marker = text.indexOf(MAP_MARKER);
            Set<String> manifests = new TreeSet<>();
            Matcher matcher = STACK_REVIEWED_ROOTS_RE.matcher(text.substring(marker + MAP_MARKER.length()));
            while (matcher.find()) {
                manifests.add(matcher.group(1));
            }
            if (manifests.size() > 1) {
                throw new IllegalArgumentException(
                    "conflicting reviewed stack root manifests in " + path + ": " + manifests);
            }
            if (manifests.isEmpty()) {
                return null;
            }

            Set<String> roots = new TreeSet<>();
            String manifest = manifests.iterator().next();
            if (!"none".equals(manifest)) {
                for (String identifier : manifest.split("\\.")) {
                    String name = rootName(identifier);
                    if (!roots.add(name)) {
                        throw new IllegalArgumentException("duplicate reviewed stack root '" + name + "' in " + path);
                    }
                }
            }
            return roots;
        } catch (IOException e) {
            throw new IllegalArgumentException("failed to read " + path, e);
        }
    }

    static String canonicalizeFunction(String function) {
        return CLONE_RE.matcher(function).replaceAll("");
    }

    static ReviewedBaseline parseReviewedBaseline(String value) {
        if (value.isEmpty()) {
            return null;
        }
        String[] parts = value.split("\\|", 3);
        if (parts.length != 3 || parts[2].isEmpty()) {
            throw new IllegalArgumentException("invalid reviewed stack metadata '" + value + "'");
        }
        try {
            int retained = Integer.parseInt(parts[0]);
            int directSnapshot = Integer.parseInt(parts[1]);
            if (retained <= 0 || directSnapshot < 0) {
                throw new IllegalArgumentException(
                    "reviewed retained stack must be positive and proxy snapshot must be non-negative");
            }
            return new ReviewedBaseline(retained, directSnapshot, parts[2]);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("invalid reviewed stack metadata '" + value + "'", e);
        }
    }

    private static void demangleSymbols(Map<String, Node> nodes, String cxxfilt)
        throws IOException, InterruptedException {
        List<Node> ordered = new ArrayList<>(nodes.values());
        ordered.sort(Comparator.comparing(node -> node.symbol));
        StringBuilder input = new StringBuilder();
        for (Node node : ordered) {
            int colon = node.symbol.lastIndexOf(':');
            String name = node.symbol.substring(colon + 1).replaceFirst("^\\*+", "");
            input.append(name).append('\n');
        }
        applyDemangledSymbols(nodes, run(Collections.singletonList(cxxfilt), input.toString()));
    }

    static void applyDemangledSymbols(Map<String, Node> nodes, String output) {
        List<Node> ordered = new ArrayList<>(nodes.values());
        ordered.sort(Comparator.comparing(node -> node.symbol));
        List<String> functions = splitLines(output);
        if (functions.size() != ordered.size()) {
            throw new IllegalArgumentException(
                "c++filt returned " + functions.size() + " names for " + ordered.size() + " symbols");
        }
        for (int i = 0; i < ordered.size(); i++) {
            ordered.get(i).function = canonicalizeFunction(functions.get(i));
        }
    }

    static String findSymbol(Map<String, Node> nodes, String function) {
        List<String> matches = new ArrayList<>();
        for (Node node : nodes.values()) {
            if (function.equals(node.function)) {
                matches.add(node.symbol);
            }
        }
        Collections.sort(matches);
        if (matches.size() != 1) {
            throw new IllegalArgumentException("function '" + function + "' matched " + matches.size() + " symbols");
        }
        return matches.get(0);
    }

    static List<Set<String>> findComponents(Map<String, Node> nodes) {
        Tarjan tarjan = new Tarjan(nodes);
        List<String> symbols = new ArrayList<>(nodes.keySet());
        Collections.sort(symbols);
        for (String symbol : symbols) {
            if (!tarjan.indices.containsKey(symbol)) {
                tarjan.connect(symbol);
            }
        }
        return tarjan.components;
    }

    static Analysis analyze(Map<String, Node> nodes, String root) {
        return new Analyzer(nodes, findComponents(nodes)).analyze(root);
    }

    private static Analysis analyzeRoot(Map<String, Node> nodes, Root root, List<Set<String>> components) {
        if (root.function == null) {
            throw new IllegalArgumentException("root '" + root.name + "' has no entry function");
        }
        return new Analyzer(nodes, components).analyze(findSymbol(nodes, root.function));
    }

    private static BigInteger budgetFor(Root root, Map<String, Integer> stackSizes) {
        if (root.budget instanceof BigInteger) {
            return (BigInteger) root.budget;
        }
        Integer result = stackSizes.get(root.budget);
        if (result == null) {
            throw new IllegalArgumentException("missing linked stack size " + root.budget);
        }
        return BigInteger.valueOf(result);
    }

    private static String detail(Analysis result) {
        List<String> issues = new ArrayList<>();
        if (!result.unknown.isEmpty()) {
            issues.add(result.unknown.size() + " unknown");
        }
        if (!result.indirect.isEmpty()) {
            issues.add(result.indirect.size() + " indirect");
        }
        if (!result.dynamic.isEmpty()) {
            issues.add(result.dynamic.size() + " dynamic");
        }
        if (result.recursion) {
            issues.add("recursion");
        }
        return issues.isEmpty() ? "direct graph resolved" : String.join(", ", issues);
    }

    static String render(String profileName, List<Graph> graphs) {
        List<String> rows = new ArrayList<>();
        List<String> frameRows = new ArrayList<>();

        for (Graph graph : graphs) {
            List<Set<String>> components = findComponents(graph.nodes);
            for (Root root : graph.roots) {
                if (!root.image.equals(graph.image)) {
                    throw new IllegalArgumentException(
                        "root '" + root.name + "' belongs to " + root.image + ", not " + graph.image);
                }
                BigInteger budget = budgetFor(root, graph.stackSizes);
                if (root.function == null) {
                    rows.add("| " + graph.image + " | " + root.name + " | " + budget + " | - | - | - | - | NOT REVIEWED |");
                    continue;
                }

                Analysis result = analyzeRoot(graph.nodes, root, components);
                ReviewedBaseline baseline = root.reviewed;
                if (baseline == null) {
                    rows.add("| " + graph.image + " | " + root.name + " | " + budget
                        + " | - | - | - | " + result.stack + " | NOT REVIEWED: " + detail(result) + " |");
                } else {
                    int delta = result.stack - baseline.directSnapshot;
                    String status = delta > 0 ? "REVIEW PROXY +" + delta : "PROXY " + signed(delta);
                    if (result.stack < baseline.retained) {
                        status += ", PROXY BELOW REVIEWED";
                    }
                    BigInteger retained = BigInteger.valueOf(baseline.retained);
                    if (retained.compareTo(budget) > 0) {
                        status += ", SCENARIO OVER NOMINAL +" + retained.subtract(budget);
                    }
                    String proxyDetail = detail(result);
                    if (!"direct graph resolved".equals(proxyDetail)) {
                        status += "; partial proxy: " + proxyDetail;
                    }
                    rows.add("| " + graph.image + " | " + root.name + " | " + budget + " | "
                        + baseline.retained + " | " + markdown(baseline.scenario) + " | "
                        + baseline.directSnapshot + " | " + result.stack + " | " + status + " |");
                }
            }

            List<Node> largest = new ArrayList<>();
            for (Node node : graph.nodes.values()) {
                if (node.stack != null) {
                    largest.add(node);
                }
            }
            largest.sort(Comparator
                .comparing((Node node) -> node.stack, Comparator.reverseOrder())
                .thenComparing(node -> node.function.isEmpty() ? node.label : node.function));
            for (Node node : largest.subList(0, Math.min(10, largest.size()))) {
                frameRows.add("| " + graph.image + " | " + node.stack + " | "
                    + markdown(node.function.isEmpty() ? node.label : node.function) + " | "
                    + markdown(node.source.isEmpty() ? "-" : node.source) + " |");
            }
        }

        List<String> report = new ArrayList<>();
        report.add("# Stack Usage Overview: " + profileName);
        report.add("");
        report.add("Reviewed bytes are one manually traced realistic scenario, not a root-wide maximum. "
            + "The direct proxy is intentionally biased and can miss indirect or non-dominant growth; "
            + "it is compared only with its snapshot, never added to the reviewed value. Interrupts and "
            + "context switching are not included. Statuses remain report-only.");
        report.add("");
        report.add("## Configured Stacks");
        report.add("");
        report.add("| Image | Stack / entry | Nominal | Reviewed | Scenario | Proxy snapshot | Current proxy | Result |");
        report.add("|---|---|---:|---:|---|---:|---:|---|");
        report.addAll(rows);
        report.add("");
        report.add("## Largest Post-LTO Frames");
        report.add("");
        report.add("| Image | Frame bytes | Function | Source |");
        report.add("|---|---:|---|---|");
        report.addAll(frameRows);
        return String.join("\n", report);
    }

    private static String markdown(Object value) {
        return String.valueOf(value).replace("|", "\\|").replace('\n', ' ');
    }

    private static String signed(int value) {
        return String.format(Locale.ROOT, "%+d", value);
    }

    private static Node getNode(Map<String, Node> nodes, String symbol) {
        Node node = nodes.get(symbol);
        if (node == null) {
            node = new Node(symbol);
            nodes.put(symbol, node);
        }
        return node;
    }

    private static String read(Path path) throws IOException {
        return new String(Files.readAllBytes(path), StandardCharsets.UTF_8);
    }

    private static List<String> splitLines(String text) {
        List<String> lines = new ArrayList<>();
        Collections.addAll(lines, text.split("\\r?\\n", -1));
        if (!lines.isEmpty() && lines.get(lines.size() - 1).isEmpty()) {
            lines.remove(lines.size() - 1);
        }
        return lines;
    }

    private static List<Path> find(Path directory, String glob) throws IOException {
        List<Path> matches = new ArrayList<>();
        try (DirectoryStream<Path> stream = Files.newDirectoryStream(directory, glob)) {
            for (Path path : stream) {
                matches.add(path);
            }
        }
        Collections.sort(matches);
        return matches;
    }

    private static Path findOne(Path directory, String glob) throws IOException {
        List<Path> matches = find(directory, glob);
        if (matches.size() != 1) {
            throw new IllegalArgumentException(
                "expected one " + glob + " in " + directory + ", found " + matches.size());
        }
        return matches.get(0);
    }

    static String run(List<String> command, String input) throws IOException, InterruptedException {
        Process process = new ProcessBuilder(command).start();
        ExecutorService readers = Executors.newFixedThreadPool(2);
        Future<String> output = readers.submit(() -> readStream(process.getInputStream()));
        Future<String> error = readers.submit(() -> readStream(process.getErrorStream()));
        try {
            try (Writer writer = new OutputStreamWriter(process.getOutputStream(), StandardCharsets.UTF_8)) {
                if (input != null) {
                    writer.write(input);
                }
            }
            int status = process.waitFor();
            String stdout = output.get();
            String stderr = error.get();
            if (status != 0) {
                throw new IOException(command.get(0) + " exited " + status + ": " + stderr);
            }
            return stdout;
        } catch (ExecutionException e) {
            throw new IOException("failed to read output from " + command.get(0), e.getCause());
        } finally {
            readers.shutdownNow();
        }
    }

    private static String readStream(InputStream stream) throws IOException {
        StringBuilder result = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(stream, StandardCharsets.UTF_8))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (result.length() > 0) {
                    result.append('\n');
                }
                result.append(line);
            }
        }
        return result.toString();
    }

    static class Node {
        final String symbol;
        String label = "";
        String source = "";
        String function = "";
        Integer stack;
        String qualifier = "";
        final Set<String> callees = new TreeSet<>();
        int indirectCalls;

        Node(String symbol) {
            this.symbol = symbol;
        }
    }

    static class Root {
        final String image;
        final String name;
        final String function;
        final Object budget;
        final ReviewedBaseline reviewed;

        Root(String image, String name, String function, Object budget, ReviewedBaseline reviewed) {
            this.image = image;
            this.name = name;
            this.function = function;
            this.budget = budget instanceof BigInteger
                ? budget
                : budget instanceof Number ? BigInteger.valueOf(((Number) budget).longValue()) : budget;
            this.reviewed = reviewed;
        }

        @Override
        public boolean equals(Object object) {
            if (!(object instanceof Root)) {
                return false;
            }
            Root other = (Root) object;
            return image.equals(other.image) && name.equals(other.name)
                && equal(function, other.function) && budget.equals(other.budget)
                && equal(reviewed, other.reviewed);
        }

        @Override
        public int hashCode() {
            int result = image.hashCode();
            result = 31 * result + name.hashCode();
            result = 31 * result + (function == null ? 0 : function.hashCode());
            result = 31 * result + budget.hashCode();
            return 31 * result + (reviewed == null ? 0 : reviewed.hashCode());
        }
    }

    static class Analysis {
        final int stack;
        final Set<String> unknown;
        final Set<String> dynamic;
        final Set<String> indirect;
        final boolean recursion;

        Analysis(int stack, Set<String> unknown, Set<String> dynamic, Set<String> indirect, boolean recursion) {
            this.stack = stack;
            this.unknown = unknown;
            this.dynamic = dynamic;
            this.indirect = indirect;
            this.recursion = recursion;
        }
    }

    static class ReviewedBaseline {
        final int retained;
        final int directSnapshot;
        final String scenario;

        ReviewedBaseline(int retained, int directSnapshot, String scenario) {
            this.retained = retained;
            this.directSnapshot = directSnapshot;
            this.scenario = scenario;
        }

        @Override
        public boolean equals(Object object) {
            if (!(object instanceof ReviewedBaseline)) {
                return false;
            }
            ReviewedBaseline other = (ReviewedBaseline) object;
            return retained == other.retained && directSnapshot == other.directSnapshot
                && scenario.equals(other.scenario);
        }

        @Override
        public int hashCode() {
            int result = retained;
            result = 31 * result + directSnapshot;
            return 31 * result + scenario.hashCode();
        }
    }

    static class Graph {
        final String image;
        final Map<String, Node> nodes;
        final List<Root> roots;
        final Map<String, Integer> stackSizes;

        Graph(String image, Map<String, Node> nodes, List<Root> roots, Map<String, Integer> stackSizes) {
            this.image = image;
            this.nodes = nodes;
            this.roots = roots;
            this.stackSizes = stackSizes;
        }
    }

    private static class SectionRoot {
        final String identifier;
        final String budget;

        SectionRoot(String identifier, String budget) {
            this.identifier = identifier;
            this.budget = budget;
        }
    }

    private static class Tarjan {
        final Map<String, Node> nodes;
        final Map<String, Integer> indices = new HashMap<>();
        final Map<String, Integer> lowlinks = new HashMap<>();
        final Deque<String> stack = new ArrayDeque<>();
        final Set<String> onStack = new HashSet<>();
        final List<Set<String>> components = new ArrayList<>();
        int nextIndex;

        Tarjan(Map<String, Node> nodes) {
            this.nodes = nodes;
        }

        void connect(String symbol) {
            indices.put(symbol, nextIndex);
            lowlinks.put(symbol, nextIndex);
            nextIndex++;
            stack.push(symbol);
            onStack.add(symbol);

            for (String callee : nodes.get(symbol).callees) {
                if (!indices.containsKey(callee)) {
                    connect(callee);
                    lowlinks.put(symbol, Math.min(lowlinks.get(symbol), lowlinks.get(callee)));
                } else if (onStack.contains(callee)) {
                    lowlinks.put(symbol, Math.min(lowlinks.get(symbol), indices.get(callee)));
                }
            }

            if (!lowlinks.get(symbol).equals(indices.get(symbol))) {
                return;
            }
            Set<String> component = new LinkedHashSet<>();
            while (true) {
                String member = stack.pop();
                onStack.remove(member);
                component.add(member);
                if (member.equals(symbol)) {
                    break;
                }
            }
            components.add(component);
        }
    }

    private static class Analyzer {
        final Map<String, Node> nodes;
        final List<Set<String>> components;
        final Map<String, Integer> componentFor = new HashMap<>();
        final Map<Integer, Analysis> cache = new HashMap<>();

        Analyzer(Map<String, Node> nodes, List<Set<String>> components) {
            this.nodes = nodes;
            this.components = components;
            for (int i = 0; i < components.size(); i++) {
                for (String symbol : components.get(i)) {
                    componentFor.put(symbol, i);
                }
            }
        }

        Analysis analyze(String root) {
            return visit(componentFor.get(root));
        }

        Analysis visit(int index) {
            Analysis cached = cache.get(index);
            if (cached != null) {
                return cached;
            }

            Set<String> component = components.get(index);
            int stack = 0;
            Set<String> unknown = new HashSet<>();
            Set<String> dynamic = new HashSet<>();
            Set<String> indirect = new HashSet<>();
            Set<Integer> childIndices = new TreeSet<>();
            boolean recursion = component.size() > 1;
            for (String symbol : component) {
                Node node = nodes.get(symbol);
                if (node.stack == null) {
                    unknown.add(symbol);
                } else {
                    stack += node.stack;
                }
                if (node.qualifier.contains("dynamic") && !node.qualifier.contains("bounded")) {
                    dynamic.add(symbol);
                }
                if (node.indirectCalls > 0) {
                    indirect.add(symbol);
                }
                if (node.callees.contains(symbol)) {
                    recursion = true;
                }
                for (String child : node.callees) {
                    int childIndex = componentFor.get(child);
                    if (childIndex != index) {
                        childIndices.add(childIndex);
                    }
                }
            }

            int largestChild = 0;
            for (int childIndex : childIndices) {
                Analysis child = visit(childIndex);
                largestChild = Math.max(largestChild, child.stack);
                unknown.addAll(child.unknown);
                dynamic.addAll(child.dynamic);
                indirect.addAll(child.indirect);
                recursion |= child.recursion;
            }
            Analysis result = new Analysis(stack + largestChild, unknown, dynamic, indirect, recursion);
            cache.put(index, result);
            return result;
        }
    }

    private static class BudgetParser {
        static final Pattern DECIMAL = Pattern.compile("(?:0|[1-9](?:_?[0-9])*)");
        static final Pattern HEXADECIMAL = Pattern.compile("0[xX]_?[0-9a-fA-F](?:_?[0-9a-fA-F])*");
        static final Pattern OCTAL = Pattern.compile("0[oO]_?[0-7](?:_?[0-7])*");
        static final Pattern BINARY = Pattern.compile("0[bB]_?[01](?:_?[01])*");

        final String input;
        int position;

        BudgetParser(String input) {
            this.input = input;
        }

        BigInteger parse() {
            BigInteger result = shift();
            whitespace();
            if (position != input.length()) {
                throw new IllegalArgumentException("unsupported expression");
            }
            return result;
        }

        BigInteger shift() {
            BigInteger result = add();
            while (true) {
                if (consume("<<")) {
                    result = result.shiftLeft(shiftCount(add()));
                } else if (consume(">>")) {
                    result = result.shiftRight(shiftCount(add()));
                } else {
                    return result;
                }
            }
        }

        BigInteger add() {
            BigInteger result = multiply();
            while (true) {
                if (consume("+")) {
                    result = result.add(multiply());
                } else if (consume("-")) {
                    result = result.subtract(multiply());
                } else {
                    return result;
                }
            }
        }

        BigInteger multiply() {
            BigInteger result = unary();
            while (true) {
                if (consume("*")) {
                    result = result.multiply(unary());
                } else if (consume("//")) {
                    result = floorDivide(result, unary());
                } else {
                    return result;
                }
            }
        }

        BigInteger unary() {
            if (consume("+")) {
                return unary();
            }
            if (consume("-")) {
                return unary().negate();
            }
            if (consume("(")) {
                BigInteger result = shift();
                if (!consume(")")) {
                    throw new IllegalArgumentException("missing parenthesis");
                }
                return result;
            }
            whitespace();
            int start = position;
            while (position < input.length()) {
                char character = input.charAt(position);
                if (!(Character.isLetterOrDigit(character) || character == '_')) {
                    break;
                }
                position++;
            }
            if (start == position) {
                throw new IllegalArgumentException("expected integer");
            }
            String literal = input.substring(start, position);
            int radix = 10;
            if (HEXADECIMAL.matcher(literal).matches()) {
                radix = 16;
            } else if (OCTAL.matcher(literal).matches()) {
                radix = 8;
            } else if (BINARY.matcher(literal).matches()) {
                radix = 2;
            } else if (!DECIMAL.matcher(literal).matches()) {
                throw new IllegalArgumentException("invalid integer literal");
            }
            String number = literal.replace("_", "");
            if (radix != 10) {
                number = number.substring(2);
            }
            return new BigInteger(number, radix);
        }

        int shiftCount(BigInteger value) {
            if (value.signum() < 0) {
                throw new ArithmeticException("negative shift count");
            }
            return value.intValueExact();
        }

        BigInteger floorDivide(BigInteger left, BigInteger right) {
            BigInteger[] result = left.divideAndRemainder(right);
            if (result[1].signum() != 0 && left.signum() != right.signum()) {
                return result[0].subtract(BigInteger.ONE);
            }
            return result[0];
        }

        boolean consume(String token) {
            whitespace();
            if (!input.startsWith(token, position)) {
                return false;
            }
            position += token.length();
            return true;
        }

        void whitespace() {
            while (position < input.length() && Character.isWhitespace(input.charAt(position))) {
                position++;
            }
        }
    }

    private static class Options {
        static final String USAGE = "Usage: StackUsageReport --profile <name> "
            + "[--firmware-dir <path>] [--bootloader-dir <path>] [--cxxfilt <command>] "
            + "[--readelf <command>] [--output <path>]";
        String profile;
        Path firmwareDir = Paths.get("build");
        Path bootloaderDir = Paths.get("bootloader/blbuild");
        String cxxfilt = "arm-none-eabi-c++filt";
        String readelf = "arm-none-eabi-readelf";
        Path output;
        boolean help;

        static Options parse(String[] args) {
            Options options = new Options();
            for (int i = 0; i < args.length; i++) {
                String option = args[i];
                if ("--help".equals(option) || "-h".equals(option)) {
                    options.help = true;
                    continue;
                }
                String value;
                int equals = option.indexOf('=');
                if (equals >= 0) {
                    value = option.substring(equals + 1);
                    option = option.substring(0, equals);
                } else {
                    if (++i >= args.length) {
                        throw new IllegalArgumentException("value expected after " + option);
                    }
                    value = args[i];
                }
                switch (option) {
                    case "--profile":
                        options.profile = value;
                        break;
                    case "--firmware-dir":
                        options.firmwareDir = Paths.get(value);
                        break;
                    case "--bootloader-dir":
                        options.bootloaderDir = Paths.get(value);
                        break;
                    case "--cxxfilt":
                        options.cxxfilt = value;
                        break;
                    case "--readelf":
                        options.readelf = value;
                        break;
                    case "--output":
                        options.output = Paths.get(value);
                        break;
                    default:
                        throw new IllegalArgumentException("unknown option " + option);
                }
            }
            if (!options.help && options.profile == null) {
                throw new IllegalArgumentException("--profile is required");
            }
            return options;
        }
    }

    private static boolean equal(Object left, Object right) {
        return left == null ? right == null : left.equals(right);
    }
}
