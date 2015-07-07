///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:48 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\crc.c      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\crc.c      /
//                       -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar\D /
//                       ebug\List\ -o F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\Obj\ --no_cse --no_unroll --no_inline       /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       crc.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME crc

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC calc_crc
        PUBLIC crc32
        
          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI Resource D0:64, D1:64, D2:64, D3:64, D4:64, D5:64, D6:64, D7:64
          CFI Resource D8:64, D9:64, D10:64, D11:64, D12:64, D13:64, D14:64
          CFI Resource D15:64
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI D0 Undefined
          CFI D1 Undefined
          CFI D2 Undefined
          CFI D3 Undefined
          CFI D4 Undefined
          CFI D5 Undefined
          CFI D6 Undefined
          CFI D7 Undefined
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI D14 SameValue
          CFI D15 SameValue
          CFI EndCommon cfiCommon0
        
// F:\stuff\rusefi_sourceforge\firmware\util\crc.c
//    1 /**
//    2  * @file    crc.c
//    3  * @date Sep 20, 2013
//    4  * @author Andrey Belomutskiy, (c) 2012-2015
//    5  */
//    6 
//    7 #include "crc.h"
//    8 
//    9 #define WIDTH  (8)
//   10 #define TOPBIT (1 << (WIDTH - 1))
//   11 #define POLYNOMIAL 0xD8
//   12 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function calc_crc
          CFI NoCalls
        THUMB
//   13 crc_t calc_crc(const crc_t message[], int nBytes) {
calc_crc:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R2,R0
//   14 	crc_t remainder = 0;
        MOVS     R0,#+0
//   15 
//   16 	/*
//   17 	 * Perform modulo-2 division, a byte at a time.
//   18 	 */
//   19 	for (int byte = 0; byte < nBytes; ++byte) {
        MOVS     R3,#+0
??calc_crc_0:
        CMP      R3,R1
        BGE.N    ??calc_crc_1
//   20 		/*
//   21 		 * Bring the next byte into the remainder.
//   22 		 */
//   23 		remainder ^= (message[byte] << (WIDTH - 8));
        LDRB     R4,[R3, R2]
        EORS     R0,R4,R0
//   24 
//   25 		/*
//   26 		 * Perform modulo-2 division, a bit at a time.
//   27 		 */
//   28 		for (unsigned char bit = 8; bit > 0; --bit) {
        MOVS     R4,#+8
??calc_crc_2:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??calc_crc_3
//   29 			/*
//   30 			 * Try to divide the current data bit.
//   31 			 */
//   32 			if (remainder & TOPBIT) {
        LSLS     R5,R0,#+24
        BPL.N    ??calc_crc_4
//   33 				remainder = (remainder << 1) ^ POLYNOMIAL;
        LSLS     R0,R0,#+1
        EORS     R0,R0,#0xD8
        B.N      ??calc_crc_5
//   34 			} else {
//   35 				remainder = (remainder << 1);
??calc_crc_4:
        LSLS     R0,R0,#+1
//   36 			}
//   37 		}
??calc_crc_5:
        SUBS     R4,R4,#+1
        B.N      ??calc_crc_2
//   38 	}
??calc_crc_3:
        ADDS     R3,R3,#+1
        B.N      ??calc_crc_0
//   39 
//   40 	/*
//   41 	 * The final remainder is the CRC result.
//   42 	 */
//   43 	return remainder;
??calc_crc_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   44 }
//   45 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   46 static uint32_t crc32_tab[] = { 0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535,
crc32_tab:
        DATA
        DC32 0, 1996959894, 3993919788, 2567524794, 124634137, 1886057615
        DC32 3915621685, 2657392035, 249268274, 2044508324, 3772115230
        DC32 2547177864, 162941995, 2125561021, 3887607047, 2428444049
        DC32 498536548, 1789927666, 4089016648, 2227061214, 450548861
        DC32 1843258603, 4107580753, 2211677639, 325883990, 1684777152
        DC32 4251122042, 2321926636, 335633487, 1661365465, 4195302755
        DC32 2366115317, 997073096, 1281953886, 3579855332, 2724688242
        DC32 1006888145, 1258607687, 3524101629, 2768942443, 901097722
        DC32 1119000684, 3686517206, 2898065728, 853044451, 1172266101
        DC32 3705015759, 2882616665, 651767980, 1373503546, 3369554304
        DC32 3218104598, 565507253, 1454621731, 3485111705, 3099436303
        DC32 671266974, 1594198024, 3322730930, 2970347812, 795835527
        DC32 1483230225, 3244367275, 3060149565, 1994146192, 31158534
        DC32 2563907772, 4023717930, 1907459465, 112637215, 2680153253
        DC32 3904427059, 2013776290, 251722036, 2517215374, 3775830040
        DC32 2137656763, 141376813, 2439277719, 3865271297, 1802195444
        DC32 476864866, 2238001368, 4066508878, 1812370925, 453092731
        DC32 2181625025, 4111451223, 1706088902, 314042704, 2344532202
        DC32 4240017532, 1658658271, 366619977, 2362670323, 4224994405
        DC32 1303535960, 984961486, 2747007092, 3569037538, 1256170817
        DC32 1037604311, 2765210733, 3554079995, 1131014506, 879679996
        DC32 2909243462, 3663771856, 1141124467, 855842277, 2852801631
        DC32 3708648649, 1342533948, 654459306, 3188396048, 3373015174
        DC32 1466479909, 544179635, 3110523913, 3462522015, 1591671054
        DC32 702138776, 2966460450, 3352799412, 1504918807, 783551873
        DC32 3082640443, 3233442989, 3988292384, 2596254646, 62317068
        DC32 1957810842, 3939845945, 2647816111, 81470997, 1943803523
        DC32 3814918930, 2489596804, 225274430, 2053790376, 3826175755
        DC32 2466906013, 167816743, 2097651377, 4027552580, 2265490386
        DC32 503444072, 1762050814, 4150417245, 2154129355, 426522225
        DC32 1852507879, 4275313526, 2312317920, 282753626, 1742555852
        DC32 4189708143, 2394877945, 397917763, 1622183637, 3604390888
        DC32 2714866558, 953729732, 1340076626, 3518719985, 2797360999
        DC32 1068828381, 1219638859, 3624741850, 2936675148, 906185462
        DC32 1090812512, 3747672003, 2825379669, 829329135, 1181335161
        DC32 3412177804, 3160834842, 628085408, 1382605366, 3423369109
        DC32 3138078467, 570562233, 1426400815, 3317316542, 2998733608
        DC32 733239954, 1555261956, 3268935591, 3050360625, 752459403
        DC32 1541320221, 2607071920, 3965973030, 1969922972, 40735498
        DC32 2617837225, 3943577151, 1913087877, 83908371, 2512341634
        DC32 3803740692, 2075208622, 213261112, 2463272603, 3855990285
        DC32 2094854071, 198958881, 2262029012, 4057260610, 1759359992
        DC32 534414190, 2176718541, 4139329115, 1873836001, 414664567
        DC32 2282248934, 4279200368, 1711684554, 285281116, 2405801727
        DC32 4167216745, 1634467795, 376229701, 2685067896, 3608007406
        DC32 1308918612, 956543938, 2808555105, 3495958263, 1231636301
        DC32 1047427035, 2932959818, 3654703836, 1088359270, 936918000
        DC32 2847714899, 3736837829, 1202900863, 817233897, 3183342108
        DC32 3401237130, 1404277552, 615818150, 3134207493, 3453421203
        DC32 1423857449, 601450431, 3009837614, 3294710456, 1567103746
        DC32 711928724, 3020668471, 3272380065, 1510334235, 755167117
//   47 		0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
//   48 		0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856,
//   49 		0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e,
//   50 		0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6,
//   51 		0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
//   52 		0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87,
//   53 		0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
//   54 		0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97,
//   55 		0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
//   56 		0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158,
//   57 		0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc,
//   58 		0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010,
//   59 		0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
//   60 		0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739,
//   61 		0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d,
//   62 		0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671,
//   63 		0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
//   64 		0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda,
//   65 		0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a,
//   66 		0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92,
//   67 		0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
//   68 		0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b,
//   69 		0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b,
//   70 		0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3,
//   71 		0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
//   72 		0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c,
//   73 		0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8,
//   74 		0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d };
//   75 
//   76 /**
//   77  * Online CRC calculator:
//   78  * http://www.zorc.breitbandkatze.de/crc.html
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function crc32
          CFI NoCalls
        THUMB
//   80 uint32_t crc32(const void *buf, uint32_t size) {
crc32:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R2,R0
//   81 	const uint8_t *p;
//   82 	uint32_t crc = 0;
        MOVS     R0,#+0
//   83 
//   84 	p = buf;
        MOVS     R3,R2
//   85 	crc = crc ^ 0xFFFFFFFF;
        MOVS     R4,#-1
        EORS     R0,R4,R0
//   86 
//   87 	while (size--) {
??crc32_0:
        MOVS     R4,R1
        SUBS     R1,R4,#+1
        CMP      R4,#+0
        BEQ.N    ??crc32_1
//   88 		crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
        LDRB     R4,[R3, #+0]
        EORS     R4,R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.N    R5,??DataTable0
        LDR      R4,[R5, R4, LSL #+2]
        EORS     R0,R4,R0, LSR #+8
        ADDS     R3,R3,#+1
        B.N      ??crc32_0
//   89         }
//   90 
//   91 	return crc ^ 0xFFFFFFFF;
??crc32_1:
        MOVS     R4,#-1
        EORS     R0,R4,R0
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   92 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     crc32_tab

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 1 024 bytes in section .data
//   108 bytes in section .text
// 
//   108 bytes of CODE memory
// 1 024 bytes of DATA memory
//
//Errors: none
//Warnings: none
