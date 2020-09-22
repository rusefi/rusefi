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
    private final String age;
    private final String implementation;

    public PublicSession(UserDetails vehicleOwner, ControllerInfo controllerInfo, boolean isUsed, String tunerName, String age, String implementation) {
        this.vehicleOwner = vehicleOwner;
        this.controllerInfo = controllerInfo;
        this.isUsed = isUsed;
        this.tunerName = tunerName;
        this.age = age;
        this.implementation = implementation;
    }

    public String getImplementation() {
        return implementation;
    }

    public String getAge() {
        return age;
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
