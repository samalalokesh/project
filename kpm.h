// kpm.h

#ifndef _KPM_H_
#define _KPM_H_

#include "types.h"

/*-------------------------------------------------------
  Initialize Keypad Module

  Configures keypad row and column pins.
-------------------------------------------------------*/
void InitKPM(void);

/*-------------------------------------------------------
  Scan Keypad Columns

  Returns:
      Active column number
-------------------------------------------------------*/
u8 ColScan(void);

/*-------------------------------------------------------
  Check Keypad Rows

  Returns:
      Active row number
-------------------------------------------------------*/
u8 RowCheck(void);

/*-------------------------------------------------------
  Check Keypad Columns

  Returns:
      Active column number
-------------------------------------------------------*/
u8 ColCheck(void);

/*-------------------------------------------------------
  Scan Complete Keypad

  Returns:
      Pressed key value
-------------------------------------------------------*/
u8 KeyScan(void);

/*-------------------------------------------------------
  Read Numeric Value From Keypad

  Returns:
      Entered number as unsigned integer

  Example:
      1234
-------------------------------------------------------*/
u32 ReadNum(void);

/*-------------------------------------------------------
  Read Multi-digit Number

  Parameters:
      num  -> Stores entered number
      last -> Stores last key pressed

  Used for:
      RTC editing
      Lab timing modification
-------------------------------------------------------*/
void ReadNum2(u32 *num, u8 *last);

/*-------------------------------------------------------
  Note:
  Your password.c uses:

      ReadNum3(&pwd,&lastKey);

  So kpm.h should also contain:
-------------------------------------------------------*/

void ReadNum3(u32 *num, u8 *last);

#endif
