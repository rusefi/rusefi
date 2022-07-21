
Source code here is different from but is heavily inspired by MC33816 example as conveniently comes with MC33816 Dev Studio.

## Workflow:

### PT2001 Studio: Compiling/Generating C/header Files

1. Open "PT2001 Developer Studio", and open project `firmware/hw_layer/mc33816/rusefi/project.xml`.
2. Press the "Build" button at the right/center of the window to assemble the microcode.
3. Use `Tools` -> `Generate PT2001 Load Data Code` to generate C/header files used by rusEFI to program the PT2001 over SPI at boot (see `mc33816/rusefi/sample_code/`).

### PSC Simulator: Simulating microcode/config changes

1. Follow "PT2001 Studio" steps to build the project first.
2. Open "PSC Simulator", add the project if not added yet (`Project` -> `Add Existing`), and open it from the tree view.
3. Press `Compile All` in the toolbar.
4. In the top bar, push `Run`, and type in the desired simulation length. `4 ms` (with a space) is a good starting point for the existing stimulus.
5. Press OK on the `Auto Load` dialog box that comes up (all boxes checked).
6. Wait for the simulation to run. There's an indication in the bottom left corner of the main window about simulation progress.
7. Inspect the simulation results!

Just in case we have a backup of tools at https://github.com/rusefi/rusefi_external_utils/tree/master/NXP-GDI
