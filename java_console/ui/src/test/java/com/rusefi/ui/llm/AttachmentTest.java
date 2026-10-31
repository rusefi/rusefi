package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Base64;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.ui.llm.ChatGptAgentTest.array;
import static com.rusefi.ui.llm.ChatGptClient.*;
import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class AttachmentTest {
    @TempDir Path directory;

    @Test void textFilesGoIntoTheMessageText() throws Exception {
        Path log = directory.resolve("cruise.msl");
        Files.write(log, "Time\tRPM\n0.0\t850\n".getBytes(StandardCharsets.UTF_8));
        Attachment attachment = Attachment.fromFile(log);
        assertEquals(Attachment.Kind.TEXT, attachment.kind);

        JSONObject message = Attachment.userMessage("Why does it hunt?", Collections.singletonList(attachment));
        JSONArray parts = (JSONArray) message.get("content");
        assertEquals(1, parts.size());
        String text = (String) ((JSONObject) parts.get(0)).get("text");
        assertTrue(text.startsWith("Why does it hunt?"));
        assertTrue(text.contains("Attached file cruise.msl"));
        assertTrue(text.contains("0.0\t850"));
    }

    @Test void withoutAttachmentsTheMessageStaysPlainText() {
        assertEquals("Hello", Attachment.userMessage("Hello", Collections.emptyList()).get("content"));
    }

    @Test void binaryAndOversizedFilesAreRefusedWithAReason() throws Exception {
        Path mlg = directory.resolve("log.mlg");
        Files.write(mlg, new byte[]{'M', 'L', 'V', 'L', 'G', 0, 1, 2});
        IOException binary = assertThrows(IOException.class, () -> Attachment.fromFile(mlg));
        assertTrue(binary.getMessage().contains(".msl or .csv export"), binary.getMessage());

        Path big = directory.resolve("huge.txt");
        Files.write(big, new String(new char[Attachment.MAX_TEXT_CHARS + 1]).replace('\0', 'x').getBytes(StandardCharsets.UTF_8));
        IOException tooLong = assertThrows(IOException.class, () -> Attachment.fromFile(big));
        assertTrue(tooLong.getMessage().contains("too long"), tooLong.getMessage());
    }

    @Test void picturesAreShrunkAndSentAsImages() throws Exception {
        BufferedImage large = new BufferedImage(3000, 2000, BufferedImage.TYPE_INT_RGB);
        Path png = directory.resolve("screenshot.png");
        ImageIO.write(large, "png", png.toFile());
        Attachment picture = Attachment.fromFile(png);
        assertEquals(Attachment.Kind.IMAGE, picture.kind);
        assertNotNull(picture.thumbnail);
        assertTrue(picture.content.startsWith("data:image/"));
        BufferedImage sent = ImageIO.read(new ByteArrayInputStream(Base64.getDecoder()
                .decode(picture.content.substring(picture.content.indexOf(',') + 1))));
        assertEquals(Attachment.MAX_IMAGE_SIDE, sent.getWidth());
        assertEquals(Attachment.MAX_IMAGE_SIDE * 2 / 3, sent.getHeight(), 1);

        JSONArray parts = (JSONArray) Attachment.userMessage("See this", Collections.singletonList(picture)).get("content");
        assertEquals("input_image", ((JSONObject) parts.get(1)).get("type"));
        assertEquals(picture.content, ((JSONObject) parts.get(1)).get("image_url"));
    }

    @Test void picturesGoWithTheirTurnOnlyAndNotIntoTheKeptHistory() throws Exception {
        Attachment picture = Attachment.fromImage(new BufferedImage(40, 30, BufferedImage.TYPE_INT_RGB), "Pasted picture 1.png");
        JSONObject answer = object("type", "message", "role", "assistant", "content",
                array(object("type", "output_text", "text", "The gauge reads 14.7", "annotations", array())));
        AtomicReference<JSONArray> requested = new AtomicReference<>();
        JSONArray history = ChatGptAgent.run((input, definitions, delta, c) -> {
                    requested.set((JSONArray) input.clone());
                    return new Response("", array(answer));
                }, new NoTools(), new JSONArray(), "What does the gauge show?", Collections.singletonList(picture),
                ignored -> { }, ignored -> { }, new Cancellation());

        assertTrue(requested.get().toJSONString().contains("input_image"), "the model sees the picture");
        assertFalse(history.toJSONString().contains("input_image"), "the kept history does not carry it again");
        assertFalse(history.toJSONString().contains(picture.content));
        assertTrue(history.toJSONString().contains("A picture was attached here"));
        assertEquals(answer, history.get(1));
    }

    @Test void savedTranscriptShowsAttachmentsOnTheMessage() {
        ChatTranscript chat = new ChatTranscript("empty");
        chat.load("> Please look\n[Attached: cruise.msl]\n[Attached: Pasted picture 1.png]\n\nLooks fine.\n");
        assertEquals(Arrays.asList("user[cruise.msl, Pasted picture 1.png]: Please look", "assistant: Looks fine."),
                chat.describeForTest());
    }

    private static final class NoTools implements ChatGptAgent.Tools {
        @Override public JSONArray definitions() { return new JSONArray(); }
        @Override public void checkConnected() { }
        @Override public JSONObject execute(String name, JSONObject arguments, Runnable checkCancellation) {
            throw new AssertionError("no tools expected");
        }
    }
}
