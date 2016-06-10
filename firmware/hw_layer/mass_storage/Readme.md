https://github.com/tegesoft/Chibios-USB-Devices/
http://www.chibios.com/forum/viewtopic.php?f=3&t=619

Mass Storage Device
===================

This driver implements a USB mass storage device. It requires a Chibios block device (e.g mmc_spi or SDC)

Example usage:
--------------
```c

USBMassStorageDriver UMSD1;

mmcObjectInit(&MMCD1);
mmcStart(&MMCD1, &mmccfg);
mmcConnect(&MMCD1);

msdInit(&USBD1, &MMCD1, &UMSD1);
```

Events:
--------------
```c
chEvtRegisterMask(&UMSD1.evt_connected, &listener_connected, 1);
chEvtRegisterMask(&UMSD1.evt_ejected, &listener_ejected, 2);

while(TRUE) {
  if(chEvtWaitOneTimeout(1, TIME_IMMEDIATE)) {
    /* drive is now connected */
      
    /* wait until the drive is ejected */
    chEvtWaitOne(2);
      
    /* drive is now ejected. do something */
  }

  chThdSleepMilliseconds(1000);
}
```