package com.rusefi.pcb;

import com.rusefi.PcbCopyTool;
import com.rusefi.misc.NameAndOffset;
import com.rusefi.misc.ChangesModel;
import com.rusefi.misc.RemoveUnneededTraces;
import com.rusefi.misc.TwoFileRequest;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * (c) Andrey Belomutskiy
 * 12/16/13.
 */
public class PcbMergeTool {
    private static Networks networks = new Networks();

    public static void main(String[] args) throws IOException {
        if (args.length != 3) {
            System.out.println("Three parameters expected: SOURCE_PCB_FILENAME DESTINATION_PCB_FILENAME CHANGES_LIST_FILENAME");
            return;
        }
        String sourcePcb = args[0];
        String destination = args[1];
        String changes = args[2];

        ChangesModel.readConfiguration(changes);

        log("Running COPY commands");
        for (TwoFileRequest or : ChangesModel.getInstance().COPY_REQUESTS)
            PcbCopyTool.copy(or.input, or.output);

        log("Running OPTIMIZE commands");
        for (TwoFileRequest or : ChangesModel.getInstance().OPTIMIZE_REQUESTS)
            RemoveUnneededTraces.optimize(or.input, or.output);

        PcbNode destNode = PcbNode.readFromFile(sourcePcb);


        for (PcbNode net : destNode.iterate("net")) {
            String netName = net.getChild(1); // todo: nicer method?
            if (!Networks.isLocalNetwork(netName))
                networks.registerNetworkIfPcbSpecific(netName);
        }

        log("Running ADD commands");
        for (NameAndOffset addRequest : ChangesModel.getInstance().ADD_REQUESTS) {
            PcbNode node = PcbMoveTool.readAndMove(addRequest.getName(), addRequest.x, addRequest.y);

            mergePcb(destNode, node);
        }

        log("Running MOVE commands");
        for (NameAndOffset moveRequest : ChangesModel.getInstance().MOVE_REQUESTS) {
            String moduleName = moveRequest.getName();
            ModuleNode module = findModuleByName(destNode, moduleName);
            if (module == null) {
                log("Module not found: " + moduleName);
                continue;
            }

            PointNode at = module.at;
            at.setLocation(at.x + moveRequest.x, at.y + moveRequest.y);
        }

        removeNodes(destNode);

        destNode.write(destination);

        RemoveUnneededTraces.optimize(destination, destination);
    }

    private static ModuleNode findModuleByName(PcbNode destNode, String moduleName) {
        for (PcbNode node : destNode.iterate("module")) {
            ModuleNode mn = (ModuleNode) node;
            if (moduleName.toLowerCase().equals(mn.getReference().toLowerCase()))
                return mn;
        }
        return null;
    }

    private static void mergePcb(PcbNode destNode, PcbNode source) throws IOException {
        /**
         * original local net name > new net name in combined PCB
         */
        Map<String, String> netNameMapping = new HashMap<>();
        /**
         * original local net ID (as string) > new net ID
         */
        Map<String, Integer> netIdMapping = new HashMap<>();

        for (PcbNode net : source.iterate("net")) {
            String netId = net.getChild(0);
            String netName = net.getChild(1); // todo: nicer method?
            String newName = networks.registerNetworkIfPcbSpecific(netName);
            netNameMapping.put(netName, newName);
            netIdMapping.put(netId, networks.getId(newName));
        }

        List<PcbNode> zones = source.iterate("zone");
        log("Processing  " + zones.size() + " zone(s)");
        for (PcbNode z : zones) {
            ZoneNode zone = (ZoneNode) z;
            if (zone.getLayerNode().isSikscreenLayer())
                destNode.addChild(zone);
        }

        List<PcbNode> arcs = source.iterate("gr_arc");
        log("Processing  " + arcs.size() + " arc(s)");
        for (PcbNode arc : arcs)
            destNode.addChild(arc);


        List<PcbNode> lines = source.iterate("gr_line");
        log("Processing  " + lines.size() + " line(s)");
        for (PcbNode l : lines) {
            GrLineNode line = (GrLineNode) l;
            if (line.layerNode.isSikscreenLayer())
                destNode.addChild(line);
        }


        List<PcbNode> labels = source.iterate("gr_text");
        log("Processing  " + labels.size() + " label(s)");
        for (PcbNode label : labels) {
            destNode.addChild(label);
        }

        List<PcbNode> modules = source.iterate("module");
        log("Processing  " + modules.size() + " module(s)");
        for (PcbNode module : modules) {
            for (PcbNode pad : module.iterate("pad")) {
                if (!pad.hasChild("net"))
                    continue;
                fixNetId(netIdMapping, netNameMapping, pad);
//                PcbNode net = pad.find("net");
//                String localName = netNameMapping.get(net.getChild(1));
//                net.setString(1, localName);
//                net.setInt(0, networks.getId(localName));
            }
            destNode.addChild(module);
        }

        List<PcbNode> segments = source.iterate("segment");
        log("Processing " + segments.size() + " segments");
        for (PcbNode segment : segments) {
//            if (!segment.hasChild("net"))
//                continue;
            fixNetId(netIdMapping, netNameMapping, segment);

            destNode.addChild(segment);
        }

        List<PcbNode> vias = source.iterate("via");
        log("Processing " + vias.size() + " vias");
        for (PcbNode via : vias) {
            fixNetId(netIdMapping, netNameMapping, via);

            destNode.addChild(via);
        }

//        for (PcbNode zone : source.iterate("zone")) {
//            fixNetId(netIdMapping, zone);
//            destNode.addChild(zone);
//        }
    }

    public static void removeNodes(PcbNode source) {
        for (PcbNode module : source.iterate("module")) {
            if (shouldRemove((ModuleNode) module))
                source.removeChild(module);
        }
    }

    private static boolean shouldRemove(ModuleNode module) {
        for (PcbNode fp_text : module.iterate("fp_text")) {
            if ("reference".equals(fp_text.getChild(0))) {
                String name = fp_text.getChild(1);
                if (ChangesModel.getInstance().DEL_REQUESTS.contains(name))
                    return true;
            }
        }
        return false;
    }

    private static void fixNetId(Map<String, Integer> netIdMapping, Map<String, String> netNameMapping, PcbNode node) {
        NetNode net = (NetNode) node.find("net");
        String originalId = net.id;
        Integer currentNetId = netIdMapping.get(originalId);
        String globalName = networks.nameById.get(currentNetId);
//        String newName = netNameMapping.get(originalName);
//        if (newName == null)
//            throw new NullPointerException("?");

        if (ChangesModel.getInstance().NET_MERGE_REQUESTS.containsKey(globalName)) {
            String newName = ChangesModel.getInstance().NET_MERGE_REQUESTS.get(globalName);
            log("Will merge " + globalName + " into " + newName + ". ID was " + currentNetId);
            currentNetId = networks.networks.get(newName);
            if (currentNetId == null)
                throw new NullPointerException("Cannot find net: " + newName);
            log("New ID: " + currentNetId);
            globalName = newName;
        }
        net.setInt(0, currentNetId);
        if (net.name != null)
            net.setString(1, globalName);
    }

    private static class Networks {
        /**
         * Net name > Net Id
         */
        private Map<String, Integer> networks = new HashMap<>();
        private Map<Integer, String> nameById = new HashMap<>();

        public String registerNetworkIfPcbSpecific(String name) {
            if (isLocalNetwork(name)) {
                String newName = "F-0000" + networks.size();
                log("Board-specific net: " + name + " would be " + newName);

                registerNet(newName);
                int newId = networks.get(newName);
                log(newName + " is " + newId);
                return newName;
            } else {
                if (networks.containsKey(name)) {
                    log("Existing global net: " + name);
                    return name;
                }

                log("New global net: " + name);
                registerNet(name);
                return name;
            }
        }

        private static boolean isLocalNetwork(String name) {
            return name.startsWith("N-00");
        }

        private void registerNet(String name) {
            networks.put(name, networks.size());
            nameById.put(networks.get(name), name);
        }

        public int getId(String localName) {
            Integer value = networks.get(localName);
            if (value == null)
                throw new NullPointerException("No id for " + localName);
            return value;
        }
    }

    public static void log(String s) {
        System.out.println(s);
    }
}
