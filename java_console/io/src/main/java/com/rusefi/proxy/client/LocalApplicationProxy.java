package com.rusefi.proxy.client;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.StreamStatistics;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.ui.StatusConsumer;
import org.apache.http.Consts;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.Closeable;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicLong;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.BINARY_IO_TIMEOUT;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

/**
 * Remote user process which facilitates connection between local tuning application and real ECU via rusEFI proxy service
 */
public class LocalApplicationProxy implements Closeable {
    private static final ThreadFactory THREAD_FACTORY = new NamedThreadFactory("gauge poking", true);
    private static final Logging log = getLogging(LocalApplicationProxy.class);
    public static final int SERVER_PORT_FOR_APPLICATIONS = HttpUtil.getIntProperty("applications.port", 8002);
    private final ApplicationRequest applicationRequest;
    /**
     * local TCP server socket which local tuning application connects to
     */
    private final ServerSocketReference serverHolder;
    private final IoStream authenticatorToProxyStream;

    public LocalApplicationProxy(ApplicationRequest applicationRequest, ServerSocketReference serverHolder, IoStream authenticatorToProxyStream) {
        this.applicationRequest = applicationRequest;
        this.serverHolder = serverHolder;
        this.authenticatorToProxyStream = authenticatorToProxyStream;
    }

    public IoStream getAuthenticatorToProxyStream() {
        return authenticatorToProxyStream;
    }

    public static HttpResponse requestSoftwareUpdate(int httpPort, ApplicationRequest applicationRequest, UpdateType type) throws IOException {
        HttpPost httpPost = new HttpPost(ProxyClient.getHttpAddress(httpPort) + ProxyClient.UPDATE_CONNECTOR_SOFTWARE);

        List<NameValuePair> form = new ArrayList<>();
        form.add(new BasicNameValuePair(ProxyClient.JSON, applicationRequest.toJson()));
        form.add(new BasicNameValuePair(ProxyClient.UPDATE_TYPE, type.name()));
        UrlEncodedFormEntity entity = new UrlEncodedFormEntity(form, Consts.UTF_8);

        httpPost.setEntity(entity);

        HttpClient httpclient = new DefaultHttpClient();
        return httpclient.execute(httpPost);
    }

    public ApplicationRequest getApplicationRequest() {
        return applicationRequest;
    }

    /**
     * @param context
     * @param applicationRequest remote session we want to connect to
     * @param jsonHttpPort
     * @param disconnectListener
     * @param connectionListener
     */
    public static ServerSocketReference startAndRun(LocalApplicationProxyContext context, ApplicationRequest applicationRequest, int jsonHttpPort, TcpIoStream.DisconnectListener disconnectListener, ConnectionListener connectionListener) throws IOException {
        String version = context.executeGet(ProxyClient.getHttpAddress(jsonHttpPort) + ProxyClient.VERSION_PATH);
        log.info("Server says version=" + version);
        if (!version.contains(ProxyClient.BACKEND_VERSION)) {
            String message = "Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION;
            log.error(message);
            System.out.println(message);
            /**
             * let's give wrapper script a chance to update us
             */
            throw new IncompatibleBackendException(message);
        }

        AbstractIoStream authenticatorToProxyStream = new TcpIoStream("authenticatorToProxyStream ", rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, context.serverPortForRemoteApplications()), disconnectListener);
        LocalApplicationProxy.sendHello(authenticatorToProxyStream, applicationRequest);

        AtomicLong lastActivity = new AtomicLong(System.currentTimeMillis());
        BinaryProtocolProxy.ClientApplicationActivityListener clientApplicationActivityListener = () -> lastActivity.set(System.currentTimeMillis());

        /**
         * We need to entertain proxy server and remote controller while user has already connected to proxy but has not yet started TunerStudio
         */
        THREAD_FACTORY.newThread(() -> {
            try {
                while (true) {
                    sleep(context.gaugePokingPeriod());
                    if (isTimeForApplicationToConnect(lastActivity.get(), BINARY_IO_TIMEOUT / 2)) {
                        // TODO: why is this logic duplicated from BinaryProtocol?
                        byte[] commandPacket = new byte[5];
                        commandPacket[0] = Fields.TS_OUTPUT_COMMAND;
                        System.arraycopy(GetOutputsCommand.createRequest(), 0, commandPacket, 1, 4);

                        // we do not really need the data, we just need to take response from the socket
                        authenticatorToProxyStream.sendAndGetPacket(commandPacket, "Gauge Poker");
                    }

                    if (isTimeForApplicationToConnect(lastActivity.get(), context.startUpIdle())) {
                        // we should not keep controller blocked since we are not connecting application, time to auto-disconnect
                        authenticatorToProxyStream.close();
                        disconnectListener.onDisconnect("Giving up connection");
                    }
                }


            } catch (IOException e) {
                log.error("Gauge poker", e);
            }
        }).start();


        ServerSocketReference serverHolder = BinaryProtocolProxy.createProxy(authenticatorToProxyStream, context.authenticatorPort(), clientApplicationActivityListener, StatusConsumer.ANONYMOUS);
        LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(applicationRequest, serverHolder, authenticatorToProxyStream);
        connectionListener.onConnected(localApplicationProxy, authenticatorToProxyStream);
        return serverHolder;
    }

    private static boolean isTimeForApplicationToConnect(long start, int idle) {
        return System.currentTimeMillis() - start > idle;
    }

    public static void sendHello(IoStream authenticatorToProxyStream, ApplicationRequest applicationRequest) throws IOException {
        log.info("Pushing " + applicationRequest);
        // right from connection push session authentication data
        new HelloCommand(applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }

    public static void start() {
        throw new UnsupportedOperationException("TODO");
    }

    @Override
    public void close() {
        serverHolder.close();
        byte[] request = new byte[2];
        request[0] = Fields.TS_ONLINE_PROTOCOL;
        request[1] = NetworkConnector.DISCONNECT;
        try {
            authenticatorToProxyStream.sendPacket(request);
        } catch (IOException ignored) {
        }
        authenticatorToProxyStream.close();
    }

    public interface ConnectionListener {
        ConnectionListener VOID = (localApplicationProxy, authenticatorToProxyStream) -> {
        };

        void onConnected(LocalApplicationProxy localApplicationProxy, StreamStatistics authenticatorToProxyStream);
    }
}
