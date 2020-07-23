package com.rusefi.server;

import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonObjectBuilder;
import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;

public class Monitoring {
    public static final String STATUS = "/status";
    final FkRegex showStatistics;
    private final Backend backend;

    public Monitoring(Backend backend) {
        this.backend = backend;
        showStatistics = new FkRegex(STATUS,
                (Take) req -> getStatus());
    }

    private static String formatSize(long v) {
        if (v < 1024) return v + " B";
        int z = (63 - Long.numberOfLeadingZeros(v)) / 10;
        return String.format("%.1f %sB", (double) v / (1L << (z * 10)), " KMGTPE".charAt(z));
    }

    private RsJson getStatus() throws IOException {
        JsonObjectBuilder builder = Json.createObjectBuilder();
        OperatingSystemMXBean operatingSystemMXBean = ManagementFactory.getOperatingSystemMXBean();
        builder.add("CPU_Load", operatingSystemMXBean.getSystemLoadAverage());
        builder.add("free_ram", formatSize(Runtime.getRuntime().freeMemory()));
        builder.add("max_ram", formatSize(Runtime.getRuntime().maxMemory()));
        builder.add("threads", Thread.getAllStackTraces().size());
        builder.add("sessions", Backend.totalSessions.get());
        builder.add("serverPortForApplications", backend.serverPortForApplications);
        builder.add("serverPortForControllers", backend.serverPortForControllers);
        builder.add("applicationsCount", backend.getApplicationsCount());
        builder.add("controllersCount", backend.getControllersCount());

        return new RsJson(builder.build());
    }
}
