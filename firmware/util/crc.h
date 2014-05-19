/**
 * @file    crc.h
 *
 * @date Sep 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef CRC_H_
#define CRC_H_

typedef unsigned char crc_t;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

crc_t calc_crc(const crc_t message[], int nBytes);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CRC_H_ */
