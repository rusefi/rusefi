package com.rusefi.panama;

import com.devexperts.logging.Logging;
import org.apache.http.HttpEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.mime.MultipartEntityBuilder;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;

import java.io.File;

public class PanamaClient {
    public static final String FILES_API = "/api/files";

    private final static Logging log = Logging.getLogging(PanamaClient.class);

    public static void uploadFile(String url, File file, String userName, int mcuSerial) {
        try {
            HttpPost httppost = new HttpPost(url);

            FileBody fileBody = new FileBody(file);
            StringBody userId = new StringBody(userName, ContentType.TEXT_PLAIN);
            StringBody stmId = new StringBody(Integer.toString(mcuSerial), ContentType.TEXT_PLAIN);

            HttpEntity reqEntity = MultipartEntityBuilder.create()
                    .addPart("file", fileBody)
                    .addPart("userName", userId)
                    .addPart("stmId", stmId)
                    .build();

            httppost.setEntity(reqEntity);

            log.info("executing request " + httppost.getRequestLine());

            try (CloseableHttpClient httpclient = HttpClients.createDefault();
                 CloseableHttpResponse response = httpclient.execute(httppost)) {
                log.info(response.getStatusLine().toString());
                HttpEntity resEntity = response.getEntity();
                if (resEntity != null) {
                    log.info("Response content length: " + resEntity.getContentLength());
                }
                EntityUtils.consume(resEntity);
            }
        } catch (Throwable e) {
            log.error("Error uploading", e);
        }
    }
}
