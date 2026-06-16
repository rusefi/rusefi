package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.core.Pair;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.net.UniversalApiClient;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import org.jetbrains.annotations.NotNull;

import java.io.FileNotFoundException;

import static com.devexperts.logging.Logging.getLogging;

public class RealIniFileProvider implements IniFileProvider {
    private static final Logging log = getLogging(RealIniFileProvider.class);

    @Override
    @NotNull
    public IniFileModel provide(String signature) throws IniNotFoundException {
        String localIniFile = null;

        RusEfiSignature sig = SignatureHelper.parse(signature);

        // 1. Try API — resolves .ini (with redirect) and returns X-Bundle-Url for background download.
        //    Falls back gracefully when the API is unreachable (5 s timeout).
        if (sig != null) {
            Pair<String, String> resolved = UniversalApiClient.resolveIni(
                sig.getBundleTarget(), sig.getHash());
            if (resolved != null) {
                localIniFile = resolved.first;
                if (resolved.second != null) {
                    BackgroundBundleDownloader.INSTANCE.ensureBundle(
                        sig.getBundleTarget(), sig.getHash(), resolved.second);
                }
            }
        }

        // 2. API miss — fall back to direct rusefi.com download (existing cache + download logic)
        if (localIniFile == null) {
            log.warn("Failed to use rusEFI API, using hardcoded ini path as fallback!");
            localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        }

        // 3. Local fallbacks
        if (localIniFile == null) {
            log.info("Failed to download " + signature + " maybe custom board?");
            localIniFile = IniLocator.findIniFile(".");
        }
        if (localIniFile == null) {
            localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH);
        }
        if (localIniFile == null) {
            throw new IniNotFoundException("Failed to locate .ini file in five different places!");
        }

        IniFileModel iniFileModel;
        try {
            iniFileModel = IniFileReaderUtil.readIniFileChecked(localIniFile);
        } catch (IniParsingException e) {
            throw new IniNotFoundException("Parsing error: " + e, e);
        } catch (FileNotFoundException e) {
            throw new IniNotFoundException(e.toString());
        }
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
        return iniFileModel;
    }
}
