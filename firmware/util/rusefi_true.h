/*
 * rusefi_true.h
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// we still have a minor mess with headers, this header should better be included as high as possible

#ifndef FALSE
#define FALSE (0)
#endif /* FALSE */

#ifndef TRUE
#define TRUE (!(FALSE))
#endif /* TRUE */
