package com.rusefi.server;

import com.devexperts.logging.Logging;
import org.takes.Request;
import org.takes.Response;
import org.takes.Take;
import org.takes.rq.RqForm;
import org.takes.rq.form.RqFormBase;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonObjectBuilder;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class UpdateRequestHandler implements Take {
    private static final Logging log = getLogging(UpdateRequestHandler.class);
    private static final String AUTH_TOKEN = "auth_token";
    private static final String VEHICLE_TOKEN = "vehicle_token";

    @Override
    public Response act(Request req) throws IOException {

        JsonObjectBuilder objectBuilder = Json.createObjectBuilder();

        try {
            RqForm rqForm = new RqFormBase(req);
            String authToken = get(rqForm, AUTH_TOKEN);
            String vehicleToken = get(rqForm, VEHICLE_TOKEN);
            log.debug("Update request " + authToken + " " + vehicleToken);
        } catch (IOException e) {
            objectBuilder.add("result", "error");
            return new RsJson(objectBuilder.build());
        }

        objectBuilder.add("result", "OK");
        return new RsJson(objectBuilder.build());
    }

    private String get(RqForm rqForm, String name) throws IOException {
        return rqForm.param(name).iterator().next();
    }
}
