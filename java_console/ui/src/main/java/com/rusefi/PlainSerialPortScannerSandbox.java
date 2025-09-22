package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.updater.PlainSerialPortScanner;

public class PlainSerialPortScannerSandbox {
    public static void main(String[] args) {

        PlainSerialPortScanner.findEcu((port, iniFileModel) -> {
            RusEfiSignature s = SignatureHelper.parse(iniFileModel.getSignature());
            System.out.println(port + " with OpenBlt, signature=" + s);

            String updateUrl = iniFileModel.getProtocolMeta().get("re_auto_update_root_url");
            if (updateUrl == null) {
                System.out.println("updateUrl not found");
                return;
            }
            System.out.println("bi " + s.asBundleInfo());


        }, 5000);
    }

}
