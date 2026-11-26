// led.h

#ifndef _LED_H_
#define _LED_H_

#include "types.h"

/*-------------------------------------------------------
  Check Whether Current RTC Time Lies Within
  Configured Lab Time

  Parameters:
      s_hour -> Start Hour
      e_hour -> End Hour

      s_min  -> Start Minute
      e_min  -> End Minute

      s_sec  -> Start Second
      e_sec  -> End Second

  Return:
      1 -> Lab Time Active
      0 -> Lab Time Inactive
-------------------------------------------------------*/
s32 lab_time(u32 *s_hour,
             u32 *e_hour,
             u32 *s_min,
             u32 *e_min,
             u32 *s_sec,
             u32 *e_sec);

/*-------------------------------------------------------
  RC5 Remote LED Control Function

  Command Mapping:

      1 -> LED1 OFF
      2 -> LED1 ON
      3 -> LED2 OFF
      4 -> LED2 ON
      5 -> LED1 OFF, LED2 OFF
      6 -> LED1 ON, LED2 ON
      7 -> Display RTC for 10 seconds

  Parameters:
      command -> RC5 command received
      F1      -> LED1 status flag
      F2      -> LED2 status flag
-------------------------------------------------------*/
void IR_LED_Control(u32 command,
                    s32 *F1,
                    s32 *F2);

#endif
