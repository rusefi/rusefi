package com.rusefi.maintenance;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.startsWith;
import static org.mockito.Mockito.*;

public class FirmwareFlashEligibilityTest {
    @TempDir
    Path temp;

    private final BundleInfo bundle = new BundleInfo("development", null, "test_board");

    @Test
    public void bundledUpdateUsesBundleIdentity() {
        assertSame(bundle, FirmwareFlashEligibility.resolveDestination(null, bundle));
    }

    @Test
    public void explicitArtifactOverridesActiveBundle() {
        BundleInfo destination = FirmwareFlashEligibility.resolveDestination(
            "some/path/rusefi_lts-test_2026-09-09_test_board_123_hash_update.srec", bundle);
        assertNotNull(destination);
        assertEquals("lts-test", destination.getBranchName());
        assertEquals("test_board", destination.getTarget());
    }

    @Test
    public void masterArtifactNormalizesToDevelopment() {
        assertEquals(BundleInfo.DEVELOPMENT, FirmwareFlashEligibility.resolveDestination(
            "rusefi_master_2026-09-09_test_board_123_hash_update.srec", bundle).getBranchName());
    }

    @Test
    public void unknownExplicitArtifactDoesNotBorrowBundleIdentity() {
        assertNull(FirmwareFlashEligibility.resolveDestination("custom.srec", bundle));
    }

    @Test
    public void noProviderPreservesExistingBehavior() throws IOException {
        withProviders("", () -> {
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            assertTrue(FirmwareFlashEligibility.isAllowed(null, null, callbacks));
            verifyNoInteractions(callbacks);
        });
    }

    @Test
    public void providerReceivesRunningSignatureAndSelectedDestination() throws IOException {
        withProviders(RejectingPolicy.class.getName(), () -> {
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            assertFalse(FirmwareFlashEligibility.isAllowed("running signature",
                "rusefi_lts-test_2026-09-09_test_board_123_hash_update.srec", callbacks));
            verify(callbacks).firmwareUpdateBlocked("running signature -> lts-test");
        });
    }

    @Test
    public void brokenProviderAbortsInsteadOfAllowingFlash() throws IOException {
        assertProviderFailure("does.not.Exist");
    }

    @Test
    public void multipleProvidersAbortInsteadOfSelectingOne() throws IOException {
        assertProviderFailure(RejectingPolicy.class.getName() + "\n" + ThrowingPolicy.class.getName());
    }

    @Test
    public void providerExceptionAbortsInsteadOfAllowingFlash() throws IOException {
        assertProviderFailure(ThrowingPolicy.class.getName());
    }

    @Test
    public void directAutomaticApisRejectBeforeHardwareAccess() throws IOException {
        withProviders(RejectingPolicy.class.getName(), () -> {
            // Lima has the automatic serial updater, but not master's automatic SocketCAN updater.
            for (String port : new String[]{"TEST_SERIAL_PORT"}) {
                LinkManager lm = mock(LinkManager.class);
                BinaryProtocol bp = mock(BinaryProtocol.class);
                bp.signature = "running signature";
                when(lm.getLastTriedPort()).thenReturn(port);
                ConnectivityContext connectivity = mock(ConnectivityContext.class);
                UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
                PortResult ecu = new PortResult(port, SerialPortType.EcuWithOpenblt);
                String file = "rusefi_lts-test_2026-09-09_test_board_123_hash_update.srec";

                assertFalse(ProgramSelector.flashOpenbltSerialAutomatic(null, ecu, bp, lm,
                    callbacks, connectivity, file));

                verifyNoMoreInteractions(lm);
                verifyNoInteractions(bp, connectivity);
                verify(callbacks).firmwareUpdateBlocked("running signature -> lts-test");
            }
        });
    }

    private void assertProviderFailure(String providers) throws IOException {
        withProviders(providers, () -> {
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            assertFalse(FirmwareFlashEligibility.isAllowed(null, null, callbacks));
            verify(callbacks).firmwareUpdateBlocked(startsWith("Unable to verify firmware flashing eligibility:"));
        });
    }

    private void withProviders(String providers, Runnable test) throws IOException {
        URL resource = Files.write(temp.resolve("providers"), providers.getBytes(StandardCharsets.UTF_8)).toUri().toURL();
        ClassLoader previous = Thread.currentThread().getContextClassLoader();
        ClassLoader loader = new ClassLoader(getClass().getClassLoader()) {
            @Override
            public Enumeration<URL> getResources(String name) throws IOException {
                if (name.equals("META-INF/services/" + FirmwareFlashPolicy.class.getName())) {
                    return Collections.enumeration(Collections.singletonList(resource));
                }
                return super.getResources(name);
            }
        };
        try {
            Thread.currentThread().setContextClassLoader(loader);
            test.run();
        } finally {
            Thread.currentThread().setContextClassLoader(previous);
        }
    }

    public static class RejectingPolicy implements FirmwareFlashPolicy {
        @Override
        public Optional<String> getRejectionReason(String currentSignature, BundleInfo destination) {
            return Optional.of(currentSignature + " -> " + destination.getBranchName());
        }
    }

    public static class ThrowingPolicy implements FirmwareFlashPolicy {
        @Override
        public Optional<String> getRejectionReason(String currentSignature, BundleInfo destination) {
            throw new IllegalStateException("policy failed");
        }
    }
}
