package com.rusefi.mcp.tools;

import com.opensr5.ini.MandatoryLineMissing;
import com.opensr5.ini.RawIniFile;
import com.rusefi.core.SignatureHelper;
import org.json.simple.JSONObject;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Host-side .ini utilities for the MCP server. Kept separate from
 * {@code EcuMcpServer} so the stdio dispatch layer stays thin and the .ini-cache
 * logic can be exercised directly from tests.
 *
 * <p>The real cache folder is hardcoded inside {@link SignatureHelper} to
 * {@code ~/.rusEFI/ini_database/}; this class is a façade that converts user-facing
 * arguments (JSON) into that call plus structured success/error bodies.
 */
public final class IniTools {
    private IniTools() {}

    /**
     * Implements the {@code import_ini_to_cache} MCP tool. Returns a JSON body with either
     * {@code success: true} and the cached file info, or {@code success: false} and a human
     * readable {@code error}. See {@link SignatureHelper#importIntoCache(String, File)} for
     * the underlying copy and {@link com.rusefi.maintenance.ManualIniFilePicker} for the
     * Swing-side counterpart that uses the same cache layout.
     */
    @SuppressWarnings("unchecked")
    public static JSONObject importIniToCache(JSONObject args) throws IOException {
        Object inputPath = args.get("path");
        if (!(inputPath instanceof String) || ((String) inputPath).trim().isEmpty()) {
            return errorBody("'path' must be a non-empty string");
        }
        File source = new File(((String) inputPath).trim());
        if (!source.isFile()) {
            return errorBody("INI path is not an existing regular file: " + source);
        }

        Object signatureArg = args.get("signature");
        if (signatureArg != null && !(signatureArg instanceof String)) {
            return errorBody("'signature' must be a string if provided");
        }
        String signature;
        if (signatureArg instanceof String && !((String) signatureArg).trim().isEmpty()) {
            signature = ((String) signatureArg).trim();
        } else {
            try {
                signature = readSignature(source);
            } catch (MandatoryLineMissing e) {
                return errorBody("INI file has no 'signature' line: " + source);
            }
        }

        String cached = SignatureHelper.importIntoCache(signature, source);
        if (cached == null) {
            return errorBody("Unparseable firmware signature: '" + signature + "'");
        }

        JSONObject o = new JSONObject();
        o.put("success", true);
        o.put("path", cached);
        o.put("signature", signature);
        o.put("fileSize", source.length());
        return o;
    }

    /** Read the first {@code signature} line from a .ini without the full field/page validation. */
    public static String readSignature(File source) throws FileNotFoundException, MandatoryLineMissing {
        return RawIniFile.read(source.getAbsolutePath()).getValues("signature").get(0);
    }

    @SuppressWarnings("unchecked")
    private static JSONObject errorBody(String message) {
        JSONObject o = new JSONObject();
        o.put("success", false);
        o.put("error", message);
        return o;
    }
}
