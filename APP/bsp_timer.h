#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "system.h"
#include "platform.h"


void tim6_init(uint16_t per, uint16_t psc);
void tim7_init(uint16_t per, uint16_t psc);

#endif // !_BSP_TIMER_H
