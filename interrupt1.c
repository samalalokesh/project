//interrupt1.c
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "delay.h"
#include <LPC214x.h>
void int_sw1(void) __irq;

void Init_eint(void){
	PINSEL0&=~(3<<(1*2));
	PINSEL0|=(3<<2);
	VICIntSelect&=~(1<<14);
	VICVectCntl0=(1<<5)|14;
	VICVectAddr0=(unsigned)int_sw1;
	VICIntEnable=(1<<14);
	EXTINT=1<<0;
	EXTMODE=1<<0;
	EXTPOLAR=0;
}


void rtc_edit(u32 *num,u8 *last,
             u32 *start_hour,u32 *end_hour,
             u32 *start_min,u32 *end_min,
             u32 *start_sec,u32 *end_sec)
{
    u32 temp,op;
    
    
        CmdLCD(CLEAR_LCD);
        StrLCD("Edit 1.rtc 2.Lab");
        op=ReadNum();
        CmdLCD(CLEAR_LCD);
		

        if(op==0){
        CmdLCD(CLEAR_LCD);
		return ;
		}
        /* RTC EDITING */
        if(op==1)
        {
            StrLCD("Edit1.HH2.MM3.SS");
            op=ReadNum();
            CmdLCD(CLEAR_LCD);

            if(op==1)
            {
                temp=HOUR;
                StrLCD("Enter hours");
                ReadNum2(num,last);

                if(*num<=23)
                    HOUR=*num;
                else
                    HOUR=temp;

                *num=0;
                
                CmdLCD(CLEAR_LCD);
            }
            else if(op==2)
            {
                temp=MIN;
                StrLCD("Enter Minutes");
                ReadNum2(num,last);

                if(*num<=59)
                    MIN=*num;
                else
                    MIN=temp;

                *num=0;
    
                CmdLCD(CLEAR_LCD);
            }
            else if(op==3)
            {
                temp=SEC;
                StrLCD("Enter Seconds");
                ReadNum2(num,last);

                if(*num<=59)
                    SEC=*num;
                else
                    SEC=temp;

                *num=0;
            
                CmdLCD(CLEAR_LCD);
            }
        }

        /* LAB EDITING */
        else if(op==2)
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("1.HH 2.MM 3.SS");
            op=ReadNum();
            CmdLCD(CLEAR_LCD);

            if(op==1)
            {
                StrLCD("1.S-hour2.E-hour");
                op=ReadNum();
                CmdLCD(CLEAR_LCD);

                if(op==1)
                {
                    StrLCD("Enter S-hour");
                    ReadNum2(num,last);

                    if(*num<=23)
                        *start_hour=*num;

                    *num=0;
					CmdLCD(CLEAR_LCD);
                }
                else if(op==2)
                {
                    StrLCD("Enter E-hour");
                    temp=*end_hour;

                    ReadNum2(num,last);

                    if(*num>=*start_hour && *num<=23)
                        *end_hour=*num;
                    else
                        *end_hour=temp;
					
					CmdLCD(CLEAR_LCD);

                    *num=0;
                }
            }
            else if(op==2)
            {
                StrLCD("1.S-Min2.E-Min");
                op=ReadNum();
                CmdLCD(CLEAR_LCD);

                if(op==1)
                {
                    StrLCD("Enter S-min");
                    ReadNum2(num,last);

                    if(*num<=59)
                        *start_min=*num;

                    *num=0;
					CmdLCD(CLEAR_LCD);
                }
                else if(op==2)
                {
                    StrLCD("Enter E-min");
                    temp=*end_min;

                    ReadNum2(num,last);

                    if(*num>=*start_min && *num<=59)
                        *end_min=*num;
                    else
                        *end_min=temp;

                    *num=0;
					CmdLCD(CLEAR_LCD);
                }
            }
            else if(op==3)
            {
                StrLCD("1.S-Sec 2.E-Sec");
                op=ReadNum();
                CmdLCD(CLEAR_LCD);

                if(op==1)
                {
                    StrLCD("Enter S-sec");
                    ReadNum2(num,last);

                    if(*num<=59)
                        *start_sec=*num;

                    *num=0;
					CmdLCD(CLEAR_LCD);
                }
                else if(op==2)
                {
                    StrLCD("Enter E-sec");
                    temp=*end_sec;

                    ReadNum2(num,last);

                    if(*num>=*start_sec && *num<=59)
                        *end_sec=*num;
                    else
                        *end_sec=temp;

                    *num=0;
					CmdLCD(CLEAR_LCD);
                }
            }
        }
    
}
