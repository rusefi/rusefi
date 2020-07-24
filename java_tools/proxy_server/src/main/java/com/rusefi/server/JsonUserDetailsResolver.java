package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.tools.online.HttpUtil;
import org.apache.http.HttpResponse;
import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.io.IOException;

public class JsonUserDetailsResolver implements UserDetailsResolver {
    @Override
    @Nullable
    public UserDetails apply(String authToken) {

        try {
            String responseString = HttpUtil.executeGet(Logger.CONSOLE,HttpUtil.RUSEFI_ONLINE_JSON_API_PREFIX + "getUserByToken&rusefi_token=" + authToken);
            JSONObject json = HttpUtil.getJsonResponse(responseString);
            System.out.println("String " + json);
            Object getUserByToken = json.get("getUserByToken");
            if (getUserByToken instanceof String) {
                System.out.println("AUTH FAILED: Result " + getUserByToken);
                return null;
            }
            JSONObject details = (JSONObject) getUserByToken;
            String id = (String) details.get("ID");
            String name = (String) details.get("NAME");

            UserDetails result = new UserDetails(name, Integer.parseInt(id));
            System.out.println("AUTH SUCCESS " + result);
            return result;
        } catch (IOException | ParseException e) {
            System.out.println("JsonUserDetailsResolver error" + e);
            e.printStackTrace();
            return null;
        }
    }
}
