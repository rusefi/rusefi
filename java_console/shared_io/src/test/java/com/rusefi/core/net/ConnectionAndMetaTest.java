package com.rusefi.core.net;

import org.junit.jupiter.api.Test;

import javax.net.ssl.HttpsURLConnection;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.security.Principal;
import java.security.cert.Certificate;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class ConnectionAndMetaTest {
    @Test
    void metadataRequestUsesHeadAndReadsHeadersWithoutOpeningZipBody() throws Exception {
        RecordingConnection connection = new RecordingConnection(new URL("https://updates.test/zip"));
        ConnectionAndMeta metadata = new ConnectionAndMeta("bundle.zip", ignored -> connection).invokeMetadata(
            "https://updates.test/");

        assertEquals("HEAD", connection.getRequestMethod());
        assertEquals(200, metadata.getResponseCode());
        assertEquals("application/zip", metadata.getContentType());
        assertEquals(1_800_000_000_000L, metadata.getLastModified());
        assertEquals(0, connection.bodyReads);
        connection.disconnect();
        assertTrue(connection.disconnected);
        assertFalse(connection.getRequestMethod().equals("GET"));
    }

    private static final class RecordingConnection extends HttpsURLConnection {
        private boolean disconnected;
        private int bodyReads;

        private RecordingConnection(URL url) {
            super(url);
        }

        @Override public int getResponseCode() { return HTTP_OK; }
        @Override public String getContentType() { return "application/zip"; }
        @Override public int getContentLength() { return 2048; }
        @Override public long getLastModified() { return 1_800_000_000_000L; }
        @Override public InputStream getInputStream() {
            bodyReads++;
            return new ByteArrayInputStream(new byte[0]);
        }
        @Override public void disconnect() { disconnected = true; }
        @Override public boolean usingProxy() { return false; }
        @Override public void connect() { }
        @Override public String getCipherSuite() { return "test"; }
        @Override public Certificate[] getLocalCertificates() { return null; }
        @Override public Certificate[] getServerCertificates() { return new Certificate[0]; }
        @Override public Principal getPeerPrincipal() { return null; }
        @Override public Principal getLocalPrincipal() { return null; }
    }
}
