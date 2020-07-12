package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.TcpCommunicationIntegrationTest;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.server.Backend;
import com.rusefi.server.ClientConnectionState;
import com.rusefi.server.UserDetails;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.junit.Test;

import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import static com.rusefi.server.Backend.LIST_PATH;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * integration test of the rusEFI online backend process
 * At the moment this test is very loose with timing it must be unreliable?
 */
public class ServerTest {
    private final static Logger logger = Logger.CONSOLE;

    @Test
    public void testSessionTimeout() throws InterruptedException, IOException {
        int serverPort = 7000;

        CountDownLatch serverCreated = new CountDownLatch(1);


        Function<String, UserDetails> userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));

        CountDownLatch allClientsDisconnected = new CountDownLatch(1);
        CountDownLatch onConnected = new CountDownLatch(2);

        int httpPort = 8000;
        Backend backend = new Backend(userDetailsResolver, httpPort) {
            @Override
            public void register(ClientConnectionState clientConnectionState) {
                super.register(clientConnectionState);
                onConnected.countDown();
            }

            @Override
            public void close(ClientConnectionState inactiveClient) {
                super.close(inactiveClient);
                if (getCount() == 0)
                    allClientsDisconnected.countDown();
            }
        };

        BinaryProtocolServer.tcpServerSocket(serverPort, "Server", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket clientSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        ClientConnectionState clientConnectionState = new ClientConnectionState(clientSocket, logger, backend.getUserDetailsResolver());
                        try {
                            clientConnectionState.requestControllerInfo();

                            backend.register(clientConnectionState);
                            clientConnectionState.runEndlessLoop();
                        } catch (IOException e) {
                            backend.close(clientConnectionState);
                        }
                    }
                };
            }
        }, logger, parameter -> serverCreated.countDown());

        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
        assertEquals(0, backend.getCount());


        new MockRusEfiDevice("00000000-1234-1234-1234-123456789012", "rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPort);
        new MockRusEfiDevice("12345678-1234-1234-1234-123456789012", "rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPort);

        assertTrue(onConnected.await(30, TimeUnit.SECONDS));

        List<ClientConnectionState> clients = backend.getClients();
        assertEquals(2, clients.size());

        List<UserDetails> onlineUsers = getOnlineUsers(httpPort);
        assertEquals(2, onlineUsers.size());

        assertTrue(allClientsDisconnected.await(30, TimeUnit.SECONDS));

    }

    @NotNull
    private List<UserDetails> getOnlineUsers(int httpPort) throws IOException {
        HttpClient httpclient = new DefaultHttpClient();
        String url = "http://" + TcpCommunicationIntegrationTest.LOCALHOST + ":" + httpPort + LIST_PATH;
        System.out.println("Connecting to " + url);
        HttpGet httpget = new HttpGet(url);
        HttpResponse httpResponse = httpclient.execute(httpget);

        HttpEntity entity = httpResponse.getEntity();
        String responseString = EntityUtils.toString(entity, "UTF-8");
        JSONParser parser = new JSONParser();
        List<UserDetails> userLists = new ArrayList<>();
        try {
            JSONArray array = (JSONArray) parser.parse(responseString);

            for (int i = 0; i < array.size(); i++) {
                JSONObject element = (JSONObject) array.get(i);

                userLists.add(UserDetails.valueOf(element));
            }

            System.out.println("object=" + array);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }
        return userLists;
    }
}
