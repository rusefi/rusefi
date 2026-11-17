package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.util.Arrays;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.regex.Pattern;

/**
 * What the signal sidebar remembers from one log to the next and across console runs: which sections are open
 * and the Hide zero / Hide constant switches. Sections are kept as changes from their defaults (groups closed,
 * the profiles closed, the plots and the lists open), so a group added in a later version starts closed like the others. Saved with
 * the console config on exit.
 */
final class SidebarState {
    private static final String OPENED = "opened";
    private static final String CLOSED = "closed";
    private static final String HIDE_ZERO = "hide_zero";
    private static final String HIDE_CONSTANT = "hide_constant";
    /** Between titles in one config value; list names cannot contain it ({@link SignalLists#problem}). */
    private static final String SEPARATOR = "|";

    private final Node node;
    private final Set<String> collapsedByDefault = SignalSections.defaultCollapsed();

    /** State that is forgotten with the viewer, e.g. in the sandbox. */
    SidebarState() {
        this(new Node());
    }

    SidebarState(Node node) {
        this.node = node;
    }

    boolean isCollapsed(String section) {
        return titles(CLOSED).contains(section)
                || startsCollapsed(section) && !titles(OPENED).contains(section);
    }

    void setCollapsed(String section, boolean collapsed) {
        Set<String> opened = titles(OPENED);
        Set<String> closed = titles(CLOSED);
        opened.remove(section);
        closed.remove(section);
        if (collapsed != startsCollapsed(section)) {
            (collapsed ? closed : opened).add(section);
        }
        node.setProperty(OPENED, String.join(SEPARATOR, opened));
        node.setProperty(CLOSED, String.join(SEPARATOR, closed));
    }

    /** Forgets a deleted list, so a new list of the same name starts open. */
    void forget(String section) {
        setCollapsed(section, startsCollapsed(section));
    }

    private boolean startsCollapsed(String section) {
        return SignalSections.collapsedByDefault(collapsedByDefault, section);
    }

    /** A renamed list keeps its open or closed state. */
    void rename(String from, String to) {
        boolean collapsed = isCollapsed(from);
        forget(from);
        setCollapsed(to, collapsed);
    }

    boolean hideZero() {
        return node.getBoolProperty(HIDE_ZERO, true);
    }

    void setHideZero(boolean hide) {
        node.setBoolProperty(HIDE_ZERO, hide);
    }

    boolean hideConstant() {
        return node.getBoolProperty(HIDE_CONSTANT, false);
    }

    void setHideConstant(boolean hide) {
        node.setBoolProperty(HIDE_CONSTANT, hide);
    }

    private Set<String> titles(String key) {
        Set<String> titles = new LinkedHashSet<>(Arrays.asList(node.getProperty(key, "").split(Pattern.quote(SEPARATOR))));
        titles.remove("");
        return titles;
    }
}
