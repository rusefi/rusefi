package com.rusefi.server;

import com.devexperts.logging.Logging;
import com.rusefi.Listener;
import com.rusefi.LocalApplicationProxy;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Sensor;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import net.jcip.annotations.GuardedBy;
import org.jetbrains.annotations.NotNull;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.facets.fork.TkFork;
import org.takes.http.*;
import org.takes.rs.RsHtml;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import javax.json.JsonObjectBuilder;
import java.io.Closeable;
import java.io.IOException;
import java.net.BindException;
import java.util.*;
import java.util.concurrent.atomic.AtomicLong;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.tools.online.HttpUtil.getIntProperty;

/**
 * See NetworkConnectorStartup - NetworkConnector connects an ECU to this backend
 *
 * @see ControllerConnectionState ECU session
 * @see ApplicationConnectionState tuning application session
 * @see BackendLauncher
 */
public class Backend implements Closeable {
    private static final Logging log = getLogging(Backend.class);

    /**
     * @see HttpUtil#PROXY_JSON_API_HTTP_PORT
     * @see LocalApplicationProxy#SERVER_PORT_FOR_APPLICATIONS
     */
    public static final int SERVER_PORT_FOR_CONTROLLERS = getIntProperty("controllers.port", 8003);
    private static final String MAX_PACKET_GAP = "MAX_PACKET_GAP";

    /**
     * Application with exclusive access should connect tuning application within 3 minutes
     * Should be at least twice less than USER_IO_TIMEOUT to prevent controller disconnect
     *
     * @see BinaryProtocolProxy#USER_IO_TIMEOUT
     */
    private static final int APPLICATION_INACTIVITY_TIMEOUT = 3 * Timeouts.MINUTE;
    static final String AGE = "age";

    private final FkRegex showOnlineControllers = new FkRegex(ProxyClient.LIST_CONTROLLERS_PATH,
            (Take) req -> getControllersOnline()
    );
    private final FkRegex showOnlineApplications = new FkRegex(ProxyClient.LIST_APPLICATIONS_PATH,
            (Take) req -> getApplicationsOnline()
    );

    private boolean isClosed;

    // for simplicity let's have shared locks for both applications and controllers, should be
    // good enough to begin with
    private final Object lock = new Object();

    @GuardedBy("lock")
    private final Set<ControllerConnectionState> controllers = new HashSet<>();
    @GuardedBy("lock")
    private final HashMap<ControllerKey, ControllerConnectionState> controllersByKey = new HashMap<>();
    @GuardedBy("lock")
    private final Set<ApplicationConnectionState> applications = new HashSet<>();

    private final int applicationTimeout;
    private final UserDetailsResolver userDetailsResolver;
    public final static AtomicLong totalSessions = new AtomicLong();
    public int serverPortForApplications;
    public int serverPortForControllers;

    public Backend(UserDetailsResolver userDetailsResolver, int httpPort) {
        this(userDetailsResolver, httpPort, APPLICATION_INACTIVITY_TIMEOUT);
    }

    public Backend(UserDetailsResolver userDetailsResolver, int httpPort, int applicationTimeout) {
        this.applicationTimeout = applicationTimeout;
        this.userDetailsResolver = userDetailsResolver;


        new Thread(() -> {
            try {
                log.info("Starting http backend on " + httpPort);
                try {
                    Take forkTake = new TkFork(showOnlineControllers,
                            showOnlineApplications,
                            new Monitoring(this).showStatistics,
                            new FkRegex(ProxyClient.VERSION_PATH, ProxyClient.BACKEND_VERSION),
                            new FkRegex("/", new RsHtml("<html><body>\n" +
                                    "<br/><a href='https://rusefi.com/online/'>rusEFI Online</a>\n" +
                                    "<br/><br/><br/>\n" +
                                    "<img src='https://rusefi.com/style/rusefi_online_color.png'/>" +
                                    "<br/><br/><br/>\n" +
                                    "<br/><br/><br/><a href='" + Monitoring.STATUS + "'>Status</a>\n" +
                                    "<br/><br/><br/><a href='" + ProxyClient.VERSION_PATH + "'>Version</a>\n" +
                                    "<br/><br/><br/><a href='" + ProxyClient.LIST_CONTROLLERS_PATH + "'>Controllers</a>\n" +
                                    "<br/><br/><br/><a href='" + ProxyClient.LIST_APPLICATIONS_PATH + "'>Applications</a>\n" +
                                    "</body></html>\n"))
                    );
                    Front frontEnd = new FtBasic(new BkParallel(new BkSafe(new BkBasic(forkTake)), 4), httpPort);
                    frontEnd.start(() -> isClosed);
                } catch (BindException e) {
                    throw new IllegalStateException("While binding " + httpPort, e);
                }
                log.info("Shutting down backend on port " + httpPort);
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }

        }, "Http Server Thread").start();

        new Thread(() -> {
            while (true) {
                log.info(getApplicationsCount() + " applications, " + getControllersCount() + " controllers");
                runApplicationConnectionsCleanup();
                BinaryProtocol.sleep(applicationTimeout);
            }
        }, "rusEFI Application connections Cleanup").start();

        new Thread(() -> {
            while (true) {
                grabOutputs();
                BinaryProtocol.sleep(SECOND);
            }
        }, "rusEFI gauge poker").start();
    }

    private void grabOutputs() {
        List<ControllerConnectionState> controllers = getControllers();
        for (ControllerConnectionState controller : controllers) {
            if (System.currentTimeMillis() - controller.getStream().getStreamStats().getPreviousPacketArrivalTime() > 20 * SECOND) {
                if (controller.getTwoKindSemaphore().acquireForShortTermUsage()) {
                    try {
                        controller.grabOutputs(this);
                    } finally {
                        controller.getTwoKindSemaphore().releaseFromShortTermUsage();
                    }
                }
            }
        }
    }

    public void runApplicationConnector(int serverPortForApplications, Listener<?> serverSocketCreationCallback) {
        this.serverPortForApplications = serverPortForApplications;
        // connection from authenticator app which proxies for Tuner Studio
        // authenticator pushed hello packet on connect
        log.info("Starting application connector at " + serverPortForApplications);
        BinaryProtocolServer.tcpServerSocket(applicationSocket -> () -> {
            log.info("new application connection!");
            totalSessions.incrementAndGet();
            // connection from authenticator app which proxies for Tuner Studio
            IoStream applicationClientStream = null;
            ApplicationConnectionState applicationConnectionState = null;
            try {
                applicationClientStream = new TcpIoStream("[app] ", applicationSocket);

                // authenticator pushed hello packet on connect
                String jsonString = HelloCommand.getHelloResponse(applicationClientStream.getDataBuffer());
                if (jsonString == null) {
                    log.error("ERROR: null HELLO");
                    return;
                }
                ApplicationRequest applicationRequest = ApplicationRequest.valueOf(jsonString);
                log.info("Application Connected: " + applicationRequest);
                String authToken = applicationRequest.getSessionDetails().getAuthToken();
                UserDetails userDetails = userDetailsResolver.apply(authToken);
                if (userDetails == null) {
                    log.info("Authentication failed for application " + authToken);
                    return;
                }

                ControllerKey controllerKey = new ControllerKey(applicationRequest.getTargetUserId(), applicationRequest.getSessionDetails().getControllerInfo());
                ControllerConnectionState state;
                synchronized (lock) {
                    state = acquire(controllerKey, userDetails);
                }
                if (state == null) {
                    log.info("No controller for " + controllerKey);
                    return;
                }
                applicationConnectionState = new ApplicationConnectionState(userDetails, applicationClientStream, state);
                synchronized (lock) {
                    applications.add(applicationConnectionState);
                }

                BinaryProtocolProxy.runProxy(state.getStream(), applicationClientStream);

            } catch (Throwable e) {
                log.info("Application Connector: Got error " + e);
            } finally {
                if (applicationClientStream != null)
                    applicationClientStream.close();
                close(applicationConnectionState);
            }
        }, serverPortForApplications, "ApplicationServer", serverSocketCreationCallback, BinaryProtocolServer.SECURE_SOCKET_FACTORY);
    }

    private ControllerConnectionState acquire(ControllerKey controllerKey, UserDetails userDetails) {
        synchronized (lock) {
            ControllerConnectionState state = controllersByKey.get(controllerKey);
            if (state == null) {
                // no such controller
                return null;
            }
            if (!state.getTwoKindSemaphore().acquireForLongTermUsage(userDetails)) {
                // someone is already talking to this controller
                return null;
            }
            return state;
        }
    }

    protected void close(ApplicationConnectionState applicationConnectionState) {
        if (applicationConnectionState != null)
            applicationConnectionState.close();
        onDisconnectApplication(applicationConnectionState);
    }

    protected void onDisconnectApplication(ApplicationConnectionState applicationConnectionState) {
        if (applicationConnectionState != null) {
            synchronized (lock) {
                applications.remove(applicationConnectionState);
            }
        }
        log.info("Disconnecting application " + applicationConnectionState);
    }

    public void runControllerConnector(int serverPortForControllers, Listener<?> serverSocketCreationCallback) {
        this.serverPortForControllers = serverPortForControllers;
        log.info("Starting controller connector at " + serverPortForControllers);
        BinaryProtocolServer.tcpServerSocket(controllerSocket -> () -> {
            totalSessions.incrementAndGet();
            ControllerConnectionState controllerConnectionState = new ControllerConnectionState(controllerSocket, getUserDetailsResolver());
            try {
                controllerConnectionState.requestControllerInfo();

                // IMPORTANT: has to happen before we register controller while we still have exclusive access
                controllerConnectionState.getOutputs();

                register(controllerConnectionState);
            } catch (Throwable e) {
                close(controllerConnectionState);
            }
        }, serverPortForControllers, "ControllerServer", serverSocketCreationCallback, BinaryProtocolServer.SECURE_SOCKET_FACTORY);
    }

    @NotNull
    private RsJson getApplicationsOnline() throws IOException {
        JsonArrayBuilder builder = Json.createArrayBuilder();
        List<ApplicationConnectionState> applications = getApplications();
        for (ApplicationConnectionState application : applications) {
            JsonObject applicationObject = Json.createObjectBuilder()
                    .add(UserDetails.USER_ID, application.getUserDetails().getUserId())
                    .add(UserDetails.USERNAME, application.getUserDetails().getUserName())
                    .add(AGE, application.getBirthday().getDuration())
                    .add(MAX_PACKET_GAP, application.getClientStream().getStreamStats().getMaxPacketGap())
                    .build();
            builder.add(applicationObject);
        }
        return new RsJson(builder.build());
    }

    @NotNull
    private RsJson getControllersOnline() throws IOException {
        JsonArrayBuilder builder = Json.createArrayBuilder();
        List<ControllerConnectionState> clients = getControllers();
        for (ControllerConnectionState client : clients) {
            // todo: at the moment we use current OutputChannel layout - a better way would be to take
            // todo: OutputChannel from .ini file based on controller signature
            int rpm = (int) client.getSensorsHolder().getValue(Sensor.RPM);
            double clt = client.getSensorsHolder().getValue(Sensor.CLT);
            UserDetails owner = client.getTwoKindSemaphore().getOwner();
            JsonObjectBuilder objectBuilder = Json.createObjectBuilder()
                    .add(UserDetails.USER_ID, client.getUserDetails().getUserId())
                    .add(UserDetails.USERNAME, client.getUserDetails().getUserName())
                    .add(AGE, client.getBirthday().getDuration())
                    .add(ProxyClient.IS_USED, client.getTwoKindSemaphore().isUsed())
                    .add(ControllerStateDetails.RPM, rpm)
                    .add(ControllerStateDetails.CLT, clt)
                    .add(ControllerInfo.SIGNATURE, client.getSessionDetails().getControllerInfo().getSignature())
                    .add(ControllerInfo.VEHICLE_NAME, client.getSessionDetails().getControllerInfo().getVehicleName())
                    .add(ControllerInfo.ENGINE_MAKE, client.getSessionDetails().getControllerInfo().getEngineMake())
                    .add(ControllerInfo.ENGINE_CODE, client.getSessionDetails().getControllerInfo().getEngineCode())
                    .add(MAX_PACKET_GAP, client.getStream().getStreamStats().getMaxPacketGap());
            if (owner != null) {
                objectBuilder = objectBuilder.add(ProxyClient.OWNER, owner.getUserName());
            }

            JsonObject controllerObject = objectBuilder
                    .build();
            builder.add(controllerObject);
        }
        return new RsJson(builder.build());
    }

    public UserDetailsResolver getUserDetailsResolver() {
        return userDetailsResolver;
    }

    /**
     * we do not push anything into connected applications so we have to run a clean-up loop
     * that's different from controllers since we periodically pull outputs from controllers which allows us to detect disconnects
     */
    private void runApplicationConnectionsCleanup() {
        List<ApplicationConnectionState> inactiveApplications = new ArrayList<>();

        synchronized (lock) {
            long now = System.currentTimeMillis();
            for (ApplicationConnectionState client : applications) {
                if (now - client.getClientStream().getStreamStats().getPreviousPacketArrivalTime() > applicationTimeout)
                    inactiveApplications.add(client);
            }
        }

        for (ApplicationConnectionState inactiveClient : inactiveApplications) {
            log.error("Kicking out application " + inactiveClient);
            close(inactiveClient);
        }
    }

    public void register(ControllerConnectionState controllerConnectionState) {
        Objects.requireNonNull(controllerConnectionState.getControllerKey(), "ControllerKey");
        synchronized (lock) {
            controllers.add(controllerConnectionState);
            controllersByKey.put(controllerConnectionState.getControllerKey(), controllerConnectionState);
        }
        onRegister(controllerConnectionState);
    }

    protected void onRegister(ControllerConnectionState controllerConnectionState) {
    }

    public void close(ControllerConnectionState inactiveClient) {
        inactiveClient.close();
        synchronized (lock) {
            // in case of exception in the initialization phase we do not even add client into the the collection
            controllers.remove(inactiveClient);
            controllersByKey.remove(inactiveClient.getControllerKey());
        }
    }

    @Override
    public void close() {
        isClosed = true;
    }

    public boolean isClosed() {
        return isClosed;
    }

    public List<ControllerConnectionState> getControllers() {
        synchronized (lock) {
            return new ArrayList<>(controllers);
        }
    }

    public List<ApplicationConnectionState> getApplications() {
        synchronized (lock) {
            return new ArrayList<>(applications);
        }
    }

    public int getControllersCount() {
        synchronized (lock) {
            return controllers.size();
        }
    }

    public int getApplicationsCount() {
        synchronized (lock) {
            return applications.size();
        }
    }
}
