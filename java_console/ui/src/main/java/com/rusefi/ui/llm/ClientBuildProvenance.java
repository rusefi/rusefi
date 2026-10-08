package com.rusefi.ui.llm;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

/** Generated when the Console is built; never runs Git on the user's machine. */
final class ClientBuildProvenance {
    static final ClientBuildProvenance CURRENT = load(ClientBuildProvenance.class.getResourceAsStream("client-build.properties"));
    final String revision;
    final Boolean dirty;

    private ClientBuildProvenance(String revision, Boolean dirty) {
        this.revision = revision;
        this.dirty = dirty;
    }

    static ClientBuildProvenance load(InputStream input) {
        try (InputStream resource = input) {
            Properties properties = new Properties();
            if (resource != null) { properties.load(resource); }
            String revision = properties.getProperty("revision", "unknown");
            String dirty = properties.getProperty("dirty", "unknown");
            if (revision.matches("[0-9a-f]{40}") && (dirty.equals("true") || dirty.equals("false"))) {
                return new ClientBuildProvenance(revision, Boolean.valueOf(dirty));
            }
        } catch (IOException ignored) { }
        return new ClientBuildProvenance("unknown", null);
    }
}
