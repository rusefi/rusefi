package com.rusefi.maintenance.migration.migrators;

// Configuration for a single array field scale migration
// Similar to BooleanField for boolean migrations
public class ArrayFieldScaleMigration {
    private final String fieldName;
    private final double oldScale;
    private final double newScale;
    private final double scaleFactor;

    public ArrayFieldScaleMigration(String fieldName, double oldScale, double newScale) {
        this.fieldName = fieldName;
        this.oldScale = oldScale;
        this.newScale = newScale;
        this.scaleFactor = oldScale / newScale;
    }

    public ArrayFieldScaleMigration(String fieldName, double oldScale, double newScale, double scaleFactor) {
        this.fieldName = fieldName;
        this.oldScale = oldScale;
        this.newScale = newScale;
        this.scaleFactor = scaleFactor;
    }

    public boolean shouldMigrate(double prevScale, double updatedScale) {
        return Math.abs(prevScale - oldScale) < 0.0001 && Math.abs(updatedScale - newScale) < 0.0001;
    }

    public double migrateValue(double prevValue) {
        return prevValue * scaleFactor;
    }

    public String getFieldName() {
        return fieldName;
    }

    public double getOldScale() {
        return oldScale;
    }

    public double getNewScale() {
        return newScale;
    }

    public double getScaleFactor() {
        return scaleFactor;
    }
}
