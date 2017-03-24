package com.rusefi.misc;

import com.rusefi.util.FileUtils;

import java.io.File;
import java.io.IOException;
import java.util.*;

import static com.rusefi.pcb.PcbMergeTool.log;

/**
 * 1/19/14
 * (c) Andrey Belomutskiy
 */
public class ChangesModel {
    private static final ChangesModel instance = new ChangesModel();
    private static final String REMOVE = "remove";
    private static final String ADD = "add";
    private static final String MOVE = "move_module";
    private static final String OPTIMIZE = "optimize";
    private static final String COPY = "copy";
    private static final String MERGE_NET = "merge_net";

    public final Set<String> DEL_REQUESTS = new TreeSet<String>(String.CASE_INSENSITIVE_ORDER);
    public final List<NameAndOffset> ADD_REQUESTS = new ArrayList<NameAndOffset>();
    public final List<NameAndOffset> MOVE_REQUESTS = new ArrayList<NameAndOffset>();

    public final List<TwoFileRequest> OPTIMIZE_REQUESTS = new ArrayList<TwoFileRequest>();
    public final List<TwoFileRequest> COPY_REQUESTS = new ArrayList<TwoFileRequest>();
    /**
     * Old net name > New net name
     */
    public final Map<String, String> NET_MERGE_REQUESTS = new TreeMap<String, String>(String.CASE_INSENSITIVE_ORDER);

    public static ChangesModel getInstance() {
        return instance;
    }

    public static void readConfiguration(String fileName) throws IOException {
        if (!new File(fileName).isFile()) {
            log("No such file: " + fileName);
            return;
        }
        log("Reading commands from " + fileName);
        List<String> a = FileUtils.readFileToList(fileName);

        getInstance().read(a);
    }

    public void read(List<String> lines) {
        int lineIndex = 0;
        for (String line : lines) {
            lineIndex++;
            line = line.trim();
            if (line.isEmpty())
                continue;
            if (line.startsWith("#"))
                continue; // this line is a comment

            if (line.toLowerCase().startsWith(REMOVE)) {
                DEL_REQUESTS.add(line.substring(REMOVE.length()).trim());
                continue;
            } else if (line.toLowerCase().startsWith(ADD)) {
                addAddRequest(line.substring(ADD.length()).trim());
                continue;
            } else if (line.toLowerCase().startsWith(MOVE)) {
                addMoveRequest(line.substring(MOVE.length()).trim());
                continue;
            } else if (line.toLowerCase().startsWith(OPTIMIZE)) {
                OPTIMIZE_REQUESTS.add(TwoFileRequest.parseTwoFile(line.substring(OPTIMIZE.length()).trim(), lineIndex));
                continue;
            } else if (line.toLowerCase().startsWith(COPY)) {
                COPY_REQUESTS.add(TwoFileRequest.parseTwoFile(line.substring(COPY.length()).trim(), lineIndex));
                continue;
            } else if (line.toLowerCase().startsWith(MERGE_NET)) {
                TwoFileRequest req = TwoFileRequest.parseTwoFile(line.substring(MERGE_NET.length()).trim(), lineIndex);
                NET_MERGE_REQUESTS.put(req.input, req.output);
                log("Net " + req.input + " to be merged into " + req.output);
                continue;
            }

            System.err.println("ChangesModel: Ignoring invalid line: " + line);

        }
        log("Got " + DEL_REQUESTS.size() + " remove request(s)");
        log("Got " + ADD_REQUESTS.size() + " add request(s)");
        log("Got " + OPTIMIZE_REQUESTS.size() + " optimize request(s)");
        log("Got " + NET_MERGE_REQUESTS.size() + " merge net request(s)");
    }

    private void addMoveRequest(String request) {
        MOVE_REQUESTS.add(NameAndOffset.parseNameAndOffset(request));
    }

    private void addAddRequest(String request) {
        ADD_REQUESTS.add(NameAndOffset.parseNameAndOffset(request));
    }

    public void clear() {
        DEL_REQUESTS.clear();
        ADD_REQUESTS.clear();
        OPTIMIZE_REQUESTS.clear();
        MOVE_REQUESTS.clear();
    }
}
