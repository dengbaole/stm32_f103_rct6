#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "system.h"
#include "platform.h"


#define LED1_PORT 			GPIOB
#define LED1_PIN 			GPIO_Pin_5
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOB

#define LED2_PORT 			GPIOE
#define LED2_PIN 			GPIO_Pin_5
#define LED2_PORT_RCC		RCC_APB2Periph_GPIOE


void tim6_init(uint16_t per, uint16_t psc);
void tim7_init(uint16_t per, uint16_t psc);

#endif // !_BSP_TIMER_H
