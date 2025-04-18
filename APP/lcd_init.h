#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "AllHeader.h"

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
	#define LCD_W 80
	#define LCD_H 160

#else
	#define LCD_W 160
	#define LCD_H 80
#endif


// 首先在头文件中添加以下定义
#define LCD_SPI               SPI2
#define LCD_SPI_CLK           RCC_APB1Periph_SPI2
#define LCD_SPI_GPIO          GPIOB
#define LCD_SPI_GPIO_CLK      RCC_APB2Periph_GPIOB
#define LCD_SPI_PIN_MOSI      GPIO_Pin_15
#define LCD_SPI_PIN_SCK       GPIO_Pin_13

#define LCD_SPI               SPI2
#define LCD_SPI_CLK           RCC_APB1Periph_SPI2
#define LCD_SPI_GPIO          GPIOB
#define LCD_SPI_GPIO_CLK      RCC_APB2Periph_GPIOB
#define LCD_SPI_PIN_MOSI      GPIO_Pin_15
#define LCD_SPI_PIN_SCK       GPIO_Pin_13

#define LCD_CS_PIN           GPIO_Pin_2  // PD2
#define LCD_DC_PIN           GPIO_Pin_12  // PC12
#define LCD_RES_PIN          GPIO_Pin_11  // PC11
#define LCD_BLK_PIN          GPIO_Pin_10 // PC10

// DMA配置
#define LCD_DMA              DMA1
#define LCD_DMA_CLK          RCC_AHBPeriph_DMA1
#define LCD_DMA_TX_CHANNEL   DMA1_Channel5
#define LCD_DMA_IRQn         DMA1_Channel5_IRQn
#define LCD_DMA_IRQHandler   DMA1_Channel5_IRQHandler


#define LCD_SCLK_Clr() GPIO_ResetBits(LCD_SPI_GPIO,LCD_SPI_PIN_SCK)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(LCD_SPI_GPIO,LCD_SPI_PIN_SCK)

#define LCD_MOSI_Clr() GPIO_ResetBits(LCD_SPI_GPIO,LCD_SPI_PIN_MOSI)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(LCD_SPI_GPIO,LCD_SPI_PIN_MOSI)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOC,LCD_RES_PIN)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOC,LCD_RES_PIN)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOC,LCD_DC_PIN)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOC,LCD_DC_PIN)

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOD,LCD_CS_PIN)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOD,LCD_CS_PIN)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOC,LCD_BLK_PIN)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOC,LCD_BLK_PIN)



void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2); //设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




