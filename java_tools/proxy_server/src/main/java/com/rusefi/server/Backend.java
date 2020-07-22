package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.Listener;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.tools.online.ProxyClient;
import net.jcip.annotations.GuardedBy;
import org.jetbrains.annotations.NotNull;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.facets.fork.TkFork;
import org.takes.http.FtBasic;
import org.takes.rs.RsHtml;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import java.io.Closeable;
import java.io.IOException;
import java.net.BindException;
import java.util.*;
import java.util.concurrent.atomic.AtomicLong;

/**
 * See NetworkConnectorStartup
 */
public class Backend implements Closeable {
    public static final int SERVER_PORT_FOR_CONTROLLERS = 8003;
    private static final String MAX_PACKET_GAP = "MAX_PACKET_GAP";
    private static final String IS_USED = "isUsed";

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
    private final Logger logger;
    public final static AtomicLong totalSessions = new AtomicLong();
    public int serverPortForApplications;
    public int serverPortForControllers;

    public Backend(UserDetailsResolver userDetailsResolver, int httpPort, Logger logger) {
        this(userDetailsResolver, httpPort, logger, Timeouts.READ_IMAGE_TIMEOUT);
    }

    public Backend(UserDetailsResolver userDetailsResolver, int httpPort, Logger logger, int applicationTimeout) {
        this.applicationTimeout = applicationTimeout;
        this.userDetailsResolver = userDetailsResolver;
        this.logger = logger;


        new Thread(() -> {
            try {
                System.out.println("Starting http backend on " + httpPort);
                try {
                    new FtBasic(
                            new TkFork(showOnlineControllers,
                                    showOnlineApplications,
                                    new Monitoring(this).showStatistics,
                                    new FkRegex(ProxyClient.VERSION_PATH, ProxyClient.BACKEND_VERSION),
                                    new FkRegex("/", new RsHtml("<html><body>\n" +
                                            "<a href='https://rusefi.com/online/'>rusEFI Online</a>\n" +
                                            "<br/>\n" +
                                            "<a href='" + Monitoring.STATUS + "'>Status</a>\n" +
                                            "<br/>\n" +
                                            "<a href='" + ProxyClient.VERSION_PATH + "'>Version</a>\n" +
                                            "<a href='" + ProxyClient.LIST_CONTROLLERS_PATH + "'>Controllers</a>\n" +
                                            "<a href='" + ProxyClient.LIST_APPLICATIONS_PATH + "'>Applications</a>\n" +
                                            "<br/>\n" +
                                            "<br/>\n" +
                                            "</body></html>\n"))
                            ), httpPort
                    ).start(() -> isClosed);
                } catch (BindException e) {
                    throw new IllegalStateException("While binding " + httpPort, e);
                }
                logger.info("Shutting down backend on port " + httpPort);
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }

        }, "Http Server Thread").start();

        new Thread(() -> {
            while (true) {
                runCleanup();
                BinaryProtocol.sleep(applicationTimeout);
            }
        }, "rusEFI Application connections Cleanup").start();
    }

    public void runApplicationConnector(int serverPortForApplications, Listener<?> serverSocketCreationCallback) {
        this.serverPortForApplications = serverPortForApplications;
        // connection from authenticator app which proxies for Tuner Studio
        // authenticator pushed hello packet on connect
        System.out.println("Starting application connector at " + serverPortForApplications);
        BinaryProtocolServer.tcpServerSocket(logger, applicationSocket -> () -> {
            totalSessions.incrementAndGet();
            // connection from authenticator app which proxies for Tuner Studio
            IoStream applicationClientStream = null;
            ApplicationConnectionState applicationConnectionState = null;
            try {
                applicationClientStream = new TcpIoStream("[app] ", logger, applicationSocket);

                // authenticator pushed hello packet on connect
                String jsonString = HelloCommand.getHelloResponse(applicationClientStream.getDataBuffer(), logger);
                if (jsonString == null)
                    return;
                ApplicationRequest applicationRequest = ApplicationRequest.valueOf(jsonString);
                logger.info("Application Connected: " + applicationRequest);
                String authToken = applicationRequest.getSessionDetails().getAuthToken();
                UserDetails userDetails = userDetailsResolver.apply(authToken);
                if (userDetails == null) {
                    logger.info("Authentication failed for application " + authToken);
                    return;
                }

                ControllerKey controllerKey = new ControllerKey(applicationRequest.getTargetUserId(), applicationRequest.getSessionDetails().getControllerInfo());
                ControllerConnectionState state;
                synchronized (lock) {
                    state = acquire(controllerKey);
                }
                if (state == null) {
                    logger.info("No controller for " + controllerKey);
                    return;
                }
                applicationConnectionState = new ApplicationConnectionState(userDetails, applicationClientStream, state);
                synchronized (lock) {
                    applications.add(applicationConnectionState);
                }

                BinaryProtocolProxy.runProxy(state.getStream(), applicationClientStream);

            } catch (Throwable e) {
                logger.info("Application Connector: Got error " + e);
            } finally {
                if (applicationClientStream != null)
                    applicationClientStream.close();
                close(applicationConnectionState);
            }
        }, serverPortForApplications, "ApplicationServer", serverSocketCreationCallback, BinaryProtocolServer.SECURE_SOCKET_FACTORY);
    }

    private ControllerConnectionState acquire(ControllerKey controllerKey) {
        synchronized (lock) {
            ControllerConnectionState state = controllersByKey.get(controllerKey);
            if (state == null) {
                // no such controller
                return null;
            }
            if (!state.acquire()) {
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
        logger.info("Disconnecting application");
    }

    public void runControllerConnector(int serverPortForControllers, Listener<?> serverSocketCreationCallback) {
        this.serverPortForControllers = serverPortForControllers;
        logger.info("Starting controller connector at " + serverPortForControllers);
        BinaryProtocolServer.tcpServerSocket(logger, controllerSocket -> () -> {
            totalSessions.incrementAndGet();
            ControllerConnectionState controllerConnectionState = new ControllerConnectionState(controllerSocket, logger, getUserDetailsResolver());
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
            JsonObject controllerObject = Json.createObjectBuilder()
                    .add(UserDetails.USER_ID, client.getUserDetails().getUserId())
                    .add(UserDetails.USERNAME, client.getUserDetails().getUserName())
                    .add(IS_USED, client.isUsed())
                    .add(ControllerInfo.SIGNATURE, client.getSessionDetails().getControllerInfo().getSignature())
                    .add(ControllerInfo.VEHICLE_NAME, client.getSessionDetails().getControllerInfo().getVehicleName())
                    .add(ControllerInfo.ENGINE_MAKE, client.getSessionDetails().getControllerInfo().getEngineMake())
                    .add(ControllerInfo.ENGINE_CODE, client.getSessionDetails().getControllerInfo().getEngineCode())
                    .add(MAX_PACKET_GAP, client.getStream().getStreamStats().getMaxPacketGap())
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
    private void runCleanup() {
        List<ApplicationConnectionState> inactiveApplications = new ArrayList<>();

        synchronized (lock) {
            long now = System.currentTimeMillis();
            for (ApplicationConnectionState client : applications) {
                if (now - client.getClientStream().getStreamStats().getPreviousPacketArrivalTime() > applicationTimeout)
                    inactiveApplications.add(client);
            }
        }

        for (ApplicationConnectionState inactiveClient : inactiveApplications) {
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

}
