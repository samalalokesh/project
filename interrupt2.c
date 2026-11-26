// interrupt2.c

#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "RTC.h"
#include "delay.h"
#include "interrupt2.h"
#include <LPC214x.h>

// External Interrupt 1 ISR Declaration
void int_sw2(void) __irq;

/*-------------------------------------------------------
  Initialize External Interrupt 1 (EINT1)

  P0.3 --> EINT1

  Used for Emergency Switch (SW2)
-------------------------------------------------------*/
void Init_eint2()
{
    /* Configure P0.3 as EINT1 */

    PINSEL0 &= ~(3 << (3 * 2));   // Clear P0.3 bits
    PINSEL0 |= EINT1;             // Set P0.3 as EINT1

    /* Configure interrupt as IRQ */

    VICIntSelect &= ~(1 << INTERRUPT_CHANNEL2);

    /* Assign ISR to Vector Slot 1 */

    VICVectCntl1 = (1 << 5) | INTERRUPT_CHANNEL2;
    VICVectAddr1 = (unsigned)int_sw2;

    /* Enable EINT1 interrupt */

    VICIntEnable |= (1 << INTERRUPT_CHANNEL2);

    /* Clear any pending interrupt */

    EXTINT = (1 << 1);

    /* Edge-triggered interrupt */

    EXTMODE = (1 << 1);

    /* Falling-edge trigger */

    EXTPOLAR = 0;
}
