# Connector technical drawings with imagegen

Use this workflow to turn connector photographs into matching technical front
views and update the interactive pinout coordinates. The uaEFI connector files
are in `firmware/config/boards/hellen/uaefi/connectors/`; the four-row super-uaEFI
family is in `firmware/config/boards/hellen/super-uaefi/connectors/`.

The approved style reference is
[uaefi-A-front.png](../firmware/config/boards/hellen/uaefi/connectors/uaefi-A-front.png).
Use each connector's original JPG for geometry and orientation, and A's drawing
for style. Keep the original photographs.

## Drawing requirements

- Orthographic view straight into the mating face, latch at the top.
- White background, crisp black outlines and light-gray housing. Preserve the
  terminal shape: uaEFI has white keyed cavities with gray square terminals;
  super-uaEFI has round terminal faces in a common recessed front plate.
- Preserve the photographed cavity keying, pin count, rows, columns and latch.
  Keep terminal rows horizontal and paired terminals vertically aligned.
- No perspective, environmental background, shadows, texture or gradients.
- No baked-in labels, numbers, dimension lines, arrows, logos or title.
  The interactive pinout supplies its own labels.
- Give wider connectors more image width, keeping cavity and terminal sizes
  comparable across the family.

The unannotated front-view drawings follow the no-label rule above. Placement
variants may add a labeled connector schematic in unused image space while
preserving the terminal positions. See [Connector placement overlays](AI/connector-placement-overlays.md)
for the reusable method and a super-uaEFI example.

## Shared coordinates

Every final image has a **1024-pixel canvas height**. The five two-row uaEFI
connectors use these exact YAML row coordinates:

| Row | Y coordinate |
| --- | ---: |
| Top | 389 |
| Bottom | 660 |

These are terminal-center coordinates, not the top and bottom edges of the
housing. Coordinates refer to the image's natural pixel size. Use the same
X coordinate for the upper and lower terminal in each column. Width varies
with the number of columns; do not squeeze every connector into A's width.

| Connector | Pins | Columns | Top row, left to right | Bottom row, left to right | Final PNG size | YAML |
| --- | ---: | ---: | --- | --- | --- | --- |
| A | 8 | 4 | A8 through A5 | A4 through A1 | 1536x1024 | uaefi-A.yaml |
| B | 18 | 9 | B18 through B10 | B9 through B1 | 2914x1024 | B.yaml |
| C | 20 | 10 | C20 through C11 | C10 through C1 | 3216x1024 | C.yaml |
| D | 16 | 8 | D16 through D9 | D8 through D1 | 2671x1024 | D.yaml |
| E | 6 | 3 | E6 through E4 | E3 through E1 | 1127x1024 | E.yaml |

The existing YAML determines pin order. Do not mirror the drawing or infer
numbering from the order of the electrical `pins:` entries.

### Four-row super-uaEFI connectors

Preserve the rounded socket rims, rectangular indexing tabs and ribs, circular
terminals, and staggered rows shown in these photos. The middle two rows are
offset by half a column pitch. Do not copy uaEFI's square cavities or two-row
layout into this family.

All three images use four common row coordinates, retaining the same outer
anchors as the two-row family:

| Row, top to bottom | Y coordinate |
| --- | ---: |
| 1 | 389 |
| 2 | 479 |
| 3 | 570 |
| 4 | 660 |

Numbering increases left to right within each row. A and C are 34-pin sockets;
B and D are 26-pin sockets. A is the left socket and B the right socket in the
combined 60-pin housing. Preserve the number-before-letter names in this
family, such as `1A`, rather than converting them to uaEFI's `A1` convention.

| Socket | Row 1 | Row 2 | Row 3 | Row 4 |
| --- | --- | --- | --- | --- |
| A | 1A-9A | 10A-17A | 18A-25A | 26A-34A |
| B | 1B-7B | 8B-13B | 14B-19B | 20B-26B |
| C | 1C-9C | 10C-17C | 18C-25C | 26C-34C |
| D | 1D-7D | 8D-13D | 14D-19D | 20D-26D |

| Drawing | Physical terminals | Final PNG size | YAML |
| --- | ---: | --- | --- |
| 60pins-front.png | 60 (34 A + 26 B) | 1973x1024 | 60pin.yaml |
| 34pins-4k-front.png | 34 C | 1163x1024 | C-34pins-4k.yaml |
| 26pins-4k-front.png | 26 D | 1097x1024 | 26pin_4k.yaml |

The combined 60-pin housing has two internal locating keys along the top of
each socket and one centered key along the bottom of each socket. The
`60pins.jpg` connector-face photo shows this arrangement; preserve it in the
base drawing and both board-specific annotated versions. These keys sit
outside the terminal circles, so correcting them does not change `info.pins`
coordinates.

Pair X coordinates between rows 1/4 and between rows 2/3. Do not align the
middle rows to the outer rows: that would remove the physical stagger.
Draw every physical position listed under `info.pins`, including positions
without an electrical assignment; do not invent electrical definitions.

## Cavity keying (two-row uaEFI)

Read each pattern left to right. `S` means square; `C` means both lower corners
are chamfered, with square upper corners. These patterns were read from the
original uaEFI photos; another connector family needs its own inspection.

| Connector | Top row | Bottom row |
| --- | --- | --- |
| A | C S S C | S C C S |
| B | C C S S C C S S C | S S C C S S C C S |
| C | S C C S S C C S S C | C S S C C S S C C S |
| D | C S S C C S S C | S C C S S C C S |
| E | S S C | C C S |

## Workflow

1. Inspect the photograph, YAML and approved style reference before generating.
   Record the pin count, row order and cavity keying.
2. Use the built-in imagegen tool with two explicit references: the connector
   photo for geometry, and A's drawing for style. Generate one connector per
   call. Set `transparent_background: false`.
3. State the exact number of rows, columns and terminals, spell out the keying
   sequence, and request the shared row positions. Describe pin order for
   orientation but explicitly forbid rendering the labels.
4. Inspect the generated result. Count the cavities and terminals and compare
   every keyed cavity with the photograph. Regenerate if geometry is wrong.
5. Align the export to the shared coordinates as described below. Requesting
   pixel positions in the prompt alone does not guarantee them: the tool can
   return a different canvas size and slightly different placement.
6. Save the result beside the original as `<original-stem>-front.png`, for
   example `uaefi-A-front.png` or `60pins-front.png`.
   Update only `info.image.file`, `info.pins` coordinates and explanatory
   comments in the corresponding YAML. Preserve all electrical definitions,
   metadata, functions, colors and original photos.
7. Run the validation below, inspect the final PNGs, and append the work and
   validation results to `docs/report.md`.

## Aligning the exported images

Measure the terminal outlines in the generated image. Use the median center
Y of the top row (`t`) and bottom row (`b`) to calculate a uniform scale:

```text
scale = (660 - 389) / (b - t)
new_width = round(original_width * scale)
new_height = round(original_height * scale)
```

Resize both axes proportionally, then translate vertically to put the upper
row at 389. With a pixel-center convention, an initial integer translation is:

```text
shift_y = round(389 - ((t + 0.5) * new_height / original_height - 0.5))
```

Crop or pad only the surrounding white margin to make the final canvas height
1024. Keep the entire housing and latch visible. Remeasure the exported pixels;
resampling and integer rounding can move a measured outline by a pixel.

For B-E, imagegen produced the artwork and Sharp 0.34.5 performed this export
step with Lanczos3 resizing and white padding. It did not reconstruct the
connector geometry. The temporary export tooling was outside the repository;
no new project runtime dependency is required.

For four-row connectors, also measure the two middle rows. Refine the uniform
scale and vertical translation against all four target row positions, then
remeasure the final PNG. Keep every row within the same 2-pixel tolerance;
do not hide irregular spacing by assigning different Y values per connector.
The 26-pin drawing needed a targeted imagegen correction before export to
bring the row spacing into tolerance.

Use the mean of the two measured terminal-center X values in each column,
rounded to an integer, for both pins in that column. Write the fixed row Y
values to the YAML. The exported terminal centers should be within 2 pixels
of the coordinates; if a generated row has larger drift, correct the artwork
before accepting it.

## Validation

- Parse each YAML and load the PNG named by `info.image.file`.
- Verify every physical pin occurs exactly once in `info.pins`: uaEFI has
  A=8, B=18, C=20, D=16, E=6; super-uaEFI has A/B=60, C=34, D=26.
  Non-connector items such as On-board MAP are electrical definitions, not
  extra terminals to draw.
- Verify image height is 1024, every coordinate is in bounds, YAML row
  values match the family table, and aligned rows share column X values.
  For super-uaEFI, also verify each middle-row pin lies between the adjacent
  outer-row columns and A/B remain in their original left/right order.
- Check the numbering against the row-order table and the keying against the
  photos, with latch up and no mirroring.
- Measure all terminal centers and confirm alignment. For these drawings,
  connected components of grayscale pixels below 70 isolate the terminal
  outlines; retain near-square components about 40-85 pixels across. Confirm
  the component count before trusting the measurement. For super-uaEFI's
  circular outlines, use bounding boxes about 35-75 pixels across and check
  the four row counts (9/8/8/9 or 7/6/6/7) separately for each socket.
- Compare parsed electrical definitions and unrelated `info` fields against
  their previous versions. Verify original JPGs are unchanged.
- Run `git diff --check` and review the PNGs at their final resolution.
  Image and coordinate changes do not require a firmware build.

## Prompts used

The original connector A prompt is recorded in the
[connector A session report](report.md#2026-10-17---uaefi-connector-a-technical-front-view).
The following are the built-in imagegen prompts used for B-E before export
alignment. Requested dimensions and X coordinates are generation guidance;
the final PNGs and YAML contain the measured, aligned coordinates.

<details>
<summary>Connector B</summary>

```text
Use case: style-transfer.
Create one technical orthographic front/mating-face drawing of uaEFI connector B.
Reference image 1 is the photograph of connector B: use it for its 18-terminal layout, keying and orientation. Reference image 2 is the approved drawing of connector A: match its style and vertical layout.
Output canvas ideally 3072 pixels wide by 1024 pixels tall. This is another member of the SAME connector family. Preserve A's vertical geometry: top edge of housing y=228, bottom edge y=836, top latch y=148..228, upper-row terminal centers y=389, lower-row terminal centers y=660. Absolutely horizontal rows. 9 columns of terminals at x=312,618,924,1230,1536,1842,2148,2454,2760. Each terminal is one small 60x60 gray square with a black outline, exactly centered in its cavity. Cavities have clean black outlines and white interiors, about 237 pixels high.
Exactly TWO rows of 9 cavities each, exactly 18 terminals. Counting columns left-to-right, the top row keying is: chamfered, chamfered, square, square, chamfered, chamfered, square, square, chamfered. Bottom row: square, square, chamfered, chamfered, square, square, chamfered, chamfered, square. Chamfered means BOTH lower corners cut diagonally; all upper corners remain square. Square means all four corners square. Follow this sequence exactly and do not add any holes or pins.
One light-gray rectangular housing enclosing all 18 cavities; single centered trapezoidal latch projecting upward. Match A's black outlines and sparse side indexing tabs. White background, no perspective, no shadows, no texture, no gradient. Preserve cavity proportions similar to the photo and A's style reference; adjust the width for the number of columns. Generous white margins, complete housing visible.
Orientation: top row left-to-right B18,B17,B16,B15,B14,B13,B12,B11,B10; bottom B9,B8,B7,B6,B5,B4,B3,B2,B1. These labels are interpretation only: DO NOT DRAW ANY TEXT or numbers. No labels, arrows, dimensions, title, logos or watermark. Each terminal row must be exactly horizontal and uniform so it can be registered to common pinout coordinates y=389 and y=660.
```

</details>

<details>
<summary>Connector C</summary>

```text
Use case: style-transfer.
Create one technical orthographic front/mating-face drawing of uaEFI connector C.
Reference image 1 is the photograph of connector C: use it for its 20-terminal layout, keying and orientation. Reference image 2 is the approved drawing of connector A: match its style and vertical layout.
Output canvas ideally 3072 pixels wide by 1024 pixels tall. This is another member of the SAME connector family. Preserve A's vertical geometry: top edge of housing y=228, bottom edge y=836, top latch y=148..228, upper-row terminal centers y=389, lower-row terminal centers y=660. Absolutely horizontal rows. 10 columns of terminals at x=316,587,858,1129,1400,1671,1942,2213,2484,2755. Each terminal is one small 60x60 gray square with a black outline, exactly centered in its cavity. Cavities have clean black outlines and white interiors, about 237 pixels high.
Exactly TWO rows of 10 cavities each, exactly 20 terminals. Counting columns left-to-right, the top row keying is: square, chamfered, chamfered, square, square, chamfered, chamfered, square, square, chamfered. Bottom row: chamfered, square, square, chamfered, chamfered, square, square, chamfered, chamfered, square. Chamfered means BOTH lower corners cut diagonally; all upper corners remain square. Square means all four corners square. Follow this sequence exactly and do not add any holes or pins.
One light-gray rectangular housing enclosing all 20 cavities; single centered trapezoidal latch projecting upward. Match A's black outlines and sparse side indexing tabs. White background, no perspective, no shadows, no texture, no gradient. Preserve cavity proportions similar to the photo and A's style reference; adjust the width for the number of columns. Generous white margins, complete housing visible.
Orientation: top row left-to-right C20,C19,C18,C17,C16,C15,C14,C13,C12,C11; bottom C10,C9,C8,C7,C6,C5,C4,C3,C2,C1. These labels are interpretation only: DO NOT DRAW ANY TEXT or numbers. No labels, arrows, dimensions, title, logos or watermark. Each terminal row must be exactly horizontal and uniform so it can be registered to common pinout coordinates y=389 and y=660.
```

</details>

<details>
<summary>Connector D</summary>

```text
Use case: style-transfer.
Create one technical orthographic front/mating-face drawing of uaEFI connector D.
Reference image 1 is the photograph of connector D: use it for its 16-terminal layout, keying and orientation. Reference image 2 is the approved technical drawing of connector A: match its style and EXACT vertical layout.
Output canvas: 2768 pixels wide by 1024 pixels tall. This is a wider member of the SAME connector family, not an enlarged image. Preserve A's vertical geometry: top edge of housing y=228, bottom edge y=836, top latch y=148..228, upper-row terminal centers y=389, lower-row terminal centers y=660. Absolutely horizontal rows. Eight columns of terminals at x=313,619,925,1231,1537,1843,2149,2455. Each terminal is a 60x60 pixel small gray square with a black outline. Each cavity approximately 265 pixels wide by 237 pixels high, with clean black outline and white interior.
Exactly TWO rows of EIGHT cavities each, exactly SIXTEEN terminals. Counting columns left-to-right 1..8, the top row keying is: chamfered, square, square, chamfered, chamfered, square, square, chamfered. Bottom row: square, chamfered, chamfered, square, square, chamfered, chamfered, square. Chamfered means BOTH lower corners cut diagonally; all upper corners remain square. Square means all four corners square.
One wide light-gray rectangular housing enclosing all sixteen cavities; single centered trapezoidal latch projecting upward. Match A's black outlines and sparse side indexing tabs. White background, no perspective, no shadows, no texture, no gradient.
Orientation: top row left-to-right D16,D15,D14,D13,D12,D11,D10,D9; bottom D8,D7,D6,D5,D4,D3,D2,D1. These labels are interpretation only: DO NOT DRAW ANY TEXT or numbers. No labels, arrows, dimensions, title, logos or watermark. Keep the cavities square-like as in the photo; extend the width, do not compress eight columns into A's four-column width. All terminals on each row must align EXACTLY with A at y=389 and y=660 on the final 1024-high canvas.
```

</details>

<details>
<summary>Connector E</summary>

```text
Use case: style-transfer.
Create one technical orthographic front/mating-face drawing of uaEFI connector E.
Reference image 1 is the photograph of connector E: use it for its 6-terminal layout, keying and orientation. Reference image 2 is the approved drawing of connector A: match its style and vertical layout.
Output canvas ideally 1232 pixels wide by 1024 pixels tall. This is another member of the SAME connector family. Preserve A's vertical geometry: top edge of housing y=228, bottom edge y=836, top latch y=148..228, upper-row terminal centers y=389, lower-row terminal centers y=660. Absolutely horizontal rows. 3 columns of terminals at x=310,616,922. Each terminal is one small 60x60 gray square with a black outline, exactly centered in its cavity. Cavities have clean black outlines and white interiors, about 237 pixels high.
Exactly TWO rows of 3 cavities each, exactly 6 terminals. Counting columns left-to-right, the top row keying is: square, square, chamfered. Bottom row: chamfered, chamfered, square. Chamfered means BOTH lower corners cut diagonally; all upper corners remain square. Square means all four corners square. Follow this sequence exactly and do not add any holes or pins.
One light-gray rectangular housing enclosing all 6 cavities; single centered trapezoidal latch projecting upward. Match A's black outlines and sparse side indexing tabs. White background, no perspective, no shadows, no texture, no gradient. Preserve cavity proportions similar to the photo and A's style reference; adjust the width for the number of columns. Generous white margins, complete housing visible.
Orientation: top row left-to-right E6,E5,E4; bottom E3,E2,E1. These labels are interpretation only: DO NOT DRAW ANY TEXT or numbers. No labels, arrows, dimensions, title, logos or watermark. Each terminal row must be exactly horizontal and uniform so it can be registered to common pinout coordinates y=389 and y=660.
```

</details>

## Super-uaEFI prompts used

These prompts use the built-in imagegen tool. The single-socket drawings use
their original photo for geometry and uaEFI A for style. The combined A/B
drawing uses its original photo plus the generated single-socket drawings.
The 26-pin correction is applied before the final export alignment.

<details>
<summary>26-pin D</summary>

```text
Use case: style-transfer.
Asset: technical orthographic front/mating-face illustration of the super-uaEFI 26-pin connector D, for an interactive pinout.
Reference image 1 is the actual connector photo, the authority for its physical geometry, orientation and keying. Reference image 2 is an approved drawing of a DIFFERENT connector; use it ONLY for the clean engineering line-art STYLE: white background, black outlines, light-gray housing. Do NOT borrow its square cavities, 2-row layout or trapezoidal latch.
Draw the 26-pin connector directly facing the viewer, eliminating the perspective of the photo. One rounded rectangular socket housing, thick rounded rim with an inset rim contour and light gray front plate. Preserve the photographed rectangular indexing ribs along the upper and lower inside edges and the short rectangular centered tab at the very top. Outline housing in black and use light neutral gray plastic. No surrounding board, neighboring sockets, wires, reflections, cast shadows, texture or gradients. Avoid decorative circles or manufacturing marks which could be mistaken for additional terminals.
CRITICAL exact pin geometry: FOUR horizontal rows with respectively 7, 6, 6, 7 circular metal terminal faces. TOTAL exactly 26 terminals. Each terminal is one small flat gray circular disk with a sharp black circular outline, about 28 pixels in diameter on the requested canvas. All equal size. No square cavities, no hollow holes or projecting pins in perspective. The middle two rows are shifted horizontally half a column pitch relative to the outer rows. Upper and bottom rows align vertically with each other. Middle two rows align vertically with each other.
Requested final canvas 1024 by 1024. Terminal center Y positions top-to-bottom: 389,479,570,660. Horizontal column pitch 76 pixels. Top and bottom row X coordinates: 284,360,436,512,588,664,740. Middle row X coordinates: 322,398,474,550,626,702. These coordinates are production layout guidance; count and staggering are essential. Keep both vertical margins generous, the whole rounded rim and top tab visible. Housing face spans approximately y=240..830; do not fill the canvas with an enlarged connector. The drawing should have level horizontal edges and perfectly straight rows.
Orientation follows the photo without mirroring or rotation. Pin rows left-to-right, for interpretation ONLY: 1D through 7D; 8D through 13D; 14D through 19D; 20D through 26D. DO NOT render any of this text or any numbers; labels are supplied by the pinout software. No dimensions, arrows, labels, title, logo, legend or watermark.
```

</details>

<details>
<summary>26-pin D row-spacing correction</summary>

```text
Use case: precise-object-edit.
Edit this technical connector drawing with ONE targeted correction: make the four circular-terminal rows evenly spaced vertically. Preserve the housing, rim, keys, tab, palette, white background, framing, all horizontal terminal positions and the exact counts 7,6,6,7.
The input canvas is 1254x1254. Its row centers are approximately y=471,574,680.5,779.5. Keep the first, second and fourth rows fixed. Move the entire THIRD row of six circular terminals UP by 4 pixels, to y=676.5. This should make the three row gaps equal. Keep all six terminals of that row at exactly the same height. No new terminals, no missing terminals, no shape changes, no added text or numbers. Keep the output dimensions and framing identical to the input.
```

</details>

<details>
<summary>34-pin C</summary>

```text
Use case: style-transfer.
Asset: technical orthographic front/mating-face illustration of the super-uaEFI 34-pin connector C, for an interactive pinout.
Reference image 1 is the actual connector photo, the authority for its physical geometry, orientation and keying. Reference image 2 is an approved drawing of a DIFFERENT connector; use it ONLY for the clean engineering line-art STYLE: white background, black outlines, light-gray housing. Do NOT borrow its square cavities, 2-row layout or trapezoidal latch.
Draw the 34-pin connector directly facing the viewer, eliminating the perspective of the photo. One rounded rectangular socket housing, thick rounded rim with an inset rim contour and light gray front plate. Preserve the photographed rectangular indexing ribs along the upper and lower inside edges and the short rectangular centered tab at the very top. Outline housing in black and use light neutral gray plastic. No surrounding board, neighboring sockets, wires, reflections, cast shadows, texture or gradients. Avoid decorative circles or manufacturing marks which could be mistaken for additional terminals.
CRITICAL exact pin geometry: FOUR horizontal rows with respectively 9, 8, 8, 9 circular metal terminal faces. TOTAL exactly 34 terminals. Each terminal is one small flat gray circular disk with a sharp black circular outline, about 28 pixels in diameter on the requested canvas. All equal size. No square cavities, no hollow holes or projecting pins in perspective. The middle two rows are shifted horizontally half a column pitch relative to the outer rows. Upper and bottom rows align vertically with each other. Middle two rows align vertically with each other.
Requested final canvas 1152 by 1024. Terminal center Y positions top-to-bottom: 389,479,570,660. Horizontal column pitch 76 pixels. Top and bottom row X coordinates: 272,348,424,500,576,652,728,804,880. Middle row X coordinates: 310,386,462,538,614,690,766,842. These coordinates are production layout guidance; count and staggering are essential. Keep both vertical margins generous, the whole rounded rim and top tab visible. Housing face spans approximately y=240..830; do not fill the canvas with an enlarged connector. The drawing should have level horizontal edges and perfectly straight rows.
Orientation follows the photo without mirroring or rotation. Pin rows left-to-right, for interpretation ONLY: 1C through 9C; 10C through 17C; 18C through 25C; 26C through 34C. DO NOT render any of this text or any numbers; labels are supplied by the pinout software. No dimensions, arrows, labels, title, logo, legend or watermark.
```

</details>

<details>
<summary>60-pin A/B</summary>

```text
Use case: style-transfer.
Asset: technical orthographic front/mating-face drawing of the super-uaEFI combined 60-pin A/B header.
Reference image 1 is the actual 60-pin connector photograph and determines geometry and orientation. Reference images 2 and 3 are the approved technical line drawings of related 34-pin and 26-pin connectors. Match their clean black outlines, white background, light gray plastic, gray circular terminal faces, rounded rims and indexing ribs.
Draw ONE assembly with TWO rounded rectangular sockets side by side in a common housing, separated by a vertical central divider, just as in photo 1. Left socket A is wider and contains 34 terminals. Right socket B is narrower and contains 26 terminals. Keep both sockets' upper and lower edges on the same horizontal lines. Preserve rectangular top indexing tabs and small rectangular keying ribs at the inner upper and lower edges as seen in photo 1. Do not add decorative holes, screws or circular manufacturing marks.
CRITICAL terminal count and arrangement:
LEFT socket: exactly FOUR horizontal rows, containing 9,8,8,9 small circular terminal faces, top to bottom. Outer rows have nine aligned columns; middle two rows have eight aligned columns shifted half a column pitch.
RIGHT socket: exactly FOUR horizontal rows containing 7,6,6,7 small circular terminal faces, top to bottom. Outer rows have seven aligned columns; middle two rows have six aligned columns shifted half a column pitch.
TOTAL exactly 60 circular terminal faces. Each is a flat gray disk with a sharp thin black circular outline, same diameter for all 60. Use exactly the same row height, row pitch and column pitch in both sockets. Every row of the left socket must continue in a perfect horizontal line across the right socket. Count carefully: left 34, right 26. No square cavities or visible projecting pins.
Wide landscape canvas, ideally 2304x1024 pixels. All four terminal-row centers are at y=389,479,570,660. Complete housing visible with white margin above, below and at the sides. Keep height comparable to the single-socket references; widen the canvas to accommodate both sockets. Draw straight-on orthographic engineering line art with no perspective, gradients, shadows, reflections, photo texture, board, wires or background environment.
Orientation MUST match photo 1, no mirroring or rotation. For interpretation ONLY, left socket rows read left-to-right: 1A..9A, 10A..17A, 18A..25A, 26A..34A. Right socket rows: 1B..7B, 8B..13B, 14B..19B, 20B..26B. DO NOT DRAW THESE LABELS or any text, numbers, arrows, dimensions, titles, logos or watermarks.
```

</details>
