package com.rusefi.server;

import com.rusefi.proxy.NetworkConnector;
import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONObject;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Objects;
import java.util.Random;

/**
 * A session from Controller, including some sensitive information
 */
public class SessionDetails {
    public static final String VEHICLE_TOKEN = "vehicleToken";
    public static final String AUTH_TOKEN = "authToken";
    public static final String CONNECTOR_VERSION = "connectorVersion";
    public static final String IMPLEMENTATION = "implementation";
    public static final String AGE = "age";
    public static final String LOCAL_IP = "local_ip";

    private static final String CONTROLLER = "controller";
    private static final String HARDCODED_ONE_TIME_CODE = System.getProperty("ONE_TIME_CODE");

    private final ControllerInfo controllerInfo;

    private final int vehicleToken;
    private final String authToken;
    private final NetworkConnector.Implementation implementation;
    private final int consoleVersion;
    @Nullable
    private final String localIpAddress;

    public SessionDetails(NetworkConnector.Implementation implementation, ControllerInfo controllerInfo, String authToken, int oneTimeCode, int consoleVersion) {
        this(implementation, controllerInfo, authToken, oneTimeCode, consoleVersion, findLocalIpAddress());
    }

    private static String findLocalIpAddress() {
        try {
            String hostAddress = InetAddress.getLocalHost().getHostAddress();
            if (hostAddress == null)
                return "unable to resolve local IP";
            return hostAddress;
        } catch (UnknownHostException e) {
            return "unknown";
        }
    }

    public SessionDetails(NetworkConnector.Implementation implementation, ControllerInfo controllerInfo, String authToken, int oneTimeCode, int consoleVersion, String localIpAddress) {
        this.implementation = Objects.requireNonNull(implementation);
        this.consoleVersion = consoleVersion;
        this.localIpAddress = localIpAddress;
        Objects.requireNonNull(controllerInfo);
        Objects.requireNonNull(authToken);
        this.controllerInfo = controllerInfo;
        this.vehicleToken = oneTimeCode;
        this.authToken = authToken;
    }

    public String getLocalIpAddress() {
        return localIpAddress;
    }

    public static int createOneTimeCode() {
        return HARDCODED_ONE_TIME_CODE == null ? new Random().nextInt(100000) : Integer.parseInt(HARDCODED_ONE_TIME_CODE);
    }

    public NetworkConnector.Implementation getImplementation() {
        return implementation;
    }

    public int getOneTimeToken() {
        return vehicleToken;
    }

    public ControllerInfo getControllerInfo() {
        return controllerInfo;
    }

    public String getAuthToken() {
        return authToken;
    }

    public int getConsoleVersion() {
        return consoleVersion;
    }

    public String toJson() {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(CONTROLLER, controllerInfo.toJson());
        jsonObject.put(VEHICLE_TOKEN, vehicleToken);
        jsonObject.put(AUTH_TOKEN, authToken);
        jsonObject.put(CONNECTOR_VERSION, consoleVersion);
        jsonObject.put(IMPLEMENTATION, implementation.name());
        jsonObject.put(LOCAL_IP, localIpAddress);
        return jsonObject.toJSONString();
    }

    public static SessionDetails valueOf(String jsonString) {
        JSONObject jsonObject = HttpUtil.parse(jsonString);

        String authToken = (String) jsonObject.get(AUTH_TOKEN);
        long oneTimeCode = (Long) jsonObject.get(VEHICLE_TOKEN);
        long connectorVersion = (long) jsonObject.get(CONNECTOR_VERSION);
        String localIp = (String) jsonObject.get(LOCAL_IP);
        NetworkConnector.Implementation implementation = NetworkConnector.Implementation.find((String) jsonObject.get(IMPLEMENTATION));

        ControllerInfo controllerInfo = ControllerInfo.valueOf((String) jsonObject.get(CONTROLLER));

        return new SessionDetails(implementation, controllerInfo, authToken, (int) oneTimeCode, (int) connectorVersion, localIp);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        SessionDetails that = (SessionDetails) o;
        return vehicleToken == that.vehicleToken &&
                controllerInfo.equals(that.controllerInfo) &&
                authToken.equals(that.authToken);
    }

    @Override
    public int hashCode() {
        return Objects.hash(controllerInfo, vehicleToken, authToken);
    }

    @Override
    public String toString() {
        return "SessionDetails{" +
                "controllerInfo=" + controllerInfo +
                ", authToken='" + authToken + '\'' +
                ", implementation='" + implementation + '\'' +
                ", consoleVersion='" + consoleVersion + '\'' +
                '}';
    }
}
