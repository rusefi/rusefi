package com.rusefi.tools.online;

import com.rusefi.server.UserDetails;
import org.apache.http.HttpResponse;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ProxyClient {
    public static final String LIST_PATH = "/list_online";

    public static List<UserDetails> getOnlineUsers(int httpPort) throws IOException {
        return getOnlineUsers(HttpUtil.RUSEFI_PROXY_JSON_API_PREFIX + ":" + httpPort + LIST_PATH);
    }

    @NotNull
    public static List<UserDetails> getOnlineUsers(String url) throws IOException {
        HttpResponse httpResponse = HttpUtil.executeGet(url);

        List<UserDetails> userLists = new ArrayList<>();
        try {
            JSONArray array = HttpUtil.getJsonResponse(httpResponse);

            for (int i = 0; i < array.size(); i++) {
                JSONObject element = (JSONObject) array.get(i);

                userLists.add(UserDetails.valueOf(element));
            }

            System.out.println("object=" + array);
        } catch (ParseException e) {
            throw new IOException(e);
        }
        return userLists;
    }

}
