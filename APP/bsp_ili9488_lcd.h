#ifndef  _BSP_ILI9488_LCD_H
#define _BSP_ILI9488_LCD_H

#include "platform.h"

#define LCD_D0_PIN  GPIO_Pin_14
#define LCD_D0_PORT GPIOD

#define LCD_D1_PIN  GPIO_Pin_15
#define LCD_D1_PORT GPIOD

#define LCD_D2_PIN  GPIO_Pin_0
#define LCD_D2_PORT GPIOD

#define LCD_D3_PIN  GPIO_Pin_1
#define LCD_D3_PORT GPIOD

#define LCD_D4_PIN  GPIO_Pin_7
#define LCD_D4_PORT GPIOE

#define LCD_D5_PIN  GPIO_Pin_8
#define LCD_D5_PORT GPIOE

#define LCD_D6_PIN  GPIO_Pin_9
#define LCD_D6_PORT GPIOE

#define LCD_D7_PIN  GPIO_Pin_10
#define LCD_D7_PORT GPIOE

#define LCD_D8_PIN  GPIO_Pin_11
#define LCD_D8_PORT GPIOE

#define LCD_D9_PIN  GPIO_Pin_12
#define LCD_D9_PORT GPIOE

#define LCD_D10_PIN  GPIO_Pin_13
#define LCD_D10_PORT GPIOE

#define LCD_D11_PIN  GPIO_Pin_14
#define LCD_D11_PORT GPIOE

#define LCD_D12_PIN  GPIO_Pin_15
#define LCD_D12_PORT GPIOE

#define LCD_D13_PIN  GPIO_Pin_8
#define LCD_D13_PORT GPIOD

#define LCD_D14_PIN  GPIO_Pin_9
#define LCD_D14_PORT GPIOD

#define LCD_D15_PIN  GPIO_Pin_10
#define LCD_D15_PORT GPIOD

#define LCD_NWE_PIN  GPIO_Pin_5
#define LCD_NWE_PORT GPIOD

#define LCD_NE4_PIN  GPIO_Pin_12
#define LCD_NE4_PORT GPIOG

#define LCD_A10_PIN  GPIO_Pin_0
#define LCD_A10_PORT GPIOG

#define LCD_NOE_PIN  GPIO_Pin_4
#define LCD_NOE_PORT GPIOD

#define LCD_BL_PIN  GPIO_Pin_0
#define LCD_BE_PORT GPIOB

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


typedef struct {
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;            //LCD 方向
} _tftlcd_data;

typedef struct {
	uint16_t LCD_CMD;
	uint16_t LCD_DATA;
} TFTLCD_TypeDef;
//LCD参数
extern _tftlcd_data tftlcd_data;	//管理LCD重要参数
//LCD的前端颜色和背景色
extern uint16_t  FRONT_COLOR;//前端颜色 默认红色
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

#define BL_ON()  GPIO_SetBits(GPIOB, GPIO_Pin_0)	//背光
#define	LCD_LED PBout(0) //LCD背光  PB0
#define TFTLCD_DIR	0	//0：竖屏  1：横屏  默认竖屏
#define TFTLCD_ILI9488
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)


typedef struct SBITMAP_TABLE {
	uint8_t x;
	uint8_t y;
	const sBITMAP* bitmap;
} SBITMAP_TABLE;

//芯片sram 64k
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 480
#define SPLIT_SCREEN 16   //DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN 需要整除
#define DISPLAY_BUF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN)
extern uint8_t display_buff[DISPLAY_BUF_SIZE];


void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd, u16 data);
void LCD_WriteData_Color(u16 color);

extern SBITMAP_TABLE default_component[25];
uint8_t set_display_component(uint8_t index, uint8_t x, uint8_t y, const sBITMAP* bitmap);
uint8_t display_num(uint8_t index, uint8_t x, uint8_t y, sBITMAP* bitmap, uint16_t num);
// 背景图放最前面
void display_component(SBITMAP_TABLE* bitmap_table);
void TFTLCD_Init(void); //初始化
void lcd_setwindows(u16 sx, u16 sy, u16 width, u16 height); //设置窗口
void LCD_Display_Dir(u8 dir);//设置屏幕显示方向
void screen_clear(void);//清屏
void LCD_Fill(u16 xState, u16 yState, u16 xEnd, u16 yEnd, u16 color); //填充单色
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16* color); //在指定区域内填充指定颜色块
void LCD_DrawPoint(u16 x, u16 y); //画点
void LCD_DrawFRONT_COLOR(u16 x, u16 y, u16 color); //指定颜色画点
u16 LCD_ReadPoint(u16 x, u16 y); //读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);//画线
void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2, u16 color); //指定颜色画线
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);//画十字标记
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);//画矩形
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r); //画圆
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode); //显示一个字符
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size); //显示一个数字
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode); //显示数字
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8* p); //显示字符串
void LCD_ShowFontHZ(u16 x, u16 y, u8* cn);//显示汉字
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high, u8* pic); //显示图片
void LCD_2ShowPicture(u16 x, u16 y,  const sBITMAP* bitmap);


#endif // !1
