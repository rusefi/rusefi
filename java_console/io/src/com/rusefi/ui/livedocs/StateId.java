package com.rusefi.ui.livedocs;

import java.util.Objects;

/**
 * Type-safe unique structure identifier
 * <p>
 * (c) Andrey Belomutskiy 2013-2019
 */
public class StateId {
    private final int id;

    public StateId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    @Override
    public String toString() {
        return "StateId{" +
                "id=" + id +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        StateId stateId = (StateId) o;
        return id == stateId.id;
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }
}
