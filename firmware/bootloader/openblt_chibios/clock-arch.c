/**
 * @file clock-arch.c
 *
 * @date Apr 17, 2026
 * @author Andrey Gusakov, 2026
 */

#include "clock-arch.h"
#include "boot.h"

/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  // Return mS time
  return (clock_time_t)TimerGet();
}
/*---------------------------------------------------------------------------*/
