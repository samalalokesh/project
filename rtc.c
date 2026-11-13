#include<LPC214x.h>
#include "lcd.h"
#include "lcd_definesh.h"

void init_rtc(void){
	
	CCR=1<<1;
	CCR=1<<0;
	PREINT=456;
	PREFRAC=25024;
	
	HOUR = 21;
	MIN = 30;
	SEC = 0;
	DOM = 12;
	MONTH = 11;
	YEAR = 2026;

}

void rtc_lcd(){
	StrLCD("                ");
	CmdLCD(GOTO_LINE1_POS0);
	CharLCD((HOUR/10)+48);
	CharLCD((HOUR%10)+48);
	CharLCD(':');
	CharLCD((MIN/10)+48);
	CharLCD((MIN%10)+48);
	CharLCD(':');
	CharLCD((SEC/10)+48);
	CharLCD((SEC%10)+48);
	
	CmdLCD(GOTO_LINE2_POS0);
	U32LCD(DOM);
	CharLCD('/');
	U32LCD(MONTH);
	CharLCD('/');
	U32LCD(YEAR);

}