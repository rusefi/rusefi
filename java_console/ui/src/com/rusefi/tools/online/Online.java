package com.rusefi.tools.online;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.nio.file.Files;

public class Online {
    static final String charset = "UTF-8";
    static final String url = "https://rusefi.com/online/upload.php";
    static String CRLF = "\r\n"; // Line separator required by multipart/form-data.

    public static void upload(File xmlFile, String authTokenValue) throws IOException {
        String boundary = Long.toHexString(System.currentTimeMillis()); // Just generate some unique random value.

        URLConnection connection = new URL(url).openConnection();
        connection.setDoOutput(true);
        connection.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + boundary);

        try (
                OutputStream output = connection.getOutputStream();
                PrintWriter writer = new PrintWriter(new OutputStreamWriter(output, charset), true);
        ) {
            sendParameter(boundary, writer, "auth_token", authTokenValue);

            // Send text file.
            writer.append("--" + boundary).append(CRLF);
            writer.append("Content-Disposition: form-data; name=\"textFile\"; filename=\"" + xmlFile.getName() + "\"").append(CRLF);
            writer.append("Content-Type: text/plain; charset=" + charset).append(CRLF); // Text file itself must be saved in this charset!
            writer.append(CRLF).flush();
            Files.copy(xmlFile.toPath(), output);
            output.flush(); // Important before continuing with writer!
            writer.append(CRLF).flush(); // CRLF is important! It indicates end of boundary.

            // End of multipart/form-data.
            writer.append("--" + boundary + "--").append(CRLF).flush();
        }


        HttpURLConnection httpURLConnection = (HttpURLConnection) connection;
        int responseCode = httpURLConnection.getResponseCode();
        System.out.println(responseCode); // Should be 200

        BufferedReader br;
        if (responseCode >= 200 && responseCode <= 299) {
            br = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream()));
        } else {
            br = new BufferedReader(new InputStreamReader(httpURLConnection.getErrorStream()));
        }
        String output;
        while ((output = br.readLine()) != null) {
            System.out.println(output);
        }
    }

    private static void sendParameter(String boundary, PrintWriter writer, String parameterName, String value) {
        // Send normal param.
        writer.append("--" + boundary).append(CRLF);
        writer.append("Content-Disposition: form-data; name=\"" + parameterName +"\"").append(CRLF);
        writer.append("Content-Type: text/plain; charset=" + charset).append(CRLF);
        writer.append(CRLF).append(value).append(CRLF).flush();
    }
}
