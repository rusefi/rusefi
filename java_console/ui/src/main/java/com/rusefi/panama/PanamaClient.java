package com.rusefi.panama;

import com.devexperts.logging.Logging;

import org.apache.hc.client5.http.classic.methods.HttpPost;
import org.apache.hc.client5.http.impl.classic.HttpClients;
import org.apache.hc.client5.http.impl.classic.CloseableHttpClient;
import org.apache.hc.client5.http.impl.classic.CloseableHttpResponse;
import org.apache.hc.client5.http.entity.mime.FileBody;
import org.apache.hc.client5.http.entity.mime.StringBody;
import org.apache.hc.client5.http.entity.mime.MultipartEntityBuilder;

import org.apache.hc.core5.http.HttpEntity;
import org.apache.hc.core5.http.ContentType;
import org.apache.hc.core5.http.io.entity.EntityUtils;

import java.io.File;

public class PanamaClient {
    public static final String FILES_API = "/api/files";

    private final static Logging log = Logging.getLogging(PanamaClient.class);

    public static boolean uploadFile(String url, File file, String userName, int mcuSerial) {
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

            log.info("executing request " + httppost.getRequestUri());

            try (CloseableHttpClient httpclient = HttpClients.createDefault();
                 CloseableHttpResponse response = httpclient.execute(httppost)) {
                log.info("request code: " + response.getCode());
                HttpEntity resEntity = response.getEntity();
                if (resEntity != null) {
                    log.info("Response content length: " + resEntity.getContentLength());
                }
                EntityUtils.consume(resEntity);
            }
            return true;
        } catch (Throwable e) {
            log.error("Error uploading", e);
            return false;
        }
    }
}
