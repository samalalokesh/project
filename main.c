#include<LPC214x.h>
#include "defines.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "interrupt1.h"
#include "interrupt2.h"
#include "led.h"
#include "tsop_rc5.h"

void int_sw1(void) __irq;
void int_sw2(void) __irq;
void IR_LED_Control(u32 command);


volatile unsigned int flag;

u32 ir_address;
u32 ir_command;
u32 ir_toggle;

u32 last_address = 0;
u32 last_command = 0;
u32 last_toggle = 0;

u8 previous_key_valid = 0;

int main()
	{
		unsigned char last;
		unsigned int num=0,start_hour=10,end_hour=20,start_min=0,end_min=59,start_sec=0,end_sec=59;
	
		IODIR0|=(1<<BUZZER)|(1<<LED1)|(1<<LED2);
	
		InitKPM();
		InitLCD();
		Init_eint();
		Init_eint2();
		
		password();
		init_rtc();

		while(1)
		{
			rtc_lcd();
			
			 if(TSOP1738_ReadFrame(&ir_address,
                              &ir_command,
                              &ir_toggle))
			{
            // Duplicate frame filtering

				if(!(previous_key_valid &&
					ir_address == last_address &&
					ir_command == last_command &&
					ir_toggle == last_toggle))
				{
						last_address = ir_address;
						last_command = ir_command;
						last_toggle = ir_toggle;

						previous_key_valid = 1;

						IR_LED_Control(ir_command);
				}
			}

			
			if(flag==1)
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("Interupt Occured");
				delay_ms(500);
				CmdLCD(CLEAR_LCD);
				password();
				rtc_edit(&num,&last,&start_hour,&end_hour,&start_min,&end_min,&start_sec,&end_sec);
				
				flag = 0;	
			}
			if(flag==2)
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("Emergency..!");
				delay_ms(500);				
				password();
				IOSET0 = 1<<BUZZER;
				IOCLR0 = (1<<LED1)|(1<<LED2);
				delay_s(2);
				IOCLR0 = 1<<BUZZER;
				flag = 0;
			}
		}
	}

void int_sw1(void) __irq
	{
	flag=1;
	EXTINT=1<<0;
	VICVectAddr=0;
	}

void int_sw2(void) __irq
	{
	flag=2;	
	EXTINT=1<<1;
	VICVectAddr=0;
	}
void IR_LED_Control(u32 command)
{
    switch(command)
    {
        case 1:
            IOSET0 = 1 << LED1;
            break;

        case 2:
            IOCLR0 = 1 << LED1;
            break;

        case 3:
            IOSET0 = 1 << LED2;
            break;

        case 4:
            IOCLR0 = 1 << LED2;
            break;

        case 5:
            IOSET0 = 1 << (LED1)|(LED2);
            break;

        case 6:
            IOCLR0 = 1 << (LED1)|(LED2);
            break;
    }
}
