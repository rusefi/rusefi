package com.rusefi.ui;

import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

public class LogoHelper {
    public static final String LINK_TEXT = "rusEFI (c) 2012-2024";
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
        String bundle = BundleUtil.readBundleFullNameNotNull();
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
        return AutoupdateUtil.loadIcon(logoName);
    }

    @NotNull
    public static URLLabel createUrlLabel() {
        return new URLLabel(LINK_TEXT, URI);
    }
}
