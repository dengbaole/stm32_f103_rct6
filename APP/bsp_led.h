#ifndef _bsp_led_H
#define _bsp_led_H
#include "platform.h"

//LED引脚配置
#define LEDR_PIN       GPIO_Pin_5
#define LEDR_PORT      GPIOB
#define LEDR_SPEED     GPIO_Speed_50MHz
#define LEDR_MODE      GPIO_Mode_Out_PP
#define LEDR_GPIO_CLK  RCC_APB2Periph_GPIOB

// PE5引脚复用为5*5矩阵按键驱动
#define LEDG_PIN       GPIO_Pin_5
#define LEDG_PORT      GPIOE
#define LEDG_SPEED     GPIO_Speed_50MHz
#define LEDG_MODE      GPIO_Mode_Out_PP
#define LEDG_GPIO_CLK  RCC_APB2Periph_GPIOE

extern volatile uint8_t g_sort_pwm;
extern volatile uint8_t r_sort_pwm;
extern  const uint8_t spwmWave[200];
extern const uint8_t npwmWave[200];
void led_init(void);
void setledg_pwm(uint8_t p);
void setledr_pwm(uint8_t p);

#define LED_G(a)  if(a){\
		GPIO_ResetBits(LEDG_PORT,LEDG_PIN);}\
	else{\
		GPIO_SetBits(LEDG_PORT,LEDG_PIN);}
#define LED_R(a)  if(a){\
		GPIO_ResetBits(LEDR_PORT,LEDR_PIN);}\
	else{\
		GPIO_SetBits(LEDR_PORT,LEDR_PIN);}


#endif
