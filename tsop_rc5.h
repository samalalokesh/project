// tsop_rc5.h

#ifndef _TSOP_RC5_H_
#define _TSOP_RC5_H_

#include "types.h"

/*-------------------------------------------------------
  TSOP1738 Initialization

  Configures TSOP IR receiver pin as input.
-------------------------------------------------------*/
void TSOP1738_Init(void);

/*-------------------------------------------------------
  Read RC5 Frame

  Parameters:
      address -> RC5 device address
      command -> RC5 command value
      toggle  -> RC5 toggle bit

  Return:
      0 -> Frame received successfully
      1 -> Error / Invalid frame

  RC5 Frame Format:
      S1 S2 T A4 A3 A2 A1 A0 C5 C4 C3 C2 C1 C0
-------------------------------------------------------*/
u8 TSOP1738_ReadFrame(u32 *address,
                      u32 *command,
                      u32 *toggle);

/*-------------------------------------------------------
  Hardware Connection
-------------------------------------------------------*/

// TSOP1738 Output connected to P0.20
#define TSOP_PIN 20

/*-------------------------------------------------------
  Read TSOP Input Pin
-------------------------------------------------------*/
#define RC5_PIN ((IO0PIN >> TSOP_PIN) & 1)

/*-------------------------------------------------------
  RC5 Timing Constants (Microseconds)

  T  = Full Bit Time
  H  = Half Bit Time
  Q  = Quarter Bit Time
-------------------------------------------------------*/
#define RC5_T   1778    // 1.778 ms
#define RC5_H   889     // Half bit period
#define RC5_Q   444     // Quarter bit period

/*-------------------------------------------------------
  Sampling Resolution

  Delay step used while decoding RC5 data.
-------------------------------------------------------*/
#define STEP_US 20

#endif
