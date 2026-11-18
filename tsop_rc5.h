#include "types.h"

void TSOP1738_Init(void);

u8 TSOP1738_ReadFrame(u32 *address,u32 *command,u32 *toggle);
					  