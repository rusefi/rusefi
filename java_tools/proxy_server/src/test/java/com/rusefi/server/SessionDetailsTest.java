package com.rusefi.server;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class SessionDetailsTest {
    @Test
    public void testSerialization() {
        ControllerInfo ci = new ControllerInfo("name", "make", "code", "sign");

        SessionDetails sd = new SessionDetails(ci, "auth", 123);

        String json = sd.toJson();

        SessionDetails fromJson = SessionDetails.valueOf(json);

        assertEquals(sd, fromJson);
    }

}
