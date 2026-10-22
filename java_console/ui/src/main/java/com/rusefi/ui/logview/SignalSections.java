package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

/**
 * Rows of the signal list: a heading per section with its members below unless collapsed. Sections are
 * Plotted (always first), Common, then the {@link SignalCategories.Group}s. A signal is listed in every
 * section it belongs to: plotting or listing it in Common does not take it out of its group. Flat signals
 * (one value for the whole log) can be left out of the groups, separately for zero signals, mostly unused
 * features, and constant ones at another value, which can matter (an active error, the detected gear, the
 * firmware version). Hidden signals stay in Plotted and Common and in search results.
 */
final class SignalSections {
    static final String PLOTTED = "Plotted";
    static final String COMMON = "Common";

    /** A section heading ({@link #field} -1) or a signal under one. */
    static final class Row {
        final String section;
        final int field;
        /** Members of the section, for a heading. */
        final int count;
        final boolean expanded;

        private Row(String section, int field, int count, boolean expanded) {
            this.section = section;
            this.field = field;
            this.count = count;
            this.expanded = expanded;
        }

        static Row heading(String section, int count, boolean expanded) {
            return new Row(section, -1, count, expanded);
        }

        static Row signal(String section, int field) {
            return new Row(section, field, 0, true);
        }

        boolean isHeading() {
            return field < 0;
        }
    }

    private SignalSections() {
    }

    /** Group sections start collapsed; Plotted and Common start open. */
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
    static List<Row> rows(CsvLogReader log, PlotLayout layout, SignalCategories categories, String query,
                          boolean hideZero, boolean hideConstant, Set<String> collapsed) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        boolean searching = !needle.isEmpty();
        List<Row> rows = new ArrayList<>();

        List<Integer> plotted = new ArrayList<>();
        for (int plot = 0; plot < layout.plotCount(); plot++) {
            for (int field : layout.fields(plot)) {
                if (matches(log, field, needle)) {
                    plotted.add(field);
                }
            }
        }
        addSection(rows, PLOTTED, plotted, searching, collapsed);

        List<Integer> common = new ArrayList<>();
        for (int field : categories.common()) {
            if (matches(log, field, needle)) {
                common.add(field);
            }
        }
        addSection(rows, COMMON, common, searching, collapsed);

        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            List<Integer> members = new ArrayList<>();
            for (int field = 1; field < log.fieldCount(); field++) {
                boolean shown = searching
                        || !(hideZero && isZero(log, field)) && !(hideConstant && isConstant(log, field));
                if (categories.group(field) == group && shown && matches(log, field, needle)) {
                    members.add(field);
                }
            }
            addSection(rows, group.title, members, searching, collapsed);
        }
        return rows;
    }

    private static boolean matches(CsvLogReader log, int field, String needle) {
        return needle.isEmpty() || log.name(field).toLowerCase(Locale.ROOT).contains(needle);
    }

    /** Empty sections are left out. */
    private static void addSection(List<Row> rows, String title, List<Integer> members, boolean searching,
                                   Set<String> collapsed) {
        if (members.isEmpty()) {
            return;
        }
        boolean expanded = searching || !collapsed.contains(title);
        rows.add(Row.heading(title, members.size(), expanded));
        if (expanded) {
            for (int field : members) {
                rows.add(Row.signal(title, field));
            }
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
