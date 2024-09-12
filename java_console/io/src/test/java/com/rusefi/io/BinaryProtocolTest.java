package com.rusefi.io;

import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.CommandExecutorMock;
import com.rusefi.binaryprotocol.BinaryProtocol;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.mockito.Mockito.mock;

import java.io.IOException;

public class BinaryProtocolTest {
    private final IoStream dummyIoStream = mock(IoStream.class);
    private LinkManager dummyLinkManager;
    private ConfigurationImage configurationImageWithDisabledLaunchControl;
    private ConfigurationImage configurationImageWithEnabledLaunchControl;
    private CommandExecutorMock testCommandExecutor;
    private BinaryProtocol binaryProtocol;

    @BeforeEach
    public void setUp() throws IOException {
        dummyLinkManager = new LinkManager();
        configurationImageWithDisabledLaunchControl = ConfigurationImageFile.readFromFile(
            "..\\..\\java_tools\\tune-tools\\src\\test\\resources\\uaefi_test_configuration_image_with_disabled_launch_control.binary_image"
        );
        configurationImageWithEnabledLaunchControl = ConfigurationImageFile.readFromFile(
            "..\\..\\java_tools\\tune-tools\\src\\test\\resources\\uaefi_test_configuration_image_with_enabled_launch_control.binary_image"
        );
        testCommandExecutor = new CommandExecutorMock(configurationImageWithEnabledLaunchControl);
        binaryProtocol = new BinaryProtocol(dummyLinkManager, dummyIoStream, testCommandExecutor);
    }

    @AfterEach
    public void tearDown() {
        binaryProtocol.close();
        dummyLinkManager.close();
    }

    @Test
    public void testUploadChanges() {
        assertArrayEquals(
            testCommandExecutor.getConfigurationImage().getContent(),
            configurationImageWithEnabledLaunchControl.getContent()
        );
        // Below we manually set current configuration image in binary protocol to avoid NPE in
        // BinaryProtocol.uploadChanges method.
        // TODO: think how we can avoid this manual setting
        binaryProtocol.setController(configurationImageWithEnabledLaunchControl);

        binaryProtocol.uploadChanges(configurationImageWithDisabledLaunchControl);

        assertArrayEquals(
            testCommandExecutor.getConfigurationImage().getContent(),
            configurationImageWithDisabledLaunchControl.getContent()
        );
    }
}
