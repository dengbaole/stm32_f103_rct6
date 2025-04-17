#ifndef __BSP_SPI_LCD_H
#define __BSP_SPI_LCD_H
#include "platform.h"


//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB0
//              DC    接PB1
//              CS    接PA4
//				BL	  接PB10

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160


#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	    //灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1  00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111

#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 0


#define LCD_CTRLA   	  	GPIOA		//定义TFT数据端口





#define LCD_RST_PIN     	GPIO_Pin_1	//PB10--->>TFT --RST
#define LCD_DC_PIN         	GPIO_Pin_2	//PB11--->>TFT --RS/DC
#define LCD_CS_PIN        	GPIO_Pin_3  //MCU_PB11--->>TFT --CS/CE
#define LCD_BLK_PIN        	GPIO_Pin_4  //MCU_PB9--->>TFT --BL
//#define LCD_CS_PIN_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS_PIN)|(x ? LCD_CS_PIN:0)

//液晶控制口置1操作语句宏定义
#define	LCD_CS_PIN_SET  	LCD_CTRLA->BSRR=LCD_CS_PIN
#define	LCD_BLK_PIN_SET  	LCD_CTRLA->BSRR=LCD_BLK_PIN
#define	LCD_DC_PIN_SET  	LCD_CTRLA->BSRR=LCD_DC_PIN
#define	LCD_RST_PIN_SET  	LCD_CTRLA->BSRR=LCD_RST_PIN
//液晶控制口置0操作语句宏定义
#define	LCD_CS_PIN_CLR  	LCD_CTRLA->BRR=LCD_CS_PIN
#define	LCD_BLK_PIN_CLR  	LCD_CTRLA->BRR=LCD_BLK_PIN
#define	LCD_RST_PIN_CLR  	LCD_CTRLA->BRR=LCD_RST_PIN
#define	LCD_DC_CLR  	    LCD_CTRLA->BRR=LCD_DC_PIN

void Lcd_Init(void);
void Lcd_SetXY(u16 x, u16 y);
void Gui_DrawPoint(u16 x, u16 y, u16 Data);
unsigned int Lcd_ReadPoint(u16 x, u16 y);
void Lcd_Clear(u16 Color);
void my_showimage(uint8_t x, uint8_t y, const sBITMAP* p);
#endif
