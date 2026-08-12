package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

import static com.devexperts.logging.Logging.getLogging;

public class LogoHelper {
    private static final Logging log = getLogging(LogoHelper.class);
    public static final String LINK_TEXT = "rusEFI (c) 2012-2026";
    public static final String URI = "http://rusefi.com/?java_console";

    public static JLabel createLogoLabel() {
        ImageIcon logoIcon = getBundleSpecificIcon();
        if (logoIcon == null)
            return null;
        JLabel logo = new JLabel(logoIcon);
        logo.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 10));
        URLLabel.addUrlAction(logo, URLLabel.createUri(URI));
        logo.setCursor(new Cursor(Cursor.HAND_CURSOR));
        return logo;
    }

    @Nullable
    public static ImageIcon getBundleSpecificIcon() {
        String bundle = BundleUtil.readBundleFullNameNotNull().getTarget();
        String logoName;
        // these should be about 213px wide
        if (bundle.contains("proteus")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_proteus.png";
//        } else if (bundle.contains("honda")) {
//            logoName = LOGO_PATH + "logo_tutomo.png";
        } else if (bundle.contains("alphax")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_alphax.png";
        } else if (bundle.contains(".mre")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_mre.png";
        } else {
            logoName = BasicLogoHelper.GENERIC_LOGO;
        }
        ImageIcon imageIcon = AutoupdateUtil.loadIcon(logoName);
        log.info(imageIcon + " for " + logoName);
        return imageIcon;
    }

    @NotNull
    public static URLLabel createUrlLabel() {
        return new URLLabel(LINK_TEXT, URI);
    }
}
