// password.c

#include <LPC214x.h>
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"

/*-------------------------------------------------------
  Password Authentication Function

  Default Password : 1111

  Steps:
  1. Display password prompt.
  2. Read password from keypad.
  3. Compare entered password with predefined password.
  4. Grant or deny access.
-------------------------------------------------------*/
void password(void)
{
    u32 pwd = 0;
    u8 lastKey;

    // Display password prompt
    StrLCD("<---PASSWORD--->");

    while(1)
    {
        // Move cursor to second line
        CmdLCD(GOTO_LINE2_POS0);

        // Read password from keypad
        ReadNum3(&pwd, &lastKey);

        // Check password
        if(pwd == 1111)
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("PWD MATCH");

            delay_ms(500);

            CmdLCD(CLEAR_LCD);
            StrLCD("Access Granted");

            delay_ms(500);

            CmdLCD(CLEAR_LCD);

            // Reset password variable
            pwd = 0;

            break;      // Exit function
        }
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("PWD FAIL");

            // Reset password variable for next attempt
            pwd = 0;
        }
    }
}
