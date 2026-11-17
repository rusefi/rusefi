package com.rusefi.sensor_logs;

import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

/**
 * Which signals a binary log records. Full records every recordable output channel, as logging always did.
 * Trigger and Idle start from a recommended set: the .ini gauge category of the same name plus a few context
 * signals. The user's changes to them are kept as added and removed channels rather than a copy, so a later
 * release's recommended set still reaches users who edited it, and Reset drops the changes. Custom profiles
 * start empty. {@link #LOCKED} channels are in every profile; the time is part of every log row anyway.
 * Signals are named by .ini output channel, not by log label, so profiles survive label changes and
 * channels the connected ECU's .ini lacks are skipped. The ECU is read the same way for every profile: a
 * profile only makes the file smaller.
 */
public final class LoggingProfile {
    public enum Kind {FULL, RECOMMENDED, CUSTOM}

    public static final String FULL_ID = "full";
    public static final String TRIGGER_ID = "trigger";
    public static final String IDLE_ID = "idle";
    /** Recorded by every profile: a log without RPM can hardly be read. */
    public static final List<String> LOCKED = Collections.singletonList("RPMValue");

    private static final List<String> TRIGGER_CONTEXT = Arrays.asList(
            "MAPValue", "TPSValue", "VBatt", "correctedIgnitionAdvance");
    private static final List<String> IDLE_CONTEXT = Arrays.asList(
            "idleTarget", "coolant", "MAPValue", "TPSValue", "lambdaValue", "correctedIgnitionAdvance", "VBatt");

    private final String id;
    private String name;
    private final Kind kind;
    private final Set<String> added = channelSet();
    private final Set<String> removed = channelSet();

    LoggingProfile(String id, String name, Kind kind) {
        this.id = id;
        this.name = name;
        this.kind = kind;
    }

    static List<LoggingProfile> builtIn() {
        return Arrays.asList(
                new LoggingProfile(FULL_ID, "Full", Kind.FULL),
                new LoggingProfile(TRIGGER_ID, "Trigger", Kind.RECOMMENDED),
                new LoggingProfile(IDLE_ID, "Idle", Kind.RECOMMENDED));
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    /** Only custom profiles can be renamed. */
    public void setName(String name) {
        if (kind == Kind.CUSTOM) {
            this.name = name;
        }
    }

    public Kind getKind() {
        return kind;
    }

    public boolean isFull() {
        return kind == Kind.FULL;
    }

    /** A recommended profile the user has changed. */
    public boolean isEdited() {
        return kind == Kind.RECOMMENDED && !(added.isEmpty() && removed.isEmpty());
    }

    /** Channels the user added; for a custom profile, all of its own channels. */
    public Set<String> getAdded() {
        return Collections.unmodifiableSet(added);
    }

    /** Recommended channels the user removed. */
    public Set<String> getRemoved() {
        return Collections.unmodifiableSet(removed);
    }

    /** One line for menus: what the profile records. */
    public String describe() {
        switch (kind) {
            case FULL:
                return "Every signal";
            case RECOMMENDED:
                return isEdited() ? "Recommended, edited" : "Recommended";
            default:
                return "Your own";
        }
    }

    /** The recommended channels of a Trigger or Idle profile, before the user's changes; empty otherwise. */
    public Set<String> recommended(IniFileModel ini) {
        Set<String> channels = channelSet();
        if (kind != Kind.RECOMMENDED) {
            return channels;
        }
        String category = TRIGGER_ID.equals(id) ? "Trigger" : "Idle";
        Map<String, GaugeCategoryModel> categories = ini == null ? null : ini.getGaugeCategories();
        if (categories != null) {
            for (GaugeCategoryModel model : categories.values()) {
                if (category.equalsIgnoreCase(model.getName().trim())) {
                    for (GaugeModel gauge : model.getGauges()) {
                        channels.add(gauge.getChannel());
                    }
                }
            }
        }
        channels.addAll(TRIGGER_ID.equals(id) ? TRIGGER_CONTEXT : IDLE_CONTEXT);
        return channels;
    }

    /** The channels this profile records, locked ones included; every recordable channel for Full. */
    public Set<String> channels(IniFileModel ini) {
        Set<String> channels = channelSet();
        if (isFull()) {
            if (ini == null) {
                return channels;
            }
            for (CustomBinaryLogEntry entry : SensorLogger.getOutputChannels(ini)) {
                channels.add(entry.getChannel());
            }
            return channels;
        }
        channels.addAll(LOCKED);
        channels.addAll(recommended(ini));
        channels.removeAll(removed);
        channels.addAll(added);
        return channels;
    }

    /** The entries of the full channel list this profile records, in the full list's order. */
    public List<CustomBinaryLogEntry> select(List<CustomBinaryLogEntry> all, IniFileModel ini) {
        if (isFull()) {
            return all;
        }
        Set<String> channels = channels(ini);
        List<CustomBinaryLogEntry> selected = new ArrayList<>();
        for (CustomBinaryLogEntry entry : all) {
            if (channels.contains(entry.getChannel())) {
                selected.add(entry);
            }
        }
        return selected;
    }

    /** Adds a channel; adding back a removed recommended channel just restores it. Full can't be edited. */
    public void add(String channel) {
        if (isFull() || isLocked(channel)) {
            return;
        }
        if (!removed.remove(channel)) {
            added.add(channel);
        }
    }

    /** Removes a channel; a recommended one is remembered as removed. Locked channels stay. */
    public void remove(String channel, IniFileModel ini) {
        if (isFull() || isLocked(channel)) {
            return;
        }
        added.remove(channel);
        if (kind == Kind.RECOMMENDED && recommended(ini).contains(channel)) {
            removed.add(channel);
        }
    }

    public static boolean isLocked(String channel) {
        for (String locked : LOCKED) {
            if (locked.equalsIgnoreCase(channel)) {
                return true;
            }
        }
        return false;
    }

    /** Drops the user's changes to a recommended profile. */
    public void reset() {
        if (kind == Kind.RECOMMENDED) {
            added.clear();
            removed.clear();
        }
    }

    /** A custom profile recording what this one records today, e.g. to start from Full and remove signals. */
    LoggingProfile duplicate(String newId, String newName, IniFileModel ini) {
        LoggingProfile copy = new LoggingProfile(newId, newName, Kind.CUSTOM);
        for (String channel : channels(ini)) {
            copy.add(channel);
        }
        return copy;
    }

    void setChanges(Collection<String> added, Collection<String> removed) {
        this.added.clear();
        this.added.addAll(added);
        this.removed.clear();
        if (kind == Kind.RECOMMENDED) {
            this.removed.addAll(removed);
        }
        this.added.removeIf(LoggingProfile::isLocked);
    }

    /** .ini channel names are matched without regard to case, as the .ini reader does. */
    static Set<String> channelSet() {
        return new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
    }

    @Override
    public String toString() {
        return name;
    }
}
