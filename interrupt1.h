// interrupt1.h

#ifndef _INTERRUPT1_H_
#define _INTERRUPT1_H_

#include "types.h"

/*-------------------------------------------------------
  EINT0 Initialization Function
  Configures External Interrupt 0 (SW1)
-------------------------------------------------------*/
void Init_eint(void);

/*-------------------------------------------------------
  RTC and Lab Timing Edit Function

  num        -> Stores keypad-entered value
  last       -> Stores last keypad key

  start_hour -> Lab start hour
  end_hour   -> Lab end hour

  start_min  -> Lab start minute
  end_min    -> Lab end minute

  start_sec  -> Lab start second
  end_sec    -> Lab end second
-------------------------------------------------------*/
void rtc_edit(u32 *num,
              u8 *last,
              u32 *start_hour,
              u32 *end_hour,
              u32 *start_min,
              u32 *end_min,
              u32 *start_sec,
              u32 *end_sec);

/*-------------------------------------------------------
  LPC2148 Interrupt Definitions
-------------------------------------------------------*/

// VIC channel number for EINT0
#define INTERRUPT_CHANNEL1 14

// PINSEL value for EINT0 (P0.1)
#define EINT0 0x0000000C

#endif
