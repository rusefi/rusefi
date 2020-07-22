package com.rusefi.server;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.Objects;

public class ApplicationRequest {
    private static final String SESSION = "session";
    private static final String USER_ID = "user_id";

    private final SessionDetails sessionDetails;
    private final int targetUserId;

    public ApplicationRequest(SessionDetails sessionDetails, int targetUserId) {
        this.sessionDetails = sessionDetails;
        this.targetUserId = targetUserId;
    }

    public SessionDetails getSessionDetails() {
        return sessionDetails;
    }

    public int getTargetUserId() {
        return targetUserId;
    }

    public String toJson() {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SESSION, sessionDetails.toJson());
        jsonObject.put(USER_ID, targetUserId);
        return jsonObject.toJSONString();
    }

    public static ApplicationRequest valueOf(String jsonString) {
        JSONParser parser = new JSONParser();
        JSONObject jsonObject;
        try {
            jsonObject = (JSONObject) parser.parse(jsonString);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }

        long targetUserId = (Long) jsonObject.get(USER_ID);

        SessionDetails session = SessionDetails.valueOf((String) jsonObject.get(SESSION));
        return new ApplicationRequest(session, (int)targetUserId);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ApplicationRequest that = (ApplicationRequest) o;
        return targetUserId == that.targetUserId &&
                sessionDetails.equals(that.sessionDetails);
    }

    @Override
    public int hashCode() {
        return Objects.hash(sessionDetails, targetUserId);
    }
}
