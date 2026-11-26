// tsop_rc5.c

#include <LPC214x.h>
#include "types.h"
#include "delay.h"
#include "tsop_rc5.h"

/*-------------------------------------------------------
  Wait for Signal Transition (Edge Detection)

  level  : Current logic level
  max_us : Maximum waiting time in microseconds

  Returns:
      1 -> Edge detected
      0 -> Timeout occurred
-------------------------------------------------------*/
static u8 WaitEdge(u32 level, u32 max_us)
{
    u32 t = 0;

    while(t < max_us)
    {
        if(RC5_PIN != level)
            return 1;

        delay_us(STEP_US);
        t += STEP_US;
    }

    return 0;
}

/*-------------------------------------------------------
  Wait Until RC5 Transmission Completes

  Ensures receiver is idle before accepting
  another RC5 frame.
-------------------------------------------------------*/
static void WaitIdle(void)
{
    u32 t = 0;
    u32 timeout = 0;

    while(t < 4000 && timeout < 20000)
    {
        if(RC5_PIN == 0)
            t = 0;
        else
            t += STEP_US;

        delay_us(STEP_US);
        timeout += STEP_US;
    }
}

/*-------------------------------------------------------
  Receive One RC5 Frame

  RC5 Frame Format:

  S1 S2 T A4 A3 A2 A1 A0 C5 C4 C3 C2 C1 C0

  S1,S2  -> Start Bits
  T      -> Toggle Bit
  A4-A0  -> Address
  C5-C0  -> Command
-------------------------------------------------------*/
static u8 RC5_Receive(u32 *address,
                      u32 *command,
                      u32 *toggle)
{
    u32 data = 1;
    u32 i, level;

    // Move sampling point to next bit center
    delay_us(RC5_T - RC5_Q);

    for(i = 0; i < 13; i++)
    {
        level = RC5_PIN;

        // Wait for Manchester transition
        if(!WaitEdge(level, RC5_T))
            return 0;

        delay_us(RC5_Q);

        // TSOP output is active LOW (inverted)
        if(RC5_PIN == 0)
            data = (data << 1) | 1;
        else
            data = (data << 1);

        delay_us(RC5_H);
    }

    // Verify second start bit
    if(((data >> 12) & 1) != 1)
        return 0;

    // Extract RC5 fields
    *toggle  = (data >> 11) & 1;
    *address = (data >> 6) & 0x1F;
    *command = data & 0x3F;

    return 1;
}

/*-------------------------------------------------------
  Read RC5 Frame from TSOP1738

  Parameters:
      address -> Device Address
      command -> Command Value
      toggle  -> Toggle Bit

  Returns:
      1 -> Valid RC5 Frame Received
      0 -> No Frame / Invalid Frame
-------------------------------------------------------*/
u8 TSOP1738_ReadFrame(u32 *address,
                      u32 *command,
                      u32 *toggle)
{
    int status = 0;

    // Configure P0.20 as GPIO
    PINSEL1 &= ~(3 << 8);

    // Configure TSOP pin as Input
    IO0DIR &= ~(1 << TSOP_PIN);

    // No IR signal present
    if(RC5_PIN == 1)
        return 0;

    // Decode RC5 frame
    status = RC5_Receive(address,
                         command,
                         toggle);

    // Wait until frame transmission ends
    WaitIdle();

    return status;
}
