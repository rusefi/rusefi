package com.rusefi.sensor_logs;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

/**
 * The logging profiles and the one data logging records with, kept in the console config: Full, Trigger and
 * Idle always exist, then the user's own profiles. Recommended profiles store only the user's changes (see
 * {@link LoggingProfile}). Changes are written to the config node as they are made; the console saves its
 * config on exit. One instance is shared by data logging and the Log Viewer, which listen for changes.
 */
public final class LoggingProfiles {
    static final String SELECTED = "selected";
    static final String CUSTOM = "custom";
    static final String NAME = "name";
    static final String ADDED = "added";
    static final String REMOVED = "removed";
    private static final String SEPARATOR = ",";

    private final Node node;
    private final List<LoggingProfile> profiles = new ArrayList<>(LoggingProfile.builtIn());
    private LoggingProfile selected;
    private final List<Runnable> listeners = new ArrayList<>();

    public LoggingProfiles(Node node) {
        this.node = node;
        for (String id : split(node.getProperty(CUSTOM, ""))) {
            if (get(id) == null) {
                profiles.add(new LoggingProfile(id, node.getChild(id).getProperty(NAME, id),
                        LoggingProfile.Kind.CUSTOM));
            }
        }
        for (LoggingProfile profile : profiles) {
            if (!profile.isFull()) {
                Node child = node.getChild(profile.getId());
                profile.setChanges(split(child.getProperty(ADDED, "")), split(child.getProperty(REMOVED, "")));
            }
        }
        LoggingProfile chosen = get(node.getProperty(SELECTED, LoggingProfile.FULL_ID));
        selected = chosen != null ? chosen : profiles.get(0);
    }

    /** Told after any change: a profile edited, created or deleted, or another one chosen. */
    public void addListener(Runnable listener) {
        listeners.add(listener);
    }

    public void removeListener(Runnable listener) {
        listeners.remove(listener);
    }

    /** Renames a custom profile; built-in ones keep their names. */
    public void rename(LoggingProfile profile, String name) {
        profile.setName(name.trim());
        save();
    }

    /** Why {@code name} can't name a new profile, or null: empty, or another profile's name. */
    public String problem(String name) {
        String trimmed = name == null ? "" : name.trim();
        if (trimmed.isEmpty()) {
            return "Enter a name for the profile.";
        }
        for (LoggingProfile profile : profiles) {
            if (profile.getName().toLowerCase(Locale.ROOT).equals(trimmed.toLowerCase(Locale.ROOT))) {
                return "There is already a profile called \"" + profile.getName() + "\".";
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

    /** Deletes a custom profile; recording falls back to Full if it was selected. */
    public void delete(String id) {
        LoggingProfile profile = get(id);
        if (profile == null || profile.getKind() != LoggingProfile.Kind.CUSTOM) {
            return;
        }
        profiles.remove(profile);
        if (selected == profile) {
            selected = profiles.get(0);
        }
        node.getConfig().remove(id);
        save();
    }

    /** Writes every profile to the config node; call after editing a profile. */
    public void save() {
        node.setProperty(SELECTED, selected.getId());
        List<String> custom = new ArrayList<>();
        for (LoggingProfile profile : profiles) {
            if (profile.isFull()) {
                continue;
            }
            if (profile.getKind() == LoggingProfile.Kind.CUSTOM) {
                custom.add(profile.getId());
            }
            Node child = node.getChild(profile.getId());
            child.setProperty(NAME, profile.getName());
            child.setProperty(ADDED, String.join(SEPARATOR, profile.getAdded()));
            child.setProperty(REMOVED, String.join(SEPARATOR, profile.getRemoved()));
        }
        node.setProperty(CUSTOM, String.join(SEPARATOR, custom));
        for (Runnable listener : new ArrayList<>(listeners)) {
            listener.run();
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
