package com.rusefi.misc;

import com.rusefi.pcb.*;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class RemoveUnneededTraces {
    private final static Set<String> alreadyRemoved = new HashSet<String>();

    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("two parameters expected: INPUT_FILE OUTPUT_FILE");
            return;
        }
        String input = args[0];
        String output = args[1];

        optimize(input, output);
    }

    public static void optimize(String input, String output) throws IOException {
        PcbNode destNode = PcbNode.readFromFile(input);


        while (removeUnusedSegments(destNode) || removeUnusedVias(destNode)) {
            System.out.println("Still removing...");
        }

        destNode.write(output);
    }

    private static boolean removeUnusedVias(PcbNode destNode) {
        List<ViaNode> unused = findUnusedVias(destNode);
        for (ViaNode via : unused) {
            System.out.println("Removing via: " + via);
            boolean removed = destNode.removeChild(via);
            if (!removed)
                throw new IllegalStateException("not removed: " + removed);

        }
        return !unused.isEmpty();
    }

    private static List<ViaNode> findUnusedVias(PcbNode destNode) {
        List<ViaNode> result = new ArrayList<ViaNode>();

        List<PcbNode> stuff = destNode.iterate("segment");
//        stuff.addAll(destNode.iterate("segment"));

        for (PcbNode n : destNode.iterate("via")) {
            ViaNode via = (ViaNode) n;

            int count = 0;

            for (PcbNode segment : stuff)
                if (segment.isConnected(via.location))
                    count++;

            if (via.netId == NetNode.GND_NET_ID) {
                if (count == 0)
                    result.add(via);
            } else {
                if (count < 2)
                    result.add(via);
            }
        }
        return result;
    }

    private static boolean removeUnusedSegments(PcbNode destNode) {
        List<PcbNode> stuff = new ArrayList<PcbNode>(destNode.iterate("module"));
        stuff.addAll(destNode.iterate("via"));


        Object o = destNode.iterate("segment");
        List<SegmentNode> segments = (List<SegmentNode>) o;
        System.out.println(segments.size() + " segment(s)");

        List<SegmentNode> unused = findUnusedSegments(segments, stuff);
        for (SegmentNode segment : unused) {
            boolean removed = destNode.removeChild(segment);
            if (!removed)
                throw new IllegalStateException();
            String netName = segment.net.id;
            if (!alreadyRemoved.contains(netName)) {
                alreadyRemoved.add(netName);
                System.out.println("Unused segment in network " + netName + ": " + segment);
            }
        }
        return !unused.isEmpty();
    }

    private static List<SegmentNode> findUnusedSegments(List<SegmentNode> segments, List<PcbNode> modules) {
        List<SegmentNode> unused = new ArrayList<SegmentNode>();
        for (SegmentNode segment : segments) {
            if (isUnused(segments, segment, modules)) {
//                System.out.println("Unused on " + segment.net.id + ": " + segment);
                unused.add(segment);
            }
        }
        return unused;
    }

    public static boolean isUnused(List<SegmentNode> segments, SegmentNode segment, List<PcbNode> modules) {
        PointNode start = segment.start;
        PointNode end = segment.end;
        if (isConnected(start, segments, segment) == null && isConnected(start, modules, null) == null) {
            System.out.println("Not connected start: " + segment);
            return true;
        }

        PcbNode endModule = isConnected(end, modules, null);
        if (isConnected(end, segments, segment) == null && endModule == null) {
            System.out.println("Not connected end: " + segment);
            return true;
        }
        return false;
    }

    private static PcbNode isConnected(PointNode point, List<? extends PcbNode> elements, SegmentNode parent) {
        for (PcbNode segmentNode : elements) {
            if (segmentNode == parent)
                continue;

            if (segmentNode.isConnected(point))
                return segmentNode;
        }
        return null;
    }
}
