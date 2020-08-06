package com.rusefi.server;

import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONObject;

import java.util.Objects;

/**
 * Controller description without any sensitive information
 */
public class ControllerInfo {
    public static final String VEHICLE_NAME = "vehicleName";
    public static final String ENGINE_MAKE = "engineMake";
    public static final String ENGINE_CODE = "engineCode";
    public static final String SIGNATURE = "signature";

    private final String vehicleName;
    private final String engineMake;
    private final String engineCode;
    private final String signature;

    public ControllerInfo(String vehicleName, String engineCode, String engineMake, String signature) {
        Objects.requireNonNull(vehicleName);
        Objects.requireNonNull(engineMake);
        Objects.requireNonNull(engineCode);

        this.vehicleName = vehicleName;
        this.engineCode = engineCode;
        this.engineMake = engineMake;
        this.signature = signature;
    }

    @Override
    public String toString() {
        return "ControllerInfo{" +
                "vehicleName='" + vehicleName + '\'' +
                ", engineMake='" + engineMake + '\'' +
                ", engineCode='" + engineCode + '\'' +
                ", signature='" + signature + '\'' +
                '}';
    }

    public String getVehicleName() {
        return vehicleName;
    }

    public String getEngineMake() {
        return engineMake;
    }

    public String getEngineCode() {
        return engineCode;
    }

    public String getSignature() {
        return signature;
    }

    @NotNull
    public static ControllerInfo valueOf(String jsonString) {
        JSONObject jsonObject = HttpUtil.parse(jsonString);
        return valueOf(jsonObject);
    }

    @NotNull
    public static ControllerInfo valueOf(JSONObject jsonObject) {
        String vehicleName = (String) jsonObject.get(VEHICLE_NAME);
        String engineMake = (String) jsonObject.get(ENGINE_MAKE);
        String engineCode = (String) jsonObject.get(ENGINE_CODE);
        String signature = (String) jsonObject.get(SIGNATURE);

        return new ControllerInfo(vehicleName, engineCode, engineMake, signature);
    }

    public String toJson() {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(ENGINE_MAKE, engineMake);
        jsonObject.put(ENGINE_CODE, engineCode);
        jsonObject.put(VEHICLE_NAME, vehicleName);
        jsonObject.put(SIGNATURE, signature);
        return jsonObject.toJSONString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ControllerInfo that = (ControllerInfo) o;
        return vehicleName.equals(that.vehicleName) &&
                engineMake.equals(that.engineMake) &&
                engineCode.equals(that.engineCode) &&
                signature.equals(that.signature);
    }

    @Override
    public int hashCode() {
        return Objects.hash(vehicleName, engineMake, engineCode, signature);
    }
}
