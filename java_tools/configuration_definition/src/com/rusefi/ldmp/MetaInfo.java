package com.rusefi.ldmp;

import java.util.*;

public class MetaInfo {
    public Map<String, List<Request>> map = new TreeMap<>();


    public List<Request> start(String content) {
        map.putIfAbsent(content, new ArrayList<>());
        return map.get(content);
    }

    public List<Request> first() {
        return map.values().iterator().next();
    }
}
