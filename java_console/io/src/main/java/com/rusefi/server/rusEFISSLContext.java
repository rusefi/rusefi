package com.rusefi.server;

import javax.net.ssl.*;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.KeyStore;
import java.security.cert.X509Certificate;
import java.util.Objects;

public class rusEFISSLContext {
    private static final String TLS = "TLS";

    // I assume that jenkins own setup is interfering with our attempts to change system properties :(
    // also open question to get SSL context adjusted without touching system properties
    private static boolean isJenkins = true;//System.getProperty("JENKINS_URL") != null;

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
        if (!isJenkins)
            setupCertificates(new File(fileName), password);

        //key = getFromPath(fileName, "PKCS12", password);
    }

    public static ServerSocket getSSLServerSocket(int port) {
        try {
            if (isJenkins)
                return new ServerSocket(port);
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

    public static Socket getSSLSocket(String host, int port) throws IOException {
        if (isJenkins) {
            Socket socket = new Socket(host, port);
            // responsiveness matters (getting a byte sent to be received as promptly as possible)
            socket.setTcpNoDelay(true);
            return socket;
        }
        return getSSLSocketFactory(null /*key*/, TLS).createSocket(host, port);
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

    public static void setupCertificates(File certificate, String password) throws MalformedURLException {
        if (!certificate.exists())
            throw new IllegalStateException("Certificate not found " + certificate);
        Objects.requireNonNull(password, "password");

        String file = certificate.toURI().toURL().getFile();
        System.setProperty("javax.net.ssl.keyStore", file);
        System.setProperty("javax.net.ssl.keyStorePassword", password);
        System.setProperty("javax.net.ssl.trustStore", file);
        System.setProperty("javax.net.ssl.trustStorePassword", password);
        System.setProperty("javax.net.ssl.keyStoreType", "PKCS12");
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
