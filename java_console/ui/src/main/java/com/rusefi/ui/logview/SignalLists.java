package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

/**
 * The signal lists of the sidebar: Common and the user's own lists, kept by signal label in the console config
 * so they apply to every log. Lists hold copies: adding a signal to a list leaves it in its group. Common
 * follows the defaults of {@link SignalCategories} until it is edited; Reset goes back to them.
 */
final class SignalLists {
    private static final String NAMES = "names";
    private static final String MEMBERS = "members.";
    /** Between names and labels in one config value, as for the recent logs; never in a signal label. */
    private static final String SEPARATOR = "|";

    private final Node node;
    private final List<Runnable> listeners = new ArrayList<>();

    /** Lists that are forgotten with the viewer, e.g. in the sandbox. */
    SignalLists() {
        this(new Node());
    }

    SignalLists(Node node) {
        this.node = node;
    }

    /** Called after every change, e.g. to save the config. */
    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    /** The user's lists in the order they were made; Common is not one of them. */
    List<String> customNames() {
        return split(node.getProperty(NAMES, ""));
    }

    boolean isList(String section) {
        return SignalSections.COMMON.equals(section) || customNames().contains(section);
    }

    /** Labels of a list, or null for Common while it follows the defaults. */
    List<String> members(String list) {
        String stored = node.getProperty(MEMBERS + list, null);
        return stored == null ? null : split(stored);
    }

    boolean isCommonEdited() {
        return members(SignalSections.COMMON) != null;
    }

    void setMembers(String list, List<String> labels) {
        node.setProperty(MEMBERS + list, String.join(SEPARATOR, labels));
        changed();
    }

    /** Back to the default Common signals. */
    void resetCommon() {
        node.getConfig().remove(MEMBERS + SignalSections.COMMON);
        changed();
    }

    /** Adds an empty list after the others; the name must have passed {@link #problem}. */
    void create(String name) {
        List<String> names = customNames();
        names.add(name);
        node.setProperty(NAMES, String.join(SEPARATOR, names));
        node.setProperty(MEMBERS + name, "");
        changed();
    }

    void rename(String from, String to) {
        List<String> names = customNames();
        names.set(names.indexOf(from), to);
        node.setProperty(NAMES, String.join(SEPARATOR, names));
        Object members = node.getConfig().remove(MEMBERS + from);
        node.getConfig().put(MEMBERS + to, members == null ? "" : members);
        changed();
    }

    void delete(String name) {
        List<String> names = customNames();
        names.remove(name);
        node.setProperty(NAMES, String.join(SEPARATOR, names));
        node.getConfig().remove(MEMBERS + name);
        changed();
    }

    /**
     * Why a name cannot be used for a new list or a rename, or null if it can. Section titles must stay unique,
     * since the sidebar finds sections by title.
     *
     * @param current the list being renamed, or null
     */
    String problem(String name, String current) {
        String trimmed = name.trim();
        if (trimmed.isEmpty()) {
            return "Enter a name for the list.";
        }
        if (trimmed.contains(SEPARATOR)) {
            return "A list name cannot contain " + SEPARATOR + ".";
        }
        if (SignalSections.plotOf(trimmed) >= 0) {
            return "\"" + trimmed + "\" is the name of a plot's section.";
        }
        List<String> taken = new ArrayList<>(Arrays.asList(SignalSections.COMMON));
        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            taken.add(group.title);
        }
        taken.addAll(customNames());
        for (String title : taken) {
            if (title.equalsIgnoreCase(trimmed) && !title.equals(current)) {
                return "There is already a section called \"" + title + "\".";
            }
        }
        return null;
    }

    private static List<String> split(String joined) {
        List<String> parts = new ArrayList<>();
        for (String part : joined.split(Pattern.quote(SEPARATOR))) {
            if (!part.isEmpty()) {
                parts.add(part);
            }
        }
        return parts;
    }
}
