// led.c

#include "types.h"
#include <LPC214x.h>
#include "defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "interrupt1.h"
#include "interrupt2.h"
#include "delay.h"

/*-------------------------------------------------------
  Global Time Variables
-------------------------------------------------------*/
u32 current_time = 0;
u32 start_time;
u32 end_time;

/*-------------------------------------------------------
  Check Lab Timing

  Returns:
      1 -> Current RTC time is within lab hours
      0 -> Outside lab hours
-------------------------------------------------------*/
s32 lab_time(u32 *s_hour,
             u32 *e_hour,
             u32 *s_min,
             u32 *e_min,
             u32 *s_sec,
             u32 *e_sec)
{
    current_time = (HOUR * 3600) +
                   (MIN * 60) +
                   SEC;

    start_time = (*s_hour * 3600) +
                 (*s_min * 60) +
                 *s_sec;

    end_time = (*e_hour * 3600) +
               (*e_min * 60) +
               *e_sec;

    if(current_time > start_time &&
       current_time < end_time)
    {
        return 1;
    }

    return 0;
}

/*-------------------------------------------------------
  RC5 Remote LED Control

  Command 1 -> LED1 OFF
  Command 2 -> LED1 ON
  Command 3 -> LED2 OFF
  Command 4 -> LED2 ON
  Command 5 -> LED1 OFF, LED2 OFF
  Command 6 -> LED1 ON, LED2 ON
  Command 7 -> Display RTC for 10 seconds
-------------------------------------------------------*/
void IR_LED_Control(u32 command,
                    s32 *f1,
                    s32 *f2)
{
    int i = 10;

    switch(command)
    {
        /*-----------------------------------
          LED1 OFF
        -----------------------------------*/
        case 1:

            IOSET0 = 1 << LED1;      // Active Low LED OFF
            *f1 = 0;

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("LED1:OFF");

            CmdLCD(GOTO_LINE2_POS0);

            if(*f2 == 1)
                StrLCD("LED2:ON");
            else
                StrLCD("LED2:OFF");

            break;

        /*-----------------------------------
          LED1 ON
        -----------------------------------*/
        case 2:

            IOCLR0 = 1 << LED1;      // Active Low LED ON
            *f1 = 1;

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("LED1:ON");

            CmdLCD(GOTO_LINE2_POS0);

            if(*f2 == 1)
                StrLCD("LED2:ON");
            else
                StrLCD("LED2:OFF");

            break;

        /*-----------------------------------
          LED2 OFF
        -----------------------------------*/
        case 3:

            IOSET0 = 1 << LED2;
            *f2 = 0;

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("LED2:OFF");

            CmdLCD(GOTO_LINE1_POS0);

            if(*f1 == 1)
                StrLCD("LED1:ON");
            else
                StrLCD("LED1:OFF");

            break;

        /*-----------------------------------
          LED2 ON
        -----------------------------------*/
        case 4:

            IOCLR0 = 1 << LED2;
            *f2 = 1;

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("LED2:ON");

            CmdLCD(GOTO_LINE1_POS0);

            if(*f1 == 1)
                StrLCD("LED1:ON");
            else
                StrLCD("LED1:OFF");

            break;

        /*-----------------------------------
          LED1 OFF + LED2 OFF
        -----------------------------------*/
        case 5:

            IOSET0 = 1 << LED1;
            IOSET0 = 1 << LED2;

            *f1 = 0;
            *f2 = 0;

            CmdLCD(CLEAR_LCD);

            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("LED1:OFF");

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("LED2:OFF");

            break;

        /*-----------------------------------
          LED1 ON + LED2 ON
        -----------------------------------*/
        case 6:

            IOCLR0 = 1 << LED1;
            IOCLR0 = 1 << LED2;

            *f1 = 1;
            *f2 = 1;

            CmdLCD(CLEAR_LCD);

            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("LED1:ON");

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("LED2:ON");

            break;

        /*-----------------------------------
          Display RTC for 10 Seconds
        -----------------------------------*/
        case 7:

            while(i--)
            {
                rtc_lcd();
                delay_s(1);
            }

            break;

        default:
            break;
    }
}
