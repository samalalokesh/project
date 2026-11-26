// lcd.h

#ifndef _LCD_H_
#define _LCD_H_

#include "types.h"

/*-------------------------------------------------------
  Write Data Byte to LCD

  Sends 8-bit data directly to LCD data bus.
-------------------------------------------------------*/
void WriteLCD(u8 byte);

/*-------------------------------------------------------
  Send LCD Command

  Examples:
      CLEAR_LCD
      GOTO_LINE1_POS0
      GOTO_LINE2_POS0
-------------------------------------------------------*/
void CmdLCD(u8 cmd);

/*-------------------------------------------------------
  Initialize LCD

  Configures LCD in:
      8-bit mode
      2-line display
      Display ON
-------------------------------------------------------*/
void InitLCD(void);

/*-------------------------------------------------------
  Display Single Character

  Example:
      CharLCD('A');
-------------------------------------------------------*/
void CharLCD(u8 asciiVal);

/*-------------------------------------------------------
  Display String

  Example:
      StrLCD("WELCOME");
-------------------------------------------------------*/
void StrLCD(s8 *str);

/*-------------------------------------------------------
  Display Unsigned 32-bit Integer

  Example:
      U32LCD(1234);
-------------------------------------------------------*/
void U32LCD(u32 num);

/*-------------------------------------------------------
  Display Signed 32-bit Integer

  Example:
      S32LCD(-100);
-------------------------------------------------------*/
void S32LCD(s32 num);

/*-------------------------------------------------------
  Display Floating Point Number

  Parameters:
      fNum -> Float value
      nDP  -> Number of decimal places

  Example:
      F32LCD(12.345,2);

  LCD Output:
      12.34
-------------------------------------------------------*/
void F32LCD(f32 fNum, u8 nDP);

/*-------------------------------------------------------
  Build Custom Characters in CGRAM

  Parameters:
      p      -> Character pattern array
      nBytes -> Number of bytes

  Used for creating:
      - Symbols
      - Icons
      - Custom characters
-------------------------------------------------------*/
void BuildCGRAM(const u8 *p, u8 nBytes);

#endif
