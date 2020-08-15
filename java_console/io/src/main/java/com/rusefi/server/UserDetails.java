package com.rusefi.server;

import org.json.simple.JSONObject;

/**
 * rusEFI Online user - ID and username
 */
public class UserDetails {
    public static final String USER_ID = "user_id";
    public static final String USERNAME = "username";
    private final String userName;
    private final int userId;

    public UserDetails(String userName, int userId) {
        this.userName = userName;
        this.userId = userId;
    }

    public static UserDetails valueOf(JSONObject element) {
        long userId = (long) element.get(USER_ID);
        String userName = (String) element.get(USERNAME);
        return new UserDetails(userName, (int) userId);
    }

    void put(JSONObject jsonObject) {
        jsonObject.put(USER_ID, getUserId());
        jsonObject.put(USERNAME, getUserName());
    }

    public String getUserName() {
        return userName;
    }

    public int getUserId() {
        return userId;
    }

    @Override
    public String toString() {
        return "UserDetails{" +
                "userName='" + userName + '\'' +
                ", id=" + userId +
                '}';
    }
}
