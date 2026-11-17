package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.time.Instant;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.UUID;
import java.util.stream.Stream;

import static com.rusefi.ui.llm.ChatGptClient.object;

/** Private, account-scoped local conversations. A complete turn replaces one file atomically. */
final class TroubleshootingSessionStore {
    private static final int MAX_FILE_BYTES = 8 * 1024 * 1024;
    static final int MAX_TRANSCRIPT = 1024 * 1024;
    private static final DateTimeFormatter DISPLAY_TIME = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm")
            .withZone(ZoneId.systemDefault());
    private final Path directory;

    TroubleshootingSessionStore(Path directory) {
        this.directory = directory;
    }

    static final class Session {
        final String id;
        final String accountId;
        final String model;
        final String title;
        final long createdAt;
        final long updatedAt;
        final JSONArray history;
        final String transcript;
        final JSONObject evidence;

        private Session(String id, String accountId, String model, String title, long createdAt, long updatedAt,
                        JSONArray history, String transcript, JSONObject evidence) {
            this.id = id;
            this.accountId = accountId;
            this.model = model;
            this.title = title;
            this.createdAt = createdAt;
            this.updatedAt = updatedAt;
            this.history = history;
            this.transcript = transcript;
            this.evidence = evidence;
        }

        static Session start(String accountId, String model, String firstPrompt) {
            String firstLine = firstPrompt.trim().split("\\R", 2)[0].trim();
            String title = firstLine.length() > 100 ? firstLine.substring(0, 100) : firstLine;
            long now = System.currentTimeMillis();
            return new Session(UUID.randomUUID().toString(), accountId, model, title, now, now,
                    new JSONArray(), "", null);
        }

        Session completed(String model, JSONArray history, String transcript, JSONObject evidence) {
            return new Session(id, accountId, model, title, createdAt, System.currentTimeMillis(),
                    history, transcript, evidence);
        }

        Summary summary() { return new Summary(id, title, updatedAt); }
    }

    static final class Summary {
        final String id;
        final String title;
        final long updatedAt;

        private Summary(String id, String title, long updatedAt) {
            this.id = id;
            this.title = title;
            this.updatedAt = updatedAt;
        }

        @Override public String toString() {
            return DISPLAY_TIME.format(Instant.ofEpochMilli(updatedAt)) + "  " + title;
        }
    }

    List<Summary> list(String accountId) throws IOException {
        if (!Files.exists(directory, LinkOption.NOFOLLOW_LINKS)) { return new ArrayList<>(); }
        checkDirectory();
        List<Summary> result = new ArrayList<>();
        try (Stream<Path> files = Files.list(directory)) {
            for (Path file : (Iterable<Path>) files::iterator) {
                String name = file.getFileName().toString();
                if (!name.endsWith(".json")) { continue; }
                try {
                    Session session = read(file, name.substring(0, name.length() - 5));
                    if (session.accountId.equals(accountId)) { result.add(session.summary()); }
                } catch (IOException ignored) {
                    // A damaged session does not hide other sessions. Loading it directly still reports an error.
                }
            }
        }
        result.sort(Comparator.comparingLong((Summary item) -> item.updatedAt).reversed());
        return result;
    }

    Session load(String accountId, String id) throws IOException {
        checkDirectory();
        Session session = read(file(id), id);
        if (!session.accountId.equals(accountId)) { throw new IOException("This session belongs to another account."); }
        return session;
    }

    void save(Session session) throws IOException {
        validateId(session.id);
        if (session.accountId.isEmpty() || session.model.isEmpty() || session.title.isEmpty()
                || session.title.length() > 100 || session.transcript.length() > MAX_TRANSCRIPT
                || session.history.toJSONString().length() > ChatGptAgent.MAX_HISTORY) {
            throw new IOException("Troubleshooting session exceeded its storage limit.");
        }
        JSONObject data = object("schema_version", 1, "id", session.id, "account_id", session.accountId,
                "model", session.model, "title", session.title, "created_at", session.createdAt,
                "updated_at", session.updatedAt, "history", session.history,
                "transcript", session.transcript, "evidence", session.evidence);
        byte[] payload = data.toJSONString().getBytes(StandardCharsets.UTF_8);
        if (payload.length > MAX_FILE_BYTES) { throw new IOException("Troubleshooting session exceeded its storage limit."); }
        ensureDirectory();
        Path target = file(session.id);
        if (Files.exists(target, LinkOption.NOFOLLOW_LINKS)
                && !read(target, session.id).accountId.equals(session.accountId)) {
            throw new IOException("This session belongs to another account.");
        }
        Path temporary = Files.createTempFile(directory, ".session-", ".tmp");
        try {
            ChatGptStore.protect(temporary, false);
            Files.write(temporary, payload);
            try {
                Files.move(temporary, target, StandardCopyOption.ATOMIC_MOVE,
                        StandardCopyOption.REPLACE_EXISTING);
            } catch (AtomicMoveNotSupportedException e) {
                throw new IOException("Troubleshooting sessions require atomic file replacement.", e);
            }
        } finally {
            Files.deleteIfExists(temporary);
        }
    }

    void delete(String accountId, String id) throws IOException {
        load(accountId, id);
        Files.delete(file(id));
    }

    private Session read(Path file, String id) throws IOException {
        validateId(id);
        if (!Files.isRegularFile(file, LinkOption.NOFOLLOW_LINKS) || Files.size(file) > MAX_FILE_BYTES) {
            throw new IOException("Cannot read troubleshooting session.");
        }
        byte[] payload = Files.readAllBytes(file);
        if (payload.length > MAX_FILE_BYTES) { throw new IOException("Cannot read troubleshooting session."); }
        try {
            JSONObject data = (JSONObject) new JSONParser().parse(new String(payload, StandardCharsets.UTF_8));
            if (!Long.valueOf(1).equals(data.get("schema_version")) || !id.equals(data.get("id"))
                    || !(data.get("account_id") instanceof String) || !(data.get("model") instanceof String)
                    || !(data.get("title") instanceof String) || !(data.get("created_at") instanceof Number)
                    || !(data.get("updated_at") instanceof Number) || !(data.get("history") instanceof JSONArray)
                    || !(data.get("transcript") instanceof String)
                    || (data.get("evidence") != null && !(data.get("evidence") instanceof JSONObject))) {
                throw new IllegalArgumentException();
            }
            String title = (String) data.get("title");
            String transcript = (String) data.get("transcript");
            JSONArray history = (JSONArray) data.get("history");
            if (title.isEmpty() || title.length() > 100 || transcript.length() > MAX_TRANSCRIPT
                    || history.toJSONString().length() > ChatGptAgent.MAX_HISTORY) {
                throw new IllegalArgumentException();
            }
            return new Session(id, (String) data.get("account_id"), (String) data.get("model"), title,
                    ((Number) data.get("created_at")).longValue(), ((Number) data.get("updated_at")).longValue(),
                    history, transcript, (JSONObject) data.get("evidence"));
        } catch (Exception e) {
            throw new IOException("Cannot read troubleshooting session.", e);
        }
    }

    private Path file(String id) throws IOException {
        validateId(id);
        return directory.resolve(id + ".json");
    }

    private static void validateId(String id) throws IOException {
        try {
            if (!UUID.fromString(id).toString().equals(id)) { throw new IllegalArgumentException(); }
        } catch (RuntimeException e) {
            throw new IOException("Invalid troubleshooting session ID.", e);
        }
    }

    private void checkDirectory() throws IOException {
        if (Files.isSymbolicLink(directory) || !Files.isDirectory(directory, LinkOption.NOFOLLOW_LINKS)) {
            throw new IOException("Invalid troubleshooting session directory.");
        }
    }

    private void ensureDirectory() throws IOException {
        if (Files.exists(directory, LinkOption.NOFOLLOW_LINKS)) {
            checkDirectory();
        } else {
            Files.createDirectories(directory);
        }
        ChatGptStore.protect(directory, true);
    }
}
