#include<LPC214x.h>
#include "defines.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_definesh.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "interrupt1.h"
#include "interrupt2.h"
#include "led.h"

void int_sw1(void) __irq;
void int_sw2(void) __irq;

unsigned int flag;


int main(){
	unsigned char last;
	unsigned int num=0,start_hour=10,end_hour=20,start_min=0,end_min=59,start_sec=0,end_sec=59;
	
	IODIR0|=(1<<BUZZER)|(1<<LED1)|(1<<LED2);
	
		InitKPM();
		InitLCD();
		Init_eint();
		VICVectAddr0=(unsigned)int_sw1;
		Init_eint2();
		VICVectAddr1=(unsigned)int_sw2;
		password();
		init_rtc();

		while(1){
			rtc_lcd();
			flag = rtc_edit(flag,&num,&last,&start_hour,&end_hour,&start_min,&end_min,&start_sec,&end_sec);
			lab_time(&start_hour,&end_hour,&start_min,&end_min,&start_sec,&end_sec);
			if(flag==2){
				CmdLCD(CLEAR_LCD);
				StrLCD("Emergency..!");
				delay_ms(500);				
				password();
				IOSET0 = 1<<BUZZER;
				IOCLR0 = (1<<LED1)|(1<<LED2);
				delay_s(2);
				IOCLR0 = 1<<BUZZER;
				flag=0;
			}
		}
}

void int_sw1(void) __irq{
	CmdLCD(CLEAR_LCD);
	StrLCD("Interupt Occured");
	delay_ms(500);
	CmdLCD(CLEAR_LCD);
	password();
	flag=1;
	EXTINT=1<<0;
	VICVectAddr=0;
}

void int_sw2() __irq{
	flag=2;	
	EXTINT=1<<1;
	VICVectAddr=0;
	
}
