package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.StringIniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.IniFieldMigrationUtils;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Page;

import java.util.*;

/** Builds candidate page images without changing the working tune or writing to an ECU. */
public final class TuneLoadPreparation {
    public final Map<Integer, ConfigurationImage> incoming;
    public final List<String> warnings;

    private TuneLoadPreparation(Map<Integer, ConfigurationImage> incoming, List<String> warnings) {
        this.incoming = Collections.unmodifiableMap(new TreeMap<>(incoming));
        this.warnings = Collections.unmodifiableList(new ArrayList<>(warnings));
    }

    public static TuneLoadPreparation prepare(Msq msq, IniFileModel sourceIni, IniFileModel targetIni,
                                               Map<Integer, ConfigurationImage> current,
                                               UpdateOperationCallbacks callbacks) {
        return prepare(msq, sourceIni, targetIni, current, callbacks, false);
    }

    private static TuneLoadPreparation prepare(Msq msq, IniFileModel sourceIni, IniFileModel targetIni,
                                                Map<Integer, ConfigurationImage> current,
                                                UpdateOperationCallbacks callbacks, boolean failOnInvalidValue) {
        Objects.requireNonNull(msq, "msq");
        Objects.requireNonNull(targetIni, "targetIni");
        Objects.requireNonNull(current, "current");
        Objects.requireNonNull(callbacks, "callbacks");

        List<String> warnings = new ArrayList<>();
        UpdateOperationCallbacks reporting = reportingCallbacks(callbacks, warnings);
        Map<Integer, ConfigurationImage> result = cloneImages(current);
        String sourceSignature = msq.getVersionInfo() == null ? null : msq.getVersionInfo().getSignature();
        if (sourceIni == null) {
            if (sourceSignature == null || !sourceSignature.equals(targetIni.getSignature())) {
                warning(reporting, "tune: a matching source INI is required for this signature.");
                return new TuneLoadPreparation(result, warnings);
            }
            applySameSignature(msq, targetIni, result, reporting, failOnInvalidValue);
        } else {
            if (sourceSignature == null || !sourceSignature.equals(sourceIni.getSignature())) {
                warning(reporting, "tune: the source INI does not match its signature.");
                return new TuneLoadPreparation(result, warnings);
            }
            Msq targetMsq = MsqFactory.valueOf(result, targetIni);
            TuneMigrationContext context = new TuneMigrationContext(
                sourceIni, msq, targetIni, targetMsq, reporting, Collections.emptySet());
            ComposedTuneMigrator.INSTANCE.migrateTune(context);
            for (Map.Entry<String, Constant> migrated : context.getMigratedConstants().entrySet()) {
                if (isPcVariable(targetIni, migrated.getKey())) {
                    continue;
                }
                Optional<IniField> fieldResult = targetIni.findIniField(migrated.getKey());
                if (!fieldResult.isPresent()) {
                    warning(reporting, migrated.getKey() + ": destination field is not available.");
                    continue;
                }
                IniField field = fieldResult.get();
                int pageIdentifier = field.getPageIndex();
                ConfigurationImage page = result.get(pageIdentifier);
                if (page == null) {
                    warning(reporting, migrated.getKey() + ": destination page is unavailable.");
                    continue;
                }
                try {
                    ConfigurationImage candidate = page.clone();
                    validateLuaValue(field, migrated.getValue().getValue());
                    applyTuneConstant(migrated.getValue(), field, candidate);
                    result.put(pageIdentifier, candidate);
                } catch (RuntimeException e) {
                    warning(reporting, migrated.getKey() + ": " + safeMessage(e));
                }
            }
        }
        return new TuneLoadPreparation(result, warnings);
    }

    public static Map<Integer, ConfigurationImage> openOffline(Msq msq, IniFileModel ini) {
        Objects.requireNonNull(ini, "ini");
        String signature = msq.getVersionInfo() == null ? null : msq.getVersionInfo().getSignature();
        if (signature == null || !signature.equals(ini.getSignature())) {
            throw new IllegalArgumentException("Tune signature does not match the supplied INI.");
        }
        Map<Integer, ConfigurationImage> pages = new TreeMap<>();
        pages.put(0, imageForPage(ini, 0));
        for (Page page : msq.page) {
            for (Constant constant : page.constant) {
                if (isPcVariable(ini, constant.getName())) {
                    continue;
                }
                Optional<IniField> field = ini.findIniField(constant.getName());
                if (field.isPresent() && field.get().getPageIndex() != 0) {
                    int pageIdentifier = field.get().getPageIndex();
                    pages.computeIfAbsent(pageIdentifier, id -> imageForPage(ini, id));
                }
            }
        }
        TuneLoadPreparation prepared = prepare(msq, null, ini, pages, UpdateOperationCallbacks.DUMMY, true);
        return prepared.incoming;
    }

    private static void applySameSignature(Msq msq, IniFileModel ini,
                                           Map<Integer, ConfigurationImage> images,
                                           UpdateOperationCallbacks callbacks, boolean failOnInvalidValue) {
        boolean foundPage = false;
        for (Page page : msq.page) {
            if (page.getSize() == null) {
                if (failOnInvalidValue) {
                    for (Constant constant : page.constant) {
                        if (!isPcVariable(ini, constant.getName()) && ini.findIniField(constant.getName()).isPresent()) {
                            throw new IllegalArgumentException("Tune field " + constant.getName()
                                + " is stored in a page without a declared size.");
                        }
                    }
                }
                continue;
            }
            foundPage = true;
            for (Constant constant : page.constant) {
                String name = constant.getName();
                if (name.startsWith("UNALLOCATED_SPACE") || isPcVariable(ini, name)) {
                    continue;
                }
                Optional<IniField> foundField = ini.findIniField(name);
                if (!foundField.isPresent()) {
                    warning(callbacks, name + ": not available.");
                    continue;
                }
                IniField field = foundField.get();
                int identifier = field.getPageIndex();
                ConfigurationImage image = images.get(identifier);
                if (image == null) {
                    if (failOnInvalidValue) {
                        throw new IllegalArgumentException("Tune field " + name
                            + " targets a page absent from the offline tune.");
                    }
                    warning(callbacks, name + ": destination page is unavailable.");
                    continue;
                }
                if (isLegacyBlankEnum(field, constant.getValue())) {
                    continue;
                }
                try {
                    ConfigurationImage candidate = image.clone();
                    validateLuaValue(field, constant.getValue());
                    applyTuneConstant(constant, field, candidate);
                    images.put(identifier, candidate);
                    image = candidate;
                } catch (RuntimeException e) {
                    if (failOnInvalidValue) {
                        throw new IllegalArgumentException("Invalid offline tune value for " + name
                            + ": " + safeMessage(e), e);
                    }
                    warning(callbacks, name + ": " + safeMessage(e));
                }
            }
        }
        if (!foundPage) {
            warning(callbacks, "Tune has no values to import.");
        }
    }

    static void applyTuneConstant(Constant constant, IniField field, ConfigurationImage image) {
        String sourceUnits = constant.getUnits();
        String destinationUnits = field.getUnits();
        boolean sourceUnitsMissing = sourceUnits == null || sourceUnits.isEmpty();
        boolean destinationUnitsMissing = destinationUnits == null || destinationUnits.isEmpty();
        if (sourceUnitsMissing != destinationUnitsMissing
                || !IniFieldMigrationUtils.checkIfUnitsCanBeMigrated(sourceUnits, destinationUnits)) {
            throw new IllegalArgumentException("units do not match");
        }
        if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            if (!Objects.equals(constant.getRows(), Integer.toString(array.getRows()))
                    || !Objects.equals(constant.getCols(), Integer.toString(array.getCols()))) {
                throw new IllegalArgumentException("table size does not match");
            }
        } else if (constant.getRows() != null || constant.getCols() != null) {
            throw new IllegalArgumentException("table size does not match");
        }
        if (field instanceof EnumIniField) {
            applyExactEnumValue((EnumIniField) field, constant, image);
        } else {
            validateNumericValues(field, constant);
            ConfigurationImageGetterSetter2.setValue(field, image, constant);
        }
    }

    private static void validateNumericValues(IniField field, Constant constant) {
        if (field instanceof ScalarIniField) {
            ScalarIniField scalar = (ScalarIniField) field;
            validateNumericValue(constant.getValue(), scalar.getType(), scalar.getMultiplier(),
                scalar.getSerializationOffset());
        } else if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            String[][] values;
            try {
                values = array.getValues(constant.getValue());
            } catch (RuntimeException e) {
                throw new IllegalArgumentException("table size does not match");
            }
            for (String[] row : values) {
                for (String value : row) {
                    validateNumericValue(value, array.getType(), array.getMultiplier(), 0);
                }
            }
        }
    }

    private static void validateNumericValue(String value, FieldType type, double multiplier,
                                            double serializationOffset) {
        double parsed;
        double raw;
        try {
            parsed = Double.parseDouble(value);
            raw = (parsed - serializationOffset) / multiplier;
        } catch (RuntimeException e) {
            throw new IllegalArgumentException("invalid value");
        }
        if (!Double.isFinite(parsed) || !Double.isFinite(raw)) {
            throw new IllegalArgumentException("value is out of range");
        }

        if (type == FieldType.FLOAT) {
            if (!Float.isFinite((float) raw)) {
                throw new IllegalArgumentException("value is out of range");
            }
            return;
        }

        long rounded = Math.round(raw);
        long min;
        long max;
        switch (type) {
            case INT8:
                min = Byte.MIN_VALUE;
                max = Byte.MAX_VALUE;
                break;
            case UINT8:
                min = 0;
                max = 0xff;
                break;
            case INT16:
                min = Short.MIN_VALUE;
                max = Short.MAX_VALUE;
                break;
            case UINT16:
                min = 0;
                max = 0xffff;
                break;
            case INT:
                min = Integer.MIN_VALUE;
                max = Integer.MAX_VALUE;
                break;
            default:
                throw new IllegalArgumentException("invalid value");
        }
        if (rounded < min || rounded > max) {
            throw new IllegalArgumentException("value is out of range");
        }
    }

    private static void applyExactEnumValue(EnumIniField field, Constant constant, ConfigurationImage image) {
        String value = constant.getValue();
        if (value != null && !EnumIniField.isQuoted(value) && EnumIniField.isUnknownOrdinalValue(value)) {
            throw new IllegalArgumentException("unknown pin");
        }
        String label = value != null && EnumIniField.isQuoted(value)
            ? javax.management.ObjectName.unquote(value)
            : value;
        if (label == null) {
            throw new IllegalArgumentException("unknown pin");
        }
        if (field.isPinEnum() && "INVALID".equalsIgnoreCase(label)) {
            throw new IllegalArgumentException("unknown pin");
        }
        long matches = field.getEnums().values().stream().filter(label::equals).count();
        int ordinal = field.getEnums().indexOf(value);
        if (matches != 1 || ordinal < 0) {
            throw new IllegalArgumentException(matches > 1
                ? "pin label is ambiguous"
                : "pin is not available");
        }
        image.setBitValue(field, ordinal);
    }

    private static boolean isPcVariable(IniFileModel ini, String name) {
        int subscript = name.indexOf('[');
        return ini.getPcVariableNames().contains(subscript < 0 ? name : name.substring(0, subscript));
    }

    private static Map<Integer, ConfigurationImage> cloneImages(Map<Integer, ConfigurationImage> images) {
        Map<Integer, ConfigurationImage> result = new TreeMap<>();
        images.forEach((page, image) -> result.put(page, image.clone()));
        return result;
    }

    private static ConfigurationImage imageForPage(IniFileModel ini, int identifier) {
        for (int pageIndex = 0; pageIndex < ini.getMetaInfo().getnPages(); pageIndex++) {
            if (ini.getMetaInfo().getPageIdentifier(pageIndex) == identifier) {
                return new ConfigurationImage(ini.getMetaInfo().getPageSize(pageIndex), ini.getByteOrder());
            }
        }
        throw new IllegalArgumentException(String.format("INI does not declare page 0x%04X", identifier));
    }

    private static void validateLuaValue(IniField field, String value) {
        if (!(field instanceof StringIniField) || !"luascript".equalsIgnoreCase(field.getName())) {
            return;
        }
        if (value == null) {
            throw new IllegalArgumentException("invalid value");
        }
        for (int i = 0; i < value.length(); i++) {
            char character = value.charAt(i);
            if (character == 0 || character > 0x7f) {
                throw new IllegalArgumentException("Lua script must be NUL-free ASCII");
            }
        }
    }

    private static boolean isLegacyBlankEnum(IniField field, String value) {
        return field instanceof EnumIniField
            && (value == null || value.isEmpty())
            && !((EnumIniField) field).getEnums().values().contains("");
    }

    private static String safeMessage(RuntimeException e) {
        return e.getMessage() == null ? "invalid value" : e.getMessage();
    }

    private static void warning(UpdateOperationCallbacks callbacks, String message) {
        callbacks.logLine("Skipped " + message);
    }

    private static UpdateOperationCallbacks reportingCallbacks(UpdateOperationCallbacks delegate,
                                                                List<String> warnings) {
        return new UpdateOperationCallbacks() {
            @Override
            public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
                if (message != null && (message.contains("WARNING") || message.startsWith("Skipped "))) {
                    warnings.add(message);
                }
                delegate.log(message, breakLineOnTextArea, sendToLogger);
            }

            @Override public void done() { delegate.done(); }
            @Override public void warning() { delegate.warning(); }
            @Override public void error() { delegate.error(); }
            @Override public void clear() { delegate.clear(); }
        };
    }
}
