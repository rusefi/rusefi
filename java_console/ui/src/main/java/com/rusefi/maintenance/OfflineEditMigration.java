package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.MsqFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

/**
 * [tag:offline_tune]
 * Migrates the user's offline edits onto an ECU that has a different .ini signature.
 *
 * <p>"Edits" are the fields whose value differs between the loaded baseline and the current
 * (edited) image, both interpreted with the .ini the tune was loaded with. A field can be
 * migrated only if it also exists in the target ECU's .ini — otherwise it is reported as
 * incompatible so the user can decide whether to proceed.
 */
public final class OfflineEditMigration {
    private static final Logging log = getLogging(OfflineEditMigration.class);

    private OfflineEditMigration() {
    }

    public static final class Plan {
        /** Edited fields that exist in the target .ini and will be written. */
        public final List<Constant> migratable;
        /** Names of edited fields absent from the target .ini (cannot be migrated). */
        public final List<String> incompatible;

        Plan(List<Constant> migratable, List<String> incompatible) {
            this.migratable = migratable;
            this.incompatible = incompatible;
        }

        public boolean isEmpty() {
            return migratable.isEmpty() && incompatible.isEmpty();
        }
    }

    /**
     * Computes which edited fields can be carried over to a different-signature ECU.
     *
     * @param baseline  image as originally loaded (no edits)
     * @param edited    current image with the user's edits
     * @param sourceIni .ini the offline tune was loaded/edited with
     * @param targetIni .ini of the connected ECU
     */
    public static Plan computePlan(ConfigurationImage baseline, ConfigurationImage edited,
                                   IniFileModel sourceIni, IniFileModel targetIni) {
        Map<String, Constant> baseMap = MsqFactory.valueOf(baseline, sourceIni).getConstantsAsMap();
        Map<String, Constant> editMap = MsqFactory.valueOf(edited, sourceIni).getConstantsAsMap();

        List<Constant> migratable = new ArrayList<>();
        List<String> incompatible = new ArrayList<>();

        for (Map.Entry<String, Constant> e : editMap.entrySet()) {
            String name = e.getKey();
            Constant editedConst = e.getValue();
            Constant baseConst = baseMap.get(name);
            // Unchanged fields are not edits — skip them.
            if (baseConst != null && Objects.equals(baseConst.getValue(), editedConst.getValue())) {
                continue;
            }
            if (targetIni.findIniField(name).isPresent()) {
                migratable.add(editedConst);
            } else {
                incompatible.add(name);
            }
        }
        return new Plan(migratable, incompatible);
    }

    /**
     * Applies the migratable edits onto a clone of the target ECU image.
     * Fields that fail to set (type/scale mismatch) are skipped and logged.
     *
     * @return a new image with the edits applied
     */
    public static ConfigurationImage apply(ConfigurationImage targetImage, IniFileModel targetIni, List<Constant> migratable) {
        ConfigurationImage merged = targetImage.clone();
        for (Constant c : migratable) {
            Optional<IniField> field = targetIni.findIniField(c.getName());
            if (!field.isPresent()) {
                continue;
            }
            try {
                ConfigurationImageGetterSetter2.setValue(field.get(), merged, c);
            } catch (Throwable t) {
                log.warn("Skipping field `" + c.getName() + "` during offline-edit migration: " + t.getMessage());
            }
        }
        return merged;
    }
}
