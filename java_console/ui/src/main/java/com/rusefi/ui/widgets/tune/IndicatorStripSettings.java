package com.rusefi.ui.widgets.tune;

import com.rusefi.core.preferences.storage.Node;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.Function;

/**
 * Which indicators the indicator strip shows, and in what order. One instance is shared by every
 * {@link IndicatorStrip}, so hiding or moving a chip on the Gauges tab changes the Tuning tab too.
 * <p>
 * Indicators are identified by {@link #keyOf their expression}, which survives firmware updates
 * that add or relabel indicators. Indicators missing from the stored order (new firmware ones)
 * follow the stored ones in INI order.
 * <p>
 * It also holds the user's {@link CustomIndicator custom indicators}. Saving a gauge layout writes all
 * of this into the file ({@link #toFile}), and loading one that has it replaces it ({@link #replaceWith}).
 */
public class IndicatorStripSettings {
    public static final String NODE = "indicator_strip";
    static final String ORDER_KEY = "order";
    static final String HIDDEN_KEY = "hidden";
    static final String CUSTOM_COUNT = "custom_count";
    static final String CUSTOM = "custom_";
    public static final int MAX_CUSTOM = 64;
    // INI expressions are single lines, but may contain '|'
    private static final String SEPARATOR = "\n";

    private final List<Runnable> listeners = new CopyOnWriteArrayList<>();
    private Node config;

    public IndicatorStripSettings(Node config) {
        this.config = config;
    }

    /** Switches to the console's saved preferences; called once at startup. */
    public void bind(Node config) {
        this.config = config;
        fireChanged();
    }

    public void addListener(Runnable listener) {
        listeners.add(listener);
    }

    public void removeListener(Runnable listener) {
        listeners.remove(listener);
    }

    /**
     * The key an indicator is stored under: its expression without braces and extra spaces.
     * {@code taken} holds the keys handed out so far for one INI, so a repeated expression gets {@code #2}.
     */
    static String keyOf(String expression, Set<String> taken) {
        String key = expression == null ? "" : expression.replaceAll("[{}]", " ").replaceAll("\\s+", " ").trim();
        String unique = key;
        for (int n = 2; !taken.add(unique); n++) {
            unique = key + "#" + n;
        }
        return unique;
    }

    public List<String> getOrder() {
        return split(config.getProperty(ORDER_KEY, ""));
    }

    public void setOrder(List<String> order) {
        config.setProperty(ORDER_KEY, String.join(SEPARATOR, order));
        fireChanged();
    }

    public boolean hasCustomOrder() {
        return !getOrder().isEmpty();
    }

    public void resetOrder() {
        config.getConfig().remove(ORDER_KEY);
        fireChanged();
    }

    public boolean isHidden(String key) {
        return getHidden().contains(key);
    }

    public Set<String> getHidden() {
        return new LinkedHashSet<>(split(config.getProperty(HIDDEN_KEY, "")));
    }

    public void setHidden(String key, boolean hidden) {
        Set<String> keys = getHidden();
        if (hidden ? keys.add(key) : keys.remove(key)) {
            saveHidden(keys);
        }
    }

    public void showAll() {
        saveHidden(Collections.emptySet());
    }

    private void saveHidden(Set<String> keys) {
        if (keys.isEmpty()) {
            config.getConfig().remove(HIDDEN_KEY);
        } else {
            config.setProperty(HIDDEN_KEY, String.join(SEPARATOR, keys));
        }
        fireChanged();
    }

    public List<CustomIndicator> getCustomIndicators() {
        return readCustom(config);
    }

    private static List<CustomIndicator> readCustom(Node node) {
        List<CustomIndicator> result = new ArrayList<>();
        int count = node.getIntProperty(CUSTOM_COUNT, 0);
        for (int i = 0; i < count; i++) {
            CustomIndicator indicator = CustomIndicator.read(node.getChild(CUSTOM + i));
            if (indicator != null) {
                result.add(indicator);
            }
        }
        return result;
    }

    /** Adds {@code indicator}, or replaces the one with its id; a new one goes at the end of the strip. */
    public void putCustomIndicator(CustomIndicator indicator) {
        List<CustomIndicator> list = getCustomIndicators();
        boolean replaced = false;
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId().equals(indicator.getId())) {
                list.set(i, indicator);
                replaced = true;
            }
        }
        if (!replaced) {
            if (list.size() >= MAX_CUSTOM) {
                throw new IllegalStateException("At most " + MAX_CUSTOM + " custom indicators");
            }
            list.add(indicator);
        }
        writeCustom(list);
        fireChanged();
    }

    /** Removes the custom indicator with {@code id}, together with its place and hidden state. */
    public void removeCustomIndicator(String id) {
        List<CustomIndicator> list = getCustomIndicators();
        String key = CustomIndicator.KEY_PREFIX + id;
        if (!list.removeIf(indicator -> indicator.getId().equals(id))) {
            return;
        }
        writeCustom(list);
        List<String> order = getOrder();
        if (order.remove(key)) {
            config.setProperty(ORDER_KEY, String.join(SEPARATOR, order));
        }
        Set<String> hidden = getHidden();
        if (hidden.remove(key)) {
            saveHidden(hidden);
            return;
        }
        fireChanged();
    }

    private void writeCustom(List<CustomIndicator> list) {
        config.getConfig().keySet().removeIf(key -> key.matches(CUSTOM + "\\d+"));
        config.setProperty(CUSTOM_COUNT, list.size());
        for (int i = 0; i < list.size(); i++) {
            list.get(i).write(config.getChild(CUSTOM + i));
        }
    }

    /** Writes the whole strip (order, hidden chips, custom indicators) as {@code prefix}-keyed file entries. */
    public void toFile(Properties file, String prefix) {
        put(file, prefix + ORDER_KEY, config.getProperty(ORDER_KEY, null));
        put(file, prefix + HIDDEN_KEY, config.getProperty(HIDDEN_KEY, null));
        List<CustomIndicator> custom = getCustomIndicators();
        file.setProperty(prefix + CUSTOM_COUNT, Integer.toString(custom.size()));
        for (int i = 0; i < custom.size(); i++) {
            Node node = new Node();
            custom.get(i).write(node);
            for (String key : CustomIndicator.PROPERTIES) {
                String value = node.getProperty(key);
                // an empty name is the same as none, which older files have
                if (!(key.equals(CustomIndicator.NAME) && value.isEmpty())) {
                    file.setProperty(prefix + CUSTOM + i + "." + key, value);
                }
            }
        }
    }

    private static void put(Properties file, String key, @Nullable String value) {
        if (value != null && !value.isEmpty()) {
            file.setProperty(key, value);
        }
    }

    /**
     * The strip stored in a file by {@link #toFile}, or null when the file has none (older consoles
     * did not write one, and loading such a file leaves the strip alone).
     */
    @Nullable
    public static Node fromFile(Properties file, String prefix) throws IOException {
        String countValue = file.getProperty(prefix + CUSTOM_COUNT);
        if (countValue == null) {
            return null;
        }
        int count;
        try {
            count = Integer.parseInt(countValue);
        } catch (NumberFormatException e) {
            count = -1;
        }
        if (count < 0 || count > MAX_CUSTOM) {
            throw new IOException("Invalid " + prefix + CUSTOM_COUNT + ": expected 0 to " + MAX_CUSTOM);
        }
        Node strip = new Node();
        for (String key : new String[]{ORDER_KEY, HIDDEN_KEY}) {
            String value = file.getProperty(prefix + key);
            if (value != null) {
                strip.setProperty(key, value);
            }
        }
        strip.setProperty(CUSTOM_COUNT, count);
        Set<String> ids = new HashSet<>();
        for (int i = 0; i < count; i++) {
            Node node = strip.getChild(CUSTOM + i);
            for (String key : CustomIndicator.PROPERTIES) {
                String value = file.getProperty(prefix + CUSTOM + i + "." + key);
                if (value != null) {
                    node.setProperty(key, value);
                }
            }
            CustomIndicator indicator = CustomIndicator.read(node);
            if (indicator == null) {
                throw new IOException("Invalid custom indicator " + prefix + CUSTOM + i);
            }
            if (!ids.add(indicator.getId())) {
                throw new IOException("Repeated custom indicator id " + indicator.getId());
            }
        }
        return strip;
    }

    /** Replaces the whole strip with one read by {@link #fromFile}. */
    public void replaceWith(Node strip) {
        config.getConfig().clear();
        for (String key : new String[]{ORDER_KEY, HIDDEN_KEY}) {
            String value = strip.getProperty(key, null);
            if (value != null) {
                config.setProperty(key, value);
            }
        }
        writeCustom(readCustom(strip));
        fireChanged();
    }

    /** {@code items} in the stored order; those not in it follow in their own order. */
    <T> List<T> arrange(List<T> items, Function<T, String> key) {
        Map<String, T> byKey = new LinkedHashMap<>();
        for (T item : items) {
            byKey.put(key.apply(item), item);
        }
        List<T> arranged = new ArrayList<>();
        for (String stored : getOrder()) {
            T item = byKey.remove(stored);
            if (item != null) {
                arranged.add(item);
            }
        }
        arranged.addAll(byKey.values());
        return arranged;
    }

    private void fireChanged() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    private static List<String> split(String value) {
        if (value.isEmpty()) {
            return new ArrayList<>();
        }
        return new ArrayList<>(Arrays.asList(value.split(SEPARATOR)));
    }
}
