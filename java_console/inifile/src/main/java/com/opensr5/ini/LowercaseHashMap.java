package com.opensr5.ini;

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

/**
 * A HashMap that normalizes all keys to lower-case (Locale.US) on every put/get/containsKey/remove.
 * Drop-in replacement for {@code new TreeMap<>(String.CASE_INSENSITIVE_ORDER)}: same case-insensitive
 * semantics with O(1) hash lookup instead of O(log N) tree traversal + string comparison.
 */
public class LowercaseHashMap<V> extends HashMap<String, V> {

    public LowercaseHashMap() {
        super();
    }

    public LowercaseHashMap(int initialCapacity) {
        super(initialCapacity);
    }

    @Override
    public V put(String key, V value) {
        return super.put(key.toLowerCase(Locale.US), value);
    }

    /**
     * Must override putAll: HashMap.putAll calls putVal() internally, bypassing our put() override,
     * so keys would be stored in their original case and lookups via get() would fail.
     */
    @Override
    public void putAll(Map<? extends String, ? extends V> m) {
        for (Map.Entry<? extends String, ? extends V> e : m.entrySet()) {
            put(e.getKey(), e.getValue());
        }
    }

    @Override
    public V get(Object key) {
        return super.get(lower(key));
    }

    @Override
    public boolean containsKey(Object key) {
        return super.containsKey(lower(key));
    }

    @Override
    public V remove(Object key) {
        return super.remove(lower(key));
    }

    @Override
    public V getOrDefault(Object key, V defaultValue) {
        return super.getOrDefault(lower(key), defaultValue);
    }

    private static Object lower(Object key) {
        return key instanceof String ? ((String) key).toLowerCase(Locale.US) : key;
    }
}
