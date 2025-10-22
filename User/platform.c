#include "platform.h"


uint8_t time_s = 0;
uint8_t time_m = 0;
uint8_t time_h = 0;
uint8_t beep_on_times = 3;



#define LCD_W 80
#define LCD_H 160



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

// 假设使用STM32F103系列，配置GPIOA、GPIOB、GPIOC所有引脚为模拟输入

#include "stm32f10x.h"

#include "stm32f10x.h"

void GPIO_All_AnalogInput(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能所有GPIO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOD |
						   RCC_APB2Periph_GPIOE, ENABLE);

	// 配置GPIOA所有引脚为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置GPIOB所有引脚为模拟输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置GPIOC所有引脚为模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 配置GPIOD所有引脚为模拟输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// 配置GPIOE所有引脚为模拟输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void GPIO_All_FloatingInput(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能所有GPIO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOD |
						   RCC_APB2Periph_GPIOE, ENABLE);

	// 配置GPIOA所有引脚为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置GPIOB所有引脚为浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置GPIOC所有引脚为浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 配置GPIOD所有引脚为浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// 配置GPIOE所有引脚为浮空输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void GPIO_All_PullDownInput(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能所有GPIO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOD |
						   RCC_APB2Periph_GPIOE, ENABLE);

	// 配置GPIOA所有引脚为下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置GPIOB所有引脚为下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置GPIOC所有引脚为下拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 配置GPIOD所有引脚为下拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// 配置GPIOE所有引脚为下拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void platform_init(void) {
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Init(72);
	// SysTick_Config(SystemCoreClock / 100); //开启内核定时器1ms
	GPIO_All_PullDownInput();
	//	 while(1){
	//	 	;
	//	 }
	tim6_init(36 - 1, 100); //100us
	tim7_init(720 - 1, 100);//1ms
	// led_init();
	// BEEP_Init();
	// usart_Config(115200);
	// printf("app_boot\n");
}
