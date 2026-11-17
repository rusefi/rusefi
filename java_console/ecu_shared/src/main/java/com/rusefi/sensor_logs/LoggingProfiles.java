package com.rusefi.sensor_logs;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.FileUtil;
import com.rusefi.core.preferences.storage.Node;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import static com.devexperts.logging.Logging.getLogging;

/**
 * The logging profiles and the one data logging records with: Full, Trigger and Idle always exist, then the
 * user's own profiles. Recommended profiles store only the user's changes (see {@link LoggingProfile}) in the
 * console config node, which the console saves on exit. The user's own profiles are one text file each in the
 * profiles folder, named after the profile and listing one .ini output channel per line, so they can be shared
 * by copying the file; files dropped into the folder appear on the next start. Changes are written as they are
 * made. One instance is shared by data logging and the Log Viewer, which listen for changes.
 */
public final class LoggingProfiles {
    private static final Logging log = getLogging(LoggingProfiles.class);
    static final String SELECTED = "selected";
    /** Prefix of {@link #SELECTED} for one of the user's profiles, which is identified by name. */
    static final String SELECTED_CUSTOM = "custom:";
    /** Ids of the user's profiles as older consoles kept them in the config node; moved to files once. */
    static final String CUSTOM = "custom";
    static final String NAME = "name";
    static final String ADDED = "added";
    static final String REMOVED = "removed";
    static final String EXTENSION = ".txt";
    private static final String SEPARATOR = ",";
    /** Characters Windows does not allow in file names. */
    private static final String FORBIDDEN = "\\/:*?\"<>|";

    private final Node node;
    private final File folder;
    private final List<LoggingProfile> profiles = new ArrayList<>(LoggingProfile.builtIn());
    /** The file each of the user's profiles was last written to, by profile id. */
    private final Map<String, File> files = new HashMap<>();
    private LoggingProfile selected;
    private final List<Runnable> listeners = new ArrayList<>();

    /** Where the console keeps the user's profiles. */
    public static File defaultFolder() {
        return new File(FileUtil.RUSEFI_SETTINGS_FOLDER, "logging_profiles");
    }

    /**
     * @param folder where the user's own profiles are kept, or null to keep them only until the console
     *               closes
     */
    public LoggingProfiles(Node node, File folder) {
        this.node = node;
        this.folder = folder;
        for (LoggingProfile profile : profiles) {
            if (!profile.isFull()) {
                Node child = node.getChild(profile.getId());
                profile.setChanges(split(child.getProperty(ADDED, "")), split(child.getProperty(REMOVED, "")));
            }
        }
        readFolder();
        String selectedId = node.getProperty(SELECTED, LoggingProfile.FULL_ID);
        selectedId = moveConfigProfilesToFiles(selectedId);
        LoggingProfile chosen = selectedId.startsWith(SELECTED_CUSTOM)
                ? byName(selectedId.substring(SELECTED_CUSTOM.length()))
                : get(selectedId);
        selected = chosen != null ? chosen : profiles.get(0);
    }

    private void readFolder() {
        File[] found = folder == null ? null : folder.listFiles((dir, name) -> name.toLowerCase(Locale.ROOT).endsWith(EXTENSION));
        if (found == null) {
            return;
        }
        Arrays.sort(found);
        for (File file : found) {
            String name = file.getName().substring(0, file.getName().length() - EXTENSION.length()).trim();
            if (problem(name) != null) {
                log.info("Skipping logging profile " + file + ": " + problem(name));
                continue;
            }
            try {
                List<String> channels = new ArrayList<>();
                for (String line : Files.readAllLines(file.toPath(), StandardCharsets.UTF_8)) {
                    String channel = line.trim();
                    if (!channel.isEmpty() && !channel.startsWith("#")) {
                        channels.add(channel);
                    }
                }
                LoggingProfile profile = new LoggingProfile(newId(), name, LoggingProfile.Kind.CUSTOM);
                profile.setChanges(channels, Collections.emptyList());
                profiles.add(profile);
                files.put(profile.getId(), file);
            } catch (IOException e) {
                log.error("Reading logging profile " + file + ": " + e);
            }
        }
    }

    /**
     * Older consoles kept the user's profiles in the config node. Writes each to a file, unless a file of that
     * name already exists, and drops it from the node. Returns {@code selectedId} with such a profile's old id
     * replaced by its name.
     */
    private String moveConfigProfilesToFiles(String selectedId) {
        List<String> oldIds = split(node.getProperty(CUSTOM, ""));
        if (folder == null || oldIds.isEmpty()) {
            return selectedId;
        }
        for (String oldId : oldIds) {
            Node child = node.getChild(oldId);
            String name = child.getProperty(NAME, oldId).trim();
            if (problem(name) != null && byName(name) == null) {
                name = clean(name);
            }
            LoggingProfile profile = byName(name);
            if (profile == null) {
                profile = new LoggingProfile(newId(), name, LoggingProfile.Kind.CUSTOM);
                profile.setChanges(split(child.getProperty(ADDED, "")), Collections.emptyList());
                profiles.add(profile);
            }
            if (oldId.equals(selectedId)) {
                selectedId = SELECTED_CUSTOM + profile.getName();
            }
            node.getConfig().remove(oldId);
        }
        node.getConfig().remove(CUSTOM);
        node.setProperty(SELECTED, selectedId);
        writeFiles();
        return selectedId;
    }

    /** {@code name} with the characters a file name can't hold replaced, made unique among the profiles. */
    private String clean(String name) {
        StringBuilder cleaned = new StringBuilder();
        for (char c : name.toCharArray()) {
            cleaned.append(FORBIDDEN.indexOf(c) >= 0 || c < ' ' ? '_' : c);
        }
        String base = cleaned.toString().replaceAll("^[ .]+|[ .]+$", "");
        if (base.isEmpty()) {
            base = "Profile";
        }
        String candidate = base;
        for (int number = 2; problem(candidate) != null; number++) {
            candidate = base + " " + number;
        }
        return candidate;
    }

    /** Told after any change: a profile edited, created or deleted, or another one chosen. */
    public void addListener(Runnable listener) {
        listeners.add(listener);
    }

    public void removeListener(Runnable listener) {
        listeners.remove(listener);
    }

    /** The folder holding the user's profiles, or null when they are not kept. */
    public File getFolder() {
        return folder;
    }

    /** Renames a custom profile; built-in ones keep their names. */
    public void rename(LoggingProfile profile, String name) {
        profile.setName(name.trim());
        save();
    }

    /** Why {@code name} can't name a new profile, or null: empty, unusable as a file name, or taken. */
    public String problem(String name) {
        String trimmed = name == null ? "" : name.trim();
        if (trimmed.isEmpty()) {
            return "Enter a name for the profile.";
        }
        for (char c : trimmed.toCharArray()) {
            if (FORBIDDEN.indexOf(c) >= 0 || c < ' ') {
                return "A profile name can't contain any of " + FORBIDDEN.replace("", " ").trim() + ".";
            }
        }
        if (trimmed.startsWith(".") || trimmed.endsWith(".")) {
            return "A profile name can't start or end with a dot.";
        }
        LoggingProfile existing = byName(trimmed);
        if (existing != null) {
            return "There is already a profile called \"" + existing.getName() + "\".";
        }
        return null;
    }

    private LoggingProfile byName(String name) {
        for (LoggingProfile profile : profiles) {
            if (profile.getName().equalsIgnoreCase(name.trim())) {
                return profile;
            }
        }
        return null;
    }

    public List<LoggingProfile> list() {
        return Collections.unmodifiableList(profiles);
    }

    public LoggingProfile get(String id) {
        for (LoggingProfile profile : profiles) {
            if (profile.getId().equals(id)) {
                return profile;
            }
        }
        return null;
    }

    /** The profile the next recording uses. */
    public LoggingProfile selected() {
        return selected;
    }

    public void select(String id) {
        LoggingProfile profile = get(id);
        if (profile != null) {
            selected = profile;
            save();
        }
    }

    /** A new, empty custom profile. */
    public LoggingProfile create(String name) {
        LoggingProfile profile = new LoggingProfile(newId(), name, LoggingProfile.Kind.CUSTOM);
        profiles.add(profile);
        save();
        return profile;
    }

    /** A custom profile recording what {@code source} records today. */
    public LoggingProfile duplicate(LoggingProfile source, String name, IniFileModel ini) {
        LoggingProfile profile = source.duplicate(newId(), name, ini);
        profiles.add(profile);
        save();
        return profile;
    }

    /** Deletes a custom profile and its file; recording falls back to Full if it was selected. */
    public void delete(String id) {
        LoggingProfile profile = get(id);
        if (profile == null || profile.getKind() != LoggingProfile.Kind.CUSTOM) {
            return;
        }
        profiles.remove(profile);
        if (selected == profile) {
            selected = profiles.get(0);
        }
        File file = files.remove(id);
        if (file != null && file.exists() && !file.delete()) {
            log.error("Could not delete logging profile " + file);
        }
        save();
    }

    /** Writes every profile to the config node and the profiles folder; call after editing a profile. */
    public void save() {
        node.setProperty(SELECTED, selected.getKind() == LoggingProfile.Kind.CUSTOM
                ? SELECTED_CUSTOM + selected.getName()
                : selected.getId());
        for (LoggingProfile profile : profiles) {
            if (profile.getKind() == LoggingProfile.Kind.RECOMMENDED) {
                Node child = node.getChild(profile.getId());
                child.setProperty(NAME, profile.getName());
                child.setProperty(ADDED, String.join(SEPARATOR, profile.getAdded()));
                child.setProperty(REMOVED, String.join(SEPARATOR, profile.getRemoved()));
            }
        }
        writeFiles();
        for (Runnable listener : new ArrayList<>(listeners)) {
            listener.run();
        }
    }

    /** Writes each of the user's profiles whose file is missing or out of date; a renamed one moves. */
    private void writeFiles() {
        if (folder == null) {
            return;
        }
        for (LoggingProfile profile : profiles) {
            if (profile.getKind() != LoggingProfile.Kind.CUSTOM) {
                continue;
            }
            File target = new File(folder, profile.getName() + EXTENSION);
            File previous = files.get(profile.getId());
            List<String> lines = new ArrayList<>();
            lines.add("# rusEFI logging profile: one .ini output channel per line");
            lines.addAll(profile.getAdded());
            try {
                // deleted first so a rename that only changes case also renames the file on Windows
                if (previous != null && !previous.getName().equals(target.getName())) {
                    Files.deleteIfExists(previous.toPath());
                } else if (target.exists() && Files.readAllLines(target.toPath(), StandardCharsets.UTF_8).equals(lines)) {
                    files.put(profile.getId(), target);
                    continue;
                }
                Files.createDirectories(folder.toPath());
                Files.write(target.toPath(), lines, StandardCharsets.UTF_8);
                files.put(profile.getId(), target);
            } catch (IOException e) {
                log.error("Writing logging profile " + target + ": " + e);
            }
        }
    }

    private String newId() {
        int number = 1;
        while (get("custom" + number) != null) {
            number++;
        }
        return "custom" + number;
    }

    private static List<String> split(String value) {
        List<String> parts = new ArrayList<>();
        for (String part : Arrays.asList(value.split(SEPARATOR))) {
            if (!part.trim().isEmpty()) {
                parts.add(part.trim());
            }
        }
        return parts;
    }
}
