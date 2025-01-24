/**
 * @file	error_handling_c.h
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void criticalErrorM(const char *msg);

void criticalError(const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */
