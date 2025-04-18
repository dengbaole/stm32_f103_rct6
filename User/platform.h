#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "system.h"
#include "SysTick.h"
#include <stdint.h>
#include <stdbool.h>

#include "uevent.h"
#include "even.h"
//#include "bsp_led.h"
//#include "bsp_button.h"
//#include "bsp_usart.h"
//#include "touch.h"
//#include "bsp_beep.h"
//#include "bsp_i2c_ee.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_ws2812.h"
//#include "bsp_dma.h"
//#include "SysTick.h"
//#include "bitmap.h"
//#include "bsp_spi.h"
//#include "stm32f10x.h"
//#include "bsp_timer.h"
//#include "assets.h"
//#include "bsp_ili9488_lcd.h"

#define ON  1
#define OFF 0



//事件定义
#define UEVT_APP_BASE (0xAE00)
// 上电信号
#define UEVT_APP_POWERUP (UEVT_APP_BASE | 0x01)
// Boot阶段
#define UEVT_APP_BOOT (UEVT_APP_BASE | 0x0B)
// 初始化段
#define UEVT_APP_SETUP (UEVT_APP_BASE | 0x02)
// 多任务开始
#define UEVT_APP_START (UEVT_APP_BASE | 0x03)
// 准备休眠信号
#define UEVT_APP_BEFORE_SLEEP (UEVT_APP_BASE | 0x0E)

// mark 按键事件
#define UEVT_RTC_BASE (0x0000)
#define UEVT_RTC_1MS (UEVT_RTC_BASE | 0x03)
#define UEVT_RTC_10MS (UEVT_RTC_BASE | 0x04)
#define UEVT_RTC_100MS (UEVT_RTC_BASE | 0x05)

#define UEVT_BTN_BASE (0x0100)
#define UEVT_BTN_DOWN_DOWN (UEVT_BTN_BASE | 0x01)
#define UEVT_BTN_DOWN_RELEASE (UEVT_BTN_BASE | 0x02)
#define UEVT_BTN_DOWN_LONG (UEVT_BTN_BASE | 0x03)
#define UEVT_BTN_DOWN_REPEAT (UEVT_BTN_BASE | 0x04)

#define UEVT_BTN_UP_DOWN (UEVT_BTN_BASE | 0x11)
#define UEVT_BTN_UP_RELEASE (UEVT_BTN_BASE | 0x12)
#define UEVT_BTN_UP_LONG (UEVT_BTN_BASE | 0x13)
#define UEVT_BTN_UP_REPEAT (UEVT_BTN_BASE | 0x14)


#define UEVT_BTN_RIGHT_DOWN (UEVT_BTN_BASE | 0x21)
#define UEVT_BTN_RIGHT_RELEASE (UEVT_BTN_BASE | 0x22)
#define UEVT_BTN_RIGHT_LONG_WORK (UEVT_BTN_BASE | 0x23)
#define UEVT_BTN_RIGHT_REPEAT (UEVT_BTN_BASE | 0x24)

#define UEVT_BTN_LEFT_DOWN (UEVT_BTN_BASE | 0x31)
#define UEVT_BTN_LEFT_RELEASE (UEVT_BTN_BASE | 0x32)
#define UEVT_BTN_LEFT_LONG_WORK (UEVT_BTN_BASE | 0x33)
#define UEVT_BTN_LEFT_REPEAT (UEVT_BTN_BASE | 0x34)

// 组合按键事件
#define UEVT_BTN_UPDOWN_DOWN (UEVT_BTN_BASE | 0x41)
#define UEVT_BTN_UPDOWN_LONG (UEVT_BTN_BASE | 0x43)
#define UEVT_BTN_UPMAIN_DOWN (UEVT_BTN_BASE | 0x45)
#define UEVT_BTN_DOWNMAIN_DOWN (UEVT_BTN_BASE | 0x46)


// 在触发工作之前松开为一次超短按事件
// 超短按事件发生后250ms内没有按键，则触发HIT-N事件
// 如果有按键，则触发HIT计数增加
#define UEVT_BTN_RIGHT_HIT0 (UEVT_BTN_BASE | 0x30)
#define UEVT_BTN_RIGHT_HIT1 (UEVT_BTN_RIGHT_HIT0 + 1)
#define UEVT_BTN_RIGHT_HIT2 (UEVT_BTN_RIGHT_HIT0 + 2)
#define UEVT_BTN_RIGHT_HIT3 (UEVT_BTN_RIGHT_HIT0 + 3)
#define UEVT_BTN_RIGHT_HIT4 (UEVT_BTN_RIGHT_HIT0 + 4)
#define UEVT_BTN_RIGHT_HIT5 (UEVT_BTN_RIGHT_HIT0 + 5)
#define UEVT_BTN_RIGHT_HIT6P (UEVT_BTN_RIGHT_HIT0 + 6)

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

extern void quickly_gpioinit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode, uint32_t GPIO_CLOCK);

//函数声明

extern uint8_t time_s;
extern uint8_t time_m;
extern uint8_t time_h;
extern uint8_t beep_on_times;


void platform_init(void);

#endif // !_PLATFORM_H_

