package com.rusefi.server;

import com.devexperts.logging.Logging;
import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.io.IOException;

public class JsonUserDetailsResolver implements UserDetailsResolver {
    private final static Logging log = Logging.getLogging(JsonUserDetailsResolver.class);

    @Override
    @Nullable
    public UserDetails apply(String authToken) {

        try {
            String responseString = HttpUtil.executeGet(HttpUtil.RUSEFI_ONLINE_JSON_API_PREFIX + "getUserByToken&rusefi_token=" + authToken);
            JSONObject json = HttpUtil.getJsonResponse(responseString);
            log.info("String " + json);
            Object getUserByToken = json.get("getUserByToken");
            if (getUserByToken instanceof String) {
                log.info("AUTH FAILED: Result " + getUserByToken);
                return null;
            }
            JSONObject details = (JSONObject) getUserByToken;
            String id = (String) details.get("ID");
            String name = (String) details.get("NAME");

            UserDetails result = new UserDetails(name, Integer.parseInt(id));
            log.info("AUTH SUCCESS " + result);
            return result;
        } catch (IOException | ParseException e) {
            log.error("JsonUserDetailsResolver error" + e);
            e.printStackTrace();
            return null;
        }
    }
}
