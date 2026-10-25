package com.rusefi.pinout;

import javax.imageio.ImageIO;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Base64;
import java.util.IdentityHashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

/** A self-contained, selectable patchcord pinout alongside the printable Markdown. */
final class InteractivePatchcordHtml {
    private InteractivePatchcordHtml() {
    }

    static void write(File file, String ecuName, String adapterName,
                      List<PatchCordHelper.Step> steps,
                      List<PatchCordHelper.PinRef> unmatchedEcu,
                      List<PatchCordHelper.PinRef> unmatchedAdapter) throws IOException {
        Map<PatchCordHelper.Wire, String> ids = new IdentityHashMap<>();
        Map<PatchCordHelper.Wire, Integer> stepNumbers = new IdentityHashMap<>();
        int next = 1;
        for (PatchCordHelper.Step step : steps) {
            for (PatchCordHelper.Wire wire : step.wires) {
                ids.put(wire, "w" + next++);
                stepNumbers.put(wire, step.number);
            }
        }
        Map<ConnectorYaml, Map<ConnectorYaml, List<PatchCordHelper.Wire>>> pairs = new LinkedHashMap<>();
        Map<ConnectorYaml, List<PatchCordHelper.Wire>> allEcuWires = new LinkedHashMap<>();
        Map<ConnectorYaml, List<PatchCordHelper.Wire>> allAdapterWires = new LinkedHashMap<>();
        for (PatchCordHelper.Step step : steps) {
            for (PatchCordHelper.Wire wire : step.wires) {
                pairs.computeIfAbsent(wire.ecu.connector, key -> new LinkedHashMap<>())
                        .computeIfAbsent(wire.adapter.connector, key -> new ArrayList<>())
                        .add(wire);
                allEcuWires.computeIfAbsent(wire.ecu.connector, key -> new ArrayList<>()).add(wire);
                allAdapterWires.computeIfAbsent(wire.adapter.connector, key -> new ArrayList<>()).add(wire);
            }
        }

        StringBuilder html = new StringBuilder(32_768);
        html.append("<!doctype html>\n<html lang=\"en\"><head><meta charset=\"utf-8\">\n")
                .append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n")
                .append("<title>Patchcord: ").append(esc(ecuName)).append(" to ")
                .append(esc(adapterName)).append("</title>\n<style>\n")
                .append(resource("patchcord.css")).append("\n</style></head><body>\n")
                .append("<header class=\"page-header\"><div><p class=\"eyebrow\">rusEFI · PatchCordHelper</p>")
                .append("<h1>").append(esc(ecuName)).append(" → ").append(esc(adapterName)).append("</h1>")
                .append("<p>Each row shows one ECU connector on the left and its adapter connector on the right. Select a pin or wire to see both ends.</p>")
                .append("</div><div class=\"header-count\"><strong>").append(ids.size())
                .append("</strong><span>matched wires</span></div></header>\n")
                .append("<main><div class=\"controls\"><label>Step <select id=\"step-filter\">")
                .append("<option value=\"all\">All steps</option>");
        for (PatchCordHelper.Step step : steps) {
            html.append("<option value=\"").append(step.number).append("\">Step ")
                    .append(step.number).append(": ").append(esc(step.title)).append("</option>");
        }
        html.append("</select></label><label class=\"search-label\">Find wire ")
                .append("<input id=\"wire-search\" type=\"search\" placeholder=\"Pin, function, or color\" autocomplete=\"off\">")
                .append("</label><label>Image zoom <input id=\"image-zoom\" type=\"range\" min=\"100\" max=\"220\" value=\"100\">")
                .append("</label><button id=\"copy-link\" type=\"button\">Copy selection link</button></div>\n")
                .append("<p id=\"selection\" class=\"selection\" role=\"status\">Select a marked pin or a wire row.</p>\n")
                .append("<p class=\"key\"><span class=\"key-dot\"></span> Current wire · ")
                .append("colored traces run left to right · small full-color dots show wires to other plugs · orange corners identify the adapter end</p>\n")
                .append("<p class=\"key\">Available solid wire colors: ")
                .append(esc(String.join(", ", WireColors.officialColors()))).append(". ")
                .append("Each assembly step uses a color at most once.</p>\n")
                .append("<div class=\"connector-pairs\">");
        for (Map.Entry<ConnectorYaml, Map<ConnectorYaml, List<PatchCordHelper.Wire>>> ecu : pairs.entrySet()) {
            for (Map.Entry<ConnectorYaml, List<PatchCordHelper.Wire>> adapter : ecu.getValue().entrySet()) {
                renderPair(html, ecu.getKey(), adapter.getKey(), adapter.getValue(),
                        allEcuWires.get(ecu.getKey()), allAdapterWires.get(adapter.getKey()), ids, stepNumbers);
            }
        }
        if (pairs.isEmpty()) {
            html.append("<p>No matched wires to display.</p>");
        }
        html.append("</div>\n<section class=\"wire-section\"><h2>Wiring list</h2>")
                .append("<p id=\"visible-count\"></p><div class=\"table-wrap\"><table><thead><tr>")
                .append("<th>Step</th><th>Wire color</th><th>ECU pin</th><th>Adapter pin</th><th>Function</th>")
                .append("</tr></thead><tbody>\n");
        for (PatchCordHelper.Step step : steps) {
            for (PatchCordHelper.Wire wire : step.wires) {
                String id = ids.get(wire);
                String search = wire.ecu.label() + " " + wire.adapter.label() + " "
                        + value(wire.ecu.pin.function) + " " + value(wire.color()) + " " + step.title;
                html.append("<tr class=\"wire-row\" tabindex=\"0\" data-wire=\"").append(id)
                        .append("\" data-step=\"").append(step.number).append("\" data-search=\"")
                        .append(esc(search.toLowerCase(Locale.ROOT))).append("\">")
                        .append("<td>").append(step.number).append("</td><td><span class=\"swatch\" style=\"background:")
                        .append(colorCss(wire.color())).append("\"></span>").append(esc(value(wire.color())))
                        .append("</td><td>").append(esc(wire.ecu.label())).append("</td><td>")
                        .append(esc(wire.adapter.label())).append("</td><td>")
                        .append(esc(value(wire.ecu.pin.function))).append("</td></tr>\n");
            }
        }
        html.append("</tbody></table></div></section>\n");
        renderUnmatched(html, "ECU", unmatchedEcu);
        renderUnmatched(html, "Adapter", unmatchedAdapter);
        renderOverview(html, steps, allEcuWires, allAdapterWires, ids, stepNumbers);
        html.append("<footer>Generated from connector YAML by PatchCordHelper. Review every matched circuit before wiring. ")
                .append("The <a href=\"https://github.com/chuckwagoncomputing/interactive-pinout\">interactive-pinout</a> ")
                .append("project established the connector image and pin selection pattern.</footer></main>\n")
                .append("<script>\n").append(resource("patchcord.js")).append("\n</script></body></html>\n");
        Files.write(file.toPath(), html.toString().getBytes(StandardCharsets.UTF_8));
    }

    private static void renderPair(StringBuilder html, ConnectorYaml ecu, ConnectorYaml adapter,
                                   List<PatchCordHelper.Wire> pairWires,
                                   List<PatchCordHelper.Wire> allEcuWires,
                                   List<PatchCordHelper.Wire> allAdapterWires,
                                   Map<PatchCordHelper.Wire, String> ids,
                                   Map<PatchCordHelper.Wire, Integer> stepNumbers) throws IOException {
        html.append("<section class=\"connector-pair\"><h2>ECU ").append(esc(ecu.name))
                .append(" <span aria-hidden=\"true\">→</span> Adapter ")
                .append(esc(adapter.name)).append("</h2><div class=\"pair-scroll\"><div class=\"pair-layout\">")
                .append("<div class=\"pair-sides\">");
        renderConnector(html, ecu, pairWires, allEcuWires, ids, stepNumbers, true, false);
        renderConnector(html, adapter, pairWires, allAdapterWires, ids, stepNumbers, false, false);
        html.append("</div><div class=\"pair-wires\" aria-label=\"Wires from ECU ")
                .append(esc(ecu.name)).append(" to adapter ").append(esc(adapter.name)).append("\">");
        for (PatchCordHelper.Wire wire : pairWires) {
            renderWireTrace(html, wire, ids.get(wire), stepNumbers.get(wire), false);
        }
        html.append("</div></div></div></section>\n");
    }

    private static void renderOverview(StringBuilder html, List<PatchCordHelper.Step> steps,
                                       Map<ConnectorYaml, List<PatchCordHelper.Wire>> allEcuWires,
                                       Map<ConnectorYaml, List<PatchCordHelper.Wire>> allAdapterWires,
                                       Map<PatchCordHelper.Wire, String> ids,
                                       Map<PatchCordHelper.Wire, Integer> stepNumbers) throws IOException {
        html.append("<section class=\"total-overview\" id=\"total-overview\"><h2>Complete patchcord diagram</h2>")
                .append("<p>All matched pins and wires are shown here, regardless of the step or search filter above. ")
                .append("Each connector appears once on its side.</p>");
        if (allEcuWires.isEmpty()) {
            html.append("<p>No matched wires to display.</p></section>\n");
            return;
        }
        html.append("<div class=\"overview-scroll\"><div class=\"overview-layout\">")
                .append("<div class=\"overview-bank\"><h3>ECU connectors · left</h3>");
        for (Map.Entry<ConnectorYaml, List<PatchCordHelper.Wire>> entry : allEcuWires.entrySet()) {
            renderConnector(html, entry.getKey(), entry.getValue(), entry.getValue(),
                    ids, stepNumbers, true, true);
        }
        html.append("</div><div class=\"overview-routes\"><h3>All wires · left → right</h3>");
        for (PatchCordHelper.Step step : steps) {
            for (PatchCordHelper.Wire wire : step.wires) {
                renderWireTrace(html, wire, ids.get(wire), stepNumbers.get(wire), true);
            }
        }
        html.append("</div><div class=\"overview-bank\"><h3>Adapter connectors · right</h3>");
        for (Map.Entry<ConnectorYaml, List<PatchCordHelper.Wire>> entry : allAdapterWires.entrySet()) {
            renderConnector(html, entry.getKey(), entry.getValue(), entry.getValue(),
                    ids, stepNumbers, false, true);
        }
        html.append("</div></div></div></section>\n");
    }

    private static void renderWireTrace(StringBuilder html, PatchCordHelper.Wire wire,
                                        String id, int stepNumber, boolean overview) {
        String label = wire.ecu.label() + " → " + wire.adapter.label() + " · "
                + value(wire.ecu.pin.function);
        html.append("<button class=\"").append(overview ? "overview-route" : "pair-wire")
                .append("\" type=\"button\" data-wire=\"").append(id)
                .append("\" data-step=\"").append(stepNumber)
                .append("\" aria-label=\"").append(esc(label)).append("\" style=\"--wire-color:")
                .append(colorCss(wire.color())).append("\">")
                .append("<span class=\"pair-pin pair-pin-ecu\">")
                .append(esc(overview ? wire.ecu.label() : wire.ecu.pin.pin))
                .append("</span><span class=\"wire-track\" aria-hidden=\"true\"></span>")
                .append("<span class=\"pair-pin pair-pin-adapter\">")
                .append(esc(overview ? wire.adapter.label() : wire.adapter.pin.pin))
                .append("</span><span class=\"pair-function\">")
                .append(esc(value(wire.ecu.pin.function))).append("</span></button>\n");
    }

    private static void renderConnector(StringBuilder html, ConnectorYaml connector,
                                        List<PatchCordHelper.Wire> pairWires,
                                        List<PatchCordHelper.Wire> allWires,
                                        Map<PatchCordHelper.Wire, String> ids,
                                        Map<PatchCordHelper.Wire, Integer> stepNumbers,
                                        boolean ecuSide, boolean overview) throws IOException {
        html.append("<article class=\"connector-card\"><h3>").append(esc(connector.name))
                .append(" <small>").append(ecuSide ? "ECU · left" : "adapter · right")
                .append("</small></h3>");
        if (connector.imageFile == null || !connector.imageFile.isFile()) {
            html.append("<p>Connector image unavailable</p></article>");
            return;
        }
        BufferedImage image = ImageIO.read(connector.imageFile);
        if (image == null) {
            html.append("<p>Connector image could not be read</p></article>");
            return;
        }
        html.append("<div class=\"image-scroll\"><div class=\"pinout-image\" style=\"aspect-ratio:")
                .append(image.getWidth()).append(" / ").append(image.getHeight()).append("\">")
                .append("<img alt=\"").append(esc(connector.name)).append(" mating face\" src=\"")
                .append(dataUri(connector.imageFile)).append("\">");
        if (!ecuSide) {
            html.append("<span class=\"corner tl\"></span><span class=\"corner tr\"></span>")
                    .append("<span class=\"corner bl\"></span><span class=\"corner br\"></span>");
        }
        for (PatchCordHelper.Wire wire : allWires) {
            if (pairWires.contains(wire)) {
                continue;
            }
            PatchCordHelper.PinRef pin = ecuSide ? wire.ecu : wire.adapter;
            ConnectorYaml.Coord coord = connector.coords.get(pin.pin.pin);
            if (coord == null) {
                continue;
            }
            html.append("<span class=\"context-marker\" aria-hidden=\"true\" style=\"left:")
                    .append(String.format(Locale.ROOT, "%.4f", 100.0 * coord.x / image.getWidth()))
                    .append("%;top:")
                    .append(String.format(Locale.ROOT, "%.4f", 100.0 * coord.y / image.getHeight()))
                    .append("%;background:").append(colorCss(wire.color())).append("\"></span>");
        }
        for (PatchCordHelper.Wire wire : pairWires) {
            PatchCordHelper.PinRef pin = ecuSide ? wire.ecu : wire.adapter;
            ConnectorYaml.Coord coord = connector.coords.get(pin.pin.pin);
            if (coord == null) {
                continue;
            }
            String title = wire.ecu.label() + " → " + wire.adapter.label()
                    + " · " + value(wire.ecu.pin.function);
            html.append("<button class=\"pin-marker\" type=\"button\" data-wire=\"")
                    .append(ids.get(wire)).append("\" data-step=\"").append(stepNumbers.get(wire))
                    .append(overview ? "\" data-overview=\"true" : "")
                    .append("\" title=\"").append(esc(title)).append("\" aria-label=\"")
                    .append(esc(title)).append("\" style=\"left:")
                    .append(String.format(Locale.ROOT, "%.4f", 100.0 * coord.x / image.getWidth()))
                    .append("%;top:")
                    .append(String.format(Locale.ROOT, "%.4f", 100.0 * coord.y / image.getHeight()))
                    .append("%;background:")
                    .append(colorCss(wire.color())).append("\">")
                    .append(esc(pin.pin.pin)).append("</button>");
        }
        html.append("</div></div></article>\n");
    }

    private static void renderUnmatched(StringBuilder html, String side,
                                        List<PatchCordHelper.PinRef> unmatched) {
        if (unmatched.isEmpty()) {
            return;
        }
        html.append("<details class=\"unmatched\"><summary>").append(side)
                .append(" pins without a matched function (").append(unmatched.size()).append(")</summary>")
                .append("<p>Review these pins manually against the schematics.</p><ul>");
        for (PatchCordHelper.PinRef ref : unmatched) {
            html.append("<li><strong>").append(esc(ref.label())).append("</strong> — ")
                    .append(esc(value(ref.pin.function))).append("</li>");
        }
        html.append("</ul></details>\n");
    }

    private static String dataUri(File image) throws IOException {
        String name = image.getName().toLowerCase(Locale.ROOT);
        String mime = name.endsWith(".jpg") || name.endsWith(".jpeg") ? "image/jpeg" : "image/png";
        return "data:" + mime + ";base64," + Base64.getEncoder().encodeToString(Files.readAllBytes(image.toPath()));
    }

    private static String colorCss(String wireColor) {
        Color[] colors = WireColors.resolve(wireColor);
        if (colors.length == 1) {
            return hex(colors[0]);
        }
        StringBuilder css = new StringBuilder("linear-gradient(135deg");
        for (int i = 0; i < colors.length; i++) {
            double start = i * 100.0 / colors.length;
            double end = (i + 1) * 100.0 / colors.length;
            css.append(", ").append(hex(colors[i])).append(' ')
                    .append(String.format(Locale.ROOT, "%.1f", start)).append("%, ")
                    .append(hex(colors[i])).append(' ')
                    .append(String.format(Locale.ROOT, "%.1f", end)).append('%');
        }
        return css.append(')').toString();
    }

    private static String hex(Color color) {
        return String.format(Locale.ROOT, "#%02x%02x%02x", color.getRed(), color.getGreen(), color.getBlue());
    }

    private static String value(String text) {
        return text == null ? "" : text;
    }

    private static String esc(String text) {
        return value(text).replace("&", "&amp;").replace("<", "&lt;")
                .replace(">", "&gt;").replace("\"", "&quot;").replace("'", "&#39;");
    }

    private static String resource(String name) throws IOException {
        try (InputStream input = InteractivePatchcordHtml.class.getResourceAsStream(name)) {
            if (input == null) {
                throw new IOException("Missing pinout-helper resource " + name);
            }
            byte[] bytes = new byte[8192];
            java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
            int count;
            while ((count = input.read(bytes)) != -1) {
                out.write(bytes, 0, count);
            }
            return new String(out.toByteArray(), StandardCharsets.UTF_8);
        }
    }
}
