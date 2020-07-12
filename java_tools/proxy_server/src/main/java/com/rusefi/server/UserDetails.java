package com.rusefi.server;

import org.json.simple.JSONObject;

public class UserDetails {


    public static final String USER_ID = "user_id";
    public static final String USERNAME = "username";
    private final String userName;
    private final int id;

    public UserDetails(String userName, int id) {
        this.userName = userName;
        this.id = id;
    }

    public static UserDetails valueOf(JSONObject element) {
        long userId = (long) element.get(USER_ID);
        String userName = (String) element.get(USERNAME);
        return new UserDetails(userName, (int) userId);
    }

    public String getUserName() {
        return userName;
    }

    public int getId() {
        return id;
    }

    @Override
    public String toString() {
        return "UserDetails{" +
                "userName='" + userName + '\'' +
                ", id=" + id +
                '}';
    }
}
