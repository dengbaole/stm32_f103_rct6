#include "platform.h"


uint8_t time_s = 0;
uint8_t time_m = 0;
uint8_t time_h = 0;
uint8_t beep_on_times = 3;


void quickly_gpioinit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode, uint32_t GPIO_CLOCK) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed;


	//时钟配置
	RCC_APB2PeriphClockCmd(GPIO_CLOCK, ENABLE);
	//引脚初始化.
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}



void platform_init(void) {
	// SysTick_Init(72);
	// SysTick_Config(SystemCoreClock / 100); //开启内核定时器1ms
	// tim6_init(36 - 1, 100); //100us
	// tim7_init(7200 - 1, 100);//10ms
	// led_init();
	// BEEP_Init();
	// usart_Config(115200);
	// printf("app_boot\n");
}
