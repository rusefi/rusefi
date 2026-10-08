package com.rusefi.mcp;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Map;
import java.util.TreeMap;

/** Selected fresh calibration reads; never touches the Console's editable tune cache. */
@SuppressWarnings("unchecked")
final class ConsoleTuneFields {
    static JSONObject read(LinkManager link, BinaryProtocol protocol, JSONArray names, Runnable check) throws Exception {
        return read(link, protocol, names, check, 10000);
    }

    static JSONObject read(LinkManager link, BinaryProtocol protocol, JSONArray names, Runnable check, long timeoutMs) throws Exception {
        return ConsoleReadTask.run(link, check, timeoutMs, guard -> {
            guard.run();
            IniFileModel ini = protocol.getIniFile();
            Map<String, IniField> fields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
            fields.putAll(ini.getSecondaryIniFields());
            fields.putAll(ini.getAllIniFields());
            JSONArray results = new JSONArray();
            long started = System.currentTimeMillis();
            int remainingValues = 512;
            boolean complete = true;
            for (Object name : names) {
                guard.run();
                IniField field = fields.get((String) name);
                JSONObject result = object("name", name, "success", false);
                results.add(result);
                String invalid = validate(field, ini.getMetaInfo(), remainingValues);
                if (invalid != null) {
                    result.put("error", invalid);
                    complete = false;
                    continue;
                }
                remainingValues -= count(field);
                result.put("name", field.getName());
                result.put("page", field.getPageIndex());
                result.put("offset", field.getOffset());
                try {
                    byte[] bytes = new byte[field.getSize()];
                    // Check cancellation between wire chunks without interrupting an in-flight transaction.
                    int block = Math.min(256, ini.getBlockingFactor());
                    if (block < 1) { throw new IOException("Invalid INI blocking factor."); }
                    long readStarted = System.currentTimeMillis();
                    for (int offset = 0; offset < bytes.length; offset += block) {
                        guard.run();
                        int size = Math.min(block, bytes.length - offset);
                        byte[] part = protocol.readFromPage(field.getPageIndex(), field.getOffset() + offset, size);
                        guard.run();
                        if (part == null || part.length != size) { throw new IOException("ECU field read failed."); }
                        System.arraycopy(part, 0, bytes, offset, size);
                    }
                    decode(result, field, ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN));
                    result.put("readStartedTimestampMs", readStarted);
                    result.put("readCompletedTimestampMs", System.currentTimeMillis());
                    result.put("success", true);
                } catch (IOException | IllegalArgumentException failure) {
                    result.put("error", failure.getMessage());
                    complete = false;
                }
            }
            guard.run();
            return object("success", true, "complete", complete, "source", "ecu", "fields", results,
                    "readStartedTimestampMs", started, "readCompletedTimestampMs", System.currentTimeMillis(),
                    "note", "Fresh sequential ECU RAM reads, not an atomic tune snapshot or proof of flash persistence. Values use the Console's parsed INI scaling. String/Lua fields and large arrays are excluded.");
        });
    }

    private static int count(IniField field) {
        return field instanceof ArrayIniField ? ((ArrayIniField) field).getRows() * ((ArrayIniField) field).getCols() : 1;
    }

    private static String validate(IniField field, IniFileMetaInfo meta, int remaining) {
        if (field == null) { return "Calibration field not found in the connected INI. Use an exact calibration name from local knowledge."; }
        FieldType type;
        if (field instanceof ScalarIniField) {
            type = ((ScalarIniField) field).getType();
        } else if (field instanceof EnumIniField) {
            EnumIniField bits = (EnumIniField) field;
            type = bits.getType();
            if (!type.isNumeric() || type == FieldType.FLOAT || bits.getBitPosition() < 0 || bits.getBitSize0() < 0
                    || (long) bits.getBitPosition() + bits.getBitSize0() + 1 > type.getStorageSize() * 8) {
                return "Invalid INI bit range.";
            }
        } else if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            if (array.getRows() < 1 || array.getCols() < 1 || (long) array.getRows() * array.getCols() > 64) {
                return "Array reads are limited to 64 elements per field.";
            }
            type = array.getType();
        } else {
            return "Only scalar, enum/bitfield and small numeric array calibration fields are supported; string/Lua fields are excluded.";
        }
        if (!type.isNumeric()) { return "Unsupported numeric storage type."; }
        if (count(field) > remaining) { return "Request exceeds the 512-value tune read budget."; }
        long end = (long) field.getOffset() + field.getSize();
        if (field.getOffset() < 0 || field.getPageIndex() < 0 || field.getPageIndex() > 65535 || end > 65536) {
            return "Field is outside the protocol address range.";
        }
        for (int page = 0; page < meta.getnPages(); page++) {
            if (meta.getPageIdentifier(page) == field.getPageIndex()) {
                return end <= meta.getPageSize(page) ? null : "Field is outside its INI page.";
            }
        }
        return "Field page is not declared by the INI.";
    }

    private static void decode(JSONObject result, IniField field, ByteBuffer bytes) throws IOException {
        if (field instanceof ScalarIniField) {
            ScalarIniField scalar = (ScalarIniField) field;
            double value = finite(scalar.getType().readRawValue(bytes) * scalar.getMultiplier() + scalar.getSerializationOffset());
            result.put("kind", "scalar");
            result.put("value", value);
        } else if (field instanceof EnumIniField) {
            EnumIniField bits = (EnumIniField) field;
            long raw = (long) bits.getType().readRawValue(bytes);
            long value = (raw >>> bits.getBitPosition()) & ((1L << (bits.getBitSize0() + 1)) - 1);
            String label = bits.getEnums().get((int) value);
            result.put("kind", "enum");
            result.put("value", value);
            result.put("label", label == null ? null : label.substring(0, Math.min(256, label.length())));
            result.put("labelTruncated", label != null && label.length() > 256);
        } else {
            ArrayIniField array = (ArrayIniField) field;
            JSONArray rows = new JSONArray();
            for (int row = 0; row < array.getRows(); row++) {
                JSONArray columns = new JSONArray();
                for (int col = 0; col < array.getCols(); col++) {
                    columns.add(finite(array.getType().readRawValue(bytes) * array.getMultiplier()));
                }
                rows.add(columns);
            }
            result.put("kind", "array");
            result.put("rows", array.getRows());
            result.put("columns", array.getCols());
            result.put("values", rows);
        }
        String units = field.getUnits();
        String status = units == null || units.length() > 128 ? "unknown"
                : units.contains("{") || units.contains("}") || units.contains("?") || TsStringFunction.containsStringFunction(units) ? "dynamic" : "known";
        result.put("units", "known".equals(status) ? units : null);
        result.put("unitsStatus", status);
    }

    private static double finite(double value) throws IOException {
        if (!Double.isFinite(value)) { throw new IOException("Non-finite calibration value or INI scaling."); }
        return value;
    }

    private static JSONObject object(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) { result.put(pairs[i], pairs[i + 1]); }
        return result;
    }
}
