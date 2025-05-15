#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "AllHeader.h"
#include "bitmap.h"

#define USE_HORIZONTAL 1  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
	#define LCD_W 80
	#define LCD_H 160

#else
	#define LCD_W 160
	#define LCD_H 80
#endif


// ������ͷ�ļ����������¶���
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



#define LCD_SCLK_Clr() GPIO_ResetBits(LCD_SPI_GPIO,LCD_SPI_PIN_SCK)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(LCD_SPI_GPIO,LCD_SPI_PIN_SCK)

#define LCD_MOSI_Clr() GPIO_ResetBits(LCD_SPI_GPIO,LCD_SPI_PIN_MOSI)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(LCD_SPI_GPIO,LCD_SPI_PIN_MOSI)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOC,LCD_RES_PIN)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOC,LCD_RES_PIN)

#define TFT_RS_CMD()   GPIO_ResetBits(GPIOC,LCD_DC_PIN)//DC
#define TFT_RS_DATA()   GPIO_SetBits(GPIOC,LCD_DC_PIN)

#define TFT_CS_LOW()   GPIO_ResetBits(GPIOD,LCD_CS_PIN)//CS
#define TFT_CS_HIGH()   GPIO_SetBits(GPIOD,LCD_CS_PIN)

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

#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 160
#define SPLIT_SCREEN 16   //DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN 需要整除
#define DISPLAY_BUF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN)
extern uint8_t display_buff[DISPLAY_BUF_SIZE];
extern uint8_t rx_buff[DISPLAY_BUF_SIZE+4];
extern uint8_t tx_buff[DISPLAY_BUF_SIZE+4];

void lcd_gpio_init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void tftSetWindows(u16 x1, u16 y1, u16 x2, u16 y2); //�������꺯��
void lcd_init(void);//LCD��ʼ��
void lcd_clear(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);
void LCD_ShowPicture2(u16 x, u16 y, const sBITMAP* pic);
void LCD_ShowPicture_test(u16 x, u16 y, uint32_t add);
void display_component(FLASH_sBITMAP_TABLE* bitmap_table);
#endif




