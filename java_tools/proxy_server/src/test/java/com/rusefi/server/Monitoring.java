package com.rusefi.server;

import com.devexperts.logging.Logging;
import com.rusefi.Version;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.tools.online.ProxyClient;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonObjectBuilder;
import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.util.Date;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class Monitoring {
    public static final String STATUS = "/status";
    private static final Logging log = Logging.getLogging(Monitoring.class);
    private static final int PERIOD = 10 * SECOND;
    final FkRegex showStatistics;
    private final Backend backend;

    private final Birthday birthday = new Birthday();

    public Monitoring(Backend backend) {
        this.backend = backend;
        showStatistics = new FkRegex(STATUS, (Take) req -> getStatus());
        startHeartbeatThread();
    }

    private static String formatSize(long v) {
        if (v < 1024) return v + " B";
        int z = (63 - Long.numberOfLeadingZeros(v)) / 10;
        return String.format("%.1f %sB", (double) v / (1L << (z * 10)), " KMGTPE".charAt(z));
    }

    public void startHeartbeatThread() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (!backend.isClosed()) {
                    writeLogEntry();
                    sleep(PERIOD);
                }
            }
        }, "heartbeat").start();
    }

    private void writeLogEntry() {
        OperatingSystemMXBean operatingSystemMXBean = ManagementFactory.getOperatingSystemMXBean();
        log.info("cpu=" + operatingSystemMXBean.getSystemLoadAverage() +
                ",free=" + Runtime.getRuntime().freeMemory() +
                ",sessions=" + Backend.totalSessions.get() +
                ",threads=" + Thread.getAllStackTraces().size());
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
        builder.add("backend version", ProxyClient.BACKEND_VERSION);
        builder.add("framework version", Version.CONSOLE_VERSION);
        builder.add("compiled", new Date(rusEFIVersion.classBuildTimeMillis()).toString());
        builder.add("now", System.currentTimeMillis());
        builder.add(SessionDetails.AGE, birthday.getDuration());

        return new RsJson(builder.build());
    }
}
