package com.rusefi.ui;

import com.rusefi.core.ui.AutoupdateUtil;
import org.junit.jupiter.api.Test;

import java.awt.*;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.*;

class LogoHelperTest {
    private static final int ORANGE = 0xfff26522;

    @Test
    void onADarkBackgroundTheBlackOfATransparentLogoTurnsWhite() {
        BufferedImage logo = new BufferedImage(4, 1, BufferedImage.TYPE_INT_ARGB);
        logo.setRGB(0, 0, 0x00000000);
        logo.setRGB(1, 0, 0xff101010);
        logo.setRGB(2, 0, 0x80202020);
        logo.setRGB(3, 0, ORANGE);
        BufferedImage dark = (BufferedImage) LogoHelper.forDarkBackground(logo);
        assertEquals(0x00000000, dark.getRGB(0, 0) & 0xff000000, "the background stays transparent");
        assertEquals(0xffffffff, dark.getRGB(1, 0), "black lettering turns white");
        assertEquals(0x80ffffff, dark.getRGB(2, 0), "an anti-aliased edge keeps its transparency");
        assertEquals(ORANGE, dark.getRGB(3, 0), "colour stays");
    }

    @Test
    void aLogoWithItsOwnBackgroundIsLeftAsDrawn() {
        BufferedImage logo = new BufferedImage(2, 1, BufferedImage.TYPE_INT_ARGB);
        logo.setRGB(0, 0, 0xff000000);
        logo.setRGB(1, 0, 0xffffffff);
        assertSame(logo, LogoHelper.forDarkBackground(logo), "e.g. the AlphaX and Proteus logos");
    }

    @Test
    void theRusefiLogoKeepsNoDarkGreyOnADarkBackground() {
        Image logo = AutoupdateUtil.loadIcon(BasicLogoHelper.GENERIC_LOGO).getImage();
        BufferedImage dark = (BufferedImage) LogoHelper.forDarkBackground(logo);
        int dark_grey = 0;
        int orange = 0;
        for (int y = 0; y < dark.getHeight(); y++) {
            for (int x = 0; x < dark.getWidth(); x++) {
                int argb = dark.getRGB(x, y);
                if ((argb >>> 24) < 40) {
                    continue;
                }
                int r = (argb >> 16) & 0xff;
                int g = (argb >> 8) & 0xff;
                int b = argb & 0xff;
                if (r < 80 && g < 80 && b < 80) {
                    dark_grey++;
                }
                if (r > 200 && g < 140 && b < 90) {
                    orange++;
                }
            }
        }
        assertEquals(0, dark_grey, "\"EFI\" is no longer dark");
        assertTrue(orange > 1000, "\"rus\" stays orange");
    }

    @Test
    void darkMeansADarkPanelBackground() {
        assertTrue(LogoHelper.isDark(new Color(0x1e1f22)));
        assertFalse(LogoHelper.isDark(new Color(0xf2f2f2)));
        assertFalse(LogoHelper.isDark(null));
    }
}
