package com.rusefi.server;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.Objects;
import java.util.Random;

public class SessionDetails {
    private static final String ONE_TIME_TOKEN = "oneTime";
    private static final String AUTH_TOKEN = "authToken";
    private static final String CONTROLLER = "controller";
    private static final String HARDCODED_ONE_TIME_CODE = System.getProperty("ONE_TIME_CODE");

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
        return HARDCODED_ONE_TIME_CODE == null ? new Random().nextInt(100000) : Integer.parseInt(HARDCODED_ONE_TIME_CODE);
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
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(CONTROLLER, controllerInfo.toJson());
        jsonObject.put(ONE_TIME_TOKEN, oneTimeToken);
        jsonObject.put(AUTH_TOKEN, authToken);
        return jsonObject.toJSONString();
    }

    public static SessionDetails valueOf(String jsonString) {
        JSONParser parser = new JSONParser();
        JSONObject jsonObject;
        try {
            jsonObject = (JSONObject) parser.parse(jsonString);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }

        String authToken = (String) jsonObject.get(AUTH_TOKEN);
        long oneTimeCode = (Long)jsonObject.get(ONE_TIME_TOKEN);

        ControllerInfo controllerInfo = ControllerInfo.valueOf((String) jsonObject.get(CONTROLLER));

        return new SessionDetails(controllerInfo, authToken, (int) oneTimeCode);
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
