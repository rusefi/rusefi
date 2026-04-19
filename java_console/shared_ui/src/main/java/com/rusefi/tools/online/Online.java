package com.rusefi.tools.online;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ts_plugin.ui.AuthTokenPanel;

import org.apache.hc.client5.http.classic.methods.HttpPost;
import org.apache.hc.client5.http.impl.classic.HttpClients;
import org.apache.hc.client5.http.impl.classic.CloseableHttpClient;
import org.apache.hc.client5.http.impl.classic.CloseableHttpResponse;
import org.apache.hc.client5.http.entity.mime.FileBody;
import org.apache.hc.client5.http.entity.mime.MultipartEntityBuilder;

import org.apache.hc.core5.concurrent.BasicFuture;
import org.apache.hc.core5.concurrent.FutureCallback;
import org.apache.hc.core5.http.HttpEntity;
import org.apache.hc.core5.http.ContentType;

import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class Online {
    private final static Logging log = Logging.getLogging(Online.class);

    public static final String outputXmlFileName = FileUtil.RUSEFI_SETTINGS_FOLDER + "output.msq";
    private static final String url = "https://rusefi.com/online/upload.php";

    /**
     * blocking call for http file upload
     */
    public static UploadResult upload(File fileName, String authTokenValue) {
    	CloseableHttpClient httpclient = HttpClients.custom().build();
        HttpPost httpPost = new HttpPost(url);

        String responseString;
        try {
            FileBody uploadFilePart = new FileBody(fileName);
            HttpEntity reqEntity = MultipartEntityBuilder.create()
                .addPart("upload-file", uploadFilePart)
                .addTextBody("rusefi_token", authTokenValue, ContentType.TEXT_PLAIN.withCharset(StandardCharsets.UTF_8))
                .build();

            httpPost.setEntity(reqEntity);

            CloseableHttpResponse response = httpclient.execute(httpPost);
            log.debug("response=" + response);
            log.debug("code " + response.getCode());
            responseString = HttpUtil.getResponse(response);

        } catch (IOException e) {
            return new UploadResult(true, "Upload io ERROR " + e);
        }

        try {
            JSONObject object = HttpUtil.getJsonResponse(responseString);

            log.debug("object=" + object);
            JSONArray info = (JSONArray) object.get("info");
            JSONArray error = (JSONArray) object.get("error");
            if (error != null) {
                log.error("error " + error);
                return new UploadResult(true, error);
            } else {
                log.debug("info " + info);
                return new UploadResult(false, info);
            }

        } catch (ParseException e) {
            return new UploadResult(true, "Upload Error " + responseString);
        }
    }

    /**
     * we are here in case of individual tune upload
     */
    public static BasicFuture<UploadResult> uploadTune(Msq tune, JComponent parent, FutureCallback<UploadResult> callback) {
        try {
            tune.writeXmlFile(outputXmlFileName);
        } catch (JAXBException | IOException e) {
            throw new IllegalStateException("writeXmlFile failed writing tune", e);
        }

        return uploadFile(parent, callback, outputXmlFileName);
    }

    @Nullable
    public static BasicFuture<UploadResult> uploadFile(JComponent parent, FutureCallback<UploadResult> callback, final String fileName) {
        BasicFuture<UploadResult> result = new BasicFuture<>(callback);
        String authToken = AuthTokenPanel.getAuthToken();
        if (!AuthTokenPanel.hasToken()) {
            AuthTokenPanel.showError(parent);
            return null;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                UploadResult array = upload(new File(fileName), authToken);
                result.completed(array);
            }
        }).start();
        return result;
    }
}

