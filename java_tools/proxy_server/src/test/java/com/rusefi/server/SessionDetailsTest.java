package com.rusefi.server;

import com.rusefi.TestHelper;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.core.rusEFIVersion;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class SessionDetailsTest {

    @Test
    public void testSerialization() {
        SessionDetails sd = new SessionDetails(NetworkConnector.Implementation.Unknown, TestHelper.CONTROLLER_INFO, "auth", 123, rusEFIVersion.CONSOLE_VERSION);

        String json = sd.toJson();
        SessionDetails fromJson = SessionDetails.valueOf(json);
        assertEquals(sd, fromJson);
    }

    @Test
    public void testApplicationRequest() {
        SessionDetails sd = new SessionDetails(NetworkConnector.Implementation.Unknown, TestHelper.CONTROLLER_INFO, "auth", 123, rusEFIVersion.CONSOLE_VERSION);
        ApplicationRequest ar = new ApplicationRequest(sd, new UserDetails("", 321));

        String json = ar.toJson();
        ApplicationRequest fromJson = ApplicationRequest.valueOf(json);
        assertEquals(ar, fromJson);
    }
}
