package com.rusefi.tools.online;

import com.rusefi.proxy.client.LocalApplicationProxy;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.UserDetails;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.tools.online.HttpUtil.RUSEFI_PROXY_HOSTNAME;
import static com.rusefi.tools.online.HttpUtil.getIntProperty;

public class ProxyClient {
    public static final String LIST_CONTROLLERS_PATH = "/list_controllers";
    public static final String LIST_APPLICATIONS_PATH = "/list_applications";
    public static final String VERSION_PATH = "/version";
    public static final String UPDATE_CONNECTOR_SOFTWARE = "/update_connector_software";
    public static final String BACKEND_VERSION = "0.0002";
    public static final String IS_USED = "isUsed";
    public static final String OWNER = "owner";
    /**
     * @see HttpUtil#PROXY_JSON_API_HTTP_PORT
     * @see LocalApplicationProxy#SERVER_PORT_FOR_APPLICATIONS
     */
    public static final int SERVER_PORT_FOR_CONTROLLERS = getIntProperty("controllers.port", 8003);
    public static final String JSON = "json";

    public static List<PublicSession> getOnlineApplications(int httpPort) throws IOException {
        return getOnlineApplications(getHttpAddress(httpPort) + LIST_CONTROLLERS_PATH);
    }

    @NotNull
    public static String getHttpAddress(int httpPort) {
        return HttpUtil.RUSEFI_PROXY_JSON_PROTOCOL + RUSEFI_PROXY_HOSTNAME + ":" + httpPort;
    }

    @NotNull
    public static List<PublicSession> getOnlineApplications(String url) throws IOException {
        String responseString = HttpUtil.executeGet(url);

        List<PublicSession> userLists = new ArrayList<>();
        try {
            JSONArray array = HttpUtil.getJsonResponse(responseString);

            for (int i = 0; i < array.size(); i++) {
                JSONObject element = (JSONObject) array.get(i);

                ControllerInfo ci = ControllerInfo.valueOf(element);
                UserDetails vehicleOwner = UserDetails.valueOf(element);
                boolean isUsed = (Boolean) element.get(IS_USED);
                String ownerName = (String) element.get(OWNER);
                userLists.add(new PublicSession(vehicleOwner, ci, isUsed, ownerName));
            }

            System.out.println("object=" + array);
        } catch (ParseException e) {
            throw new IOException(e);
        }
        return userLists;
    }

}
