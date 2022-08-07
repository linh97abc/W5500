#ifndef _W5X00_OSAL_H__
#define _W5X00_OSAL_H__

#include <stdint.h>

#define W5X00_DELAY_POLL 100

void w5x00_delay(uint32_t milisec);
void w5x00_yield();

#endif
