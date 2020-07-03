package com.rusefi;

import org.jetbrains.annotations.NotNull;

import java.text.FieldPosition;
import java.text.NumberFormat;
import java.util.*;

public final class Histograms {
    private static final String MAGIC = "total";
    public static final double H_ACCURACY = 0.05;
    public static final int BOUND_LENGTH = (int) (Math.log(Long.MAX_VALUE) / Math.log(1.0 + H_ACCURACY));

    public static final long LONG_MAX_INT = 0x7fffffffffffffffL;

    public static final double H_CONFIDENCE = 0.8;
    public static final int SBI_SIZE = 1000;

    // ======= Initialization =======

    private final HashMap<String, StatisticsGroup> total_stats = new HashMap<>();
    private final long start_time = System.currentTimeMillis();
    public final ThreadLocal<LocalStats> local_stats = ThreadLocal.withInitial(LocalStats::new);
    private final HashSet<LocalStats> all_local_stats = new HashSet<>();

    private long last_dump = System.currentTimeMillis();

    /**
     * this method updates totals & resets snapshot
     * todo: get rid of TOS 'Profiler' and make this method protected?
     *
     * @see #getCurrentStatistics()
     */
    @NotNull
    public List<String> dumpStats() {
        Collection<StatisticsGroup> values = takeAndResetSnapshot();
        List<StatisticsGroup> al = new ArrayList<>();
        al.addAll(values);
        synchronized (total_stats) {
            for (StatisticsGroup source : values) {
                String type = source.type + ".TOTAL";
                StatisticsGroup dest = total_stats.get(type);
                if (dest == null)
                    total_stats.put(type, dest = new StatisticsGroup(type));
                dest.add(source);
            }
            al.addAll(total_stats.values());
        }
        return sortAndAddTimes(al);
    }

    /**
     * this method displays currently gathered histogram without affecting state
     *
     * @see #dumpStats()
     */
    @SuppressWarnings("UnusedDeclaration")
    @NotNull
    public List<String> getCurrentStatistics() {
        Collection<StatisticsGroup> snapshot = getCurrentSnapshot();
        return sortAndAddTimes(new ArrayList<>(snapshot));
    }

    private List<String> sortAndAddTimes(List<StatisticsGroup> al) {
        Collections.sort(al, new Comparator<StatisticsGroup>() {
            public int compare(StatisticsGroup o1, StatisticsGroup o2) {
                return o1.type.compareTo(o2.type);
            }
        });
        long time = System.currentTimeMillis();
        List<String> result = new ArrayList<>();
        for (StatisticsGroup sg : al)
            result.add(toString(sg, time - (sg.type.endsWith(".TOTAL") ? start_time : last_dump)));
        last_dump = time;
        return result;
    }

    // ========= Internal Data Structures ========

    /**
     * Accuracy of histogram - the ratio of bar width to its value.
     */

    /**
     * Confidence interval to be printed together with min/max interval.
     */

    /**
     * Confidence quantity bounds for logging.
     */
    private final double[] confidence_bounds;

    /**
     * Confidence interval separators for logging.
     */
    private final String[] confidence_separators;

    /**
     * Bounds of histogram bars. Bar number 'i' covers interval
     * from bounds[i] inclusive to bounds[i + 1] exclusive.
     */
    private final long[] bounds;

    /**
     * Index for direct access to intervals with small bounds.
     */
    private final int[] small_bounds_index;


    /**
     * Construct new instance of profiler with specified parameters.
     */
    public Histograms() {

        confidence_bounds = new double[]{0.5 - H_CONFIDENCE * 0.5, 0.5, 0.5 + H_CONFIDENCE * 0.5};
        confidence_separators = new String[]{"(", " [", "-", "-", "] ", ")"};

        FileLog.MAIN.logLine("BOUND_LENGTH=" + BOUND_LENGTH);

        bounds = new long[BOUND_LENGTH];
        bounds[0] = 0;
        for (int i = 1; i < BOUND_LENGTH; i++) {
            long prev = bounds[i - 1];
            long next = prev + (long) ((double) prev * H_ACCURACY);
            if (next == prev) // Ensure minimum step for small numbers.
                next = prev + 1;
            if (next < prev) // Overflow over Long.MAX_VALUE occurred.
                next = LONG_MAX_INT;
            bounds[i] = next;
        }
        bounds[BOUND_LENGTH - 1] = LONG_MAX_INT;

        small_bounds_index = new int[SBI_SIZE];
        for (int i = 0, j = 0; j < SBI_SIZE; i++)
            while (j < bounds[i + 1] && j < SBI_SIZE)
                small_bounds_index[j++] = i;
    }

    /**
     * Returns histogram index for specified value.
     */
    public int getIndex(long value) {
        if (value < 0)
            return 0;
        if (value < small_bounds_index.length)
            return small_bounds_index[(int) value];
        int l = small_bounds_index[small_bounds_index.length - 1];
        int r = bounds.length - 1;
        while (l < r) {
            int m = (l + r) >> 1;
            if (bounds[m] > value)
                r = m - 1;
            else if (bounds[m + 1] <= value)
                l = m + 1;
            else
                return m;
        }
        return l;
    }

    public static final class LocalStats {
        public HashMap<ValueType, StatisticsGroup> stats; // Type -> StatisticsGroup
    }

    /**
     * Adds specified samples to statistics.
     */
    public void addValue(ValueType t, String name, long value) {
        String type = t.getName();
        if (value < 0)
            value = 0;
        int index = getIndex(value);
        LocalStats ls = local_stats.get();
        boolean create;
        create = ls.stats == null;
        if (create)
            ls.stats = new HashMap<>();
        StatisticsGroup sg = ls.stats.get(t);
        if (sg == null)
            ls.stats.put(t, sg = new StatisticsGroup(type));
        sg.add(name, index, value);
        if (create)
            synchronized (all_local_stats) {
                all_local_stats.add(ls);
            }
    }

    /**
     * returns current statistics without resetting anything
     */
    private Collection<StatisticsGroup> getCurrentSnapshot() {
        List<LocalStats> lss = getLocalStats(false);
        HashMap<String, StatisticsGroup> snapshot = new HashMap<>();
        for (LocalStats ls : lss) {
            // in case of a snapshot without reset, we have to merge under the lock
            mergeStats(snapshot, ls.stats);
        }
        return snapshot.values();
    }

    /**
     * Returns snapshot of all gathered statistics and clears them in process.
     */
    private Collection<StatisticsGroup> takeAndResetSnapshot() {
        List<LocalStats> lss = getLocalStats(true);
        HashMap<String, StatisticsGroup> snapshot = new HashMap<>();
        for (LocalStats ls : lss) {
            HashMap<ValueType, StatisticsGroup> stats;
            stats = ls.stats;
            /**
             * we will re-register this LocalStats on next #add() invocation
             * @see #add(String, String, int, long)
             */
            ls.stats = null;
            mergeStats(snapshot, stats);
        }
        return snapshot.values();
    }

    private static void mergeStats(HashMap<String, StatisticsGroup> snapshot, HashMap<ValueType, StatisticsGroup> stats) {
        if (stats != null)
            for (StatisticsGroup source : stats.values()) {
                StatisticsGroup dest = snapshot.get(source.type);
                if (dest == null)
                    snapshot.put(source.type, source);
                else
                    dest.add(source);
            }
    }

    private List<LocalStats> getLocalStats(boolean reset) {
        List<LocalStats> lss;
        synchronized (all_local_stats) {
            lss = new ArrayList<>(all_local_stats);
            if (reset)
                all_local_stats.clear();
        }
        return lss;
    }

    /**
     * Sorts specified statistics of specified statistics group for logging.
     */
    private static void sortStatistics(StatisticsGroup sg, Statistics[] sts) {
        final boolean use_total = sg.type.startsWith(MAGIC);
        Arrays.sort(sts, new Comparator<Statistics>() {
            public int compare(Statistics st1, Statistics st2) {
                if (use_total && st1.total_value != st2.total_value)
                    return st1.total_value < st2.total_value ? -1 : 1;
                return st1.name.compareTo(st2.name);
            }
        });
    }

    // ========== Formatting Methods ==========

    private static final double[] multipliers = {1.0, 10.0, 100.0};
    private final NumberFormat formatter = NumberFormat.getNumberInstance();
    private final FieldPosition field = new FieldPosition(0);

    /**
     * Formats specified value into specified string buffer. Not thread-safe.
     */
    private StringBuffer format(double d, StringBuffer sb) {
        int i = Math.abs(d) < 9.995 ? 2 : Math.abs(d) < 99.95 ? 1 : 0;
        formatter.setMinimumFractionDigits(0);
        formatter.setMaximumFractionDigits(i);
        return formatter.format(Math.floor(d * multipliers[i] + 0.5) / multipliers[i], sb, field);
    }

    /**
     * Appends header for specified statistics group for logging.
     */
    private void appendHeader(StringBuffer sb, StatisticsGroup sg, long duration) {
        double time = duration;
        String time_scale = "ms";
        if (time >= 24 * 3600 * 1000 * 0.995) {
            time /= 24.0 * 3600.0 * 1000.0;
            time_scale = "days";
        } else if (time >= 3600 * 1000 * 0.995) {
            time /= 3600.0 * 1000.0;
            time_scale = "hours";
        } else if (time >= 60 * 1000 * 0.995) {
            time /= 60.0 * 1000.0;
            time_scale = "min";
        } else if (time >= 1000 * 0.995) {
            time /= 1000.0;
            time_scale = "sec";
        }

        sb.append(sg.type).append(" statistics for ");
        format(time, sb).append(time_scale).append(" with ");
        format(H_ACCURACY * 100.0, sb).append("% accuracy and ");
        format(H_CONFIDENCE * 100.0, sb).append("% confidence:");
    }

    /**
     * Appends specified statistics (main part) for logging.
     */
    public void appendStatistics(StringBuffer sb, Statistics st, List<Long> report) {
        format(st.total_value, sb).append(" / ").append(st.total_count).append(" = ");
        format((double) st.total_value / (double) st.total_count, sb);

        if (st.total_count < 0) {
            sb.append("Total count is less then ZERO!\n");
            return;
        }

        if (st.total_count <= 5) {
            // There are too little samples for statistics.
            sb.append(" are");
            for (int j = 0; j < st.histogram.length; j++)
                for (int k = 0; k < st.histogram[j]; k++) {
                    sb.append(' ');
                    format((bounds[j] + bounds[j + 1]) / 2, sb);
                }
            return;
        }

        // Print full min/confidence/max statistics.
        sb.append(" in ").append(confidence_separators[0]);
        int min = 0;
        while (st.histogram[min] == 0)
            min++;
        // 'min' is index of interval with min sample.
        format(bounds[min], sb).append(confidence_separators[1]);
        report.add(bounds[min]);


        long acc = 0;
        // 'acc' is accumulated number of samples in [0, min - 1].
        for (int j = 0; j < confidence_bounds.length; j++) {
            long k = (long) Math.floor((double) st.total_count * confidence_bounds[j]);
            // Always drop at least 1 'non-confident' sample...
            if (k == 0)
                k = 1;
            if (k == st.total_count)
                k = st.total_count - 1;
            // 'k' is desired number of samples.
            while (acc + st.histogram[min] < k)
                acc += st.histogram[min++];
            if (k < st.total_count / 2) // Converge to median (from left).
                while (acc + st.histogram[min] <= k)
                    acc += st.histogram[min++];
            // Now: acc <= k <= acc + st.histogram[min]
            // And desired number of samples is within [min, min + 1)
            double d = bounds[min];
            if (acc != k)
                d += (double) (bounds[min + 1] - 1 - bounds[min]) *
                        (double) (k - acc) /
                        (double) st.histogram[min];
            format(d, sb).append(confidence_separators[j + 2]);
            report.add((long)d);
        }

        int max = st.histogram.length - 1;
        while (st.histogram[max] == 0)
            max--;
        // 'max' is index of interval with max sample.
        long maxValue = bounds[max + 1] - 1;
        format(maxValue, sb).append(confidence_separators[5]);
        report.add(maxValue);
    }

    /**
     * Formats specified statistics group for logging.
     */
    private String toString(StatisticsGroup sg, long duration) {
        StringBuffer sb = new StringBuffer(100 + sg.data.size() * 100);
        appendHeader(sb, sg, duration);
        Statistics[] sts = sg.data.values().toArray(new Statistics[sg.data.size()]);
        sortStatistics(sg, sts);
        for (Statistics st : sts) {
            appendStatistics(sb, st, new ArrayList<>());
        }
        return sb.toString();
    }

    // ========== Internal Classes ==========

    /**
     * Performance statistics for single operation.
     */
    public static class Statistics {
        private static final long[] EMPTY_HISTOGRAM = new long[0];

        public final String name;

        public long total_value;
        public long total_count;
        public long[] histogram;

        private Statistics(String name) {
            this.name = name;
            this.histogram = EMPTY_HISTOGRAM;
        }

        private Statistics(Statistics st) {
            this.name = st.name;
            this.total_value = st.total_value;
            this.total_count = st.total_count;
            this.histogram = st.histogram.length == 0 ? EMPTY_HISTOGRAM : st.histogram.clone();
        }

        public void add(int index, int count, long value) {
            total_value += value;
            total_count += count;
            if (index >= histogram.length) {
                int new_length = Math.max(histogram.length, 10);
                while (index >= new_length)
                    new_length = new_length << 1;
                long[] new_histogram = new long[new_length];
                System.arraycopy(histogram, 0, new_histogram, 0, histogram.length);
                histogram = new_histogram;
            }
            histogram[index] += count;
        }

        public void add(Statistics st) {
            total_value += st.total_value;
            total_count += st.total_count;
            long[] hist = st.histogram;
            if (histogram.length < st.histogram.length) {
                hist = histogram;
                histogram = st.histogram.clone();
            }
            for (int i = hist.length; --i >= 0; )
                histogram[i] += hist[i];
        }
    }

    /**
     * Group of statistics of the same type.
     */
    @SuppressWarnings("ForLoopReplaceableByForEach")
    public static class StatisticsGroup {
        public final String type;
        public final HashMap<String, Statistics> data; // Maps statistics name (String) to Statistics.

        private StatisticsGroup(String type) {
            this.type = type;
            this.data = new HashMap<>();
        }

        public void add(String name, int index, long value) {
            Statistics st = data.get(name);
            if (st == null)
                data.put(name, st = new Statistics(name));
            st.add(index, 1, value);
        }

        public void add(StatisticsGroup sg) {
            for (Iterator<Statistics> it = sg.data.values().iterator(); it.hasNext(); ) {
                Statistics source = it.next();
                Statistics dest = data.get(source.name);
                if (dest == null)
                    data.put(source.name, new Statistics(source));
                else
                    dest.add(source);
            }
        }
    }

    /**
     * The type of a value.
     */
    public enum ValueType {
        INVOCATION("Invocation");
        /**
         * Value type that is used to mean a method call.
         */

        private final String name;

        /**
         * Gets the name of this {@link ValueType}.
         *
         * @return the name.
         */
        public String getName() {
            return name;
        }

        ValueType(String name) {
            this.name = name;
        }
    }
}
