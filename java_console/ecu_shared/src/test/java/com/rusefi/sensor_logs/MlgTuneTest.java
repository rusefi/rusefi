package com.rusefi.sensor_logs;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class MlgTuneTest {
    @TempDir Path directory;

    @Test
    void extractsExactTunerStudioBytesIncludingLatin1AndAllPages() throws Exception {
        byte[] tune = ("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
                + "<msq xmlns=\"http://www.msefi.com/:msq\"><page number=\"0\">"
                + "<constant name=\"note\">caf\u00e9</constant></page>"
                + "<page number=\"1\"><constant name=\"luaScript\">print(1)</constant></page></msq>\n")
                .getBytes(StandardCharsets.ISO_8859_1);
        Path log = directory.resolve("ts.mlg");
        Files.write(log, fixture(tune, tune.length));
        Path extracted = MlgTune.extract(log, MlgTune.defaultOutput(log));
        assertEquals(directory.resolve("ts.msq"), extracted);
        assertArrayEquals(tune, Files.readAllBytes(extracted));
    }

    @Test
    void skipsOtherProvidersByLength() throws Exception {
        byte[] decoy = provider("LogStart_MAIN_TUNE", "<msq><wrong/></msq>".getBytes(StandardCharsets.UTF_8));
        byte[] tune = "<msq><right/></msq>".getBytes(StandardCharsets.UTF_8);
        ByteArrayOutputStream info = new ByteArrayOutputStream();
        info.write(provider("Some_other_provider", decoy));
        info.write('\n');
        info.write(provider("LogStart_MAIN_TUNE", tune));
        Path log = directory.resolve("providers.mlg");
        Files.write(log, withInfo(info.toByteArray()));
        assertArrayEquals(tune, MlgTune.read(log));
    }

    @Test
    void rejectsMalformedHeadersLengthsAndXmlWithoutPublishingOutput() throws Exception {
        byte[] tune = "<msq/>".getBytes(StandardCharsets.UTF_8);
        byte[] valid = fixture(tune, tune.length);
        for (int offset : new int[]{0, 5, 7, 12, 16, 22}) {
            byte[] invalid = valid.clone();
            invalid[offset] = (byte) 0xff;
            assertRejected(invalid);
        }
        for (int length : new int[]{5, 23, valid.length - 1}) {
            assertRejected(Arrays.copyOf(valid, length));
        }
        assertRejected(fixture(tune, tune.length - 1));
        assertRejected(fixture(tune, tune.length + 100));
        assertRejected(fixture(tune, -1));
        assertRejected(fixture(new byte[0], 0));
        for (String bad : new String[]{"<not-msq/>", "<msq>",
                "<!DOCTYPE msq [<!ENTITY x SYSTEM 'file:///nonexistent'>]><msq>&x;</msq>"}) {
            byte[] bytes = bad.getBytes(StandardCharsets.UTF_8);
            assertRejected(fixture(bytes, bytes.length));
        }
        assertRejected(withInfo(("NEW_INFO_PROVIDER,LogStart_MAIN_TUNE,Type:msqVisible:falseLength:"
                + "999999999999999999999999999999999\n<msq/>").getBytes(StandardCharsets.UTF_8)));
    }

    @Test
    void doesNotSearchSamplesForTuneOrOverwriteFiles() throws Exception {
        byte[] valid = fixture("<msq/>".getBytes(StandardCharsets.UTF_8), 6);
        byte[] noTune = valid.clone();
        // Put all of the apparent XML/provider in the sample region instead of the info region.
        ByteBuffer.wrap(noTune).putInt(16, 24 + 89);
        assertRejected(noTune);
        ByteBuffer.wrap(noTune).putInt(12, 0);
        assertRejected(noTune);

        Path log = directory.resolve("existing.mlg");
        Files.write(log, valid);
        Path output = directory.resolve("existing.msq");
        Files.write(output, new byte[]{42});
        assertThrows(IOException.class, () -> MlgTune.extract(log, output));
        assertThrows(IOException.class, () -> MlgTune.extract(log, log));
        assertArrayEquals(new byte[]{42}, Files.readAllBytes(output));
        assertArrayEquals(valid, Files.readAllBytes(log));
    }

    private void assertRejected(byte[] logBytes) throws Exception {
        Path log = directory.resolve("bad.mlg");
        Files.write(log, logBytes);
        Path output = directory.resolve("bad.msq");
        assertThrows(IOException.class, () -> MlgTune.extract(log, output));
        assertFalse(Files.exists(output));
        try (Stream<Path> files = Files.list(directory)) {
            assertFalse(files.anyMatch(path -> path.toString().endsWith(".tmp")));
        }
    }

    /** Independent layout observed in issue 825's TunerStudio 3.3.01 logs. */
    private static byte[] fixture(byte[] tune, int length) throws IOException {
        ByteArrayOutputStream info = new ByteArrayOutputStream();
        info.write(("\"rusEFI test signature\"\n\"Capture Date: test, File author: TunerStudio MS Ultra version 3.3.01\"\n"
                + "\nNEW_INFO_PROVIDER,LogStart_MAIN_TUNE,Type:msqVisible:falseLength:" + length + "\n")
                .getBytes(StandardCharsets.US_ASCII));
        info.write(tune);
        info.write(0);
        return withInfo(info.toByteArray());
    }

    private static byte[] provider(String name, byte[] contents) throws IOException {
        ByteArrayOutputStream info = new ByteArrayOutputStream();
        info.write(("NEW_INFO_PROVIDER," + name + ",Type:msqVisible:falseLength:" + contents.length + "\n")
                .getBytes(StandardCharsets.US_ASCII));
        info.write(contents);
        return info.toByteArray();
    }

    private static byte[] withInfo(byte[] info) throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(bytes);
        out.write(new byte[]{'M', 'L', 'V', 'L', 'G', 0});
        out.writeShort(2);
        out.writeInt(0);
        out.writeInt(24 + 89);
        out.writeInt(24 + 89 + info.length);
        out.writeShort(4);
        out.writeShort(1);
        out.write(new byte[89]);
        out.write(info);
        return bytes.toByteArray();
    }
}
