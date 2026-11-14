#include "lcd.h"
#include "lcd_definesh.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "delay.h"
#include <LPC214x.h>
void int_sw2(void) __irq;

void Init_eint2(){
	PINSEL0&=~(3<<(3*2));
	PINSEL0|=0x000000C0;
	VICIntSelect&=~(1<<15);
	VICVectCntl1=(1<<5)|15;
	//VICVectAddr0=(unsigned)eint0_fiq_isr;
	VICIntEnable=(1<<15);
	EXTINT=(1<<1);
	EXTMODE=1<<1;
	EXTPOLAR=0;
}
