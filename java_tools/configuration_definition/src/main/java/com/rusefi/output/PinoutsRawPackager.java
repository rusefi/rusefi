package com.rusefi.output;

import org.jetbrains.annotations.NotNull;
import org.yaml.snakeyaml.DumperOptions;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.util.*;
import java.util.zip.*;

/**
 * Collects connector YAML files, images, and meta-info from all boards,
 * zips them all into connectors.zip, computes its SHA256, then produces
 * boards_meta.yaml with the structure:
 * <p>
 *   sha: <sha256 of connectors.zip>
 *   data:
 *     <short_board_name>:
 *       - path/to/connector.yaml
 *
 */
public class PinoutsRawPackager {
    private static final String BOARDS_ROOT = "firmware/config/boards";
    private static final String OUTPUT_DIR = "pinouts_raw";
    private static final String ZIP_NAME = "connectors.zip";

    public static void main(String[] args) throws Exception {
        List<Path> yamls = findConnectorYamls();
        List<Path> images = findConnectorImages();
        List<Path> metaEnvs = findMetaEnvs();

        System.out.println("Found " + yamls.size() + " connector YAML files");
        System.out.println("Found " + images.size() + " connector images");
        System.out.println("Found " + metaEnvs.size() + " meta-info env files");

        prepareOutput(yamls, images, metaEnvs);
    }

    private static List<Path> findConnectorYamls() throws IOException {
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(BOARDS_ROOT), new SimpleFileVisitor<Path>() {
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

    private static List<Path> findConnectorImages() throws IOException {
        Set<String> exts = new HashSet<>(Arrays.asList(".png", ".jpg", ".jpeg", ".svg"));
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(BOARDS_ROOT), new SimpleFileVisitor<Path>() {
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

    private static List<Path> findMetaEnvs() throws IOException {
        List<Path> result = new ArrayList<>();
        Files.walkFileTree(Paths.get(BOARDS_ROOT), new SimpleFileVisitor<Path>() {
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

    private static Map<String, List<String>> buildData(List<Path> yamls, List<Path> metaEnvs) throws IOException {
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

        Map<String, List<String>> data = new TreeMap<>();
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
                data.put(name, new ArrayList<>(yamlsList));
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

    private static void prepareOutput(List<Path> yamls, List<Path> images, List<Path> metaEnvs) throws Exception {
        Path outDir = Paths.get(OUTPUT_DIR);
        if (Files.exists(outDir)) {
            Files.walkFileTree(outDir, new SimpleFileVisitor<Path>() {
                @Override
                public @NotNull FileVisitResult visitFile(@NotNull Path file, @NotNull BasicFileAttributes attrs) throws IOException {
                    Files.delete(file);
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public @NotNull FileVisitResult postVisitDirectory(@NotNull Path dir, IOException exc) throws IOException {
                    Files.delete(dir);
                    return FileVisitResult.CONTINUE;
                }
            });
        }
        Files.createDirectories(outDir);

        Path zipPath = outDir.resolve(ZIP_NAME);
        buildZip(yamls, images, zipPath);

        String sha = sha256File(zipPath);
        System.out.println("SHA256(" + ZIP_NAME + ") = " + sha);

        Map<String, List<String>> data = buildData(yamls, metaEnvs);

        Map<String, Object> boardsMeta = new LinkedHashMap<>();
        boardsMeta.put("sha", sha);
        boardsMeta.put("data", data);

        DumperOptions opts = new DumperOptions();
        opts.setDefaultFlowStyle(DumperOptions.FlowStyle.BLOCK);
        opts.setPrettyFlow(true);
        Yaml yaml = new Yaml(opts);

        Path metaPath = outDir.resolve("boards_meta.yaml");
        try (Writer w = Files.newBufferedWriter(metaPath)) {
            yaml.dump(boardsMeta, w);
        }
        System.out.println("Wrote " + metaPath + " (" + data.size() + " board entries)");
    }
}
