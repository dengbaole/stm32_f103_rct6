#ifndef __AllHeader_H
#define __AllHeader_H

#define DEBUG_USARTx USART1



//���ܿ���  1:�� 0:�ر�
#define LCD_SWITCH    1
#define IMU_SWITCH 		0
#define FLASH_SWITCH  0
#define RGB_SWITCH		1

#define DEBUG_SWITCH  1

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "stdint.h"
#include "stm32f10x.h"
#include "SysTick.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_pwr.h"
//#include "stm32f10x_bkp.h"

//#include "bsp.h"
//#include "bsp_common.h"
//#include "delay.h"

//#include "bsp_key.h"

//#include "bsp_icm20607.h"
//#include "IOI2C.h"


#include "lcd_init.h"

//#include "bsp_spi.h"
//#include "bsp_w25q64.h"

//#include "bsp_RGB.h"
//#include "bsp_usart.h"

//APP�ļ��µĺ���
/********�ⲿflash���*********/
u8 detcet_flash(void);
void flash_test(void);
void show_flash(void);

/**********IMU���************/
void imu_test(void);

extern unsigned char ICM_ADDRESS;

#endif


