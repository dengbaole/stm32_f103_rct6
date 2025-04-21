#include "led_drv.h"

void led_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LEDB_PIN | LEDR_PIN | LEDG_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	LEDB_ON();
	LEDR_ON();
	LEDG_ON();
}