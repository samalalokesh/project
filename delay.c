// delay.c

#include "types.h"

/*-------------------------------------------------------
  Microsecond Delay Function

  Creates an approximate delay in microseconds.

  Formula assumes LPC2148 running at 12 MHz.
-------------------------------------------------------*/
void delay_us(u32 dlyUS)
{
    // Loop count adjusted for 1 microsecond delay
    for(dlyUS *= 12; dlyUS > 0; dlyUS--);
}

/*-------------------------------------------------------
  Millisecond Delay Function

  Creates an approximate delay in milliseconds.

  1 ms = 1000 us
-------------------------------------------------------*/
void delay_ms(u32 dlyMS)
{
    // Loop count adjusted for 1 millisecond delay
    for(dlyMS *= 12000; dlyMS > 0; dlyMS--);
}

/*-------------------------------------------------------
  Second Delay Function

  Creates an approximate delay in seconds.

  1 s = 1000 ms
-------------------------------------------------------*/
void delay_s(u32 dlyS)
{
    // Loop count adjusted for 1 second delay
    for(dlyS *= 12000000; dlyS > 0; dlyS--);
}
