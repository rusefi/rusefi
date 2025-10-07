package com.rusefi.autoupdate;

import com.rusefi.core.FileUtil;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.net.ConnectionAndMeta;

import java.io.File;
import java.io.IOException;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class AutoupdateSandbox {
    public static void main(String[] args) throws IOException {
        BundleInfo bi = new BundleInfo(BundleInfo.DEVELOPMENT, null, "proteus_f7");
        String baseUrl = "https://rusefi.com/build_server/" + ConnectionAndMeta.AUTOUPDATE;
        Optional<Autoupdate.DownloadedAutoupdateFileInfo> autoupdateFile = Autoupdate.downloadAutoupdateZipFile(bi, baseUrl, false);
        if (autoupdateFile.isPresent()) {
            AtomicReference<String> srecName = new AtomicReference<>();
            FileUtil.unzip(autoupdateFile.get().zipFileName, new File("."), zipEntry -> {
                boolean result = zipEntry.getName().endsWith(".srec");
                if (result) {
                    srecName.set(zipEntry.getName());
                }
                return result;
            });
        }
    }
}
