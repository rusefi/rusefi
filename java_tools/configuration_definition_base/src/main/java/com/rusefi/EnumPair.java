package com.rusefi;

public class EnumPair {
    private final String keyValueForm;
    private final String arrayForm;

    public EnumPair(String keyValueForm, String arrayForm) {
        this.keyValueForm = keyValueForm;
        this.arrayForm = arrayForm;
    }

    public String getKeyValueForm() {
        return keyValueForm;
    }

    public String getArrayForm() {
        return arrayForm;
    }

    public boolean isEmpty() {
        if (arrayForm.isEmpty() ^ keyValueForm.isEmpty())
            throw new IllegalStateException("Expecting either both empty or both not empty [" + arrayForm + "][" + keyValueForm + "]");
        return arrayForm.isEmpty();
    }
}
