package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

/**
 * Rows of the signal list: a heading per section with its members below unless collapsed. Sections are
 * Plotted (always first), the lists ({@link SignalLists}: Common, then the user's), then the
 * {@link SignalCategories.Group}s. A signal is listed in every section it belongs to: plotting it or putting
 * it in a list does not take it out of its group. Flat signals (one value for the whole log) can be left out
 * of the groups, separately for zero signals, mostly unused features, and constant ones at another value,
 * which can matter (an active error, the detected gear, the firmware version). Hidden signals stay in Plotted
 * and the lists and in search results. A member of a user list that the log does not have is shown as
 * missing; Common lists only the signals the log has.
 */
final class SignalSections {
    static final String PLOTTED = "Plotted";
    static final String COMMON = "Common";

    enum Kind {
        PLOTTED,
        /** Common or a user list. */
        LIST,
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
            return new Row(list, Kind.LIST, MISSING, label, 0, true);
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

    /** Group sections start collapsed; Plotted and the lists start open. */
    static Set<String> defaultCollapsed() {
        Set<String> collapsed = new LinkedHashSet<>();
        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            collapsed.add(group.title);
        }
        return collapsed;
    }

    /**
     * @param query         search text; while searching every matching signal is shown, sections expanded
     * @param hideZero      leave signals that are 0 for the whole log out of the groups
     * @param hideConstant  leave signals that hold another single value out of the groups
     * @param collapsed     section titles whose members are hidden
     */
    static List<Row> rows(CsvLogReader log, PlotLayout layout, SignalCategories categories, SignalLists lists,
                          String query, boolean hideZero, boolean hideConstant, Set<String> collapsed) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        boolean searching = !needle.isEmpty();
        List<Row> rows = new ArrayList<>();

        List<Row> plotted = new ArrayList<>();
        for (int plot = 0; plot < layout.plotCount(); plot++) {
            for (int field : layout.fields(plot)) {
                if (matches(log.name(field), needle)) {
                    plotted.add(Row.signal(PLOTTED, Kind.PLOTTED, field));
                }
            }
        }
        addSection(rows, PLOTTED, Kind.PLOTTED, plotted, searching, collapsed, false);

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

        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            List<Row> members = new ArrayList<>();
            for (int field = 1; field < log.fieldCount(); field++) {
                boolean shown = searching
                        || !(hideZero && isZero(log, field)) && !(hideConstant && isConstant(log, field));
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
                                   Set<String> collapsed, boolean keepEmpty) {
        if (members.isEmpty() && !keepEmpty) {
            return;
        }
        boolean expanded = searching || !collapsed.contains(title);
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
