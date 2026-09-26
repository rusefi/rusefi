package com.rusefi;

import com.rusefi.ui.UIContext;
import com.rusefi.ui.plugins.StartupTabProvider;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

public class StartupTabProviderTest {
    @TempDir
    Path pluginDirectory;

    @Test
    void noProvidersKeepsBuiltInTabs() throws Exception {
        withProviders("", (tabs, context) -> assertEquals(3, tabs.getTabCount()));
    }

    @Test
    void discoversProviderAndPassesStartupContextOnEdt() throws Exception {
        withProviders(ExampleProvider.class.getName(), (tabs, context) -> {
            assertEquals(4, tabs.getTabCount());
            assertEquals("Example", tabs.getTitleAt(3));
            assertSame(context, ((JComponent) tabs.getComponentAt(3)).getClientProperty("context"));
        });
    }

    @Test
    void failingTabDoesNotPreventLaterProvider() throws Exception {
        withProviders(FailingProvider.class.getName() + "\n" + ExampleProvider.class.getName(), (tabs, context) -> {
            assertEquals(4, tabs.getTabCount());
            assertEquals("Example", tabs.getTitleAt(3));
        });
    }

    @Test
    void missingProviderClassDoesNotPreventStartup() throws Exception {
        withProviders("com.example.MissingStartupTabProvider", (tabs, context) -> assertEquals(3, tabs.getTabCount()));
    }

    private void withProviders(String providers, java.util.function.BiConsumer<JTabbedPane, UIContext> assertions)
        throws Exception {
        Path service = pluginDirectory.resolve("META-INF/services/" + StartupTabProvider.class.getName());
        Files.createDirectories(service.getParent());
        Files.write(service, providers.getBytes(StandardCharsets.UTF_8));
        try (URLClassLoader loader = new URLClassLoader(new URL[]{pluginDirectory.toUri().toURL()},
            StartupTabProviderTest.class.getClassLoader())) {
            SwingUtilities.invokeAndWait(() -> {
                ClassLoader previous = Thread.currentThread().getContextClassLoader();
                try {
                    Thread.currentThread().setContextClassLoader(loader);
                    UIContext context = mock(UIContext.class);
                    JTabbedPane tabs = new JTabbedPane();
                    tabs.addTab("Update Firmware", new JPanel());
                    tabs.addTab("Manage Tunes", new JPanel());
                    tabs.addTab("Connect", new JPanel());
                    StartupFrame.addCustomTabs(tabs, context);
                    assertEquals("Update Firmware", tabs.getTitleAt(0));
                    assertEquals("Manage Tunes", tabs.getTitleAt(1));
                    assertEquals("Connect", tabs.getTitleAt(2));
                    assertEquals(0, tabs.getSelectedIndex());
                    assertions.accept(tabs, context);
                } finally {
                    Thread.currentThread().setContextClassLoader(previous);
                }
            });
        }
    }

    public static class ExampleProvider implements StartupTabProvider {
        @Override
        public String getTitle() {
            return "Example";
        }

        @Override
        public JComponent createTab(UIContext uiContext) {
            assertTrue(SwingUtilities.isEventDispatchThread());
            JPanel panel = new JPanel();
            panel.putClientProperty("context", uiContext);
            return panel;
        }
    }

    public static class FailingProvider extends ExampleProvider {
        @Override
        public JComponent createTab(UIContext uiContext) {
            throw new IllegalStateException("Test provider failure");
        }
    }
}
