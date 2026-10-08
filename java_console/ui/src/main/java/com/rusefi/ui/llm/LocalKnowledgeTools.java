package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.*;
import java.net.URI;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.util.*;
import java.util.concurrent.TimeUnit;

import static com.rusefi.ui.llm.ChatGptClient.object;

/** Bounded, read-only text retrieval from the prepared source cache. No network or shell access. */
@SuppressWarnings("unchecked")
final class LocalKnowledgeTools {
    static final int MAX_FILE_BYTES = 2 * 1024 * 1024;
    static final int MAX_SCAN_BYTES = 64 * 1024 * 1024;
    static final int MAX_FILES = 4000;
    static final int MAX_VISITS = 12000;
    static final int MAX_RESULT_CHARS = 48000;
    private static final Set<String> ROOTS = new HashSet<>(Arrays.asList("firmware", "rusefi_documentation", "docs"));
    private static final Set<String> EXTENSIONS = new HashSet<>(Arrays.asList(
            "md", "txt", "c", "cc", "cpp", "cxx", "h", "hpp", "hxx", "inc", "ini", "lua", "yaml", "yml", "s", "mk", "cfg"));
    private final Path root;

    LocalKnowledgeTools(Path root) {
        this.root = Objects.requireNonNull(root, "root").toAbsolutePath().normalize();
    }

    static boolean handles(String name) {
        return "search_knowledge".equals(name) || "read_knowledge".equals(name);
    }

    static JSONArray definitions() {
        JSONArray definitions = new JSONArray();
        definitions.add(definition("search_knowledge",
                "Search cached rusEFI firmware/wiki text. Literal mode finds a substring (use for exact symbols); keywords mode requires every whitespace-separated term on the same line. Case-insensitive. Results include path:line citations, SHA-256 and unverified revision metadata. Narrow path_prefix if truncated.",
                object("query", object("type", "string"), "mode", object("type", "string", "enum", array("literal", "keywords")),
                        "path_prefix", object("type", "string", "description", "Optional relative directory or file under firmware/, rusefi_documentation/ or docs/."),
                        "max_results", object("type", "integer", "minimum", 1, "maximum", 20)), array("query")));
        definitions.add(definition("read_knowledge",
                "Read a bounded line range from a cached text file returned by search_knowledge. Preserves Markdown/image references. Cite path and line numbers; upstream_url is current upstream, not a verified archive revision.",
                object("path", object("type", "string"), "start_line", object("type", "integer", "minimum", 1, "maximum", 1000000),
                        "max_lines", object("type", "integer", "minimum", 1, "maximum", 120)), array("path")));
        return definitions;
    }

    JSONObject execute(String name, JSONObject args, Runnable cancellation) {
        cancellation.run();
        try {
            if ("search_knowledge".equals(name)) {
                checkKeys(args, "query", "mode", "path_prefix", "max_results");
                String query = string(args, "query", null, 256).trim();
                if (query.isEmpty()) { throw new IllegalArgumentException("query must not be blank."); }
                String mode = string(args, "mode", "literal", 16);
                if (!"literal".equals(mode) && !"keywords".equals(mode)) {
                    throw new IllegalArgumentException("mode must be literal or keywords.");
                }
                return search(query, mode, string(args, "path_prefix", "", 512),
                        integer(args, "max_results", 10, 20), new Budget(cancellation));
            }
            if ("read_knowledge".equals(name)) {
                checkKeys(args, "path", "start_line", "max_lines");
                return read(string(args, "path", null, 512), integer(args, "start_line", 1, 1000000),
                        integer(args, "max_lines", 80, 120), new Budget(cancellation));
            }
            return error("Unknown knowledge tool.");
        } catch (IllegalArgumentException e) {
            return error(e.getMessage());
        } catch (IOException e) {
            // Never expose host paths or contents from a rejected file in an error message.
            return error("Knowledge text is unavailable, unsafe, not UTF-8, or exceeds the read limits. Check the prepared source cache.");
        }
    }

    private JSONObject search(String query, String mode, String prefix, int limit, Budget budget) throws IOException {
        checkRoot();
        List<Path> files = new ArrayList<>();
        List<Path> starts = new ArrayList<>();
        if (prefix.isEmpty()) {
            // Wiki first for general questions; callers can target firmware for symbol searches.
            for (String folder : Arrays.asList("rusefi_documentation", "firmware", "docs")) {
                Path start = root.resolve(folder);
                if (Files.exists(start, LinkOption.NOFOLLOW_LINKS)) { starts.add(start); }
            }
        } else {
            starts.add(resolve(prefix));
        }
        for (Path start : starts) {
            checkSafe(start);
            Files.walkFileTree(start, EnumSet.noneOf(FileVisitOption.class), 32, new SimpleFileVisitor<Path>() {
                @Override public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) {
                    if (!budget.visit()) { return FileVisitResult.TERMINATE; }
                    return hidden(root.relativize(dir)) ? FileVisitResult.SKIP_SUBTREE : FileVisitResult.CONTINUE;
                }
                @Override public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) {
                    if (!budget.visit()) { return FileVisitResult.TERMINATE; }
                    if (attrs.isDirectory()) { budget.skipped++; } // Directory at the traversal depth limit.
                    if (attrs.isRegularFile() && allowed(file)) {
                        if (attrs.size() <= MAX_FILE_BYTES) {
                            files.add(file);
                        } else {
                            budget.skipped++;
                        }
                    }
                    return FileVisitResult.CONTINUE;
                }
                @Override public FileVisitResult visitFileFailed(Path file, IOException error) {
                    budget.skipped++;
                    return budget.visit() ? FileVisitResult.CONTINUE : FileVisitResult.TERMINATE;
                }
            });
            if (budget.limited) { break; }
        }
        files.sort(Comparator.comparing((Path path) -> relative(path).startsWith("rusefi_documentation/") ? 0 : 1)
                .thenComparing(this::relative));
        JSONArray matches = new JSONArray();
        JSONObject result = result();
        result.put("matches", matches);
        List<String> terms = "keywords".equals(mode) ? Arrays.asList(query.toLowerCase(Locale.ROOT).split("\\s+"))
                : Collections.singletonList(query.toLowerCase(Locale.ROOT));
        int scanned = 0;
        searchFiles:
        for (Path file : files) {
            if (!budget.check() || scanned >= MAX_FILES) {
                budget.limited = true;
                break;
            }
            scanned++;
            Text text;
            try {
                if (budget.bytes + Files.size(file) > MAX_SCAN_BYTES) {
                    budget.limited = true;
                    break;
                }
                text = load(file, budget);
            } catch (IOException unsafeOrNonText) {
                budget.skipped++;
                continue;
            }
            try (BufferedReader lines = new BufferedReader(new StringReader(text.content))) {
                String line;
                int number = 0;
                while ((line = lines.readLine()) != null) {
                    if (!budget.check()) { break searchFiles; }
                    number++;
                    String lower = line.toLowerCase(Locale.ROOT);
                    boolean found = true;
                    for (String term : terms) {
                        if (!lower.contains(term)) { found = false; break; }
                    }
                    if (!found) { continue; }
                    int offset = Math.min(line.length(), Math.max(0, lower.indexOf(terms.get(0)) - 100));
                    JSONObject match = metadata(file, text);
                    match.put("line", number);
                    match.put("citation", relative(file) + ":L" + number);
                    match.put("text", line.substring(offset, Math.min(line.length(), offset + 400)));
                    match.put("line_truncated", offset > 0 || line.length() > 400);
                    matches.add(match);
                    if (result.toJSONString().length() > MAX_RESULT_CHARS) {
                        matches.remove(matches.size() - 1);
                        budget.limited = true;
                        break searchFiles;
                    }
                    if (matches.size() >= limit) {
                        budget.limited = true;
                        break searchFiles;
                    }
                }
            }
        }
        result.put("files_scanned", scanned);
        result.put("files_skipped", budget.skipped);
        result.put("truncated", budget.limited || budget.skipped > 0);
        if (budget.limited || budget.skipped > 0) {
            result.put("note", "Results are partial or reached a limit. Narrow query/path_prefix; oversized or non-text files are skipped.");
        }
        return result;
    }

    private JSONObject read(String path, int start, int count, Budget budget) throws IOException {
        Path file = resolve(path);
        Text text = load(file, budget);
        JSONObject result = result();
        result.putAll(metadata(file, text));
        JSONArray passages = new JSONArray();
        result.put("lines", passages);
        int number = 0;
        boolean truncated = false;
        int chars = 0;
        try (BufferedReader lines = new BufferedReader(new StringReader(text.content))) {
            String line;
            while ((line = lines.readLine()) != null) {
                number++;
                if (!budget.check()) { truncated = true; break; }
                if (number < start) { continue; }
                if (passages.size() >= count || chars >= 6000) { truncated = true; break; }
                int length = Math.min(line.length(), Math.min(2000, 6000 - chars));
                passages.add(object("line", number, "text", line.substring(0, length), "line_truncated", length < line.length()));
                chars += length;
                truncated |= length < line.length();
            }
        }
        result.put("truncated", truncated);
        if (!passages.isEmpty()) {
            int end = ((Number) ((JSONObject) passages.get(passages.size() - 1)).get("line")).intValue();
            result.put("citation", relative(file) + ":L" + start + "-L" + end);
            if (number > end) { result.put("next_line", end + 1); }
        }
        return result;
    }

    private Text load(Path file, Budget budget) throws IOException {
        checkSafe(file);
        if (!allowed(file) || !Files.isRegularFile(file, LinkOption.NOFOLLOW_LINKS) || Files.size(file) > MAX_FILE_BYTES) {
            throw new IOException("Not an allowed bounded text file");
        }
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (InputStream input = Channels.newInputStream(Files.newByteChannel(file, StandardOpenOption.READ, LinkOption.NOFOLLOW_LINKS))) {
            byte[] buffer = new byte[8192];
            int count;
            while ((count = input.read(buffer)) != -1) {
                budget.bytes += count;
                if (!budget.check() || bytes.size() + count > MAX_FILE_BYTES || budget.bytes > MAX_SCAN_BYTES) {
                    budget.limited = true;
                    throw new IOException("Read limit reached");
                }
                bytes.write(buffer, 0, count);
            }
        }
        checkSafe(file);
        byte[] contents = bytes.toByteArray();
        String text = StandardCharsets.UTF_8.newDecoder().decode(ByteBuffer.wrap(contents)).toString();
        if (text.indexOf('\0') >= 0) { throw new IOException("Binary content"); }
        try {
            byte[] hash = MessageDigest.getInstance("SHA-256").digest(contents);
            StringBuilder hex = new StringBuilder();
            for (byte value : hash) { hex.append(String.format(Locale.ROOT, "%02x", value & 255)); }
            return new Text(text, hex.toString());
        } catch (java.security.NoSuchAlgorithmException e) {
            throw new AssertionError(e);
        }
    }

    private Path resolve(String path) throws IOException {
        if (path.isEmpty() || path.length() > 512 || path.contains("\\") || path.contains(":")) {
            throw new IllegalArgumentException("Use a relative forward-slash path within the knowledge folders.");
        }
        Path relative;
        try { relative = Paths.get(path); } catch (InvalidPathException e) { throw new IOException("Invalid path"); }
        if (relative.isAbsolute() || hidden(relative) || !ROOTS.contains(relative.getName(0).toString())) {
            throw new IllegalArgumentException("Path must stay under firmware/, rusefi_documentation/ or docs/.");
        }
        Path file = root.resolve(relative).normalize();
        checkSafe(file);
        return file;
    }

    private void checkRoot() throws IOException {
        if (Files.isSymbolicLink(root) || !Files.isDirectory(root, LinkOption.NOFOLLOW_LINKS)) {
            throw new IOException("Knowledge root unavailable");
        }
    }

    private void checkSafe(Path path) throws IOException {
        checkRoot();
        if (!path.startsWith(root)) { throw new IOException("Path outside knowledge root"); }
        Path current = root;
        for (Path part : root.relativize(path)) {
            current = current.resolve(part);
            if (Files.isSymbolicLink(current)) { throw new IOException("Symbolic links are not knowledge files"); }
        }
        if (!path.toRealPath().startsWith(root.toRealPath())) { throw new IOException("Path outside knowledge root"); }
    }

    private boolean allowed(Path path) {
        Path relative = root.relativize(path);
        if (hidden(relative) || !ROOTS.contains(relative.getName(0).toString())) { return false; }
        String name = path.getFileName().toString().toLowerCase(Locale.ROOT);
        int dot = name.lastIndexOf('.');
        return dot >= 0 && EXTENSIONS.contains(name.substring(dot + 1));
    }

    private static boolean hidden(Path relative) {
        for (Path part : relative) {
            String name = part.toString();
            if (name.startsWith(".") || name.chars().anyMatch(Character::isISOControl)) { return true; }
        }
        return false;
    }

    private String relative(Path file) { return root.relativize(file).toString().replace('\\', '/'); }

    private JSONObject metadata(Path file, Text text) {
        String path = relative(file);
        JSONObject result = object("path", path, "sha256", text.sha256);
        if (path.startsWith("rusefi_documentation/")) {
            try {
                result.put("upstream_url", new URI("https", "github.com", "/rusefi/rusefi_documentation/blob/master/"
                        + path.substring("rusefi_documentation/".length()), null).toASCIIString());
            } catch (java.net.URISyntaxException e) { throw new IllegalArgumentException("Invalid documentation path"); }
        }
        return result;
    }

    private static JSONObject result() {
        return object("success", true, "provenance", object("source_revision", "unknown", "ecu_match", "unverified",
                "note", "The cache has no revision manifest. File hashes identify retrieved content, not a matching ECU firmware build. Upstream links refer to current master."));
    }
    private static JSONObject error(String message) { return object("success", false, "error", message); }
    private static JSONObject definition(String name, String description, JSONObject properties, JSONArray required) {
        return object("type", "function", "name", name, "description", description, "strict", false,
                "parameters", object("type", "object", "properties", properties, "required", required, "additionalProperties", false));
    }
    private static JSONArray array(Object... values) {
        JSONArray array = new JSONArray();
        Collections.addAll(array, values);
        return array;
    }
    private static void checkKeys(JSONObject args, String... allowed) {
        if (!Arrays.asList(allowed).containsAll(args.keySet())) { throw new IllegalArgumentException("Unknown knowledge tool argument."); }
    }
    private static String string(JSONObject args, String key, String fallback, int max) {
        Object value = args.getOrDefault(key, fallback);
        if (!(value instanceof String) || ((String) value).length() > max) { throw new IllegalArgumentException("Invalid " + key + " string."); }
        return (String) value;
    }
    private static int integer(JSONObject args, String key, int fallback, int max) {
        Object value = args.getOrDefault(key, fallback);
        if (!(value instanceof Long || value instanceof Integer) || ((Number) value).longValue() < 1 || ((Number) value).longValue() > max) {
            throw new IllegalArgumentException(key + " must be an integer between 1 and " + max + ".");
        }
        return ((Number) value).intValue();
    }
    private static final class Text {
        final String content;
        final String sha256;
        Text(String content, String sha256) { this.content = content; this.sha256 = sha256; }
    }
    private static final class Budget {
        final Runnable cancellation;
        final long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        int visits;
        long bytes;
        int skipped;
        boolean limited;
        Budget(Runnable cancellation) { this.cancellation = cancellation; }
        boolean check() {
            cancellation.run();
            if (System.nanoTime() >= deadline) { limited = true; return false; }
            return true;
        }
        boolean visit() {
            if (!check() || ++visits > MAX_VISITS) { limited = true; return false; }
            return true;
        }
    }
}
