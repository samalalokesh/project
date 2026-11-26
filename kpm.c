// kpm.c

#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "defines.h"
#include "kpm_defines.h"
#include "lcd.h"
#include "lcd_defines.h"

/*-------------------------------------------------------
  Keypad Lookup Table (4x4 Matrix Keypad)
-------------------------------------------------------*/
u8 kpmLUT[4][4] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','=','D'}
};

/*-------------------------------------------------------
  Initialize Keypad

  ROWS -> Output
  COLS -> Input
-------------------------------------------------------*/
void InitKPM(void)
{
    // Configure row pins as output
    WRITENIBBLE(IODIR1, ROW0, 15);

    // Columns remain input by default
}

/*-------------------------------------------------------
  Scan Columns

  Return:
    0 -> Key Pressed
    1 -> No Key Pressed
-------------------------------------------------------*/
u8 ColScan(void)
{
    u8 t;

    t = (READNIBBLE(IOPIN1, COL0) < 15) ? 0 : 1;

    return t;
}

/*-------------------------------------------------------
  Detect Row Number
-------------------------------------------------------*/
u8 RowCheck(void)
{
    u8 r;

    for(r = 0; r < 4; r++)
    {
        // Ground one row at a time
        WRITENIBBLE(IOPIN1, ROW0, ~(1 << r));

        // Check whether key belongs to this row
        if(!ColScan())
            break;
    }

    // Reinitialize rows
    WRITENIBBLE(IOPIN1, ROW0, 0);

    return r;
}

/*-------------------------------------------------------
  Detect Column Number
-------------------------------------------------------*/
u8 ColCheck(void)
{
    u8 c;

    for(c = 0; c < 4; c++)
    {
        if(READBIT(IOPIN1, COL0 + c) == 0)
            break;
    }

    return c;
}

/*-------------------------------------------------------
  Scan Keypad and Return Pressed Key
-------------------------------------------------------*/
u8 KeyScan(void)
{
    u8 r, c, keyV;

    // Wait until key is pressed
    while(ColScan());

    // Find row and column
    r = RowCheck();
    c = ColCheck();

    // Get corresponding key from LUT
    keyV = kpmLUT[r][c];

    return keyV;
}

/*-------------------------------------------------------
  Read Numeric Value

  Used for menu selection.
-------------------------------------------------------*/
u32 ReadNum(void)
{
    u32 keyV, sum = 0;

    while(1)
    {
        keyV = KeyScan();

        delay_ms(150);

        if((keyV >= '0') && (keyV <= '9'))
        {
            sum = (sum * 10) + (keyV - '0');

            CmdLCD(GOTO_LINE2_POS0);
            U32LCD(sum);

            while(ColScan() == 0);
        }
        else
        {
            if(keyV == 'C')
            {
                CmdLCD(CLEAR_LCD);
            }

            while(ColScan() == 0);
            break;
        }
    }

    return sum;
}

/*-------------------------------------------------------
  Read Multi-Digit Number

  Used for:
  - RTC editing
  - Lab timing editing
-------------------------------------------------------*/
void ReadNum2(u32 *num, u8 *lastKey)
{
    u32 c = 0;

    while(1)
    {
        if(c > 3)
            break;

        *lastKey = KeyScan();

        delay_ms(150);

        if((*lastKey >= '0') && (*lastKey <= '9'))
        {
            *num = (*num * 10) + (*lastKey - '0');

            CmdLCD(GOTO_LINE2_POS0);

            c++;

            U32LCD(*num);

            while(ColScan() == 0);
        }
        else
        {
            if(*lastKey == 'C')
            {
                CmdLCD(CLEAR_LCD);
            }

            while(ColScan() == 0);
            break;
        }
    }
}

/*-------------------------------------------------------
  Read Password Digits

  Displays '*' instead of actual digits.
-------------------------------------------------------*/
void ReadNum3(u32 *num, u8 *lastKey)
{
    u32 c = 0;

    while(1)
    {
        if(c > 3)
            break;

        *lastKey = KeyScan();

        delay_ms(150);

        if((*lastKey >= '0') && (*lastKey <= '9'))
        {
            *num = (*num * 10) + (*lastKey - '0');

            CmdLCD(GOTO_LINE2_POS0 + c);

            c++;

            // Hide password digits
            CharLCD('*');

            while(ColScan() == 0);
        }
        else
        {
            if(*lastKey == 'C')
            {
                CmdLCD(CLEAR_LCD);
            }

            while(ColScan() == 0);
            break;
        }
    }
}
