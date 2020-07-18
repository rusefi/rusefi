package com.rusefi.server;

import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonReader;
import java.io.StringReader;
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

    public static ControllerInfo valueOf(String jsonString) {
        JsonReader reader = Json.createReader(new StringReader(jsonString));

        JsonObject jsonObject = reader.readObject();
        String vehicleName = jsonObject.getString(VEHICLE_NAME);
        String engineMake = jsonObject.getString(ENGINE_MAKE);
        String engineCode = jsonObject.getString(ENGINE_CODE);
        String signature = jsonObject.getString(SIGNATURE);

        return new ControllerInfo(vehicleName, engineCode, engineMake, signature);
    }

    public String toJson() {
        JsonObject jsonObject = Json.createObjectBuilder()
                .add(ENGINE_MAKE, engineMake)
                .add(ENGINE_CODE, engineCode)
                .add(VEHICLE_NAME, vehicleName)
                .add(SIGNATURE, signature)
                .build();
        return jsonObject.toString();
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
