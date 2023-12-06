package com.rusefi.ldmp;

import com.rusefi.util.LazyFile;

public class StringBufferLazyFile implements LazyFile {
    StringBuilder sb = new StringBuilder();

    @Override
    public void write(String line) {
        sb.append(line);
    }

    @Override
    public void close() {

    }
}
