package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.util.Arrays;
import java.util.Optional;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class UaefiConfigurationImageSandbox {
    private static final File ORIGINAL_CONFIGURATION_IMAGE_FILE = new File(
        "uaefi_original_configuration_image.rusefi_binary"
    );

    private static final File CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_TO_WRITE_FILE = new File(
        "uaefi_configuration_image_with_disabled_launch_control_to_write.rusefi_binary"
    );

    private static final File WRITTEN_CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_FILE = new File(
        "written_uaefi_configuration_image_with_disabled_launch_control.rusefi_binary"
    );

    private static final File CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_TO_WRITE_FILE = new File(
        "uaefi_configuration_image_with_enabled_launch_control_to_write.rusefi_binary"
    );

    private static final File WRITTEN_CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_FILE = new File(
        "written_uaefi_configuration_image_with_enabled_launch_control.rusefi_binary"
    );

    private static void writeToFile(final ConfigurationImage confImage, final File output) throws IOException {
        if (!output.exists()) {
            try (final FileOutputStream fos = new FileOutputStream(output.getAbsolutePath(), false)) {
                fos.write(confImage.getContent());
            }
        } else {
            throw new FileAlreadyExistsException(output.getAbsolutePath());
        }
    }

    private static void deletePreviousImages() {
        ORIGINAL_CONFIGURATION_IMAGE_FILE.delete();
        CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_TO_WRITE_FILE.delete();
        WRITTEN_CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_FILE.delete();
        CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_TO_WRITE_FILE.delete();
        WRITTEN_CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_FILE.delete();
    }

    private static void deleteCachedConfigurationImage() {
        final File cachedConfigurationImage = new File("current_configuration.binary_image");
        if (cachedConfigurationImage.exists()) {
            cachedConfigurationImage.delete();
        }
    }

    private static void writeCurrentConfigurationImageToFile(
        final LinkManager linkManager,
        final File output,
        final ConfigurationImageMeta meta,
        final Optional<ConfigurationImage> expectedConfImage
    ) throws InterruptedException {
        final CountDownLatch latch = new CountDownLatch(1);
        linkManager.execute(() -> {
            deleteCachedConfigurationImage();
            linkManager.getBinaryProtocol().readImage(new BinaryProtocol.Arguments(false), meta);
            final ConfigurationImage currentConfImage = linkManager.getBinaryProtocol().getControllerConfiguration();
            try {
                writeToFile(currentConfImage, output);
            } catch (IOException e) {
                System.out.printf("ERROR: Writing error to configuration image file: %s%n", output.getAbsolutePath());
            }
            if (expectedConfImage.isPresent()) {
                if (!Arrays.equals(expectedConfImage.get().getContent(), currentConfImage.getContent())) {
                    System.out.printf("ERROR: Unexpected configuration image in file: %s %n", output.getAbsolutePath());
                    throw new IllegalStateException("Unexpected configuration image in file");
                }
            }
            latch.countDown();
        });
        latch.await(1, TimeUnit.MINUTES);
    }

    public static void main(String[] args) throws InterruptedException, IOException {
        deletePreviousImages();

        final SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(callbackContext -> null);
        final String port = autoDetectResult.getSerialPort();
        System.out.println("Serial detected on " + port);

        final LinkManager linkManager = new LinkManager()
            .setNeedPullText(false)
            .setNeedPullLiveData(true);

        try {
            linkManager.connect(port).await(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException("Not connected in time");
        }

        final ConfigurationImageWithMeta testConfigurationImageWithDisabledLaunchControl = ConfigurationImageFile.readFromFile(
            "..\\java_tools\\tune-tools\\src\\test\\resources\\uaefi_test_configuration_image_with_disabled_launch_control.binary_image"
        );
        writeToFile(testConfigurationImageWithDisabledLaunchControl, CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_TO_WRITE_FILE);

        writeCurrentConfigurationImageToFile(
            linkManager,
            ORIGINAL_CONFIGURATION_IMAGE_FILE,
            testConfigurationImageWithDisabledLaunchControl.getMeta(),
            Optional.empty()
        );

        final CountDownLatch latch = new CountDownLatch(1);
        linkManager.execute(() -> {
            linkManager.getBinaryProtocol().uploadChanges(testConfigurationImageWithDisabledLaunchControl);
            latch.countDown();
        });
        latch.await(1, TimeUnit.MINUTES);

        writeCurrentConfigurationImageToFile(
            linkManager,
            WRITTEN_CONFIGURATION_IMAGE_WITH_DISABLED_LAUNCH_CONTROL_FILE,
            testConfigurationImageWithDisabledLaunchControl.getMeta(),
            Optional.of(testConfigurationImageWithDisabledLaunchControl)
        );

        final ConfigurationImageWithMeta testConfigurationImageWithEnabledLaunchControl = ConfigurationImageFile.readFromFile(
            "..\\java_tools\\tune-tools\\src\\test\\resources\\uaefi_test_configuration_image_with_enabled_launch_control.binary_image"
        );
        writeToFile(testConfigurationImageWithEnabledLaunchControl, CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_TO_WRITE_FILE);

        linkManager.execute(() -> {
            linkManager.getBinaryProtocol().uploadChanges(testConfigurationImageWithEnabledLaunchControl);
            latch.countDown();
        });
        latch.await(1, TimeUnit.MINUTES);

        writeCurrentConfigurationImageToFile(
            linkManager,
            WRITTEN_CONFIGURATION_IMAGE_WITH_ENABLED_LAUNCH_CONTROL_FILE,
            testConfigurationImageWithEnabledLaunchControl.getMeta(),
            Optional.of(testConfigurationImageWithEnabledLaunchControl)
        );
        linkManager.close();
    }
}
