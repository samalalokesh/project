#include <LPC214x.h> 
#include "defines.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "password.h"
#include "rtc.h"
#include "interrupt1.h"
#include "interrupt2.h"
#include "led.h"


/*================ RC5 SETTINGS ================*/

                  // TSOP output is connected to P0.20 (moved away from the LCD RS pin P0.16)



#define RC5_PIN  ((IO0PIN >> TSOP_PIN) & 1)


#define RC5_T   1778                    // Full RC5 bit time = 1778 us

#define RC5_H   889                     // Half bit time = 889 us (T/2)

#define RC5_Q   444                     // Quarter bit time = 444 us (T/4)

#define STEP_US 20                      // Polling step in us while waiting for an edge


/*================ EDGE / IDLE HELPERS ================*/


/* Wait until the line changes from 'lvl'. Returns 1 if the edge came within max_us, else 0 */

static unsigned char WaitEdge(unsigned int lvl, unsigned int max_us)

{

    unsigned int t = 0;                 // Elapsed time in us, starts at 0


    while(t < max_us)                   // Keep checking until the time limit is reached

    {

        if(RC5_PIN != lvl)              // Has the pin level changed from the starting level?

            return 1;                   // Yes: edge found, report success

        delay_us(STEP_US);              // No: wait one polling step (20 us)

        t += STEP_US;                   // Add the step to the elapsed time

    }

    return 0;                           // Time ran out with no edge: report failure

}


/* Wait until the line has stayed HIGH for 4 ms (frame is over) */

static void WaitIdle(void)

{

    unsigned int t = 0;                 // Time the line has been continuously HIGH


    while(t < 4000)                     // Loop until HIGH has lasted 4000 us

    {

        if(RC5_PIN == 0)                // Is the line LOW (IR burst still present)?

            t = 0;                      // Yes: restart the idle timer

        else                            // Line is HIGH

            t += STEP_US;               // Add one step to the idle time

        delay_us(STEP_US);              // Wait one polling step before checking again

    }

}


/*================ DUPLICATE FILTER =================*/


unsigned int last_address = 0;          // Address of the last accepted frame

unsigned int last_command = 0;          // Command of the last accepted frame

unsigned int last_toggle  = 0;          // Toggle bit of the last accepted frame

unsigned char previous_key_valid = 0;   // Flag: 1 once a frame has been stored


/*================ RC5 RECEIVE ================*/


/* Call right after the first falling edge (middle of start bit S1) */

unsigned char RC5_Receive(unsigned int *address,   // Output: 5-bit address

                          unsigned int *command,   // Output: 6-bit command

                          unsigned int *toggle)    // Output: toggle bit

{

    unsigned int data = 1;              // Frame data, starts with the start bit S1 = 1

    unsigned int i, lvl;                // i = bit counter, lvl = pin level before an edge


    delay_us(RC5_T - RC5_Q);            // Move from mid S1 to 0.75T, inside the first half of the next bit


    for(i = 0; i < 13; i++)             // Receive the remaining 13 bits (S2, toggle, 5 address, 6 command)

    {

        lvl = RC5_PIN;                  // Remember the current pin level


        if(!WaitEdge(lvl, RC5_T))       // Wait for the mid-bit transition (resync point)

            return 0;                   // No edge within one bit time: bad frame, give up


        delay_us(RC5_Q);                // Wait a quarter bit after the edge, into the stable second half


        if(RC5_PIN == 0)                // Sample the pin: LOW means 1 (TSOP output is inverted)

            data = (data << 1) | 1;     // Shift data left and add a 1

        else                            // Pin is HIGH, so the bit is 0

            data = (data << 1);         // Shift data left and add a 0


        delay_us(RC5_H);                // Wait half a bit to reach 0.75T of the next bit

    }


    if(((data >> 12) & 1) != 1)         // Check the second start bit S2 (bit 12), must be 1

        return 0;                       // Invalid start bit: reject the frame


    *toggle  = (data >> 11) & 1;        // Extract the toggle bit (bit 11)

    *address = (data >> 6)  & 0x1F;     // Extract the 5 address bits (bits 10-6)

    *command =  data        & 0x3F;     // Extract the 6 command bits (bits 5-0)

    return 1;                           // Report that a valid frame was received

}


/*================ MAIN ================*/


void rc5_remote()                          // Program entry point

{

    unsigned int address, command, toggle;   // Variables to hold the decoded values


    PINSEL1 &= ~(3 << 8);               // Set P0.20 to GPIO function (its function bits are 9:8 of PINSEL1)

    IO0DIR  &= ~(1 << TSOP_PIN);        // Make P0.20 an input (TSOP output)


             // Show the start-up banner on the LCD


    if(RC5_PIN == 0)                       // Run forever

    {
        if(RC5_Receive(&address, &command, &toggle))   // Decode one frame; true if it was valid

        {

            if(!(previous_key_valid &&          // Was there a previous key AND...

                 address == last_address &&     // ...same address AND...

                 command == last_command &&     // ...same command AND...

                 toggle  == last_toggle))       // ...same toggle? If so it is a repeat, so skip it

            {

                last_address = address;         // Save this address for the next comparison

                last_command = command;         // Save this command for the next comparison

                last_toggle  = toggle;          // Save this toggle bit for the next comparison

                previous_key_valid = 1;         // Mark that a stored key now exists

				switch(command){
					case 1:
						IOCLR0 = 1<<LED1;
						break;
					case 2:
						IOSET0 = 1<<LED1;
						break;
					case 3:
						IOCLR0 = 1<<LED2;
						break;
					case 4:
						IOSET0 = 1<<LED2;
						break;
					case 5:
						IOCLR0 = (1<<LED1)|(1<<LED2);
						break;
					case 6:
						IOSET0 = (1<<LED1)|(1<<LED2);
				}
            }

        }


        WaitIdle();                     // Wait until the line is HIGH for 4 ms so the same frame is not decoded twice

    }

 }

