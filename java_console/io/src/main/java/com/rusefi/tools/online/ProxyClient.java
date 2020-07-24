package com.rusefi.tools.online;

import com.opensr5.Logger;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.UserDetails;
import org.apache.http.HttpResponse;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.tools.online.HttpUtil.RUSEFI_PROXY_HOSTNAME;

public class ProxyClient {
    public static final String LIST_CONTROLLERS_PATH = "/list_controllers";
    public static final String LIST_APPLICATIONS_PATH = "/list_applications";
    public static final String VERSION_PATH = "/version";
    public static final String BACKEND_VERSION = "0.0001";

    public static List<PublicSession> getOnlineApplications(int httpPort) throws IOException {
        return getOnlineApplications(getHttpAddress(httpPort) + LIST_CONTROLLERS_PATH);
    }

    @NotNull
    public static String getHttpAddress(int httpPort) {
        return HttpUtil.RUSEFI_PROXY_JSON_PROTOCOL + RUSEFI_PROXY_HOSTNAME + ":" + httpPort;
    }

    @NotNull
    public static List<PublicSession> getOnlineApplications(String url) throws IOException {
        String responseString = HttpUtil.executeGet(Logger.CONSOLE, url);

        List<PublicSession> userLists = new ArrayList<>();
        try {
            JSONArray array = HttpUtil.getJsonResponse(responseString);

            for (int i = 0; i < array.size(); i++) {
                JSONObject element = (JSONObject) array.get(i);

                ControllerInfo ci = ControllerInfo.valueOf(element);
                UserDetails userDetails = UserDetails.valueOf(element);
                userLists.add(new PublicSession(userDetails, ci));
            }

            System.out.println("object=" + array);
        } catch (ParseException e) {
            throw new IOException(e);
        }
        return userLists;
    }

}
