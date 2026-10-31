package com.rusefi.ui.llm;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.Closeable;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.OverlappingFileLockException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.util.Collections;
import java.util.EnumSet;
import java.util.UUID;

/** Private ChatGPT credential storage. Hold a process lock to avoid racing rotating refresh tokens. */
final class ChatGptStore implements Closeable {
    private final Path directory;
    private final FileChannel channel;
    private final FileLock lock;
    final JSONObject data;

    ChatGptStore(Path directory) throws Exception {
        this.directory = directory;
        if (Files.isSymbolicLink(directory)) {
            throw new IOException("ChatGPT storage must not be a symbolic link.");
        }
        Files.createDirectories(directory);
        protect(directory, true);
        Path lockPath = directory.resolve("session.lock");
        if (Files.isSymbolicLink(lockPath)) {
            throw new IOException("Invalid ChatGPT lock file.");
        }
        channel = FileChannel.open(lockPath, StandardOpenOption.CREATE, StandardOpenOption.WRITE);
        FileLock acquired = null;
        try {
            protect(lockPath, false);
            try {
                acquired = channel.tryLock();
            } catch (OverlappingFileLockException ignored) {
                // Same JVM already owns the store.
            }
            if (acquired == null) {
                throw new IOException("Another rusEFI instance is using this account store. Close it first.");
            }
            lock = acquired;
            Path file = directory.resolve("accounts.json");
            if (Files.exists(file, LinkOption.NOFOLLOW_LINKS)) {
                if (Files.isSymbolicLink(file)) {
                    throw new IOException("Invalid ChatGPT accounts file.");
                }
                protect(file, false);
                try {
                    data = (JSONObject) new JSONParser().parse(new String(Files.readAllBytes(file), StandardCharsets.UTF_8));
                    if (!(data.get("host_id") instanceof String) || !(data.get("profiles") instanceof JSONObject)) {
                        throw new IllegalArgumentException();
                    }
                    for (Object value : ((JSONObject) data.get("profiles")).values()) {
                        if (!(value instanceof JSONObject)) { throw new IllegalArgumentException(); }
                        JSONObject profile = (JSONObject) value;
                        if (ChatGptClient.string(profile, "client_id").isEmpty()
                                || ChatGptClient.string(profile, "label").isEmpty()
                                || (!ChatGptClient.string(profile, "access_token").isEmpty()
                                    && !(profile.get("expires_at") instanceof Number))) {
                            throw new IllegalArgumentException();
                        }
                    }
                } catch (Exception e) {
                    throw new IOException("Cannot read ChatGPT accounts.json. Restore it from a backup or use a new storage directory.");
                }
            } else {
                data = ChatGptClient.object("host_id", "urn:uuid:" + UUID.randomUUID(), "profiles", new JSONObject());
                save();
            }
        } catch (Exception e) {
            if (acquired != null) {
                acquired.release();
            }
            channel.close();
            throw e;
        }
    }

    void save() throws IOException {
        Path temporary = Files.createTempFile(directory, "accounts-", ".tmp");
        try {
            // Apply permissions before writing any credentials, including on Windows/NTFS.
            protect(temporary, false);
            Files.write(temporary, data.toJSONString().getBytes(StandardCharsets.UTF_8));
            try {
                Files.move(temporary, directory.resolve("accounts.json"), StandardCopyOption.ATOMIC_MOVE,
                        StandardCopyOption.REPLACE_EXISTING);
            } catch (AtomicMoveNotSupportedException e) {
                throw new IOException("ChatGPT storage requires atomic file replacement.");
            }
        } finally {
            Files.deleteIfExists(temporary);
        }
    }

    private static void protect(Path path, boolean directory) throws IOException {
        PosixFileAttributeView posix = Files.getFileAttributeView(path, PosixFileAttributeView.class, LinkOption.NOFOLLOW_LINKS);
        if (posix != null) {
            posix.setPermissions(PosixFilePermissions.fromString(directory ? "rwx------" : "rw-------"));
            return;
        }
        AclFileAttributeView acl = Files.getFileAttributeView(path, AclFileAttributeView.class, LinkOption.NOFOLLOW_LINKS);
        if (acl == null) {
            throw new IOException("ChatGPT storage requires a filesystem with private file permissions.");
        }
        AclEntry.Builder entry = AclEntry.newBuilder().setType(AclEntryType.ALLOW).setPrincipal(acl.getOwner())
                .setPermissions(EnumSet.allOf(AclEntryPermission.class));
        if (directory) {
            entry.setFlags(AclEntryFlag.DIRECTORY_INHERIT, AclEntryFlag.FILE_INHERIT);
        }
        acl.setAcl(Collections.singletonList(entry.build()));
    }

    @Override
    public void close() throws IOException {
        try {
            lock.release();
        } finally {
            channel.close();
        }
    }
}
