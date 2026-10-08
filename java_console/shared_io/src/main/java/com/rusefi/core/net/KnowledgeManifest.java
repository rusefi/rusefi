package com.rusefi.core.net;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;

/** Provenance supplied by the archive publisher, with content integrity checks (not a signature). */
@SuppressWarnings("unchecked")
public final class KnowledgeManifest {
    public static final String NAME = "knowledge-manifest.json";
    private static final int MAX_MANIFEST_BYTES = 8 * 1024 * 1024;
    private final JSONObject manifest;
    private final String manifestHash;

    private KnowledgeManifest(JSONObject manifest, String manifestHash) {
        this.manifest = manifest;
        this.manifestHash = manifestHash;
    }

    public static KnowledgeManifest read(Path root) throws IOException {
        Path path = root.resolve(NAME);
        if (!Files.exists(path, LinkOption.NOFOLLOW_LINKS)) { return new KnowledgeManifest(null, null); }
        if (Files.isSymbolicLink(root) || Files.isSymbolicLink(path) || !Files.isRegularFile(path)
                || Files.size(path) > MAX_MANIFEST_BYTES) {
            throw new IOException("Invalid knowledge manifest file.");
        }
        byte[] bytes;
        try (InputStream input = Files.newInputStream(path, LinkOption.NOFOLLOW_LINKS)) {
            ByteArrayOutputStream contents = new ByteArrayOutputStream();
            byte[] buffer = new byte[8192];
            int count;
            while ((count = input.read(buffer)) != -1) {
                if (contents.size() + count > MAX_MANIFEST_BYTES) { throw new IOException("Knowledge manifest exceeds size limit."); }
                contents.write(buffer, 0, count);
            }
            bytes = contents.toByteArray();
        }
        try {
            JSONObject value = (JSONObject) new JSONParser().parse(new String(bytes, StandardCharsets.UTF_8));
            if (!Long.valueOf(1).equals(value.get("schema_version"))) { throw new IllegalArgumentException(); }
            JSONObject repositories = (JSONObject) value.get("repositories");
            for (String name : Arrays.asList("firmware", "libfirmware", "wiki")) {
                JSONObject repository = (JSONObject) repositories.get(name);
                if (!hex(repository.get("revision"), 40) || !(repository.get("dirty") instanceof Boolean)) {
                    throw new IllegalArgumentException();
                }
            }
            JSONObject files = (JSONObject) value.get("files");
            if (files.isEmpty() || files.size() > 100000) { throw new IllegalArgumentException(); }
            MessageDigest digest = digest();
            // Java String/UTF-16 ordering, also used by the Python producer.
            for (Object key : new TreeSet<>(files.keySet())) {
                String name = (String) key;
                JSONObject file = (JSONObject) files.get(name);
                safePath(root, name);
                if (NAME.equals(name) || !hex(file.get("sha256"), 64) || !(file.get("size") instanceof Long)
                        || (Long) file.get("size") < 0) { throw new IllegalArgumentException(); }
                digest.update((name + "\0" + file.get("sha256") + "\n").getBytes(StandardCharsets.UTF_8));
            }
            if (!toHex(digest.digest()).equals(value.get("payload_sha256"))) { throw new IllegalArgumentException(); }
            return new KnowledgeManifest(value, sha256(bytes));
        } catch (Exception e) {
            throw new IOException("Invalid knowledge manifest metadata or payload index hash.", e);
        }
    }

    /** Validate staged extraction before it replaces the usable cache. Legacy archives remain unverified. */
    public void verifyPayload(Path root, Runnable cancellation) throws IOException {
        if (manifest == null) { return; }
        JSONObject files = (JSONObject) manifest.get("files");
        for (Object key : files.keySet()) {
            cancellation.run();
            Path path = safePath(root, (String) key);
            JSONObject entry = (JSONObject) files.get(key);
            if (!Files.isRegularFile(path, LinkOption.NOFOLLOW_LINKS)
                    || Files.size(path) != ((Number) entry.get("size")).longValue()
                    || !hashFile(path, cancellation).equals(entry.get("sha256"))) {
                throw new IOException("Knowledge payload hash mismatch: " + key);
            }
        }
        // An index must cover every extracted file, not merely an arbitrary subset.
        try (java.util.stream.Stream<Path> paths = Files.walk(root)) {
            Iterator<Path> iterator = paths.iterator();
            while (iterator.hasNext()) {
                cancellation.run();
                Path path = iterator.next();
                String name = root.relativize(path).toString().replace('\\', '/');
                if (!Files.isDirectory(path, LinkOption.NOFOLLOW_LINKS) && !NAME.equals(name) && !files.containsKey(name)) {
                    throw new IOException("Knowledge payload has an unindexed file: " + name);
                }
            }
        }
    }

    public boolean contains(String path, String hash) {
        if (manifest == null) { return false; }
        JSONObject file = (JSONObject) ((JSONObject) manifest.get("files")).get(path);
        return file != null && hash.equals(file.get("sha256"));
    }

    public String cleanRevision(String repository) {
        if (manifest == null) { return null; }
        JSONObject value = (JSONObject) ((JSONObject) manifest.get("repositories")).get(repository);
        return Boolean.FALSE.equals(value.get("dirty")) ? (String) value.get("revision") : null;
    }

    public JSONObject provenance(String ecuSignature) {
        JSONObject result = new JSONObject();
        result.put("manifest_status", manifest == null ? "absent" : "present");
        result.put("manifest_sha256", manifestHash);
        result.put("payload_sha256", manifest == null ? null : manifest.get("payload_sha256"));
        for (String repository : Arrays.asList("firmware", "libfirmware", "wiki")) {
            JSONObject value = manifest == null ? null : (JSONObject) ((JSONObject) manifest.get("repositories")).get(repository);
            String key = "firmware".equals(repository) ? "source" : repository;
            result.put(key + "_revision", value == null ? "unknown" : value.get("revision"));
            result.put(key + "_dirty", value == null ? null : value.get("dirty"));
        }
        result.put("ecu_signature", ecuSignature);
        result.put("ecu_match", "unverified");
        result.put("note", "ECU HELLO identifies an INI schema/build signature, not a firmware Git revision. "
                + "No exact source/ECU match can be established from it. Manifest revisions are publisher metadata; "
                + "dirty repositories include local edits. Hashes verify content integrity, not publisher authenticity.");
        return result;
    }

    private static Path safePath(Path root, String name) throws IOException {
        if (name.isEmpty() || name.contains("\\") || name.contains(":") || name.startsWith("/")
                || name.chars().anyMatch(Character::isISOControl)) { throw new IOException("Invalid manifest path."); }
        for (String part : name.split("/", -1)) {
            if (part.isEmpty() || part.equals(".") || part.equals("..")) { throw new IOException("Invalid manifest path."); }
        }
        Path path = root.resolve(name);
        for (Path current = path; current != null && !current.equals(root); current = current.getParent()) {
            if (Files.isSymbolicLink(current)) { throw new IOException("Knowledge files must not be symbolic links."); }
        }
        return path;
    }

    private static boolean hex(Object value, int length) {
        return value instanceof String && ((String) value).matches("[0-9a-f]{" + length + "}");
    }
    public static String sha256(byte[] bytes) { return toHex(digest().digest(bytes)); }
    public static String hashFile(Path path, Runnable cancellation) throws IOException {
        MessageDigest digest = digest();
        try (InputStream input = Files.newInputStream(path, LinkOption.NOFOLLOW_LINKS)) {
            byte[] buffer = new byte[32768];
            int count;
            while ((count = input.read(buffer)) != -1) {
                cancellation.run();
                digest.update(buffer, 0, count);
            }
        }
        return toHex(digest.digest());
    }
    private static MessageDigest digest() {
        try { return MessageDigest.getInstance("SHA-256"); }
        catch (NoSuchAlgorithmException e) { throw new AssertionError(e); }
    }
    private static String toHex(byte[] bytes) {
        StringBuilder result = new StringBuilder();
        for (byte value : bytes) { result.append(String.format(Locale.ROOT, "%02x", value & 255)); }
        return result.toString();
    }
}
