package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * A log's Z signal binned by its X and Y signals, e.g. lambda by RPM and MAP. Every record counts for the time
 * until the next record, so cells show where the engine spent its time whatever the logging rate, and the mean
 * and spread are weighted by that time; min and max are those of the records themselves. A gap in the log (the
 * logger pausing, a recording stopped and restarted) counts only as long as a few typical intervals, so it does
 * not pile seconds into the cell where it happened.
 */
final class Histogram {
    /** A gap counts at most this many typical record intervals. */
    static final double GAP_LIMIT = 4;

    /** The records of one X/Y bin. */
    static final class Cell {
        int count;
        /** Seconds spent in the bin. */
        double time;
        private double weightedSum;
        private double weightedSquares;
        private double plainSum;
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;

        void add(double z, double weight) {
            count++;
            time += weight;
            weightedSum += z * weight;
            weightedSquares += z * z * weight;
            plainSum += z;
            min = Math.min(min, z);
            max = Math.max(max, z);
        }

        /** Time-weighted mean; the plain mean when the records took no time (all logged at one instant). */
        double mean() {
            return time > 0 ? weightedSum / time : plainSum / count;
        }

        /** Time-weighted standard deviation. */
        double sigma() {
            if (!(time > 0)) {
                return 0;
            }
            double mean = mean();
            return Math.sqrt(Math.max(0, weightedSquares / time - mean * mean));
        }
    }

    final CsvLogReader log;
    final int xField;
    final int yField;
    final int zField;
    final HistogramAxis xAxis;
    final HistogramAxis yAxis;
    /** Indexed [y][x]; null where no record fell. */
    private final Cell[][] cells;
    private final boolean[] hidden;
    private final int first;
    private final int end;
    private final double[] weights;
    /** Records that made it into a cell. */
    final int usedRecords;
    /** Records in the time range left out by the filters. */
    final int hiddenRecords;
    /** Seconds covered by the used records. */
    final double totalTime;

    private Histogram(CsvLogReader log, int xField, int yField, int zField, HistogramAxis xAxis,
                      HistogramAxis yAxis, boolean[] hidden, int first, int end, double[] weights) {
        this.log = log;
        this.xField = xField;
        this.yField = yField;
        this.zField = zField;
        this.xAxis = xAxis;
        this.yAxis = yAxis;
        this.hidden = hidden;
        this.first = first;
        this.end = end;
        this.weights = weights;
        cells = new Cell[yAxis.size()][xAxis.size()];
        int used = 0;
        int hiddenCount = 0;
        double total = 0;
        for (int sample = first; sample < end; sample++) {
            if (hidden[sample]) {
                hiddenCount++;
                continue;
            }
            int x = xAxis.index(log.value(sample, xField));
            int y = yAxis.index(log.value(sample, yField));
            double z = log.value(sample, zField);
            if (x < 0 || y < 0 || Double.isNaN(z)) {
                continue;
            }
            if (cells[y][x] == null) {
                cells[y][x] = new Cell();
            }
            cells[y][x].add(z, weights[sample]);
            used++;
            total += weights[sample];
        }
        usedRecords = used;
        hiddenRecords = hiddenCount;
        totalTime = total;
    }

    /**
     * Bins the records from {@code start} to {@code end} seconds (NaN for the whole log) that no enabled filter
     * hides, with {@code columns} × {@code rows} evenly spaced bins over the range of those records.
     */
    static Histogram compute(CsvLogReader log, int xField, int yField, int zField, int columns, int rows,
                             boolean[] hidden, double start, double end) {
        int[] range = sampleRange(log, start, end);
        double[] xRange = valueRange(log, xField, zField, hidden, range);
        double[] yRange = valueRange(log, yField, zField, hidden, range);
        return compute(log, xField, yField, zField, HistogramAxis.even(xRange[0], xRange[1], columns),
                HistogramAxis.even(yRange[0], yRange[1], rows), hidden, start, end);
    }

    /** Bins the records with the given axes, e.g. those of a tune table. */
    static Histogram compute(CsvLogReader log, int xField, int yField, int zField, HistogramAxis xAxis,
                             HistogramAxis yAxis, boolean[] hidden, double start, double end) {
        int[] range = sampleRange(log, start, end);
        return new Histogram(log, xField, yField, zField, xAxis, yAxis, hidden, range[0], range[1], weights(log));
    }

    /** {first, end} sample indices of a time range; NaN ends mean the start or end of the log. */
    private static int[] sampleRange(CsvLogReader log, double start, double end) {
        int first = Double.isNaN(start) ? 0 : log.firstSampleAtOrAfter(start);
        // the record at the end time still counts
        int last = Double.isNaN(end) ? log.sampleCount() : log.firstSampleAtOrAfter(Math.nextUp(end));
        return new int[]{first, Math.max(first, last)};
    }

    /** {min, max} of a field over the records that would be binned; {0, 0} when there are none. */
    private static double[] valueRange(CsvLogReader log, int field, int zField, boolean[] hidden, int[] range) {
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        for (int sample = range[0]; sample < range[1]; sample++) {
            double value = log.value(sample, field);
            if (!hidden[sample] && Double.isFinite(value) && !Double.isNaN(log.value(sample, zField))) {
                min = Math.min(min, value);
                max = Math.max(max, value);
            }
        }
        return min <= max ? new double[]{min, max} : new double[]{0, 0};
    }

    /**
     * Seconds each record stands for: the time to the next record, at most {@link #GAP_LIMIT} typical intervals;
     * the last record counts one typical interval.
     */
    static double[] weights(CsvLogReader log) {
        int count = log.sampleCount();
        double typical = typicalInterval(log);
        double limit = typical * GAP_LIMIT;
        double[] weights = new double[count];
        for (int sample = 0; sample < count; sample++) {
            double dt = sample + 1 < count ? log.value(sample + 1, 0) - log.value(sample, 0) : typical;
            weights[sample] = dt > 0 ? Math.min(dt, limit) : 0;
        }
        return weights;
    }

    /** The median time between records, from up to about 10,000 evenly picked intervals. */
    static double typicalInterval(CsvLogReader log) {
        int intervals = log.sampleCount() - 1;
        if (intervals < 1) {
            return 0;
        }
        int step = Math.max(1, intervals / 10_000);
        double[] gaps = new double[(intervals + step - 1) / step];
        int n = 0;
        for (int sample = 0; sample < intervals; sample += step) {
            double gap = log.value(sample + 1, 0) - log.value(sample, 0);
            if (gap > 0) {
                gaps[n++] = gap;
            }
        }
        if (n == 0) {
            return 0;
        }
        Arrays.sort(gaps, 0, n);
        return gaps[n / 2];
    }

    /** The cell at column {@code x}, row {@code y} (both from the low end), or null when no record fell there. */
    Cell cell(int x, int y) {
        return cells[y][x];
    }

    /** The longest time spent in one cell, for scaling the occupancy colours. */
    double maxCellTime() {
        double max = 0;
        for (Cell[] row : cells) {
            for (Cell cell : row) {
                if (cell != null) {
                    max = Math.max(max, cell.time);
                }
            }
        }
        return max;
    }

    /** The most records in one cell. */
    int maxCellCount() {
        int max = 0;
        for (Cell[] row : cells) {
            for (Cell cell : row) {
                if (cell != null) {
                    max = Math.max(max, cell.count);
                }
            }
        }
        return max;
    }

    /** The share of a cell's readings the distribution spreads out; the rest are counted as outliers. */
    static final double DISTRIBUTION_LOW = 0.05;
    static final double DISTRIBUTION_HIGH = 0.95;
    /** Fewer readings than this are all spread out: too few to call any an outlier. */
    static final int DISTRIBUTION_MIN_RECORDS = 20;

    /**
     * What the cell panel shows beyond the cell's own numbers: how its Z values spread, and the visits. The
     * spread covers the 5th to 95th percentile, so one stray reading (fuel cut, a sensor at its limit) does not
     * squeeze the rest into a bar or two; readings beyond are counted as below / above.
     */
    static final class Detail {
        /** Records per bin from {@link #low} to {@link #high}. */
        final int[] distribution;
        final double low;
        final double high;
        /** Readings under {@link #low} and over {@link #high}. */
        final int below;
        final int above;
        /** The cell's time-weighted mean, for a marker on the spread. */
        final double mean;
        /** Separate stays in the cell: runs of consecutive used records. */
        final int visits;
        final double longestVisit;

        Detail(int[] distribution, double low, double high, int below, int above, double mean, int visits,
               double longestVisit) {
            this.distribution = distribution;
            this.low = low;
            this.high = high;
            this.below = below;
            this.above = above;
            this.mean = mean;
            this.visits = visits;
            this.longestVisit = longestVisit;
        }

        int total() {
            int total = below + above;
            for (int count : distribution) {
                total += count;
            }
            return total;
        }
    }

    /** Spreads the cell's Z values over {@code bins} bins (see {@link Detail}), and counts its visits. */
    Detail detail(int x, int y, int bins) {
        Cell cell = cells[y][x];
        if (cell == null) {
            return null;
        }
        double[] values = new double[cell.count];
        int n = 0;
        for (int sample = first; sample < end && n < values.length; sample++) {
            if (isIn(sample, x, y)) {
                values[n++] = log.value(sample, zField);
            }
        }
        boolean trim = n >= DISTRIBUTION_MIN_RECORDS;
        double low = trim ? HistogramColors.quantile(values, n, DISTRIBUTION_LOW) : cell.min;
        double high = trim ? HistogramColors.quantile(values, n, DISTRIBUTION_HIGH) : cell.max;
        int[] distribution = new int[bins];
        int below = 0;
        int above = 0;
        for (int i = 0; i < n; i++) {
            double z = values[i];
            if (z < low) {
                below++;
            } else if (z > high) {
                above++;
            } else {
                int bin = high > low ? (int) Math.min(bins - 1, (z - low) / (high - low) * bins) : bins / 2;
                distribution[bin]++;
            }
        }
        List<double[]> visits = visits(x, y);
        double longest = 0;
        for (double[] visit : visits) {
            longest = Math.max(longest, visit[2]);
        }
        return new Detail(distribution, low, high, below, above, cell.mean(), visits.size(), longest);
    }

    /** The record is used and falls in the cell. */
    private boolean isIn(int sample, int x, int y) {
        return !hidden[sample] && !Double.isNaN(log.value(sample, zField))
                && xAxis.index(log.value(sample, xField)) == x && yAxis.index(log.value(sample, yField)) == y;
    }

    /**
     * The separate stays in a cell, in time order: runs of used records in it. A hidden record does not end a
     * stay, the filters only skip it. Each is {start, end, seconds}: from the first record's time to the last
     * record's time plus the time it stands for, and the time counted in the cell.
     */
    List<double[]> visits(int x, int y) {
        List<double[]> visits = new ArrayList<>();
        double[] current = null;
        for (int sample = first; sample < end; sample++) {
            if (hidden[sample]) {
                continue;
            }
            if (isIn(sample, x, y)) {
                double time = log.value(sample, 0);
                if (current == null) {
                    current = new double[]{time, time, 0};
                    visits.add(current);
                }
                current[1] = time + weights[sample];
                current[2] += weights[sample];
            } else {
                current = null;
            }
        }
        return visits;
    }
}
