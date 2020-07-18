package com.rusefi.server;

import javax.net.ssl.*;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.KeyStore;
import java.security.cert.X509Certificate;

public class rusEFISSLContext {
    private static final String TLS = "TLS";
//    private static KeyStore key;

    // todo: one day once rusEFI has a proper commercial certificate this should be removed
    private static TrustManager[] trustAllCerts = new TrustManager[]{
            new X509TrustManager() {
                public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                    return new X509Certificate[0];
                }

                public void checkClientTrusted(
                        java.security.cert.X509Certificate[] certs, String authType) {
                }

                public void checkServerTrusted(
                        java.security.cert.X509Certificate[] certs, String authType) {
                }
            }
    };

    public static void init(String fileName, String password) throws MalformedURLException {
        // system property setup does not work under Jenkins?
        Backend.setupCertificates(new File(fileName), password);

        //key = getFromPath(fileName, "PKCS12", password);
    }

    public static ServerSocket getSSLServerSocket(int port) {
        try {
            return SSLServerSocketFactory.getDefault().createServerSocket(port);
        } catch (IOException e) {
            throw new IllegalStateException("Error binding secure server socket " + port, e);
        }
//        try {
//            return getSSLServerSocketFactory(key, TLS).createServerSocket(port);
//        } catch (Exception e) {
//            throw new IllegalStateException(e);
//        }
    }

    public static Socket getSSLSocket(String host, int port) {
        try {
            return getSSLSocketFactory(null /*key*/, TLS).createSocket(host, port);
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    /*
        private static SSLServerSocketFactory getSSLServerSocketFactory(KeyStore trustKey, String sslAlgorithm) {
            try {
                TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
                tmf.init(trustKey);

                SSLContext context = SSLContext.getInstance(sslAlgorithm);
                context.init(null, tmf.getTrustManagers(), null);

                return context.getServerSocketFactory();
            } catch (Exception e) {
                throw new IllegalStateException(e);
            }
        }
    */
    private static SSLSocketFactory getSSLSocketFactory(KeyStore trustKey, String sslAlgorithm) {
        try {
//            TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
//            tmf.init(trustKey);

            SSLContext context = SSLContext.getInstance(sslAlgorithm);
            context.init(null, trustAllCerts/*tmf.getTrustManagers()*/, null);

            return context.getSocketFactory();
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }
/*
    private static KeyStore getFromPath(String path, String algorithm, String filePassword) {
        try {
            File f = new File(path);

            if (!f.exists())
                throw new RuntimeException("File not found: " + path);

            FileInputStream keyFile = new FileInputStream(f);
            KeyStore keystore = KeyStore.getInstance(algorithm);
            keystore.load(keyFile, filePassword.toCharArray());
            keyFile.close();

            return keystore;
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }
*/
}
