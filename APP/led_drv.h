#ifndef __LED_DRV_H
#define __LED_DRV_H

#include "platform.h"


#define LEDB_PIN           GPIO_Pin_0
#define LEDR_PIN           GPIO_Pin_1
#define LEDG_PIN           GPIO_Pin_2



#define LED_PORT            GPIOC

#define LEDB_ON() GPIO_ResetBits(LED_PORT,LEDB_PIN)
#define LEDB_OFF() GPIO_SetBits(LED_PORT,LEDB_PIN)

#define LEDR_ON() GPIO_ResetBits(LED_PORT,LEDR_PIN)
#define LEDR_OFF() GPIO_SetBits(LED_PORT,LEDR_PIN)

#define LEDG_ON() GPIO_ResetBits(LED_PORT,LEDG_PIN)
#define LEDG_OFF() GPIO_SetBits(LED_PORT,LEDG_PIN)

void led_init(void);

#endif




