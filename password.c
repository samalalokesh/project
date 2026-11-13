#include<LPC214x.h>
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_definesh.h"
#include "kpm.h"
#include "kpm_defines.h"

void password(void){
			u32 pwd=0; u8 lastKey;

	StrLCD("<---PASSWORD--->");
	while(1)
	{
		CmdLCD(GOTO_LINE2_POS0);
		ReadNum2(&pwd,&lastKey);
		if(pwd==9999)
		{
			CmdLCD(CLEAR_LCD);
			StrLCD("PWD MATCH");
			break;
		}
		else
		{
			CmdLCD(CLEAR_LCD);
			StrLCD("PWD FAIL");
			pwd = 0;
		}
  }
}
