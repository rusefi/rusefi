package com.rusefi.core.ui;

import org.junit.jupiter.api.Test;


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
     * The regression itself, against the real packaged resource rather than a jar built by the
     * test. appicon.png used to sit in java_console/shared_ui/resources, which is not a Gradle
     * source set - :shared_ui:processResources reported NO-SOURCE and the icon was never packaged
     * at all, so fixing the doubled slash alone would still have found nothing.
     */
    @Test
    void appIconIsOnTheClasspathAndReachableByItsBareName() {
        assertNotNull(
            AutoupdateUtilTest.class.getResource("/com/rusefi/appicon.png"),
            "appicon.png must be packaged from a real src/main/resources source set"
        );

        assertNotNull(
            AutoupdateUtil.loadIcon("/appicon.png"),
            "setAppIcon() looks the icon up by its bare name, via the secondary resource path"
        );
    }
}
