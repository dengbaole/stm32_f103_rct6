#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "AllHeader.h"
#include "bitmap.h"

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


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

#define LCD_BL_ON()  GPIO_ResetBits(GPIOC,LCD_BLK_PIN)//BLK
#define LCD_BL_OFF()  GPIO_SetBits(GPIOC,LCD_BLK_PIN)


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430
#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
#define LIGHTGREEN     	 0X841F
#define LGRAY 			 0XC618
#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12



void lcd_gpio_init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2); //设置坐标函数
void lcd_init(void);//LCD初始化
void lcd_clear(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);
void LCD_ShowPicture2(u16 x, u16 y, const sBITMAP* pic);
#endif




