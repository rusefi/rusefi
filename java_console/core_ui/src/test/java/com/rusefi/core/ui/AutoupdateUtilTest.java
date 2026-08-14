package com.rusefi.core.ui;

import org.junit.jupiter.api.Test;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 * #6928: the updater frame showed the default Java icon because the secondary resource path was
 * built by plain concatenation, producing a doubled slash which a jar entry lookup never matches.
 */
class AutoupdateUtilTest {
    @Test
    void absolutePathDoesNotProduceADoubledSlash() {
        assertEquals("/com/rusefi/appicon.png", AutoupdateUtil.secondaryResourcePath("/appicon.png"));
    }

    @Test
    void relativePathIsMappedAsWell() {
        assertEquals("/com/rusefi/appicon.png", AutoupdateUtil.secondaryResourcePath("appicon.png"));
    }

    @Test
    void nestedPathKeepsItsOwnFolders() {
        assertEquals("/com/rusefi/icons/go.png", AutoupdateUtil.secondaryResourcePath("/icons/go.png"));
    }

    @Test
    void resultNeverContainsADoubledSlash() {
        for (final String candidate : new String[]{"/appicon.png", "appicon.png", "/icons/go.png"}) {
            final String actual = AutoupdateUtil.secondaryResourcePath(candidate);
            assertFalse(actual.contains("//"), candidate + " -> " + actual);
        }
    }

    /**
     * The regression itself: loading from a jar, which is what the shipped console does. A class
     * directory tolerates the doubled slash, which is why this only ever reproduced outside the IDE.
     */
    @Test
    void iconIsFoundInsideAJar() throws Exception {
        final Path jar = createJarContainingAppIcon();

        AutoupdateUtil.getClassLoaderByJar(jar.toString());

        final ImageIcon icon = AutoupdateUtil.loadIcon("/appicon.png");
        assertNotNull(icon, "appicon.png should be found under the /com/rusefi/ resource package");
    }

    /**
     * Deliberately not a {@code @TempDir} file: the jar is added to a long lived static class
     * loader, which keeps a handle on it, and Windows refuses to delete an open file - JUnit would
     * then fail the test during temp directory cleanup even though the assertion passed.
     */
    private Path createJarContainingAppIcon() throws Exception {
        final File jarFile = File.createTempFile("rusefi-icons-", ".jar");
        jarFile.deleteOnExit();
        final Path jar = jarFile.toPath();
        try (final OutputStream fileStream = Files.newOutputStream(jar);
             final JarOutputStream jarStream = new JarOutputStream(fileStream)
        ) {
            jarStream.putNextEntry(new JarEntry("com/rusefi/appicon.png"));
            ImageIO.write(new BufferedImage(16, 16, BufferedImage.TYPE_INT_ARGB), "png", jarStream);
            jarStream.closeEntry();
        }
        return jar;
    }
}
