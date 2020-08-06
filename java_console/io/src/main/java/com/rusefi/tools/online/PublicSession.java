package com.rusefi.tools.online;

import com.rusefi.server.ControllerInfo;
import com.rusefi.server.UserDetails;

public class PublicSession {
    private final UserDetails userDetails;
    private final ControllerInfo controllerInfo;
    private final boolean isUsed;
    private final String ownerName;

    public PublicSession(UserDetails userDetails, ControllerInfo controllerInfo, boolean isUsed, String ownerName) {
        this.userDetails = userDetails;
        this.controllerInfo = controllerInfo;
        this.isUsed = isUsed;
        this.ownerName = ownerName;
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public ControllerInfo getControllerInfo() {
        return controllerInfo;
    }

    public boolean isUsed() {
        return isUsed;
    }

    public String getOwnerName() {
        return ownerName;
    }

    @Override
    public String toString() {
        return "PublicSession{" +
                "userDetails=" + userDetails +
                ", controllerInfo=" + controllerInfo +
                ", isUsed=" + isUsed +
                ", ownerName='" + ownerName + '\'' +
                '}';
    }
}
