package com.rusefi.tools.online;

import com.rusefi.server.ControllerInfo;
import com.rusefi.server.UserDetails;

public class PublicSession {
    /**
     * owner of physical ECU/vehicle
     */
    private final UserDetails vehicleOwner;
    private final ControllerInfo controllerInfo;
    private final boolean isUsed;
    /**
     * Person currently in control of tuning session
     */
    private final String tunerName;

    public PublicSession(UserDetails vehicleOwner, ControllerInfo controllerInfo, boolean isUsed, String tunerName) {
        this.vehicleOwner = vehicleOwner;
        this.controllerInfo = controllerInfo;
        this.isUsed = isUsed;
        this.tunerName = tunerName;
    }

    public UserDetails getVehicleOwner() {
        return vehicleOwner;
    }

    public ControllerInfo getControllerInfo() {
        return controllerInfo;
    }

    public boolean isUsed() {
        return isUsed;
    }

    public String getTunerName() {
        return tunerName;
    }

    @Override
    public String toString() {
        return "PublicSession{" +
                "userDetails=" + vehicleOwner +
                ", controllerInfo=" + controllerInfo +
                ", isUsed=" + isUsed +
                ", ownerName='" + tunerName + '\'' +
                '}';
    }
}
