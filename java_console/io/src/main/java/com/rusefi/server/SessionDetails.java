package com.rusefi.server;

import com.rusefi.proxy.NetworkConnector;
import com.rusefi.tools.online.HttpUtil;
import org.json.simple.JSONObject;

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

    private static final String CONTROLLER = "controller";
    private static final String HARDCODED_ONE_TIME_CODE = System.getProperty("ONE_TIME_CODE");

    private final ControllerInfo controllerInfo;

    private final int vehicleToken;
    private final String authToken;
    private final NetworkConnector.Implementation implementation;
    private final int consoleVersion;

    public SessionDetails(NetworkConnector.Implementation implementation, ControllerInfo controllerInfo, String authToken, int oneTimeCode, int consoleVersion) {
        this.implementation = Objects.requireNonNull(implementation);
        this.consoleVersion = consoleVersion;
        Objects.requireNonNull(controllerInfo);
        Objects.requireNonNull(authToken);
        this.controllerInfo = controllerInfo;
        this.vehicleToken = oneTimeCode;
        this.authToken = authToken;
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
        return jsonObject.toJSONString();
    }

    public static SessionDetails valueOf(String jsonString) {
        JSONObject jsonObject = HttpUtil.parse(jsonString);

        String authToken = (String) jsonObject.get(AUTH_TOKEN);
        long oneTimeCode = (Long)jsonObject.get(VEHICLE_TOKEN);
        long connectorVersion = (long) jsonObject.get(CONNECTOR_VERSION);
        NetworkConnector.Implementation implementation = NetworkConnector.Implementation.find((String) jsonObject.get(IMPLEMENTATION));

                ControllerInfo controllerInfo = ControllerInfo.valueOf((String) jsonObject.get(CONTROLLER));

        return new SessionDetails(implementation, controllerInfo, authToken, (int) oneTimeCode, (int) connectorVersion);
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
