// main.c

#include <LPC214x.h>
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

// External Interrupt Service Routines
void int_sw1(void) __irq;
void int_sw2(void) __irq;

// Global Flags
volatile unsigned int flag;
s32 f1, f2;
s32 rtc_flag;

// Variables to store RC5 frame data
u32 ir_address;
u32 ir_command;
u32 ir_toggle;

// Variables for duplicate key filtering
unsigned int flag1 = 1;
u32 last_address = 0;
u32 last_command = 0;
u32 last_toggle = 0;

u8 previous_key_valid = 0;

int main()
{
    unsigned char last;

    // RTC schedule variables
    unsigned int num = 0;
    unsigned int start_hour = 10, end_hour = 20;
    unsigned int start_min = 0, end_min = 59;
    unsigned int start_sec = 0, end_sec = 59;

    // Configure Buzzer, LED1 and LED2 as outputs
    IODIR0 |= (1 << BUZZER) | (1 << LED1) | (1 << LED2);

    // Turn OFF LEDs initially (Active Low)
    IOSET0 = (1 << LED1) | (1 << LED2);

    // Initialize peripherals
    InitKPM();       // Keypad
    InitLCD();       // LCD
    Init_eint();     // External Interrupt 0
    Init_eint2();    // External Interrupt 1

    // Password authentication
    password();

    // RTC initialization
    init_rtc();

    while (1)
    {
        // Check if current time is within allowed schedule
        if (lab_time(&start_hour,
                     &end_hour,
                     &start_min,
                     &end_min,
                     &start_sec,
                     &end_sec))
        {
            // Wait until a valid RC5 frame is received
            while (TSOP1738_ReadFrame(&ir_address,
                                      &ir_command,
                                      &ir_toggle) == 0)
            {
                // Display RTC continuously
                if (rtc_flag == 0)
                    rtc_lcd();

                // Interrupt 1 occurred
                if (flag == 1)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Interrupt Occured");

                    delay_ms(500);

                    CmdLCD(CLEAR_LCD);

                    password();

                    // Edit RTC timing
                    rtc_edit(&num,
                             &last,
                             &start_hour,
                             &end_hour,
                             &start_min,
                             &end_min,
                             &start_sec,
                             &end_sec);

                    flag = 0;
                }

                // Emergency Interrupt occurred
                if (flag == 2)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Emergency..!");

                    delay_ms(500);

                    CmdLCD(CLEAR_LCD);

                    password();

                    // Turn ON buzzer
                    IOSET0 = 1 << BUZZER;

                    // Turn OFF LEDs
                    IOSET0 = (1 << LED1) | (1 << LED2);

                    delay_s(2);

                    // Turn OFF buzzer
                    IOCLR0 = 1 << BUZZER;

                    flag = 0;
                }
            }

            // RC5 frame received successfully
            rtc_flag = 1;

            // Duplicate frame filtering
            // Process command only if different from previous key
            if (!(previous_key_valid &&
                  ir_address == last_address &&
                  ir_command == last_command &&
                  ir_toggle == last_toggle))
            {
                // Save current frame as previous frame
                last_address = ir_address;
                last_command = ir_command;
                last_toggle = ir_toggle;

                previous_key_valid = 1;

                // Execute LED control based on RC5 command
                IR_LED_Control(ir_command, &f1, &f2);
            }
        }
    }
}

/*-----------------------------------------------------
  External Interrupt 0 ISR
  Used for RTC editing
-----------------------------------------------------*/
void int_sw1(void) __irq
{
    flag = 1;

    // Clear interrupt flag
    EXTINT = 1 << 0;

    VICVectAddr = 0;
}

/*-----------------------------------------------------
  External Interrupt 1 ISR
  Used for Emergency operation
-----------------------------------------------------*/
void int_sw2(void) __irq
{
    flag = 2;

    // Clear interrupt flag
    EXTINT = 1 << 1;

    VICVectAddr = 0;
}
