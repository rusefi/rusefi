if uname | grep "NT"; then
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.exe
else
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.bin
fi
chmod u+x $HEX2DFU

CONTROL_SUM_OPTION="-C 0x1C"

mkdir -p deliver
rm -f deliver/*
