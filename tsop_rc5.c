
#include <LPC214x.h>
#include "types.h"
#include "delay.h"
#include "tsop_rc5.h"

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
		timeout+= STEP_US;
    }
}

static u8 RC5_Receive(u32 *address,
                      u32 *command,
                      u32 *toggle)
{
    u32 data = 1;
    u32 i, level;

    // Move to the next bit
    delay_us(RC5_T - RC5_Q);

    for(i = 0; i < 13; i++)
    {
        level = RC5_PIN;

        if(!WaitEdge(level, RC5_T))
            return 0;

        delay_us(RC5_Q);

        // TSOP output is inverted
        if(RC5_PIN == 0)
            data = (data << 1) | 1;
        else
            data = (data << 1);

        delay_us(RC5_H);
    }

    // Validate second start bit
    if(((data >> 12) & 1) != 1)
        return 0;

    *toggle  = (data >> 11) & 1;
    *address = (data >> 6) & 0x1F;
    *command = data & 0x3F;

    return 1;
}

u8 TSOP1738_ReadFrame(u32 *address,
                      u32 *command,
                      u32 *toggle)
{	
	int status = 0;
    // Configure P0.20 as GPIO instead of its alternate peripheral function
    PINSEL1 &= ~(3 << 8);               // Set P0.20 to GPIO function (its function bits are 9:8 of PINSEL1)

    // Configure the TSOP receiver pin as input
    IO0DIR  &= ~(1 << TSOP_PIN);        // Make P0.20 an input (TSOP output)

    

    // No IR signal
    if(RC5_PIN == 1)
        return 0;

    // Decode received frame
    status = RC5_Receive(address, command, toggle);

    // Wait until transmission finishes
    WaitIdle();

    return status;
}
