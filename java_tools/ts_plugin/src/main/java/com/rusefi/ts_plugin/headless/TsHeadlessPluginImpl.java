package com.rusefi.ts_plugin.headless;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.opensr5.ini.IniFileModel;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
import com.rusefi.updater.PlainSerialPortScanner;
import com.rusefi.wizard.BackgroundWizard;

/**
 * this class is invoked by TsHeadlessPlugin via reflection
 */
@SuppressWarnings("unused")
public class TsHeadlessPluginImpl implements TsHeadlessPlugin {
    public static final String RE_AUTO_UPDATE_ROOT_URL = "re_auto_update_root_url";

    public static String getUpdateUrl(IniFileModel iniFileModel) {
        return iniFileModel.getProtocolMeta().get(RE_AUTO_UPDATE_ROOT_URL);
    }

    @Override
    public void run() {
        BackgroundWizard.start(ControllerAccess::getInstance);
        PlainSerialPortScanner.findEcu(TsHeadlessPluginImpl::onEcuDiscoveredAction, 5000);
    }

    private static void onEcuDiscoveredAction(String port, IniFileModel iniFileModel) {
        RusEfiSignature s = SignatureHelper.parse(iniFileModel.getSignature());
        System.out.println(port + " with OpenBlt, signature=" + s);

        BackgroundWizard.onEcuDiscovery(s.getBundleTarget());

        String updateUrl = TsHeadlessPluginImpl.getUpdateUrl(iniFileModel);
        String isObfuscated = iniFileModel.getProtocolMeta().get("RE_obfuscated");
        if (updateUrl == null) {
            log.warn(String.format("Not sure where to download from [%s]/[%s]", updateUrl, isObfuscated));
            return;
        }
        BundleInfo bi = s.asBundleInfo();
        String downloadFrom = BundleInfoStrategy.getDownloadUrl(bi, updateUrl, BundleInfo::getBranchName);
        Autoupdate.downloadAutoupdateZipFile(bi, downloadFrom, Boolean.parseBoolean(isObfuscated));
    }

    @Override
    public boolean getDisplayPlugin(String serialSignature) {
        return BackgroundWizard.displayPlugin(serialSignature);
    }
}
