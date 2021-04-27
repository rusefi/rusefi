/*
 * mod_string.h
 *
 *  Created on: Oct 19, 2012
 *      Author: brent
 */

#ifndef MOD_STRING_H_
#define MOD_STRING_H_
#include <stddef.h>

#ifdef __cplusplus
#define BEGIN_C extern "C" {
#define END_C }
#else
#define BEGIN_C
#define END_C
#endif

BEGIN_C


void *memcpy(void *__restrict __dest, __const void *__restrict __src, size_t __n);

void *memset(void *s, int c, size_t n);

int strncmp(register const char *s1, register const char *s2, size_t n);

char *strcpy(char * __restrict s1, const char * __restrict s2);

size_t strlen(const char *s);

size_t strlcpy(char * __restrict dst, register const char * __restrict src, size_t siz);

char *strncpy(char * __restrict s1, register const char * __restrict s2, size_t n);

int strcmp(const char *s1, const char *s2);

int strcasecmp(const char *s1, const char *s2);

char *strtok(char * __restrict s1, const char * __restrict s2);

char *strtok_r(char * __restrict s1, const char * __restrict s2, char ** __restrict next_start);

size_t strspn(const char *s1, const char *s2);

char *strpbrk(const char *s1, const char *s2);

char *strcat(char * s1, const char * s2);

char *strstr (const char *phaystack, const char *pneedle);

char * strchr(const char *s, const int c);

size_t strcspn(const char *s1, const char *s2);

char * strncat(char *dst, const char *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

void * memchr(const void *s, register int c, register size_t n);

char * strrchr(const char *cp, int ch);

char * strerror(int errorno);
END_C

#endif /* MOD_STRING_H_ */
