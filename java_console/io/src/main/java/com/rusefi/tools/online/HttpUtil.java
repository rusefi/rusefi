package com.rusefi.tools.online;

import com.devexperts.logging.Logging;
import org.apache.hc.client5.http.classic.methods.HttpGet;
import org.apache.hc.client5.http.impl.classic.CloseableHttpClient;
import org.apache.hc.client5.http.impl.classic.CloseableHttpResponse;
import org.apache.hc.client5.http.impl.classic.HttpClients;
import org.apache.hc.client5.http.impl.io.PoolingHttpClientConnectionManager;
import org.apache.hc.client5.http.impl.io.PoolingHttpClientConnectionManagerBuilder;
import org.apache.hc.core5.http.HttpEntity;
import org.apache.hc.core5.http.io.SocketConfig;
import org.apache.hc.core5.http.io.entity.EntityUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.net.http.HttpResponse;

import static com.devexperts.logging.Logging.getLogging;

public class HttpUtil {
    private static final Logging log = getLogging(Logging.class);

    // todo: migrate proxy http json API server to TLS
    public static final String RUSEFI_PROXY_JSON_PROTOCOL = "http://";
    public static final int PROXY_JSON_API_HTTP_PORT = getIntProperty("http.port", 8001);

    /**
     * hostname of PROXY server, not primary rusEFI web server - those are two separate hosts at the moment
     */
    public static String RUSEFI_PROXY_HOSTNAME = System.getProperty("RUSEFI_PROXY_URL", "proxy.rusefi.com");

    public static String RUSEFI_ONLINE_JSON_API_PREFIX = "https://rusefi.com/online/api.php?method=";

    public static <T> T getJsonResponse(String responseString) throws ParseException {

        JSONParser parser = new JSONParser();
        return (T) parser.parse(responseString);
    }

    public static String getResponse(CloseableHttpResponse response) throws IOException {
		HttpEntity entity = response.getEntity();
		String responseString;
		try {
			responseString = EntityUtils.toString(entity, "UTF-8");
			log.info("responseString=" + responseString);
			return responseString;
		} catch (org.apache.hc.core5.http.ParseException | IOException e) {
			log.info("Error decoding the response string!");
			e.printStackTrace();
		}
		return "";
    }

    public static String executeGet(String url) throws IOException {
        // without this magic http response is pretty slow
		// TODO: move to a more global scope if more request types are needed (for reuse
        // the connectionManager/httpclient)
        SocketConfig socketConfig = SocketConfig.custom().setSoKeepAlive(true).setTcpNoDelay(true).build();
		PoolingHttpClientConnectionManager connectionManager = PoolingHttpClientConnectionManagerBuilder.create()
                .setDefaultSocketConfig(socketConfig).build();
        CloseableHttpClient httpclient = HttpClients.custom().setConnectionManager(connectionManager).build();

        log.info("GET " + url);
        HttpGet httpget = new HttpGet(url);

        // in case of emergency
        //  -Dorg.apache.commons.logging.Log=org.apache.commons.logging.impl.SimpleLog -Dorg.apache.commons.logging.simplelog.showdatetime=true -Dorg.apache.commons.logging.simplelog.log.org.apache.http=DEBUG -Dorg.apache.commons.logging.simplelog.log.org.apache.http.wire=ERROR
        try {
            CloseableHttpResponse httpResponse = httpclient.execute(httpget);
            return HttpUtil.getResponse(httpResponse);
        } finally {
        httpclient.close();
        }
    }

    public static JSONObject parse(String jsonString) {
        JSONParser parser = new JSONParser();
        JSONObject jsonObject;
        try {
            jsonObject = (JSONObject) parser.parse(jsonString);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }
        return jsonObject;
    }

    public static int getIntProperty(String propertyName, int defaultValue) {
        return Integer.parseInt(System.getProperty(propertyName, Integer.toString(defaultValue)));
    }
}
