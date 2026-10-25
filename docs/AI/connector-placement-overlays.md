# Connector placement overlays for pinout images

A placement overlay gives a connector's front-view pinout the context of its position among neighboring connectors. Add a small schematic in unused image space, label each connector, and highlight the connector or joined housing represented by the large drawing. Keep the schematic's viewpoint and orientation consistent with the drawing and a physical connector-face photo. Show separate housings with visible gaps and joined openings within one housing.

In the D/A/B/C and A/B/C/D previews below, A and C are 34-pin sockets and
are wider than the 26-pin B and D sockets. Keep the relative opening widths
consistent across all three annotated images for each board, including the
shared A/B housing.

## Reusable method

1. Establish the left-to-right order, housing boundaries, and viewing orientation from a physical photo. Use the connector names in the pinout YAML; do not infer order from file names or mirror a harness-side view.
2. Keep the original front-view drawing as a separate file. Create a sibling annotated image and place the schematic in an existing blank margin when it fits. Preserve the original canvas width and height and leave every terminal and its surrounding drawing at the same pixels. The schematic may name connectors; leave individual pin numbers to the interactive pinout.
3. Point the YAML's `info.image.file` at the annotated image. Existing `info.pins` coordinates remain valid only when the canvas dimensions and terminal positions are unchanged. If the schematic needs more canvas space or the drawing moves or scales, remeasure every terminal center and update the YAML coordinates before switching images. When reusing an existing technical drawing for a different board, match the complete physical pin-name sets and transfer coordinates by pin name; the board's photo coordinates do not apply to the drawing.
4. Validate the annotated image against the source: compare dimensions and the entire connector region pixel for pixel, count physical terminals, and check that every `info.pins` coordinate is in bounds and lands on its terminal. Preview the pinout when available. An image-path-only change does not need a firmware build.

The unannotated drawing and pin-order workflow is in [Connector technical drawings](../imagegen.md).

## Super-uaEFI example

Viewed directly into the ECU mating face with the indexing tabs up, the openings run **D, A, B, C** from left to right. A and B share the center 60-pin housing. The [connector photo](../../firmware/config/boards/hellen/super-uaefi/connectors/photo_2026-10-25_13-48-08.jpg) and [connector readme](../../firmware/config/boards/hellen/super-uaefi/connectors/readme-connector.md) establish this orientation.

| Pinout YAML | Original drawing | Annotated image used by YAML | Canvas | Highlight |
| --- | --- | --- | --- | --- |
| `26pin_4k.yaml` | `26pins-4k-front.png` | `superuaefi-26pins-4k-front.png` | 1097 × 1024 | D |
| `60pin.yaml` | `60pins-front.png` | `superuaefi-60pins-front.png` | 1973 × 1024 | A and B |
| `C-34pins-4k.yaml` | `34pins-4k-front.png` | `superuaefi-34pins-4k-front.png` | 1163 × 1024 | C |

All files in the table are in `firmware/config/boards/hellen/super-uaefi/connectors/`. Each annotated image carries the same D–A/B–C schematic in its top margin. Orange identifies the connector represented by the large drawing. The three housings are visually separate, with A and B joined in the center.

The schematic occupies pixel rows 10–114. From row 115 through the bottom edge, each annotated image is pixel-identical to its original drawing. The YAML terminal-center rows therefore stay at Y = 389, 479, 570 and 660; only `info.image.file` changes. The terminal counts are 26 for D, 60 for A/B, and 34 for C.

## AlphaX 8-channel example

The [AlphaX connector photo](../../firmware/config/boards/hellen/alphax-8chan/connectors/photo_2026-10-25_14-13-48.jpg) and [connector readme](../../firmware/config/boards/hellen/alphax-8chan/connectors/readme-connector.md) show **A, B, C, D** from left to right, with A/B joined on the left and C and D separate to its right. Its 60-pin A/B, 34-pin C and 26-pin D sockets use the same physical layouts as the unannotated technical drawings in `firmware/config/boards/hellen/super-uaefi/connectors/`.

| AlphaX pinout YAML | Technical drawing source | Annotated image in the AlphaX connector folder | Canvas | Highlight |
| --- | --- | --- | --- | --- |
| `8chan-A-B.yaml` | `60pins-front.png` | `alphax-8chan-60pins-front.png` | 1973 × 1024 | A and B |
| `8chan-C.yaml` | `34pins-4k-front.png` | `alphax-8chan-34pins-4k-front.png` | 1163 × 1024 | C |
| `8chan-D.yaml` | `26pins-4k-front.png` | `alphax-8chan-26pins-4k-front.png` | 1097 × 1024 | D |

The AlphaX annotations occupy rows 10–114; all lower pixels match their technical source drawings. The AlphaX YAML pin names match the corresponding technical drawing YAML pin names exactly (60, 34 and 26 positions). Their `info.pins` coordinates use that drawing's measured terminal centers, including the shared Y rows 389, 479, 570 and 660.
