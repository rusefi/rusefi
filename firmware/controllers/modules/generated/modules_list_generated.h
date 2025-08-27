// TODO: generate me with a util! (read MODULES_LIST var)
TachometerModule,
FuelPumpController,
GearDetector,
TripOdometer,
FanControl1,FanControl2,
MapAveragingModule,
ExampleModule,

#if EFI_ETHERNET
EthernetConsoleModule,
#endif

#ifdef MODULE_VVL_CONTROLLER
VvlController,
#endif