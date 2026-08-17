# TODO

## 2026-08-17

### TS burn CRC timeout (m74_9, TS-over-CAN)

Каждая загрузка тюна в логе ECU даёт:

```
TS -> Burn, waiting for CRC
TS burn CRC timeout        (~2s спустя)
Finishing pending TS burn
TS -> Burn, we are allowed to burn
```

Консоль не успевает прислать CRC-проверку (команду проверки страницы) в течение
~2-секундного окна прошивки (`settingsBurnTimer` в tunerstudio.cpp), поэтому burn
проходит без верификации. Burn сам при этом успешен (MFS write OK), так что это
не блокер, но верификация фактически не работает.

Разобраться:
- почему консоль отправляет CRC-check позже 2 секунд (порядок команд после
  `TS_BURN_COMMAND` в java-стороне, `BinaryProtocol.burn` / `getCrcFromController`);
- либо увеличить firmware-таймаут, либо ускорить отправку проверки.
