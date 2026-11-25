package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Pair;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

final class TunePageIo {
    private TunePageIo() {
    }

    static Map<Integer, ConfigurationImage> readSecondary(BinaryProtocol bp, IniFileModel ini, List<String> warnings) {
        Map<Integer, ConfigurationImage> pages = new HashMap<>();
        IniFileMetaInfo meta = ini.getMetaInfo();
        for (int pageIndex = 1; pageIndex < meta.getnPages(); pageIndex++) {
            int pageId = meta.getPageIdentifier(pageIndex);
            int pageSize = meta.getPageSize(pageIndex);
            if (pageId <= 0 || pageSize <= 0) {
                warnings.add(String.format("Skipping invalid secondary page 0x%04X (size %d)", pageId, pageSize));
                continue;
            }
            byte[] content = bp.readFromPage(pageId, 0, pageSize);
            if (content == null || content.length != pageSize) {
                warnings.add(String.format("Failed to read secondary page 0x%04X", pageId));
                continue;
            }
            pages.put(pageId, new ConfigurationImage(content, bp.getWireByteOrder()));
        }
        return pages;
    }

    static void uploadSecondary(BinaryProtocol bp, Map<Integer, ConfigurationImage> base,
                                Map<Integer, ConfigurationImage> selected) {
        if (selected.isEmpty()) {
            return;
        }
        if (base == null) {
            throw new IllegalStateException("Base images for secondary pages are required");
        }
        IniFileMetaInfo meta = bp.getIniFile().getMetaInfo();
        for (Map.Entry<Integer, ConfigurationImage> entry : selected.entrySet()) {
            int pageId = entry.getKey();
            if (pageId == 0) {
                throw new IllegalArgumentException("Page 0 is not a secondary page");
            }
            int pageIndex = findPageIndex(meta, pageId);
            if (pageIndex < 1) {
                throw new IllegalArgumentException(String.format("Undeclared secondary page 0x%04X", pageId));
            }
            ConfigurationImage original = base.get(pageId);
            if (original == null) {
                throw new IllegalStateException(String.format("No base image for secondary page 0x%04X", pageId));
            }
            ConfigurationImage target = entry.getValue();
            if (original.getSize() != target.getSize() || target.getSize() != meta.getPageSize(pageIndex)) {
                throw new IllegalArgumentException(String.format("Secondary page 0x%04X size mismatch", pageId));
            }
            if (meta.getBurnCommand(pageIndex).isEmpty()) {
                if (hasDifferences(original, target)) {
                    throw new IllegalStateException(String.format("Secondary page 0x%04X is not burnable", pageId));
                }
                continue;
            }

            int offset = 0;
            while (offset < original.getSize()) {
                Pair<Integer, Integer> range = com.rusefi.ConfigurationImageDiff.findDifferences(original, target, offset);
                if (range == null) {
                    break;
                }
                bp.writeInBlocks(target.getContent(), range.first, range.first, range.second - range.first, pageId);
                offset = range.second;
            }

            if (offset != 0) {
                byte[] readback = bp.readFromPage(pageId, 0, target.getSize());
                if (readback == null || !Arrays.equals(readback, target.getContent())) {
                    throw new IllegalStateException(String.format("Readback verification failed for page 0x%04X", pageId));
                }
            }
        }
    }

    static void burnSecondary(BinaryProtocol bp, Set<Integer> changedPages) {
        if (changedPages.isEmpty()) {
            return;
        }
        IniFileMetaInfo meta = bp.getIniFile().getMetaInfo();
        for (int pageId : changedPages) {
            if (pageId == 0) {
                throw new IllegalArgumentException("Page 0 is not a secondary page");
            }
            int pageIndex = findPageIndex(meta, pageId);
            if (pageIndex < 1) {
                throw new IllegalArgumentException(String.format("Undeclared secondary page 0x%04X", pageId));
            }
            if (meta.getBurnCommand(pageIndex).isEmpty()) {
                throw new IllegalStateException(String.format("Secondary page 0x%04X is not burnable", pageId));
            }
            if (!bp.burnPage(pageId)) {
                throw new IllegalStateException(String.format("Burn failed for secondary page 0x%04X", pageId));
            }
        }
    }

    static boolean isBurnable(IniFileModel ini, int pageId) {
        IniFileMetaInfo meta = ini.getMetaInfo();
        int index = findPageIndex(meta, pageId);
        return index >= 1 && !meta.getBurnCommand(index).isEmpty();
    }

    private static int findPageIndex(IniFileMetaInfo meta, int pageId) {
        for (int pageIndex = 1; pageIndex < meta.getnPages(); pageIndex++) {
            if (meta.getPageIdentifier(pageIndex) == pageId) {
                return pageIndex;
            }
        }
        return -1;
    }

    private static boolean hasDifferences(ConfigurationImage original, ConfigurationImage target) {
        return com.rusefi.ConfigurationImageDiff.findDifferences(original, target, 0) != null;
    }
}
