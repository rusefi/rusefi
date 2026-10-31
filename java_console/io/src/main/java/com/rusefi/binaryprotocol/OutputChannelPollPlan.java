package com.rusefi.binaryprotocol;

import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.OutputChannelDemand;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

/** Byte ranges and direct channel roots required to satisfy an output-channel demand. */
public final class OutputChannelPollPlan {
    private static final int MERGE_GAP_BYTES = 16;
    private static final String RUNTIME_DATA_RATE_GAUGE = "runtimedatarategauge";

    private final List<Range> ranges;
    private final Set<String> requestedChannelRoots;
    private final boolean full;
    private final long generation;

    private OutputChannelPollPlan(List<Range> ranges, Set<String> requestedChannelRoots, boolean full, long generation) {
        this.ranges = Collections.unmodifiableList(new ArrayList<>(ranges));
        this.requestedChannelRoots = Collections.unmodifiableSet(new LinkedHashSet<>(requestedChannelRoots));
        this.full = full;
        this.generation = generation;
    }

    public static OutputChannelPollPlan create(IniFileModel ini, OutputChannelDemand demand) {
        int blockSize = ini.getMetaInfo().getOchBlockSize();
        if (demand.isFull() || demand.getChannels().isEmpty()) {
            return full(blockSize, demand.getGeneration());
        }

        Map<String, IniField> outputChannels = normalize(ini.getAllOutputChannels());
        Map<String, String> expressions = normalize(ini.getExpressionOutputChannels());
        Map<String, GaugeModel> gauges = normalize(ini.getGauges());
        List<Range> ranges = new ArrayList<>();
        Set<String> roots = new LinkedHashSet<>();
        Set<String> visiting = new HashSet<>();

        for (String channel : demand.getChannels()) {
            if (!resolve(channel, ini, outputChannels, expressions, gauges, visiting, roots, ranges, blockSize)) {
                return full(blockSize, demand.getGeneration());
            }
        }
        // A successful output poll must still perform ECU I/O for link health and rate timing.
        if (ranges.isEmpty()) {
            return full(blockSize, demand.getGeneration());
        }
        return new OutputChannelPollPlan(merge(ranges), roots, false, demand.getGeneration());
    }

    private static boolean resolve(String name, IniFileModel ini, Map<String, IniField> outputChannels,
                                   Map<String, String> expressions, Map<String, GaugeModel> gauges,
                                   Set<String> visiting, Set<String> roots, List<Range> ranges, int blockSize) {
        if (name == null) {
            return false;
        }
        String key = normalize(name);
        IniField field = outputChannels.get(key);
        if (field != null) {
            int offset = field.getOffset();
            int size = field.getSize();
            if (offset < 0 || size <= 0 || offset > blockSize - size) {
                return false;
            }
            roots.add(key);
            ranges.add(new Range(offset, size));
            return true;
        }
        if (RUNTIME_DATA_RATE_GAUGE.equals(key)) {
            return true;
        }
        GaugeModel gauge = gauges.get(key);
        if (gauge != null) {
            return resolve(gauge.getChannel(), ini, outputChannels, expressions, gauges, visiting, roots, ranges, blockSize);
        }
        String expression = ExpressionEvaluator.looksLikeExpression(name) ? name : expressions.get(key);
        if (expression == null) {
            // Variables backed by configuration pages do not consume output-channel bytes.
            return ini.findIniField(name).isPresent();
        }
        if (!visiting.add(key)) {
            return false;
        }
        for (String variable : ExpressionEvaluator.extractVariables(expression)) {
            if (!resolve(variable, ini, outputChannels, expressions, gauges, visiting, roots, ranges, blockSize)) {
                visiting.remove(key);
                return false;
            }
        }
        visiting.remove(key);
        return true;
    }

    private static List<Range> merge(List<Range> ranges) {
        if (ranges.isEmpty()) {
            return ranges;
        }
        ranges.sort(Comparator.comparingInt(Range::getOffset));
        List<Range> merged = new ArrayList<>();
        Range current = ranges.get(0);
        for (int i = 1; i < ranges.size(); i++) {
            Range next = ranges.get(i);
            if (next.offset <= (long) current.offset + current.size + MERGE_GAP_BYTES) {
                current = new Range(current.offset, Math.max(current.offset + current.size, next.offset + next.size) - current.offset);
            } else {
                merged.add(current);
                current = next;
            }
        }
        merged.add(current);
        return merged;
    }

    private static OutputChannelPollPlan full(int blockSize, long generation) {
        return new OutputChannelPollPlan(Collections.singletonList(new Range(0, Math.max(0, blockSize))),
                Collections.emptySet(), true, generation);
    }

    private static <T> Map<String, T> normalize(Map<String, T> source) {
        Map<String, T> result = new HashMap<>();
        for (Map.Entry<String, T> entry : source.entrySet()) {
            result.put(normalize(entry.getKey()), entry.getValue());
        }
        return result;
    }

    private static String normalize(String name) {
        return name.toLowerCase(Locale.US);
    }

    public List<Range> getRanges() {
        return ranges;
    }

    public Set<String> getRequestedChannelRoots() {
        return requestedChannelRoots;
    }

    public boolean isFull() {
        return full;
    }

    public long getGeneration() {
        return generation;
    }

    public static final class Range {
        private final int offset;
        private final int size;

        public Range(int offset, int size) {
            if (offset < 0 || size < 0) {
                throw new IllegalArgumentException("offset and size must be non-negative");
            }
            this.offset = offset;
            this.size = size;
        }

        public int getOffset() {
            return offset;
        }

        public int getSize() {
            return size;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Range)) return false;
            Range range = (Range) o;
            return offset == range.offset && size == range.size;
        }

        @Override
        public int hashCode() {
            return 31 * offset + size;
        }
    }
}
