package com.rusefi.pcb;

import java.io.IOException;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 12/8/13
 */
public class PcbMoveTool {
    public static void main(String[] args) throws IOException {
        if (args.length != 4) {
            System.out.println("Four parameters expected: SRC_FILENAME DST_FILENAME X Y");
            return;
        }
        String srcFileName = args[0];
        String dstFileName = args[1];
        double x = Double.parseDouble(args[2]);
        double y = Double.parseDouble(args[3]);

        PcbNode node = readAndMove(srcFileName, x, y);
        node.write(dstFileName);
    }

    public static PcbNode readAndMove(String fileName, double x, double y) throws IOException {
        PcbNode node = PcbNode.readFromFile(fileName);

        move(node, x, y);
        return node;
    }

    public static void move(PcbNode pcbNode, double dx, double dy) {
        System.out.println("Moving " + pcbNode + ": dx="  +dx + " dy=" + dy);

        List<PcbNode> dimensions = pcbNode.iterate("dimension");
        System.out.println("Moving " + dimensions.size() + " dimension");
        for (PcbNode dimension : dimensions) {
            moveAt(dx, dy, dimension.find("gr_text"));
            movePts(dx, dy, dimension.find("feature1"));
            movePts(dx, dy, dimension.find("feature2"));
            movePts(dx, dy, dimension.find("crossbar"));
            movePts(dx, dy, dimension.find("arrow1a"));
            movePts(dx, dy, dimension.find("arrow1b"));
            movePts(dx, dy, dimension.find("arrow2a"));
            movePts(dx, dy, dimension.find("arrow2b"));
        }

        List<PcbNode> gr_lines = pcbNode.iterate("gr_line");
        System.out.println("Moving " + gr_lines.size() + " gr_lines");
        for (PcbNode gr_line : gr_lines)
            moveStartEnd(dx, dy, gr_line);

        List<PcbNode> gr_arcs = pcbNode.iterate("gr_arc");
        System.out.println("Moving " + gr_arcs.size() + " gr_arcs");
        for (PcbNode gr_arc : gr_arcs) {
            PcbNode start = gr_arc.find("start");
            moveCoordinatesInFirstChildren(dx, dy, start);

            PcbNode end = gr_arc.find("end");
            moveCoordinatesInFirstChildren(dx, dy, end);
        }

        List<PcbNode> gr_circles = pcbNode.iterate("gr_circle");
        System.out.println("Moving " + gr_circles.size() + " gr_circles");
        for (PcbNode gr_circle : gr_circles) {
            PcbNode start = gr_circle.find("center");
            moveCoordinatesInFirstChildren(dx, dy, start);

            PcbNode end = gr_circle.find("end");
            moveCoordinatesInFirstChildren(dx, dy, end);
        }

        List<PcbNode> gr_texts = pcbNode.iterate("gr_text");
        System.out.println("Moving " + gr_texts.size() + " gr_texts");
        for (PcbNode gr_text : gr_texts)
            moveAt(dx, dy, gr_text);

        List<PcbNode> zones = pcbNode.iterate("zone");
        System.out.println("Moving " + zones.size() + " zones");
        for (PcbNode zone : zones) {
            List<PcbNode> filledPolygons = zone.iterate("filled_polygon");
            for (PcbNode filledPolygon : filledPolygons)
                movePts(dx, dy, filledPolygon);
            List<PcbNode> polygons = zone.iterate("polygon");
            for (PcbNode polygon : polygons)
                movePts(dx, dy, polygon);
        }


        List<PcbNode> segments = pcbNode.iterate("segment");
        System.out.println("Moving " + segments.size() + " segments");
        for (PcbNode segment : segments)
            moveStartEnd(dx, dy, segment);

        List<PcbNode> vias = pcbNode.iterate("via");
        System.out.println("Moving " + vias.size() + " vias");
        for (PcbNode via : vias)
            moveAt(dx, dy, via);


        List<PcbNode> modules = pcbNode.iterate("module");
        System.out.println("Moving " + modules.size() + " modules");
        for (PcbNode module : modules)
            moveAt(dx, dy, module);
    }

    public static void movePts(double dx, double dy, PcbNode polygon) {
        PcbNode pts = polygon.find("pts");

        for (PcbNode point : pts.nodes())
            moveCoordinatesInFirstChildren(dx, dy, point);
    }

    public static void moveStartEnd(double dx, double dy, PcbNode segment) {
        PcbNode start = segment.find("start");
        moveCoordinatesInFirstChildren(dx, dy, start);

        PcbNode end = segment.find("end");
        moveCoordinatesInFirstChildren(dx, dy, end);
    }

    public static void moveAt(double dx, double dy, PcbNode module) {
        PcbNode at = module.find("at");
        moveCoordinatesInFirstChildren(dx, dy, at);
    }

    public static void moveCoordinatesInFirstChildren(double dx, double dy, PcbNode at) {
        moveCoordinates(dx, dy, at, 0);
    }

    private static void moveCoordinates(double dx, double dy, PcbNode at, int index) {
        double x = at.asDouble(index);
        double y = at.asDouble(index + 1);
        at.setDouble(index, x + dx);
        at.setDouble(index + 1, y + dy);
    }
}
