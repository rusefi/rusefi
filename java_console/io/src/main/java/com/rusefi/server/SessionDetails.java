package com.rusefi.server;

import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonReader;
import java.io.StringReader;
import java.util.Objects;
import java.util.Random;

public class SessionDetails {
    private static final String ONE_TIME_TOKEN = "oneTime";
    private static final String AUTH_TOKEN = "authToken";
    private static final String CONTROLLER = "controller";

    private final ControllerInfo controllerInfo;

    private final int oneTimeToken;
    private final String authToken;

    public SessionDetails(ControllerInfo controllerInfo, String authToken, int oneTimeCode) {
        Objects.requireNonNull(controllerInfo);
        Objects.requireNonNull(authToken);
        this.controllerInfo = controllerInfo;
        this.oneTimeToken = oneTimeCode;
        this.authToken = authToken;
    }

    public static int createOneTimeCode() {
        return new Random().nextInt(60000);
    }

    public int getOneTimeToken() {
        return oneTimeToken;
    }

    public ControllerInfo getControllerInfo() {
        return controllerInfo;
    }

    public String getAuthToken() {
        return authToken;
    }

    public String toJson() {
        JsonObject jsonObject = Json.createObjectBuilder()
                .add(CONTROLLER, controllerInfo.toJson())
                .add(ONE_TIME_TOKEN, oneTimeToken)
                .add(AUTH_TOKEN, authToken)
                .build();
        return jsonObject.toString();
    }

    public static SessionDetails valueOf(String jsonString) {
        JsonReader reader = Json.createReader(new StringReader(jsonString));

        JsonObject jsonObject = reader.readObject();
        String authToken = jsonObject.getString(AUTH_TOKEN);
        int oneTimeCode = jsonObject.getInt(ONE_TIME_TOKEN);

        ControllerInfo controllerInfo = ControllerInfo.valueOf(jsonObject.getString(CONTROLLER));

        return new SessionDetails(controllerInfo, authToken, oneTimeCode);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        SessionDetails that = (SessionDetails) o;
        return oneTimeToken == that.oneTimeToken &&
                controllerInfo.equals(that.controllerInfo) &&
                authToken.equals(that.authToken);
    }

    @Override
    public int hashCode() {
        return Objects.hash(controllerInfo, oneTimeToken, authToken);
    }
}
