package com.rusefi.panama;

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
import java.io.IOException;

public class PanamaSandbox {
    public static void main(String[] args) throws IOException {

        HttpPost httppost = new HttpPost("https://rusefi.com/panama-test/a.php");


        FileBody fileBody = new FileBody(new File("build.gradle"));
        StringBody userId = new StringBody("A binary file of some kind", ContentType.TEXT_PLAIN);

        HttpEntity reqEntity = MultipartEntityBuilder.create()
                .addPart("userfile", fileBody)
                .addPart("userId", userId)
                .build();

        httppost.setEntity(reqEntity);

        System.out.println("executing request " + httppost.getRequestLine());

        try (CloseableHttpClient httpclient = HttpClients.createDefault();
             CloseableHttpResponse response = httpclient.execute(httppost)) {
            System.out.println("----------------------------------------");
            System.out.println(response.getStatusLine());
            HttpEntity resEntity = response.getEntity();
            if (resEntity != null) {
                System.out.println("Response content length: " + resEntity.getContentLength());
            }
            EntityUtils.consume(resEntity);
        }
    }
}
