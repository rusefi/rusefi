package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;
import org.jetbrains.annotations.Nullable;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

/**
 * The Tuning menu pages that can open as a floating settings window, in menu order, for the global
 * settings search. Built from the INI each time it is needed, so another firmware brings its own pages.
 */
public final class SettingsPageIndex {
    public static final String PATH_SEPARATOR = " › ";

    private final List<Page> pages;

    private SettingsPageIndex(List<Page> pages) {
        this.pages = Collections.unmodifiableList(pages);
    }

    /**
     * @param image the tune that decides which pages are disabled, as in the Tuning tree; without one
     *              every page is offered
     */
    public static SettingsPageIndex build(@Nullable IniFileModel ini, @Nullable ConfigurationImage image) {
        List<Page> pages = new ArrayList<>();
        if (ini != null) {
            Set<String> seen = new HashSet<>();
            for (MenuModel menu : ini.getMenus()) {
                List<String> path = Collections.singletonList(menu.getName());
                for (MenuItem item : menu.getItems()) {
                    add(item, path, ini, image, seen, pages);
                }
            }
        }
        return new SettingsPageIndex(pages);
    }

    private static void add(MenuItem item, List<String> path, IniFileModel ini, ConfigurationImage image,
                            Set<String> seen, List<Page> pages) {
        if (item instanceof GroupMenuModel) {
            GroupMenuModel group = (GroupMenuModel) item;
            List<String> groupPath = new ArrayList<>(path);
            groupPath.add(group.getName());
            for (MenuItem child : group.getItems()) {
                add(child, groupPath, ini, image, seen, pages);
            }
        } else if (item instanceof SubMenuModel) {
            SubMenuModel subMenu = (SubMenuModel) item;
            // std_ entries and other pages without a panel cannot open as a window
            if (!new CalibrationPanelKey(subMenu.getKey()).isSupported(ini)
                    || (image != null && MainMenuTreeWidget.isDisabledByExpressions(subMenu, ini, image))
                    || !seen.add(subMenu.getKey())) {
                return;
            }
            pages.add(new Page(subMenu.getKey(), subMenu.getName(), path));
        }
    }

    public List<Page> getPages() {
        return pages;
    }

    /**
     * Pages whose menu path contains every word of the query, ignoring case, as the Tuning tree's search:
     * an exact name first, then names starting with the query, then names containing every word, then
     * the rest, each in menu order. An empty query gives every page.
     */
    public List<Page> search(String query) {
        String text = query == null ? "" : query.trim().toLowerCase(Locale.ROOT);
        if (text.isEmpty()) {
            return pages;
        }
        String[] words = text.split("\\s+");
        List<List<Page>> ranks = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            ranks.add(new ArrayList<>());
        }
        for (Page page : pages) {
            if (!containsAll(page.searchText, words)) {
                continue;
            }
            String name = page.name.toLowerCase(Locale.ROOT);
            int rank = name.equals(text) ? 0 : name.startsWith(text) ? 1 : containsAll(name, words) ? 2 : 3;
            ranks.get(rank).add(page);
        }
        List<Page> result = new ArrayList<>();
        for (List<Page> rank : ranks) {
            result.addAll(rank);
        }
        return result;
    }

    private static boolean containsAll(String text, String[] words) {
        for (String word : words) {
            if (!text.contains(word)) {
                return false;
            }
        }
        return true;
    }

    public static final class Page {
        private final String key;
        private final String name;
        private final List<String> menuPath;
        private final String searchText;

        Page(String key, String name, List<String> menuPath) {
            this.key = key;
            this.name = name;
            this.menuPath = Collections.unmodifiableList(new ArrayList<>(menuPath));
            this.searchText = (String.join(" ", menuPath) + " " + name).toLowerCase(Locale.ROOT);
        }

        /** The INI key of the dialog, table or curve. */
        public String getKey() {
            return key;
        }

        public String getName() {
            return name;
        }

        /** The menu and any groups above the page, outermost first. */
        public List<String> getMenuPath() {
            return menuPath;
        }

        /** "Idle › Idle PID". */
        public String getLabel() {
            return String.join(PATH_SEPARATOR, menuPath) + PATH_SEPARATOR + name;
        }

        @Override
        public String toString() {
            return getLabel();
        }
    }
}
