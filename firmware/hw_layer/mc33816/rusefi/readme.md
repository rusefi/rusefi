
Source code here is different from but is heavily inspired by MC33816 example as conveniently comes with MC33816 Dev Studio.

* Hit 'Build' button in MC33816 Developer Studio to get assembly stuff in 'build' folder. At the moment we use PT2001 version 
of the chip so actually "PT2001 Developer studio"

* Once you are done wit 'Build' second step is 'Generate PT2001 Load Data Code'. See sample_code/PT2001_LoadData.c. Some of the files in this directory are included/compiled in to rusEFI, in order to download the firmware and configuration to the PT2001.

* Fun fact: it looks like the difference between "MC33816 Developer Studio" and "PT2001 Developer studio" is ``assembler\cipher\key4.key`` file

Just in case we have a backup of both tools at https://github.com/rusefi/rusefi_external_utils/tree/master/NXP-GDI