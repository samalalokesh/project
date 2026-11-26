// rtc.c

#include <LPC214x.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "defines.h"

/*-------------------------------------------------------
  RTC Initialization

  - Enables RTC clock
  - Resets RTC counters
  - Sets default Date
-------------------------------------------------------*/
void init_rtc(void)
{
    // Enable RTC and reset counters
    CCR = (1 << 0) | (1 << 4);

    // Default Date
    DOM   = 12;      // Day of Month
    MONTH = 11;      // Month
    YEAR  = 2026;    // Year
}

/*-------------------------------------------------------
  Display RTC Time and Date on LCD

  Line 1 : HH:MM:SS
  Line 2 : DD/MM/YYYY
-------------------------------------------------------*/
void rtc_lcd()
{
    // Clear previous LCD characters
    StrLCD("              ");

    /*------------- Display Time -------------*/

    CmdLCD(GOTO_LINE1_POS0);

    // Hours
    CharLCD((HOUR / 10) + 48);
    CharLCD((HOUR % 10) + 48);

    CharLCD(':');

    // Minutes
    CharLCD((MIN / 10) + 48);
    CharLCD((MIN % 10) + 48);

    CharLCD(':');

    // Seconds
    CharLCD((SEC / 10) + 48);
    CharLCD((SEC % 10) + 48);

    /*------------- Display Date -------------*/

    CmdLCD(GOTO_LINE2_POS0);

    // Day
    U32LCD(DOM);

    CharLCD('/');

    // Month
    U32LCD(MONTH);

    CharLCD('/');

    // Year
    U32LCD(YEAR);
}
