#include "types.h"
#include<LPC214x.h>
#include "defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "interrupt1.h"
#include "interrupt2.h"

u32     current_time =0;
void device_on(u32 current_time,u32 start_time,u32 end_time);


u32 start_time;
u32 end_time;

void lab_time(u32 *s_hour,u32 *e_hour,u32 *s_min,u32 *e_min,u32 *s_sec,u32 *e_sec){
current_time = ((HOUR*3600)+(MIN*60)+(SEC*60));
                start_time = (*s_hour*3600)+(*s_min*60)+(*s_sec*60);
                end_time = (*e_hour*3600)+(*e_min*60)+(*e_sec*60);
				device_on(current_time,start_time,end_time);
}

void device_on(u32 current_time,u32 start_time,u32 end_time){
        if(current_time<start_time && current_time>end_time){
                IOCLR0 = (1<<LED1)|(1<<LED2);
        }
	}
