package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
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
import java.io.IOException;
import java.net.Socket;
import java.util.*;
import java.util.concurrent.CountDownLatch;
import java.util.function.Function;

public class Backend {
    public static final String VERSION_PATH = "/version";
    public static final String BACKEND_VERSION = "0.0001";

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


    public void runApplicationConnector(int serverPortForApplications, CountDownLatch serverCreated) {
        BinaryProtocolServer.tcpServerSocket(serverPortForApplications, "ApplicationServer", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket applicationSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        IoStream stream = null;
                        try {
                            stream = new TcpIoStream(logger, applicationSocket);

                            // authenticator pushed hello packet on connect
                            String jsonString = HelloCommand.getHelloResponse(stream.getDataBuffer(), logger);
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
                                stream.close();
                                onDisconnectApplication();
                                logger.info("No controller for " + controllerKey);
                            }
                        } catch (Throwable e) {
                            if (stream != null)
                                stream.close();
                            logger.error("Got error " + e);
                            onDisconnectApplication();
                        }
                    }
                };
            }
        }, logger, parameter -> serverCreated.countDown());
    }

    public void onDisconnectApplication() {
        logger.info("Disconnecting application");
    }

    public void runControllerConnector(int serverPortForControllers, CountDownLatch serverCreated) {
        BinaryProtocolServer.tcpServerSocket(serverPortForControllers, "ControllerServer", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket controllerSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        ControllerConnectionState controllerConnectionState = new ControllerConnectionState(controllerSocket, logger, getUserDetailsResolver());
                        try {
                            controllerConnectionState.requestControllerInfo();

                            register(controllerConnectionState);
                            controllerConnectionState.runEndlessLoop();
                        } catch (IOException e) {
                            close(controllerConnectionState);
                        }
                    }
                };
            }
        }, logger, parameter -> serverCreated.countDown());
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
