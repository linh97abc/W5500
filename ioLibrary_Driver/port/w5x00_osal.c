#include <ucos_ii.h>
#include <w5x00_osal.h>

void w5x00_delay(uint32_t milisec)
{
    OSTimeDlyHMSM(0, 0, 0, milisec);
}

void w5x00_yield()
{
    OSTimeDly(1);
}
