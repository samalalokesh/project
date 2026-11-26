// interrupt2.h

#ifndef _INTERRUPT2_H_
#define _INTERRUPT2_H_

/*-------------------------------------------------------
  EINT1 Initialization Function
  Configures External Interrupt 1 (SW2)
-------------------------------------------------------*/
void Init_eint2(void);

/*-------------------------------------------------------
  External Interrupt 1 ISR

  Used for Emergency Operation:
  - Sets emergency flag
  - Activates buzzer
  - Turns OFF LEDs
-------------------------------------------------------*/
void int_sw2(void) __irq;

/*-------------------------------------------------------
  LPC2148 Interrupt Definitions
-------------------------------------------------------*/

// VIC channel number for EINT1
#define INTERRUPT_CHANNEL2 15

// PINSEL value for EINT1 (P0.3)
#define EINT1 0x000000C0

#endif
