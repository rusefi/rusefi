package com.rusefi.server;

import com.rusefi.tools.online.HttpUtil;
import org.json.simple.JSONObject;

import java.util.Objects;

public class ApplicationRequest {
    private static final String SESSION = "session";

    private final SessionDetails sessionDetails;
    private final UserDetails targetUser;

    public ApplicationRequest(SessionDetails sessionDetails, UserDetails targetUser) {
        this.sessionDetails = sessionDetails;
        this.targetUser = targetUser;
    }

    public SessionDetails getSessionDetails() {
        return sessionDetails;
    }

    public UserDetails getTargetUser() {
        return targetUser;
    }

    public String toJson() {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SESSION, sessionDetails.toJson());
        targetUser.put(jsonObject);
        return jsonObject.toJSONString();
    }

    public static ApplicationRequest valueOf(String jsonString) {
        JSONObject jsonObject = HttpUtil.parse(jsonString);

        UserDetails userDetails = UserDetails.valueOf(jsonObject);

        SessionDetails session = SessionDetails.valueOf((String) jsonObject.get(SESSION));
        return new ApplicationRequest(session, userDetails);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ApplicationRequest that = (ApplicationRequest) o;
        return targetUser == that.targetUser &&
                sessionDetails.equals(that.sessionDetails);
    }

    @Override
    public int hashCode() {
        return Objects.hash(sessionDetails, targetUser);
    }

    @Override
    public String toString() {
        return "ApplicationRequest{" +
                "sessionDetails=" + sessionDetails +
                ", targetUserId=" + targetUser +
                '}';
    }
}
