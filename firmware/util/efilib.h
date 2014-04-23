/**
 * @file	efilib.h
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EFILIB_H_
#define EFILIB_H_


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#define ERROR_CODE -11223344

int indexOf(const char *string, char ch);
float atoff(const char *string);
int atoi(const char *string);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* EFILIB_H_ */
