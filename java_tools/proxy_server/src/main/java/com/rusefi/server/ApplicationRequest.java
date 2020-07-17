package com.rusefi.server;

import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonReader;
import java.io.StringReader;
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
        JsonObject jsonObject = Json.createObjectBuilder()
                .add(SESSION, sessionDetails.toJson())
                .add(USER_ID, targetUserId)
                .build();
        return jsonObject.toString();
    }

    public static ApplicationRequest valueOf(String jsonString) {
        JsonReader reader = Json.createReader(new StringReader(jsonString));

        JsonObject jsonObject = reader.readObject();
        int targetUserId = jsonObject.getInt(USER_ID);

        SessionDetails session = SessionDetails.valueOf(jsonObject.getString(SESSION));
        return new ApplicationRequest(session, targetUserId);
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
