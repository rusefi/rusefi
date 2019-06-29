
# Frankenso
The most interesting folder here is [Frankenso](frankenso/) - that's rusEfi most useful board

# Connectors
Some vehicle-specific connector boards are also pretty nice if you want to keep original harness intact

See [Connectors wiki page](http://rusefi.com/wiki/index.php?title=Hardware:OEM_connectors)
todo: move Connectors wiki page content into git

* [35 pin](MX7-C-35PL-C11-connector) Mazda TCU
* [38 pin](NGC_38_connector) Chrysler NGC connector
* [52 pin](172319-1_52pin-connector) Mitsubishi Galant VR4
* [55 pin](963063-15_55_pin_connector) BMW Lada
* [64 pin](176122-6-connector) Mazda Miata NA NB1, Honda Civic
* [72 pin](1123038-2-connector) Mazda Miata NB2 
* [121 pin](368255-2-connector) Audi
* [154 pin](284617-1-connector) Audi

# Modules
* [Brain board 48pin](mini48-stm32)
* [Brain board 100pin](brain_board)
* [Brain board 176pin](brain_board_176-pin)
* [CAN module](can_board) CAN using SN65HVD230
* [VR module](VR_Board) Variable Reluctance conditioner board VR MAX9926
* [Wideband module](CJ125_board) CJ125 Bosch Wideband board
* [VND5E025AKTR](PowerSSO-24_breakout) double channel high-side driver with analog current sense (used on Dodge Neon 2003 test mule)
* [BTS21400](DDPAK_breakout) ignition coil IGBT driver (used on Dodge Neon 2003 test mule)
* [TLE6240 module](low_side_tle6240) integrated low side switch (fabricated, not tested)
* [TLE6244 module](Breakout_TLE6244) integrated low side switch (never actually fabricated)
* [TLE8810ED module](TLE8110ED_breakout_board) integrated low side switch (fabricated, not tested)
* [MC33816](Common_Rail_MC33816) common rail driver (never actually fabricated)
* [LM1949](Low-Z_LM1949) low impedance injector driver (never actually fabricated)

New modules are being added to https://github.com/rusefi/hw_modular

Some stuff we have given up on completely is located at https://github.com/rusefi/hw_legacy

Above mentioned modules are known to work unless stated otherwise

### Frankenstein

[Our older ECU board](frankenstein)

# More info

![IBOM](../misc/www/images/ibom_icon.png) [Interactive Html Boms](https://rusefi.com/docs/ibom/)

See [Hardware status forum thread](http://rusefi.com/forum/viewtopic.php?f=4&t=260)
todo: move hardware status content into git

# Important note
rusEfi uses KICAD 4 only.
We are unhappy with how each major version of KiCAD
seems to break backwords compatibility in a major way and we plan to skip KiCAD version 5
and evaluate KICAD version 6 once it's available.


# Important note

Depends on libraries from https://github.com/rusefi/kicad-libraries

If you download complete repo do not forget to download libraries. If you git clone be sure to also

`git submodule update --init`


`git submodule update --init`


