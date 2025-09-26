package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ts_plugin.headless.TsHeadlessPluginImpl;
import com.rusefi.updater.PlainSerialPortScanner;

public class PlainSerialPortScannerSandbox {

    public static void main(String[] args) {

        PlainSerialPortScanner.findEcu((port, iniFileModel) -> {
            RusEfiSignature s = SignatureHelper.parse(iniFileModel.getSignature());
            System.out.println(port + " with OpenBlt, signature=" + s);

            String updateUrl = TsHeadlessPluginImpl.getUpdateUrl(iniFileModel);
            if (updateUrl == null) {
                System.out.println("updateUrl not found");
                return;
            }
            System.out.println("bi " + s.asBundleInfo());


        }, 5000);
    }

}
