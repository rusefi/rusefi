package com.rusefi.server;

import org.jetbrains.annotations.NotNull;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.facets.fork.TkFork;
import org.takes.http.Exit;
import org.takes.http.FtBasic;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.function.Function;

public class Backend {

    public static final String LIST_PATH = "/list_online";

    private final FkRegex showOnlineUsers = new FkRegex(LIST_PATH,
            (Take) req -> getUsersOnline()
    );

    @NotNull
    private RsJson getUsersOnline() throws IOException {
        JsonArrayBuilder builder = Json.createArrayBuilder();
        List<ClientConnectionState> clients = getClients();
        for (ClientConnectionState client : clients) {

            JsonObject clientObject = Json.createObjectBuilder()
                    .add(UserDetails.USER_ID, client.getUserDetails().getId())
                    .add(UserDetails.USERNAME, client.getUserDetails().getUserName())
                    .add("signature", client.getSignature())
                    .build();
            builder.add(clientObject);
        }
        return new RsJson(builder.build());
    }

    // guarded by own monitor
    private final Set<ClientConnectionState> clients = new HashSet<>();
    //    private final int clientTimeout;
    private final Function<String, UserDetails> userDetailsResolver;

    public Backend(Function<String, UserDetails> userDetailsResolver, int httpPort) {
//        this.clientTimeout = clientTimeout;
        this.userDetailsResolver = userDetailsResolver;


        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    new FtBasic(
                            new TkFork(showOnlineUsers,
                                    new FkRegex("/", "<a href='https://rusefi.com/online/'>rusEFI Online</a>")
                            ), httpPort
                    ).start(Exit.NEVER);
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }

            }
        }).start();


//        new Thread(() -> {
//            while (true) {
//                runCleanup();
//                sleep(clientTimeout);
//            }
//        }, "rusEFI Server Cleanup").start();
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

    public void register(ClientConnectionState clientConnectionState) {
        synchronized (clients) {
            clients.add(clientConnectionState);
        }
    }

    public void close(ClientConnectionState inactiveClient) {
        inactiveClient.close();
        synchronized (clients) {
            // in case of exception in the initialization phase we do not even add client into the the collection
            clients.remove(inactiveClient);
        }
    }

    public List<ClientConnectionState> getClients() {
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
