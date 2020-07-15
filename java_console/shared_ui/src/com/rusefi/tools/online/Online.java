package com.rusefi.tools.online;

import com.rusefi.shared.FileUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.concurrent.BasicFuture;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;

public class Online {
    public static final String outputXmlFileName = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "output.msq";
    private static final String url = "https://rusefi.com/online/upload.php";

    public static UploadResult upload(File fileName, String authTokenValue) throws IOException {
        HttpClient httpclient = new DefaultHttpClient();
        HttpPost httpPost = new HttpPost(url);

        FileBody uploadFilePart = new FileBody(fileName);
        MultipartEntity reqEntity = new MultipartEntity();
        reqEntity.addPart("upload-file", uploadFilePart);
        reqEntity.addPart("rusefi_token", new StringBody(authTokenValue));

        httpPost.setEntity(reqEntity);

        HttpResponse response = httpclient.execute(httpPost);
        System.out.println("response=" + response);
        System.out.println("code " + response.getStatusLine().getStatusCode());

        HttpEntity entity = response.getEntity();
        String responseString = EntityUtils.toString(entity, "UTF-8");
        System.out.println("responseString=" + responseString);

        JSONParser parser = new JSONParser();
        try {
            JSONObject object = (JSONObject) parser.parse(responseString);
            System.out.println("object=" + object);
            JSONArray info = (JSONArray) object.get("info");
            JSONArray error = (JSONArray) object.get("error");
            if (error != null) {
                System.out.println("error " + error);
                return new UploadResult(true, error);
            } else {
                System.out.println("info " + info);
                return new UploadResult(false, info);
            }

        } catch (ParseException e) {
            return new UploadResult(true, "Error " + e);
        }
    }

    public static BasicFuture<UploadResult> uploadTune(Msq tune, AuthTokenPanel authTokenPanel, JComponent parent, FutureCallback<UploadResult> callback) {
        BasicFuture<UploadResult> result = new BasicFuture<>(callback);
        String authToken = authTokenPanel.getToken();
        if (!authTokenPanel.hasToken()) {
            authTokenPanel.showError(parent);
            return null;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                UploadResult array = doUpload(authToken, tune);
                result.completed(array);
            }
        }).start();
        return result;
    }

    private static UploadResult doUpload(String authToken, Msq tune) {
        try {
            tune.writeXmlFile(outputXmlFileName);
            // todo: network upload should not happen on UI thread
            return upload(new File(outputXmlFileName), authToken);
        } catch (JAXBException | IOException ex) {
            return new UploadResult(true, "IO error " + ex);
        }
    }
}
