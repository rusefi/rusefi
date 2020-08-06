package com.rusefi.server;

import com.rusefi.tools.online.HttpUtil;
import org.json.simple.JSONObject;

public class ControllerStateDetails {
    public static final String RPM = "RPM";
    public static final String CLT = "CLT";
    private final double clt;
    private final int rpm;

    public ControllerStateDetails(double clt, int rpm) {
        this.clt = clt;
        this.rpm = rpm;
    }

    public double getClt() {
        return clt;
    }

    public int getRpm() {
        return rpm;
    }

    public static ControllerStateDetails valueOf(String jsonString) {
        JSONObject jsonObject = HttpUtil.parse(jsonString);
        double clt = Double.parseDouble((String) jsonObject.get(CLT));
        int rpm = Integer.parseInt((String) jsonObject.get(RPM));

        return new ControllerStateDetails(clt, rpm);
    }
}
