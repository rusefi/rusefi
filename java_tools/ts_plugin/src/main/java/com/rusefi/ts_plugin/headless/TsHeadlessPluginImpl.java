package com.rusefi.ts_plugin.headless;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.updater.PlainSerialPortScanner;

public class TsHeadlessPluginImpl implements TsHeadlessPlugin {
    @Override
    public void run() {
        PlainSerialPortScanner.findEcu((port, iniFileModel) -> {
            RusEfiSignature s = SignatureHelper.parse(iniFileModel.getSignature());
            System.out.println(port + " with OpenBlt, signature=" + s);

        }, 5000);
    }
}
