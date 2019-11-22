package com.rusefi.tracing;

public class Entry {
    private final String name;
    private final Phase phase;
    private final double timestampSeconds;

    public Entry(String name, Phase phase, double timestampSeconds) {
        this.name = name;
        this.phase = phase;
        this.timestampSeconds = timestampSeconds;
    }

    private static void AppendKeyValuePair(StringBuilder sb, String x, String y) {
        sb.append('"');
        sb.append(x);
        sb.append("\":\"");
        sb.append(y);
        sb.append('"');
    }

    private static void AppendKeyValuePair(StringBuilder sb, String x, int y) {
        sb.append('"');
        sb.append(x);
        sb.append("\":");
        sb.append(y);
    }

    private static void AppendKeyValuePair(StringBuilder sb, String x, double y) {
        sb.append('"');
        sb.append(x);
        sb.append("\":");
        sb.append(y);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        sb.append("{");
        AppendKeyValuePair(sb, "name", name);

        sb.append(",");
        AppendKeyValuePair(sb, "ph", phase.toString());
        sb.append(",");
        AppendKeyValuePair(sb, "tid", 0);
        sb.append(",");

        AppendKeyValuePair(sb, "pid", 0);
        sb.append(",");
        AppendKeyValuePair(sb, "ts", timestampSeconds);
        sb.append("}");

        return sb.toString();
    }
}
