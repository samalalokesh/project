// rtc.h

#ifndef _RTC_H_
#define _RTC_H_

/*-------------------------------------------------------
  RTC Initialization Function

  - Enables RTC peripheral
  - Starts RTC clock
  - Initializes default date/time
-------------------------------------------------------*/
void init_rtc(void);

/*-------------------------------------------------------
  RTC LCD Display Function

  Displays:
      HH:MM:SS
      DD/MM/YYYY

  on the 16x2 LCD
-------------------------------------------------------*/
void rtc_lcd(void);

#endif
