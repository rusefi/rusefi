package com.rusefi.tools.online;

import com.rusefi.server.ControllerInfo;
import com.rusefi.server.UserDetails;

public class PublicSession {
    private final UserDetails userDetails;
    private final ControllerInfo controllerInfo;

    public PublicSession(UserDetails userDetails, ControllerInfo controllerInfo) {
        this.userDetails = userDetails;
        this.controllerInfo = controllerInfo;
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public ControllerInfo getControllerInfo() {
        return controllerInfo;
    }

    @Override
    public String toString() {
        return "PublicSession{" +
                "userDetails=" + userDetails +
                ", controllerInfo=" + controllerInfo +
                '}';
    }
}
