package com.rusefi.tools.online;

import com.rusefi.server.UserDetails;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ProxyClient {
    public static final String LOCALHOST = "localhost";
    public static final String LIST_PATH = "/list_online";

    @NotNull
    public static List<UserDetails> getOnlineUsers(int httpPort) throws IOException {
        HttpClient httpclient = new DefaultHttpClient();
        String url = "http://" + LOCALHOST + ":" + httpPort + LIST_PATH;
        System.out.println("Connecting to " + url);
        HttpGet httpget = new HttpGet(url);
        HttpResponse httpResponse = httpclient.execute(httpget);

        HttpEntity entity = httpResponse.getEntity();
        String responseString = EntityUtils.toString(entity, "UTF-8");
        JSONParser parser = new JSONParser();
        List<UserDetails> userLists = new ArrayList<>();
        try {
            JSONArray array = (JSONArray) parser.parse(responseString);

            for (int i = 0; i < array.size(); i++) {
                JSONObject element = (JSONObject) array.get(i);

                userLists.add(UserDetails.valueOf(element));
            }

            System.out.println("object=" + array);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }
        return userLists;
    }
}
