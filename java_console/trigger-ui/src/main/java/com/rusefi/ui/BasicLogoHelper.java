package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;

import static com.devexperts.logging.Logging.getLogging;

public class BasicLogoHelper {
    private static final Logging log = getLogging(BasicLogoHelper.class);
    public static final String LOGO_PATH = "/com/rusefi/";
    static final String GENERIC_LOGO = LOGO_PATH + "logo.png";

    // as opposed to LogoHelper.getBundleSpecificIcon
    public static void setGenericFrameIcon(Frame frame) {
        ImageIcon genericIcon = AutoupdateUtil.loadIcon(GENERIC_LOGO);
        log.info("genericIcon " + genericIcon);
        setFrameIcon(frame, genericIcon);
    }

    public static void setFrameIcon(Frame frame, ImageIcon icon) {
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }
}
