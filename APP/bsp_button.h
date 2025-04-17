#ifndef BSP_BUTTON_H_
#define BSP_BUTTON_H_
#include "platform.h"


//按键引脚配置
#define KEYR_PIN        GPIO_Pin_4
#define KEYR_PORT       GPIOE
#define KEYR_SPEED      GPIO_Speed_0MHz
#define KEYR_MODE       GPIO_Mode_IPU
#define KEYR_GPIO_CLK   RCC_APB2Periph_GPIOE
//中断
#define KEYR_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEYR_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEYR_INT_EXTI_LINE         EXTI_Line4
#define KEYR_INT_EXTI_IRQ          EXTI4_IRQn
#define KEYR_IRQHandler            EXTI4_IRQHandler


#define KEYD_PIN        GPIO_Pin_3
#define KEYD_PORT       GPIOE
#define KEYD_SPEED      GPIO_Speed_0MHz
#define KEYD_MODE       GPIO_Mode_IPU
#define KEYD_GPIO_CLK   RCC_APB2Periph_GPIOE
//中断
#define KEYD_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEYD_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEYD_INT_EXTI_LINE         EXTI_Line3
#define KEYD_INT_EXTI_IRQ          EXTI3_IRQn
#define KEYD_IRQHandler            EXTI3_IRQHandler

#define KEYL_PIN        GPIO_Pin_2
#define KEYL_PORT       GPIOE
#define KEYL_SPEED      GPIO_Speed_0MHz
#define KEYL_MODE       GPIO_Mode_IPU
#define KEYL_GPIO_CLK   RCC_APB2Periph_GPIOE
//中断
#define KEYL_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEYL_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEYL_INT_EXTI_LINE         EXTI_Line2
#define KEYL_INT_EXTI_IRQ          EXTI2_IRQn
#define KEYL_IRQHandler            EXTI2_IRQHandler

#define KEYUP_PIN       GPIO_Pin_0
#define KEYUP_PORT      GPIOA
#define KEYUP_SPEED     GPIO_Speed_0MHz
#define KEYUP_MODE      GPIO_Mode_IPD
#define KEYUP_GPIO_CLK  RCC_APB2Periph_GPIOA
//中断
#define KEYUP_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEYUP_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEYUP_INT_EXTI_LINE         EXTI_Line0
#define KEYUP_INT_EXTI_IRQ          EXTI0_IRQn
#define KEYUP_IRQHandler            EXTI0_IRQHandler




void button_init(void);

#endif // !_BUTTON_H_

