package com.rusefi.autoupdate;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.net.ConnectionAndMeta;

public class AutoupdateSandbox {
    public static void main(String[] args) {
        BundleInfo bi = new BundleInfo(BundleInfo.DEVELOPMENT, null, "proteus_f7");
        String baseUrl = "https://rusefi.com/build_server/" + ConnectionAndMeta.AUTOUPDATE;
        Autoupdate.downloadAutoupdateZipFile(bi, baseUrl, false);
    }
}
