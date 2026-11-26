// interrupt1.c

#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "delay.h"
#include <LPC214x.h>
#include "interrupt1.h"

// External Interrupt 0 ISR declaration
void int_sw1(void) __irq;

/*-------------------------------------------------------
  Configure External Interrupt 0 (EINT0)
  Connected to SW1 for RTC/Lab timing modification
-------------------------------------------------------*/
void Init_eint(void)
{
    // Configure P0.1 as EINT0
    PINSEL0 &= ~(3 << (1 * 2));
    PINSEL0 |= (3 << 2);

    // Select IRQ mode
    VICIntSelect &= ~(1 << 14);

    // Assign ISR to vector slot 0
    VICVectCntl0 = (1 << 5) | 14;
    VICVectAddr0 = (unsigned)int_sw1;

    // Enable EINT0 interrupt
    VICIntEnable |= (1 << 14);

    // Clear pending interrupt flag
    EXTINT = 1 << 0;

    // Edge sensitive interrupt
    EXTMODE = 1 << 0;

    // Falling edge trigger
    EXTPOLAR = 0;
}

/*-------------------------------------------------------
  RTC and Lab Schedule Editing Function

  Option 1 : Edit RTC Time/Date
  Option 2 : Edit Lab Timings
-------------------------------------------------------*/
void rtc_edit(u32 *num, u8 *last,
              u32 *start_hour, u32 *end_hour,
              u32 *start_min,  u32 *end_min,
              u32 *start_sec,  u32 *end_sec)
{
    u32 temp, op;

f:
    CmdLCD(CLEAR_LCD);
    StrLCD("Edit 1.rtc 2.Lab");

    op = ReadNum();

    CmdLCD(CLEAR_LCD);

    /*===================================================
      RTC EDITING MENU
    ===================================================*/
    if(op == 1)
    {
RTC:
        StrLCD("1.Time 2.Date");

        op = ReadNum();

        CmdLCD(CLEAR_LCD);

        /*-----------------------------------------------
          TIME EDITING
        -----------------------------------------------*/
        if(op == 1)
        {
            StrLCD("1.HH 2.MM 3.SS");

            op = ReadNum();

            CmdLCD(CLEAR_LCD);

            /* Edit Hour */
            if(op == 1)
            {
Hour:
                temp = HOUR;

                StrLCD("Enter Hour");

                ReadNum2(num,last);

                if(*num <= 23)
                {
                    HOUR = *num;
                    CmdLCD(CLEAR_LCD);
                }
                else
                {
                    HOUR = temp;

                    *num = 0;

                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Command");
                    delay_ms(500);

                    goto Hour;
                }
            }

            /* Edit Minute */
            else if(op == 2)
            {
Min:
                temp = MIN;

                StrLCD("Enter Min");

                ReadNum2(num,last);

                if(*num <= 59)
                {
                    MIN = *num;
                }
                else
                {
                    MIN = temp;

                    *num = 0;

                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Command");
                    delay_ms(500);

                    goto Min;
                }
            }

            /* Edit Second */
            else if(op == 3)
            {
Sec:
                temp = SEC;

                StrLCD("Enter Sec");

                ReadNum2(num,last);

                if(*num <= 59)
                {
                    SEC = *num;
                }
                else
                {
                    SEC = temp;

                    *num = 0;

                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Command");
                    delay_ms(500);

                    goto Sec;
                }
            }
        }

        /*-----------------------------------------------
          DATE EDITING
        -----------------------------------------------*/
        else if(op == 2)
        {
            StrLCD("1.DOM2.MON3.YEAR");

            op = ReadNum();

            CmdLCD(CLEAR_LCD);

            /* Edit Day Of Month */
            if(op == 1)
            {
                temp = DOM;

                StrLCD("Enter DOM");

                ReadNum2(num,last);

                if((*num >= 1) && (*num <= 31))
                    DOM = *num;
                else
                    DOM = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }

            /* Edit Month */
            else if(op == 2)
            {
                temp = MONTH;

                StrLCD("Enter Month");

                ReadNum2(num,last);

                if((*num >= 1) && (*num <= 12))
                    MONTH = *num;
                else
                    MONTH = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }

            /* Edit Year */
            else if(op == 3)
            {
                temp = YEAR;

                StrLCD("Enter Year");

                ReadNum2(num,last);

                if(*num <= 99)
                    YEAR = *num;
                else
                    YEAR = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
        }

        /* Invalid RTC Menu Selection */
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("Invalid Command");
            delay_ms(500);

            goto RTC;
        }
    }

    /*===================================================
      LAB TIMING EDITING MENU
    ===================================================*/
    else if(op == 2)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1.HH 2.MM 3.SS");

        op = ReadNum();

        CmdLCD(CLEAR_LCD);

        /*-----------------------------------------------
          Edit Lab Hours
        -----------------------------------------------*/
        if(op == 1)
        {
            StrLCD("1.S-HR 2.E-HR");

            op = ReadNum();

            CmdLCD(CLEAR_LCD);

            /* Start Hour */
            if(op == 1)
            {
                StrLCD("Enter S-hour");

                ReadNum2(num,last);

                if(*num <= 23)
                    *start_hour = *num;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }

            /* End Hour */
            else if(op == 2)
            {
                temp = *end_hour;

                StrLCD("Enter E-hour");

                ReadNum2(num,last);

                if(*num >= *start_hour && *num <= 23)
                    *end_hour = *num;
                else
                    *end_hour = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
        }

        /*-----------------------------------------------
          Edit Lab Minutes
        -----------------------------------------------*/
        else if(op == 2)
        {
            StrLCD("1.S-MIN2.E-MIN");

            op = ReadNum();

            CmdLCD(CLEAR_LCD);

            if(op == 1)
            {
                StrLCD("Enter S-min");

                ReadNum2(num,last);

                if(*num <= 59)
                    *start_min = *num;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
            else if(op == 2)
            {
                temp = *end_min;

                StrLCD("Enter E-min");

                ReadNum2(num,last);

                if(*num >= *start_min && *num <= 59)
                    *end_min = *num;
                else
                    *end_min = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
        }

        /*-----------------------------------------------
          Edit Lab Seconds
        -----------------------------------------------*/
        else if(op == 3)
        {
            StrLCD("1.S-SEC2.E-SEC");

            op = ReadNum();

            CmdLCD(CLEAR_LCD);

            if(op == 1)
            {
                StrLCD("Enter S-sec");

                ReadNum2(num,last);

                if(*num <= 59)
                    *start_sec = *num;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
            else if(op == 2)
            {
                temp = *end_sec;

                StrLCD("Enter E-sec");

                ReadNum2(num,last);

                if(*num >= *start_sec && *num <= 59)
                    *end_sec = *num;
                else
                    *end_sec = temp;

                *num = 0;

                CmdLCD(CLEAR_LCD);
            }
        }
    }

    /* Invalid Main Menu Option */
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Command");
        delay_ms(500);

        goto f;
    }
}
