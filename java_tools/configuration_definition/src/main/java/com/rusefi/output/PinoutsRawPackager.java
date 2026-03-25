package com.rusefi.output;

import org.jetbrains.annotations.NotNull;
import org.yaml.snakeyaml.DumperOptions;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.net.URL;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.util.*;
import java.util.zip.*;

/**
 * Collects connector YAML files, images, and meta-info from all boards,
 * zips them all into a zip file, computes its SHA256, then produces/updates
 * boards_meta.yaml with the structure:
 * <p>
 *   data:
 *     &lt;short_board_name&gt;:
 *       files:
 *         - path/to/connector.yaml
 *       sha: &lt;sha256 of zip&gt;
 *       zip_file: connectors.zip
 * <p>
 * If boards_meta.yaml already exists, only the boards found in this run are
 * updated; other entries are preserved (allowing multiple repos to each
 * contribute their own board entries).
 * <p>
 * Optional CLI argument: --zip-name &lt;filename&gt;  (default: connectors.zip)
 */
public class PinoutsRawPackager {
    private static final String BOARDS_ROOT = "firmware/config/boards";
    private static final String OUTPUT_DIR = "pinouts_raw";
    private static final String ZIP_NAME = "connectors.zip";
    private static final String DEFAULT_REMOTE_META_URL = "https://rusefi.com/docs/pinouts_raw/boards_meta.yaml";

    public static void main(String[] args) throws Exception {
        String zipName = ZIP_NAME;
        String boardsRoot = BOARDS_ROOT;
        String outputDir = OUTPUT_DIR;
        String remoteMetaUrl = DEFAULT_REMOTE_META_URL;
        for (int i = 0; i < args.length - 1; i++) {
            switch (args[i]) {
                case "--zip-name":        zipName      = args[i + 1]; break;
                case "--boards-root":     boardsRoot   = args[i + 1]; break;
                case "--output-dir":      outputDir    = args[i + 1]; break;
                case "--remote-meta-url": remoteMetaUrl = args[i + 1]; break;
            }
        }

        List<Path> yamls = findConnectorYamls(boardsRoot);
        List<Path> images = findConnectorImages(boardsRoot);
        List<Path> metaEnvs = findMetaEnvs(boardsRoot);

        System.out.println("Found " + yamls.size() + " connector YAML files");
        System.out.println("Found " + images.size() + " connector images");
        System.out.println("Found " + metaEnvs.size() + " meta-info env files");

        prepareOutput(yamls, images, metaEnvs, zipName, outputDir, remoteMetaUrl);
    }

    private static List<Path> findConnectorYamls(String boardsRoot) throws IOException {
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(boardsRoot), new SimpleFileVisitor<Path>() {
            @Override
            public @NotNull FileVisitResult visitFile(@NotNull Path file, @NotNull BasicFileAttributes attrs) {
                String name = file.getFileName().toString();
                if (name.endsWith(".yaml") && "connectors".equals(file.getParent().getFileName().toString())) {
                    String stem = name.substring(0, name.lastIndexOf('.'));
                    if (!stem.equalsIgnoreCase("iobox")) {
                        result.add(file);
                    }
                }
                return FileVisitResult.CONTINUE;
            }
        });
        Collections.sort(result);
        return result;
    }

    private static List<Path> findConnectorImages(String boardsRoot) throws IOException {
        Set<String> exts = new HashSet<>(Arrays.asList(".png", ".jpg", ".jpeg", ".svg"));
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(boardsRoot), new SimpleFileVisitor<Path>() {
            @Override
            public @NotNull FileVisitResult visitFile(@NotNull Path file, @NotNull BasicFileAttributes attrs) {
                String lowerName = file.getFileName().toString().toLowerCase();
                if ("connectors".equals(file.getParent().getFileName().toString())) {
                    for (String ext : exts) {
                        if (lowerName.endsWith(ext)) {
                            result.add(file);
                            break;
                        }
                    }
                }
                return FileVisitResult.CONTINUE;
            }
        });
        Collections.sort(result);
        return result;
    }

    private static List<Path> findMetaEnvs(String boardsRoot) throws IOException {
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(boardsRoot), new SimpleFileVisitor<Path>() {
            @Override
            public @NotNull FileVisitResult visitFile(@NotNull Path file, @NotNull BasicFileAttributes attrs) {
                String name = file.getFileName().toString();
                if (name.startsWith("meta-info") && name.endsWith(".env") && !name.endsWith("._disabled_env")) {
                    result.add(file);
                }
                return FileVisitResult.CONTINUE;
            }
        });
        return result;
    }

    private static String parseShortBoardName(Path envFile) throws IOException {
        for (String line : Files.readAllLines(envFile)) {
            line = line.trim();
            if (line.startsWith("SHORT_BOARD_NAME")) {
                int eq = line.indexOf('=');
                if (eq >= 0) return line.substring(eq + 1).trim();
            }
        }
        return null;
    }

    private static Map<String, Map<String, Object>> buildData(
            List<Path> yamls, List<Path> metaEnvs, String sha, String zipFile) throws IOException {
        Map<Path, List<String>> boardYamls = new LinkedHashMap<>();
        for (Path yaml : yamls) {
            Path boardDir = yaml.getParent().getParent();
            boardYamls.computeIfAbsent(boardDir, k -> new ArrayList<>()).add(yaml.toString().replace('\\', '/'));
        }

        Map<Path, Set<String>> boardNames = new LinkedHashMap<>();
        for (Path env : metaEnvs) {
            Path boardDir = env.getParent();
            String name = parseShortBoardName(env);
            if (name != null) {
                boardNames.computeIfAbsent(boardDir, k -> new TreeSet<>()).add(name);
            }
        }

        Map<String, Map<String, Object>> data = new TreeMap<>();
        List<Path> sortedDirs = new ArrayList<>(boardYamls.keySet());
        Collections.sort(sortedDirs);
        for (Path boardDir : sortedDirs) {
            List<String> yamlsList = new ArrayList<>(boardYamls.get(boardDir));
            Collections.sort(yamlsList);
            Set<String> names = boardNames.getOrDefault(boardDir, Collections.emptySet());
            List<String> namesList = names.isEmpty()
                    ? Collections.singletonList(boardDir.getFileName().toString())
                    : new ArrayList<>(names);
            for (String name : namesList) {
                Map<String, Object> entry = new LinkedHashMap<>();
                entry.put("files", new ArrayList<>(yamlsList));
                entry.put("sha", sha);
                entry.put("zip_file", zipFile);
                data.put(name, entry);
            }
        }
        return data;
    }

    private static void buildZip(List<Path> yamls, List<Path> images, Path zipPath) throws IOException {
        try (ZipOutputStream zos = new ZipOutputStream(Files.newOutputStream(zipPath.toFile().toPath()))) {
            List<Path> all = new ArrayList<>(yamls);
            all.addAll(images);
            for (Path src : all) {
                zos.putNextEntry(new ZipEntry(src.toString().replace('\\', '/')));
                Files.copy(src, zos);
                zos.closeEntry();
            }
        }
        System.out.println("Wrote " + zipPath + " (" + yamls.size() + " yamls, " + images.size() + " images)");
    }

    private static String sha256File(Path path) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        try (InputStream is = Files.newInputStream(path)) {
            byte[] buf = new byte[65536];
            int n;
            while ((n = is.read(buf)) > 0) md.update(buf, 0, n);
        }
        StringBuilder sb = new StringBuilder();
        for (byte b : md.digest()) sb.append(String.format("%02x", b));
        return sb.toString();
    }

    @SuppressWarnings("unchecked")
    private static void prepareOutput(List<Path> yamls, List<Path> images, List<Path> metaEnvs, String zipName, String outputDir, String remoteMetaUrl) throws Exception {
        Path outDir = Paths.get(outputDir);
        Files.createDirectories(outDir);

        Path zipPath = outDir.resolve(zipName);
        buildZip(yamls, images, zipPath);

        String sha = sha256File(zipPath);
        System.out.println("SHA256(" + zipName + ") = " + sha);

        Map<String, Map<String, Object>> newEntries = buildData(yamls, metaEnvs, sha, zipName);

        DumperOptions opts = new DumperOptions();
        opts.setDefaultFlowStyle(DumperOptions.FlowStyle.BLOCK);
        opts.setPrettyFlow(true);
        Yaml yaml = new Yaml(opts);

        Path metaPath = outDir.resolve("boards_meta.yaml");

        // Load existing yaml so we can merge (other repos may have written their boards).
        // Prefer a local file if present; otherwise try to download from the remote server.
        Map<String, Object> boardsMeta = new LinkedHashMap<>();
        Map<String, Object> existingData = new TreeMap<>();
        if (Files.exists(metaPath)) {
            System.out.println("Loading existing local " + metaPath);
            try (Reader r = Files.newBufferedReader(metaPath)) {
                Map<String, Object> loaded = yaml.load(r);
                if (loaded != null) {
                    boardsMeta.putAll(loaded);
                    Object d = boardsMeta.get("data");
                    if (d instanceof Map) {
                        existingData.putAll((Map<String, Object>) d);
                    }
                }
            }
        } else if (remoteMetaUrl != null && !remoteMetaUrl.isEmpty()) {
            System.out.println("No local boards_meta.yaml found; fetching remote: " + remoteMetaUrl);
            try {
                URL url = new URL(remoteMetaUrl);
                try (InputStream is = url.openStream();
                     Reader r = new InputStreamReader(is)) {
                    Map<String, Object> loaded = yaml.load(r);
                    if (loaded != null) {
                        boardsMeta.putAll(loaded);
                        Object d = boardsMeta.get("data");
                        if (d instanceof Map) {
                            existingData.putAll((Map<String, Object>) d);
                        }
                        System.out.println("Loaded " + existingData.size() + " existing board entries from remote");
                    }
                }
            } catch (IOException e) {
                System.out.println("Could not fetch remote boards_meta.yaml (will create fresh): " + e.getMessage());
            }
        }

        // Update only the entries produced in this run; leave everything else intact
        existingData.putAll(newEntries);
        boardsMeta.put("data", new TreeMap<>(existingData));

        try (Writer w = Files.newBufferedWriter(metaPath)) {
            yaml.dump(boardsMeta, w);
        }
        System.out.println("Wrote " + metaPath + " (" + newEntries.size() + " board entries updated, "
                + existingData.size() + " total)");
    }
}
