package com.rusefi.binaryprotocol;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.OutputChannelDemand;
import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class OutputChannelRangeSelectionTest {
    @Test
    void selectsAndMergesNearbyDirectChannelRanges() {
        IniFileModel ini = ini(100, fields(field("rpm", 4, 2), field("clt", 22, 2)));

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini, OutputChannelDemand.selective(set("rpm", "clt"), 3));

        assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(4, 20)), selection.getRanges());
        assertEquals(set("rpm", "clt"), selection.getRequestedChannelRoots());
        assertFalse(selection.isFull());
    }

    @Test
    void selectsTheOutputChannelUsedByAGauge() {
        GaugeModel gauge = mock(GaugeModel.class);
        when(gauge.getChannel()).thenReturn("rpm");
        IniFileModel ini = ini(100, fields(field("rpm", 4, 2)));
        when(ini.getGauges()).thenReturn(Collections.singletonMap("engineSpeed", gauge));

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini, OutputChannelDemand.selective(set("engineSpeed"), 1));

        assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(4, 2)), selection.getRanges());
        assertEquals(set("rpm"), selection.getRequestedChannelRoots());
    }

    @Test
    void selectsOutputDependenciesWithoutReadingCalibrationVariables() {
        IniFileModel ini = ini(100, fields(field("rpm", 4, 2)));
        Map<String, String> expressions = new LinkedHashMap<>();
        expressions.put("display", "{derived + calibration}");
        expressions.put("derived", "{rpm * 2}");
        when(ini.getExpressionOutputChannels()).thenReturn(expressions);
        when(ini.findIniField("calibration")).thenReturn(Optional.of(mock(IniField.class)));

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini, OutputChannelDemand.selective(set("display"), 1));

        assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(4, 2)), selection.getRanges());
        assertEquals(set("rpm"), selection.getRequestedChannelRoots());
    }

    @Test
    void selectsOutputDependenciesFromALiteralExpression() {
        IniFileModel ini = ini(100, fields(field("rpm", 4, 2)));

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(
            ini, OutputChannelDemand.selective(set("{ rpm * 2 }"), 1));

        assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(4, 2)), selection.getRanges());
        assertEquals(set("rpm"), selection.getRequestedChannelRoots());
        assertFalse(selection.isFull());
    }

    @Test
    void requestsFullOutputForUnknownOrOutOfBoundsChannels() {
        IniFileModel unknown = ini(100, Collections.emptyMap());
        assertTrue(OutputChannelPollPlan.create(unknown, OutputChannelDemand.selective(set("missing"), 1)).isFull());

        IniFileModel invalid = ini(100, fields(field("rpm", 99, 2)));
        assertTrue(OutputChannelPollPlan.create(invalid, OutputChannelDemand.selective(set("rpm"), 1)).isFull());
    }

    @Test
    void requestsTheWholeOutputBlockForFullOrEmptyDemand() {
        IniFileModel ini = ini(100, Collections.emptyMap());
        for (OutputChannelDemand demand : new OutputChannelDemand[]{OutputChannelDemand.full(8), OutputChannelDemand.selective(Collections.emptySet(), 9)}) {
            OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini, demand);
            assertTrue(selection.isFull());
            assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(0, 100)), selection.getRanges());
        }
    }

    @Test
    void requestsFullOutputForCyclicExpressions() {
        IniFileModel ini = ini(100, Collections.emptyMap());
        Map<String, String> expressions = new LinkedHashMap<>();
        expressions.put("a", "{b}");
        expressions.put("b", "{a}");
        when(ini.getExpressionOutputChannels()).thenReturn(expressions);

        assertTrue(OutputChannelPollPlan.create(ini, OutputChannelDemand.selective(set("a"), 1)).isFull());
    }

    @Test
    void requestsFullOutputWhenOnlyHostComputedChannelsAreNeeded() {
        IniFileModel ini = ini(100, Collections.emptyMap());

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini,
                OutputChannelDemand.selective(set("runtimeDataRateGauge"), 1));

        assertTrue(selection.isFull());
        assertEquals(Collections.singletonList(new OutputChannelPollPlan.Range(0, 100)), selection.getRanges());
        assertEquals(Collections.emptySet(), selection.getRequestedChannelRoots());
    }

    @Test
    void matchesRequestedChannelsCaseInsensitivelyAndPreservesGeneration() {
        IniFileModel ini = ini(100, fields(field("RPM", 4, 2)));
        OutputChannelDemand demand = OutputChannelDemand.selective(set("RpM"), 42);

        OutputChannelPollPlan selection = OutputChannelPollPlan.create(ini, demand);

        assertEquals(set("rpm"), demand.getChannels());
        assertEquals(42, selection.getGeneration());
        assertEquals(set("rpm"), selection.getRequestedChannelRoots());
    }

    private static IniFileModel ini(int blockSize, Map<String, IniField> outputChannels) {
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getOchBlockSize()).thenReturn(blockSize);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(ini.getAllOutputChannels()).thenReturn(outputChannels);
        when(ini.getExpressionOutputChannels()).thenReturn(Collections.emptyMap());
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        when(ini.findIniField(anyString())).thenReturn(Optional.empty());
        return ini;
    }

    private static Map<String, IniField> fields(IniField... fields) {
        Map<String, IniField> result = new LinkedHashMap<>();
        for (IniField field : fields) {
            result.put(field.getName(), field);
        }
        return result;
    }

    private static IniField field(String name, int offset, int size) {
        IniField field = mock(IniField.class);
        when(field.getName()).thenReturn(name);
        when(field.getOffset()).thenReturn(offset);
        when(field.getSize()).thenReturn(size);
        return field;
    }

    private static Set<String> set(String... names) {
        java.util.LinkedHashSet<String> result = new java.util.LinkedHashSet<>();
        Collections.addAll(result, names);
        return result;
    }
}
