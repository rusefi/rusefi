package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.function.Predicate;

/**
 * Rows of the signal list: a heading per section with its members below unless collapsed. Sections are
 * Plot 1, Plot 2... with each plot's signals (always first; Histogram, with its X, Y and Z, while the Histogram view is shown), the lists ({@link SignalLists}: Common, then the user's), then the
 * {@link SignalCategories.Group}s. A signal is listed in every section it belongs to: plotting it or putting
 * it in a list does not take it out of its group. Flat signals (one value for the whole log) can be left out
 * of the groups, separately for zero signals, mostly unused features, and constant ones at another value,
 * which can matter (an active error, the detected gear, the firmware version), also while searching. Hidden
 * signals stay in the plots and the lists. A member of a user list that the log does not have is shown as
 * missing; Common lists only the signals the log has. The logging profiles follow the lists, read-only and
 * collapsed until opened, also showing the signals the log lacks: a log recorded with a profile has its signals.
 */
final class SignalSections {
    /** Title prefix of the plot sections: Plot 1, Plot 2... */
    static final String PLOT = "Plot";
    /** Takes the plot sections' place while the Histogram view is shown: its X, Y and Z signals. */
    static final String HISTOGRAM = "Histogram";
    /** The same for the Scatter view. */
    static final String SCATTER = "Scatter";
    static final String COMMON = "Common";
    /** Title prefix of the logging profile sections: "Profile: Trigger". */
    static final String PROFILE = "Profile: ";

    enum Kind {
        PLOTTED,
        HISTOGRAM,
        /** Common or a user list. */
        LIST,
        /** A logging profile's signals. */
        PROFILE,
        GROUP
    }

    /** A section heading, a signal under one, or a list member missing from the log. */
    static final class Row {
        static final int HEADING = -1;
        static final int MISSING = -2;

        final String section;
        final Kind kind;
        /** The signal, or {@link #HEADING} / {@link #MISSING}. */
        final int field;
        /** Label of a missing list member. */
        final String label;
        /** Members of the section, for a heading. */
        final int count;
        final boolean expanded;

        private Row(String section, Kind kind, int field, String label, int count, boolean expanded) {
            this.section = section;
            this.kind = kind;
            this.field = field;
            this.label = label;
            this.count = count;
            this.expanded = expanded;
        }

        static Row heading(String section, Kind kind, int count, boolean expanded) {
            return new Row(section, kind, HEADING, null, count, expanded);
        }

        static Row signal(String section, Kind kind, int field) {
            return new Row(section, kind, field, null, 0, true);
        }

        static Row missing(String list, String label) {
            return missing(list, Kind.LIST, label);
        }

        static Row missing(String section, Kind kind, String label) {
            return new Row(section, kind, MISSING, label, 0, true);
        }

        boolean isHeading() {
            return field == HEADING;
        }

        boolean isMissing() {
            return field == MISSING;
        }

        /** A signal of the log, as opposed to a heading or a missing list member. */
        boolean isSignal() {
            return field >= 0;
        }
    }

    private SignalSections() {
    }

    /** The section of a plot (0-based). */
    static String plotTitle(int plot) {
        return PLOT + " " + (plot + 1);
    }

    /** The plot (0-based) a section title names, or -1. */
    static int plotOf(String title) {
        String prefix = PLOT + " ";
        if (title.length() <= prefix.length() || !title.regionMatches(true, 0, prefix, 0, prefix.length())) {
            return -1;
        }
        String number = title.substring(prefix.length());
        for (int index = 0; index < number.length(); index++) {
            if (!Character.isDigit(number.charAt(index))) {
                return -1;
            }
        }
        return number.length() > 3 ? -1 : Integer.parseInt(number) - 1;
    }

    /** The section of a logging profile. */
    static String profileTitle(String profile) {
        return PROFILE + profile;
    }

    /** Whether a section starts collapsed: the groups and the profiles; the plots and the lists start open. */
    static boolean collapsedByDefault(Set<String> defaultCollapsed, String section) {
        return defaultCollapsed.contains(section) || section.startsWith(PROFILE);
    }

    /** Group sections start collapsed; the plots and the lists start open. */
    static Set<String> defaultCollapsed() {
        Set<String> collapsed = new LinkedHashSet<>();
        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            collapsed.add(group.title);
        }
        return collapsed;
    }

    /**
     * @param query         search text; while searching the matching signals are shown, sections expanded
     * @param hideZero      leave signals that are 0 for the whole log out of the groups
     * @param hideConstant  leave signals that hold another single value out of the groups
     * @param collapsed     whether a section (by title) hides its members
     */
    static List<Row> rows(CsvLogReader log, PlotLayout layout, SignalCategories categories, SignalLists lists,
                          String query, boolean hideZero, boolean hideConstant, Predicate<String> collapsed) {
        return rows(log, layout, null, categories, lists, query, hideZero, hideConstant, collapsed);
    }

    /**
     * @param histogramFields the histogram's X, Y and Z signals while the Histogram view is shown, listed first
     *                        in place of the plot sections (the plots are not on screen then); null for the timeline
     */
    static List<Row> rows(CsvLogReader log, PlotLayout layout, int[] histogramFields, SignalCategories categories,
                          SignalLists lists, String query, boolean hideZero, boolean hideConstant,
                          Predicate<String> collapsed) {
        return rows(log, layout, histogramFields, HISTOGRAM, categories, lists, query, hideZero, hideConstant,
                collapsed);
    }

    /** @param axesTitle the title of the axes section: {@link #HISTOGRAM} or {@link #SCATTER} */
    static List<Row> rows(CsvLogReader log, PlotLayout layout, int[] histogramFields, String axesTitle,
                          SignalCategories categories, SignalLists lists, String query, boolean hideZero,
                          boolean hideConstant, Predicate<String> collapsed) {
        return rows(log, layout, histogramFields, axesTitle, categories, lists, Collections.emptyMap(), query,
                hideZero, hideConstant, collapsed);
    }

    /** @param profiles section title ({@link #profileTitle}) to the profile's signals as logs label them */
    static List<Row> rows(CsvLogReader log, PlotLayout layout, int[] histogramFields, String axesTitle,
                          SignalCategories categories, SignalLists lists, Map<String, List<String>> profiles,
                          String query, boolean hideZero, boolean hideConstant, Predicate<String> collapsed) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        boolean searching = !needle.isEmpty();
        List<Row> rows = new ArrayList<>();

        if (histogramFields != null) {
            List<Row> axes = new ArrayList<>();
            for (int field : histogramFields) {
                if (matches(log.name(field), needle)) {
                    axes.add(Row.signal(axesTitle, Kind.HISTOGRAM, field));
                }
            }
            addSection(rows, axesTitle, Kind.HISTOGRAM, axes, searching, collapsed, !searching);
        } else {
            for (int plot = 0; plot < layout.plotCount(); plot++) {
                String title = plotTitle(plot);
                List<Row> plotted = new ArrayList<>();
                for (int field : layout.fields(plot)) {
                    if (matches(log.name(field), needle)) {
                        plotted.add(Row.signal(title, Kind.PLOTTED, field));
                    }
                }
                // shown while empty, like the plot on screen, so signals can be dropped on it; hidden only by a search
                addSection(rows, title, Kind.PLOTTED, plotted, searching, collapsed, !searching);
            }
        }

        Map<String, Integer> fieldOfLabel = fieldOfLabel(log);
        List<String> listNames = new ArrayList<>();
        listNames.add(COMMON);
        listNames.addAll(lists.customNames());
        for (String list : listNames) {
            List<Row> members = new ArrayList<>();
            List<String> labels = lists.members(list);
            if (labels == null) {
                // default Common: the usual signals this log has
                for (int field : categories.common()) {
                    if (matches(log.name(field), needle)) {
                        members.add(Row.signal(list, Kind.LIST, field));
                    }
                }
            } else {
                for (String label : labels) {
                    Integer field = fieldOfLabel.get(label);
                    // Common is "the usual signals this log has": it keeps other logs' signals without listing them
                    if (matches(label, needle) && (field != null || !COMMON.equals(list))) {
                        members.add(field != null ? Row.signal(list, Kind.LIST, field) : Row.missing(list, label));
                    }
                }
            }
            // a list stays visible while empty, so signals can be dropped on it and it can be deleted
            addSection(rows, list, Kind.LIST, members, searching, collapsed, !searching);
        }

        for (Map.Entry<String, List<String>> profile : profiles.entrySet()) {
            List<Row> members = new ArrayList<>();
            for (String label : profile.getValue()) {
                Integer field = fieldOfLabel.get(label);
                if (matches(label, needle)) {
                    members.add(field != null ? Row.signal(profile.getKey(), Kind.PROFILE, field)
                            : Row.missing(profile.getKey(), Kind.PROFILE, label));
                }
            }
            addSection(rows, profile.getKey(), Kind.PROFILE, members, searching, collapsed, !searching);
        }

        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            List<Row> members = new ArrayList<>();
            for (int field = 1; field < log.fieldCount(); field++) {
                boolean shown = !(hideZero && isZero(log, field)) && !(hideConstant && isConstant(log, field));
                if (categories.group(field) == group && shown && matches(log.name(field), needle)) {
                    members.add(Row.signal(group.title, Kind.GROUP, field));
                }
            }
            addSection(rows, group.title, Kind.GROUP, members, searching, collapsed, false);
        }
        return rows;
    }

    /** The first field with each label; Time (field 0) is not a signal. */
    static Map<String, Integer> fieldOfLabel(CsvLogReader log) {
        Map<String, Integer> fields = new HashMap<>();
        for (int field = log.fieldCount() - 1; field >= 1; field--) {
            fields.put(log.name(field), field);
        }
        return fields;
    }

    private static boolean matches(String label, String needle) {
        return needle.isEmpty() || label.toLowerCase(Locale.ROOT).contains(needle);
    }

    /** Empty sections are left out unless kept. */
    private static void addSection(List<Row> rows, String title, Kind kind, List<Row> members, boolean searching,
                                   Predicate<String> collapsed, boolean keepEmpty) {
        if (members.isEmpty() && !keepEmpty) {
            return;
        }
        boolean expanded = searching || !collapsed.test(title);
        rows.add(Row.heading(title, kind, members.size(), expanded));
        if (expanded) {
            rows.addAll(members);
        }
    }

    /** 0 for the whole log. */
    static boolean isZero(CsvLogReader log, int field) {
        return log.isConstant(field) && log.minimum(field) == 0;
    }

    /** One value other than 0 for the whole log. */
    static boolean isConstant(CsvLogReader log, int field) {
        return log.isConstant(field) && log.minimum(field) != 0;
    }

    static int zeroCount(CsvLogReader log) {
        int count = 0;
        for (int field = 1; field < log.fieldCount(); field++) {
            if (isZero(log, field)) {
                count++;
            }
        }
        return count;
    }

    static int constantCount(CsvLogReader log) {
        int count = 0;
        for (int field = 1; field < log.fieldCount(); field++) {
            if (isConstant(log, field)) {
                count++;
            }
        }
        return count;
    }
}
