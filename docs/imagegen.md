# Connector technical drawings with imagegen

Use this workflow to turn connector photographs into matching technical front
views and update the interactive pinout coordinates. The uaEFI connector files
are in `firmware/config/boards/hellen/uaefi/connectors/`.

The approved style reference is
[uaefi-A-front.png](../firmware/config/boards/hellen/uaefi/connectors/uaefi-A-front.png).
Use each connector's original JPG for geometry and orientation, and A's drawing
for style. Keep the original photographs.

## Drawing requirements

- Orthographic view straight into the mating face, latch at the top.
- White background, crisp black outlines, light-gray housing, white cavities,
  and one small gray square terminal per cavity.
- Preserve the photographed cavity keying, pin count, rows, columns and latch.
  Keep terminal rows horizontal and paired terminals vertically aligned.
- No perspective, environmental background, shadows, texture or gradients.
- No baked-in labels, numbers, dimension lines, arrows, logos or title.
  The interactive pinout supplies its own labels.
- Give wider connectors more image width, keeping cavity and terminal sizes
  comparable across the family.

## Shared coordinates

Every final image has a **1024-pixel canvas height**. All five connectors use
these exact YAML row coordinates:

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

## Cavity keying

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
6. Save the result beside the original as `uaefi-<letter>-front.png`.
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

Use the mean of the two measured terminal-center X values in each column,
rounded to an integer, for both pins in that column. Write the fixed row Y
values to the YAML. The exported terminal centers should be within 2 pixels
of the coordinates; if a generated row has larger drift, correct the artwork
before accepting it.

## Validation

- Parse each YAML and load the PNG named by `info.image.file`.
- Verify every physical pin occurs exactly once in `info.pins`: A=8, B=18,
  C=20, D=16, E=6. Non-connector items such as On-board MAP are electrical
  definitions, not extra terminals to draw.
- Verify image height is 1024, every coordinate is in bounds, both YAML row
  values are exactly 389/660, and upper/lower pins share column X values.
- Check the numbering against the row-order table and the keying against the
  photos, with latch up and no mirroring.
- Measure all terminal centers and confirm alignment. For these drawings,
  connected components of grayscale pixels below 70 isolate the terminal
  outlines; retain near-square components about 40-85 pixels across. Confirm
  the component count before trusting the measurement.
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
