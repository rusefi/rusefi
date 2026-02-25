package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringBufferInputStream;

import static com.rusefi.AssertCompatibility.assertEquals;

public class TSProjectConsumerTest {
    private static final String smallContent = "hello = \";\"\n" +
        "world;comment\n" +
        ";comment2\n" +
        "end\n";

    @Test
    public void getTsCondition() {
        assertEquals("ts", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts\r\n"));
        assertEquals("ts_show_hip9011", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts_show_hip9011"));

        assertEquals("1", TSProjectConsumer.removeToken("1@@if_ts"));
        assertEquals("12", TSProjectConsumer.removeToken("1@@if_ts 2"));
        assertEquals("H2\r\n", TSProjectConsumer.removeToken("H@@if_ts 2\r\n"));
    }

    @Test
    public void cppCornerCaseEmptyStruct() {
        String test = "struct pid_s\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);


        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();

        state.readBufferedReader(test, javaFieldsConsumer, consumer, tsProjectConsumer);

        assertEquals("; total TS size = 0\n", tsProjectConsumer.getContent());

        assertEquals("", javaFieldsConsumer.getContent());


        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 1);\n" +
                "\n", consumer.getContent());
    }

    @Test
    public void testReaderKeepComments() throws IOException {
        TSProjectConsumer consumer = new TestTSProjectConsumer(new ReaderStateImpl());
        TsFileContent content = consumer.getTsFileContent(new StringBufferInputStream(smallContent));
        assertEquals(smallContent, content.getPrefix());
        assertEquals("", content.getPostfix());
    }

    @Test
    public void testReaderDropComments() throws IOException {
        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.getVariableRegistry().put(TSProjectConsumer.TS_DROP_TEMPLATE_COMMENTS, "true");
        TsFileContent content = consumer.getTsFileContent(new StringBufferInputStream(smallContent));
        assertEquals("hello = \";\"\n" +
            "world;comment\n" +
            "end\n", content.getPrefix());
        assertEquals("", content.getPostfix());
    }}
