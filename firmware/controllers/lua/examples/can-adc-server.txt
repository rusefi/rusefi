txPayload = {}
function onTick()
  auxV = getAuxAnalog(0)
  print('Hello analog ' .. auxV )
  -- first byte: integer part, would be autoboxed to int
  txPayload[1] = auxV
  -- second byte: fractional part, would be autoboxed to int, overflow would be ignored
  txPayload[2] = auxV * 256;
  auxV = getAuxAnalog(1)
  print('Hello analog ' .. auxV )
  txPayload[3] = auxV
  txPayload[4] = auxV * 256;
  auxV = getAuxAnalog(2)
  print('Hello analog ' .. auxV )
  txPayload[5] = auxV
  txPayload[6] = auxV * 256;
  txCan(1, 0x600, 1, txPayload)
end