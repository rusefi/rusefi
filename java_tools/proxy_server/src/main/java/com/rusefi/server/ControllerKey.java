package com.rusefi.server;

import java.util.Objects;

public class ControllerKey {
    private final int userId;
    private final ControllerInfo controllerInfo;

    public ControllerKey(int userId, ControllerInfo controllerInfo) {
        this.userId = userId;
        this.controllerInfo = controllerInfo;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ControllerKey that = (ControllerKey) o;
        return userId == that.userId &&
                controllerInfo.equals(that.controllerInfo);
    }

    @Override
    public int hashCode() {
        return Objects.hash(userId, controllerInfo);
    }

    @Override
    public String toString() {
        return "ControllerKey{" +
                "userId=" + userId +
                ", controllerInfo=" + controllerInfo +
                '}';
    }
}
