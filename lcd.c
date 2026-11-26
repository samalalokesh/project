// lcd.c

#include <LPC21xx.h>
#include "lcd_defines.h"
#include "defines.h"
#include "types.h"
#include "delay.h"

/*-------------------------------------------------------
  Write a Byte to LCD

  - Selects write operation
  - Places data on data pins
  - Generates Enable pulse
-------------------------------------------------------*/
void WriteLCD(u8 byte)
{
    // LCD Write Mode (RW = 0)
    IOCLR0 = 1 << LCD_RW;

    // Put data on D0-D7
    WRITEBYTE(IOPIN0, LCD_DATA, byte);

    // Generate Enable Pulse
    IOSET0 = 1 << LCD_EN;
    delay_us(1);
    IOCLR0 = 1 << LCD_EN;

    delay_ms(2);
}

/*-------------------------------------------------------
  Send Command to LCD
-------------------------------------------------------*/
void CmdLCD(u8 cmd)
{
    // Select Command Register (RS = 0)
    IOCLR0 = 1 << LCD_RS;

    WriteLCD(cmd);
}

/*-------------------------------------------------------
  Initialize LCD

  LCD Mode:
  - 8-bit Interface
  - 2-Line Display
  - Cursor OFF
-------------------------------------------------------*/
void InitLCD(void)
{
    // Configure LCD pins as Output
    IODIR0 |= (0xFF << LCD_DATA) |
              (1 << LCD_RS) |
              (1 << LCD_RW) |
              (1 << LCD_EN);

    // Power On Delay
    delay_ms(15);

    // LCD Initialization Sequence
    CmdLCD(0x30);
    delay_ms(4);

    CmdLCD(0x30);
    delay_us(100);

    CmdLCD(0x30);

    // 8-bit, 2-line mode
    CmdLCD(MODE_8BIT_2LINE);

    // Display ON, Cursor OFF
    CmdLCD(DSP_ON_CUR_OFF);

    // Clear LCD
    CmdLCD(CLEAR_LCD);

    // Auto Increment Cursor
    CmdLCD(SHIFT_CUR_RIGHT);
}

/*-------------------------------------------------------
  Display One Character
-------------------------------------------------------*/
void CharLCD(u8 asciiVal)
{
    // Select Data Register (RS = 1)
    IOSET0 = 1 << LCD_RS;

    WriteLCD(asciiVal);
}

/*-------------------------------------------------------
  Display String
-------------------------------------------------------*/
void StrLCD(s8 *s)
{
    while(*s)
        CharLCD(*s++);
}

/*-------------------------------------------------------
  Display Unsigned Integer
-------------------------------------------------------*/
void U32LCD(u32 n)
{
    s32 i = 0;
    u8 a[10];

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        while(n > 0)
        {
            a[i++] = (n % 10) + '0';
            n /= 10;
        }

        for(--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}

/*-------------------------------------------------------
  Display Signed Integer
-------------------------------------------------------*/
void S32LCD(s32 n)
{
    if(n < 0)
    {
        CharLCD('-');
        n = -n;
    }

    U32LCD(n);
}

/*-------------------------------------------------------
  Display Floating Point Number

  fn  : Floating Value
  nDP : Number of Decimal Places

  Example:
      F32LCD(12.345,2);

  LCD Output:
      12.34
-------------------------------------------------------*/
void F32LCD(f32 fn, u8 nDP)
{
    u32 n, i;

    if(fn < 0.0)
    {
        CharLCD('-');
        fn = -fn;
    }

    n = fn;

    U32LCD(n);

    CharLCD('.');

    for(i = 0; i < nDP; i++)
    {
        fn = (fn - n) * 10;

        n = fn;

        CharLCD(n + '0');
    }
}

/*-------------------------------------------------------
  Build Custom Character in LCD CGRAM

  p       : Character Pattern Array
  nBytes  : Number of Bytes

  Used to Create User Defined Characters
-------------------------------------------------------*/
void BuildCGRAM(u8 *p, u8 nBytes)
{
    u32 i;

    // Move to CGRAM
    CmdLCD(GOTO_CGRAM_START);

    IOCLR0 = 1 << LCD_RW;
    IOSET0 = 1 << LCD_RS;

    for(i = 0; i < nBytes; i++)
    {
        WriteLCD(p[i]);
    }

    // Return to DDRAM
    CmdLCD(GOTO_LINE1_POS0);
}
