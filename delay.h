// delay.h

#ifndef _DELAY_H_
#define _DELAY_H_

#include "types.h"

/*-------------------------------------------------------
  Microsecond Delay

  Parameter:
      dlyUS - Delay time in microseconds

  Example:
      delay_us(100);
      // 100 µs delay
-------------------------------------------------------*/
void delay_us(u32 dlyUS);

/*-------------------------------------------------------
  Millisecond Delay

  Parameter:
      dlyMS - Delay time in milliseconds

  Example:
      delay_ms(500);
      // 500 ms delay
-------------------------------------------------------*/
void delay_ms(u32 dlyMS);

/*-------------------------------------------------------
  Second Delay

  Parameter:
      dlyS - Delay time in seconds

  Example:
      delay_s(2);
      // 2 second delay
-------------------------------------------------------*/
void delay_s(u32 dlyS);

#endif
