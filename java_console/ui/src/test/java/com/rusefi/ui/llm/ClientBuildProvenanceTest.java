package com.rusefi.ui.llm;

import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

class ClientBuildProvenanceTest {
    @Test void readsBuildResourceWithoutRuntimeGit() {
        assertTrue(ClientBuildProvenance.CURRENT.revision.matches("[0-9a-f]{40}"));
        assertNotNull(ClientBuildProvenance.CURRENT.dirty);
    }

    @Test void missingAndMalformedProvenanceCannotClaimCleanRevision() {
        assertEquals("unknown", ClientBuildProvenance.load(null).revision);
        ClientBuildProvenance invalid = ClientBuildProvenance.load(new ByteArrayInputStream(
                "revision=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\ndirty=maybe\n".getBytes(StandardCharsets.UTF_8)));
        assertEquals("unknown", invalid.revision);
        assertNull(invalid.dirty);
    }
}
