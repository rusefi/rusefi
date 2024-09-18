package com.rusefi.ui;

import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;

public class BasicLogoHelper {
    public static final String LOGO_PATH = "/com/rusefi/";
    static final String GENERIC_LOGO = LOGO_PATH + "logo.png";

    // as opposed to LogoHelper.getBundleSpecificIcon
    public static void setGenericFrameIcon(Frame frame) {
        ImageIcon genericIcon = AutoupdateUtil.loadIcon(GENERIC_LOGO);
        setFrameIcon(frame, genericIcon);
    }

    public static void setFrameIcon(Frame frame, ImageIcon icon) {
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }
}
