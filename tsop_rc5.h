#include "types.h"

void TSOP1738_Init(void);

u8 TSOP1738_ReadFrame(u32 *address,u32 *command,u32 *toggle);



#define TSOP_PIN 20
#define RC5_PIN ((IO0PIN >> TSOP_PIN) & 1)

#define RC5_T   1778
#define RC5_H   889
#define RC5_Q   444
#define STEP_US 20
