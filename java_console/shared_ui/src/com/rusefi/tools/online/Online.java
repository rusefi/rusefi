package com.rusefi.tools.online;

import com.devexperts.logging.Logging;
import com.rusefi.shared.FileUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.concurrent.BasicFuture;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;

public class Online {
    private final static Logging log = Logging.getLogging(Online.class);

    public static final String outputXmlFileName = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "output.msq";
    private static final String url = "https://rusefi.com/online/upload.php";

    /**
     * blocking call for http file upload
     */
    public static UploadResult upload(File fileName, String authTokenValue) {
        HttpClient httpclient = new DefaultHttpClient();
        HttpPost httpPost = new HttpPost(url);

        String responseString;
        try {
            FileBody uploadFilePart = new FileBody(fileName);
            MultipartEntity reqEntity = new MultipartEntity();
            reqEntity.addPart("upload-file", uploadFilePart);
            reqEntity.addPart("rusefi_token", new StringBody(authTokenValue));

            httpPost.setEntity(reqEntity);

            HttpResponse response = httpclient.execute(httpPost);
            log.debug("response=" + response);
            log.debug("code " + response.getStatusLine().getStatusCode());
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
    public static BasicFuture<UploadResult> uploadTune(Msq tune, AuthTokenPanel authTokenPanel, JComponent parent, FutureCallback<UploadResult> callback) {
        try {
            tune.writeXmlFile(outputXmlFileName);
        } catch (JAXBException | IOException e) {
            throw new IllegalStateException("While writing tune", e);
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
