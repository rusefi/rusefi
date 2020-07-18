package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.Listener;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.tools.online.ProxyClient;
import org.jetbrains.annotations.NotNull;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.facets.fork.TkFork;
import org.takes.http.FtBasic;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.Socket;
import java.util.*;
import java.util.function.Function;

public class Backend {
    public static final String VERSION_PATH = "/version";
    public static final String BACKEND_VERSION = "0.0001";
    public static final int SERVER_PORT_FOR_APPLICATIONS = 8002;
    public static final int SERVER_PORT_FOR_CONTROLLERS = 8003;
    public static final int HTTP_PORT = 8001;

    private final FkRegex showOnlineUsers = new FkRegex(ProxyClient.LIST_PATH,
            (Take) req -> getUsersOnline()
    );
    private boolean isClosed;

    // guarded by own monitor
    private final Set<ControllerConnectionState> clients = new HashSet<>();
    // guarded by clients
    private HashMap<ControllerKey, ControllerConnectionState> byId = new HashMap<>();
    //    private final int clientTimeout;
    private final Function<String, UserDetails> userDetailsResolver;
    private final Logger logger;

    public Backend(Function<String, UserDetails> userDetailsResolver, int httpPort, Logger logger) {
//        this.clientTimeout = clientTimeout;
        this.userDetailsResolver = userDetailsResolver;
        this.logger = logger;


        new Thread(() -> {
            try {
                new FtBasic(
                        new TkFork(showOnlineUsers,
                                new FkRegex(VERSION_PATH, BACKEND_VERSION),
                                new FkRegex("/", "<a href='https://rusefi.com/online/'>rusEFI Online</a>")
                        ), httpPort
                ).start(() -> isClosed);
                logger.info("Shutting down backend on port " + httpPort);
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }

        }, "Http Server Thread").start();


//        new Thread(() -> {
//            while (true) {
//                runCleanup();
//                sleep(clientTimeout);
//            }
//        }, "rusEFI Server Cleanup").start();
    }

    public void runApplicationConnector(int serverPortForApplications, Listener serverSocketCreationCallback) {
        // connection from authenticator app which proxies for Tuner Studio
        // authenticator pushed hello packet on connect
        BinaryProtocolServer.tcpServerSocket(logger, new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket applicationSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        // connection from authenticator app which proxies for Tuner Studio
                        IoStream applicationClientStream = null;
                        try {
                            applicationClientStream = new TcpIoStream(logger, applicationSocket);

                            // authenticator pushed hello packet on connect
                            String jsonString = HelloCommand.getHelloResponse(applicationClientStream.getDataBuffer(), logger);
                            if (jsonString == null)
                                return;
                            ApplicationRequest applicationRequest = ApplicationRequest.valueOf(jsonString);
                            logger.info("Application Connected: " + applicationRequest);

                            ControllerKey controllerKey = new ControllerKey(applicationRequest.getTargetUserId(), applicationRequest.getSessionDetails().getControllerInfo());
                            ControllerConnectionState state;
                            synchronized (clients) {
                                state = byId.get(controllerKey);
                            }
                            if (state == null) {
                                applicationClientStream.close();
                                onDisconnectApplication();
                                logger.info("No controller for " + controllerKey);
                                return;
                            }

                            BinaryProtocolProxy.runProxy(state.getStream(), applicationClientStream);

                        } catch (Throwable e) {
                            if (applicationClientStream != null)
                                applicationClientStream.close();
                            e.printStackTrace();
                            logger.error("Got error " + e);
                            onDisconnectApplication();
                        }
                    }
                };
            }
        }, serverPortForApplications, "ApplicationServer", serverSocketCreationCallback, BinaryProtocolServer.SECURE_SOCKET_FACTORY);
    }

    protected void onDisconnectApplication() {
        logger.info("Disconnecting application");
    }

    public void runControllerConnector(int serverPortForControllers, Listener serverSocketCreationCallback) {
        BinaryProtocolServer.tcpServerSocket(logger, new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket controllerSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        ControllerConnectionState controllerConnectionState = new ControllerConnectionState(controllerSocket, logger, getUserDetailsResolver());
                        try {
                            controllerConnectionState.requestControllerInfo();

                            register(controllerConnectionState);

                            controllerConnectionState.getOutputs();
                        } catch (IOException e) {
                            close(controllerConnectionState);
                        }
                    }
                };
            }
        }, serverPortForControllers, "ControllerServer", serverSocketCreationCallback, BinaryProtocolServer.SECURE_SOCKET_FACTORY);
    }

    @NotNull
    private RsJson getUsersOnline() throws IOException {
        JsonArrayBuilder builder = Json.createArrayBuilder();
        List<ControllerConnectionState> clients = getClients();
        for (ControllerConnectionState client : clients) {

            JsonObject clientObject = Json.createObjectBuilder()
                    .add(UserDetails.USER_ID, client.getUserDetails().getUserId())
                    .add(UserDetails.USERNAME, client.getUserDetails().getUserName())
                    .add(ControllerInfo.SIGNATURE, client.getSessionDetails().getControllerInfo().getSignature())
                    .build();
            builder.add(clientObject);
        }
        return new RsJson(builder.build());
    }

    public Function<String, UserDetails> getUserDetailsResolver() {
        return userDetailsResolver;
    }

//    private void runCleanup() {
//        List<ClientConnectionState> inactiveClients = new ArrayList<>();
//
//        synchronized (clients) {
//            long now = System.currentTimeMillis();
//            for (ClientConnectionState client : clients) {
//                if (now - client.getLastActivityTimestamp() > clientTimeout)
//                    inactiveClients.add(client);
//            }
//        }
//
//        for (ClientConnectionState inactiveClient : inactiveClients) {
//            close(inactiveClient);
//        }
//
//    }

    public void register(ControllerConnectionState controllerConnectionState) {
        Objects.requireNonNull(controllerConnectionState.getControllerKey(), "ControllerKey");
        synchronized (clients) {
            clients.add(controllerConnectionState);
            byId.put(controllerConnectionState.getControllerKey(), controllerConnectionState);
        }
        onRegister(controllerConnectionState);
    }

    protected void onRegister(ControllerConnectionState controllerConnectionState) {
    }

    public void close(ControllerConnectionState inactiveClient) {
        inactiveClient.close();
        synchronized (clients) {
            // in case of exception in the initialization phase we do not even add client into the the collection
            clients.remove(inactiveClient);
            byId.remove(inactiveClient.getControllerKey());
        }
    }

    public void close() {
        isClosed = true;
    }

    public List<ControllerConnectionState> getClients() {
        synchronized (clients) {
            return new ArrayList<>(clients);
        }
    }

    public int getCount() {
        synchronized (clients) {
            return clients.size();
        }
    }

}
