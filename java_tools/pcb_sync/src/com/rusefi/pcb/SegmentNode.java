package com.rusefi.pcb;

import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class SegmentNode extends PcbNode {
    public final NetNode net;
    public final PointNode start;
    public final PointNode end;
    private final SizeNode size;

    public SegmentNode(String nodeName, int closingIndex, List<Object> children) {
        super(nodeName, closingIndex, children);
        net = (NetNode) find("net");
        start = (PointNode) find("start");
        end = (PointNode) find("end");
        size = (SizeNode) find("width");
    }

    public boolean isConnected(PointNode point) {
        return point.isConnected(start, size) || point.isConnected(end, size);
    }

    @Override
    public String toString() {
        return "SegmentNode{" +
                "net=" + net +
                ", start=" + start +
                ", end=" + end +
                '}';
    }
}
