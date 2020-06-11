package com.rusefi.tools.online;

import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.io.*;

import static com.rusefi.ui.AuthTokenPanel.TOKEN_WARNING;

public class Online {
    private static final String url = "https://rusefi.com/online/upload.php";

    public static void upload(File xmlFile, String authTokenValue) throws IOException {
        HttpClient httpclient = new DefaultHttpClient();
        HttpPost httpPost = new HttpPost(url);

        FileBody uploadFilePart = new FileBody(xmlFile);
        MultipartEntity reqEntity = new MultipartEntity();
        reqEntity.addPart("upload-file", uploadFilePart);
        reqEntity.addPart("rusefi_token", new StringBody(authTokenValue));

        httpPost.setEntity(reqEntity);

        HttpResponse response = httpclient.execute(httpPost);
        System.out.println(response);

        HttpEntity entity = response.getEntity();
        String responseString = EntityUtils.toString(entity, "UTF-8");
        System.out.println(responseString);

    }

    public static void uploadTune(Msq tune, AuthTokenPanel authTokenPanel, JComponent parent) {
        String authToken = authTokenPanel.getToken();
        if (authToken.contains(TOKEN_WARNING)) {
            JOptionPane.showMessageDialog(parent, "Does not work without auth token");
            return;
        }
        new Thread(() -> doUpload(authToken, tune)).start();
    }

    private static void doUpload(String authToken, Msq tune) {
        try {
            tune.writeXmlFile(Msq.outputXmlFileName);
            // todo: network upload should not happen on UI thread
            upload(new File(Msq.outputXmlFileName), authToken);
        } catch (JAXBException | IOException ex) {
            throw new IllegalStateException(ex);
        }
    }
}
