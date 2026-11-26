// lcd_defines.h

#ifndef _LCD_DEFINES_H_
#define _LCD_DEFINES_H_

/*=======================================================
            HD44780 LCD COMMANDS
=======================================================*/

// Clear entire display and move cursor home
#define CLEAR_LCD            0x01

// Return cursor to home position
#define RET_CUR_HOME         0x02

// Cursor moves right after each character
#define SHIFT_CUR_RIGHT      0x06

// Cursor moves left after each character
#define SHIFT_CUR_LEFT       0x07

// Display OFF
#define DSP_OFF              0x08

// Display ON, Cursor OFF
#define DSP_ON_CUR_OFF       0x0C

// Display ON, Cursor ON
#define DSP_ON_CUR_ON        0x0E

// Display ON, Cursor Blinking
#define DSP_ON_CUR_BLINK     0x0F

// Shift display left
#define SHIFT_DISP_LEFT      0x10

// Shift display right
#define SHIFT_DISP_RIGHT     0x14

// 8-bit interface, 1-line display
#define MODE_8BIT_1LINE      0x30

// 4-bit interface, 1-line display
#define MODE_4BIT_1LINE      0x20

// 8-bit interface, 2-line display
#define MODE_8BIT_2LINE      0x38

// 4-bit interface, 2-line display
#define MODE_4BIT_2LINE      0x28

/*=======================================================
              LCD CURSOR POSITIONS
=======================================================*/

// First line starting position
#define GOTO_LINE1_POS0      0x80

// Second line starting position
#define GOTO_LINE2_POS0      0xC0

// Third line starting position
#define GOTO_LINE3_POS0      0x94

// Fourth line starting position
#define GOTO_LINE4_POS0      0xD4

// CGRAM start address (Custom Characters)
#define GOTO_CGRAM_START     0x40

/*=======================================================
                LCD CONNECTION PINS
=======================================================*/

// LCD Data bus connected to P0.8 - P0.15
#define LCD_DATA             8

// Register Select pin connected to P0.16
#define LCD_RS               16

// Read/Write pin connected to P0.17
#define LCD_RW               17

// Enable pin connected to P0.18
#define LCD_EN               18

#endif
