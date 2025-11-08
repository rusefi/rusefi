package com.rusefi.wizard;

import javax.swing.*;
import java.awt.*;

public class TsAccess {
    public static final String TS_TITLE_FRAGMENT = "TunerStudio";

    public enum KnownVersion {
        /**
         * these magic class names are specific to TS build
         * At the moment we use 3.1.02 names
         * http://www.tunerstudio.com/downloads2/TunerStudioMS_Setup_v3.3.00.exe
         * https://github.com/rusefi/rusefi_external_utils/blob/master/TunerStudio/TunerStudioMS_Setup_v3.1.02.exe
         */
        // todo: does same work for 3.3.01?
        TS_3_3_0("3.3.0", "aP.gX", "bq.b"),
        UNKNOWN(null, null, null),
        ;

        private final String tag;
        private final String tsPopupMainMenuClassSuffix;
        private final String tsTopLevelButtonClass;

        KnownVersion(String tag, String tsPopupMainMenuClassSuffix, String TS_TOP_LEVEL_BUTTON_CLASS) {
            this.tag = tag;
            this.tsPopupMainMenuClassSuffix = tsPopupMainMenuClassSuffix;
            tsTopLevelButtonClass = TS_TOP_LEVEL_BUTTON_CLASS;
        }

        public String getTag() {
            return tag;
        }
    }

    private static KnownVersion version = KnownVersion.UNKNOWN;

    public static String getTsPopupMainMenuClassSuffix() {
        return version.tsPopupMainMenuClassSuffix;
    }

    public static String getTsTopLevelButtonClass() {
        return version.tsTopLevelButtonClass;
    }

    public static KnownVersion getVersion() {
        return version;
    }

    public static Frame findTsMainFrame() {
        Frame[] all = JFrame.getFrames();
        for (Frame frame : all) {
            String frameTitle = frame.getTitle();
            System.out.println("findMainFrame: I see " + frameTitle);
            if (frameTitle.contains(TsAccess.TS_TITLE_FRAGMENT)) {
                for (KnownVersion v : KnownVersion.values()) {
                    if (v != KnownVersion.UNKNOWN && frameTitle.contains(v.tag)) {
                        System.out.println("Found main frame for version: " + frameTitle + "/" + v);
                        version = v;
                    }
                }
                return frame;
            }
        }
        return null;
    }
}
