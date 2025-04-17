#include "bsp_ili9488_lcd.h"
#include "stdlib.h"
#include "font.h"
#include "SysTick.h"


//LCD的画笔颜色和背景色
// u16 FRONT_COLOR = BLACK;	//画笔颜色
// u16 BACK_COLOR = WHITE; //背景色
u16 FRONT_COLOR = WHITE;	//画笔颜色
u16 BACK_COLOR = BLACK; //背景色

_tftlcd_data tftlcd_data;
uint8_t display_buff[DISPLAY_BUF_SIZE];



//写寄存器函数
//cmd:寄存器值
void LCD_WriteCmd(u16 cmd) {
	TFTLCD->LCD_CMD = cmd;
}

//写数据
//data:要写入的值
void LCD_WriteData(u16 data) {
	TFTLCD->LCD_DATA = data;
}

void LCD_WriteCmdData(u16 cmd, u16 data) {
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}


u32 LCD_RGBColor_Change(u16 color) {
	u8 r, g, b = 0;

	r = (color >> 11) & 0x1f;
	g = (color >> 5) & 0x3f;
	b = color & 0x1f;

	return ((r << 13) | (g << 6) | (b << 1));
}
void LCD_WriteData_Color(u16 color) {
	// TFTLCD->LCD_DATA = color >> 8;
	// TFTLCD->LCD_DATA = color & 0xff;
	LCD_WriteData(color >> 8);
	LCD_WriteData(color & 0xff);
}

//读数据
//返回值:读到的值
u16 LCD_ReadData(void) {
	u16 ram;
	ram = TFTLCD->LCD_DATA;
	ram = ram << 8;
	ram |= TFTLCD->LCD_DATA;
	return ram;
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir) {
	tftlcd_data.dir = dir;       //横屏/竖屏
	if(dir == 0) { //默认竖屏方向
		LCD_WriteCmdData(0x36, 0x08);
		tftlcd_data.height = 480;
		tftlcd_data.width = 320;
	} else {
		LCD_WriteCmdData(0x36, 0x28);
		LCD_WriteCmd(0XB6);
		LCD_WriteData(0x00);
		LCD_WriteData(0x02);
		LCD_WriteData(0x3B);
		tftlcd_data.height = 320;
		tftlcd_data.width = 480;
	}
}


//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
void lcd_setwindows(u16 sx, u16 sy, u16 width, u16 height) {
	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx / 256);
	LCD_WriteData(sx % 256);
	LCD_WriteData(width / 256);
	LCD_WriteData(width % 256);
	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy / 256);
	LCD_WriteData(sy % 256);
	LCD_WriteData(height / 256);
	LCD_WriteData(height % 256);
	LCD_WriteCmd(0x2C);
}

//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x, u16 y) {
	u16 r = 0, g = 0, b = 0;
	u16 r1, r2, r3;
	u32 value;

	if(x >= tftlcd_data.width || y >= tftlcd_data.height) {
		return 0;	//超过了范围,直接返回
	}
	lcd_setwindows(x, y, x, y);
	LCD_WriteCmd(0X2e);
	r = LCD_ReadData();								//dummy Read
	r = LCD_ReadData();								//实际坐标颜色
	b = r >> 8;
	g = r << 8;
	r = g + b;
	//	printf("r=%X\r\n",r);
	//printf("g=%X\r\n",g);
	//printf("b=%X\r\n",b);
	return r;
}

void TFTLCD_GPIO_Init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能C端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOC
	GPIO_SetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能D端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //初始化GPIOD
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

void TFTLCD_FSMC_Init(void) {
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadTimingInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteTimingInitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);	//使能FSMC时钟

	FSMC_ReadTimingInitStructure.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
	FSMC_ReadTimingInitStructure.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到
	FSMC_ReadTimingInitStructure.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
	FSMC_ReadTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_ReadTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_ReadTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A

	FSMC_WriteTimingInitStructure.FSMC_AddressSetupTime = 0x15;	 //地址建立时间（ADDSET）为16个HCLK
	FSMC_WriteTimingInitStructure.FSMC_AddressHoldTime = 0x15;	 //地址保持时间
	FSMC_WriteTimingInitStructure.FSMC_DataSetupTime = 0x05;		 //数据保存时间为6个HCLK
	FSMC_WriteTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_WriteTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_WriteTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A


	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM; // FSMC_MemoryType_SRAM;  //SRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; // FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadTimingInitStructure; //读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTimingInitStructure;  //写时序

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1

}

//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void LCD_SSD_BackLightSet(u8 pwm) {
	LCD_WriteCmd(0xBE);	//配置PWM输出
	LCD_WriteData(0x05);	//1设置PWM频率
	LCD_WriteData(pwm * 2.55); //2设置PWM占空比
	LCD_WriteData(0x01);	//3设置C
	LCD_WriteData(0xFF);	//4设置D
	LCD_WriteData(0x00);	//5设置E
	LCD_WriteData(0x00);	//6设置F
}

void TFTLCD_Init(void) {
	u16 i;

	TFTLCD_GPIO_Init();
//	LCD_RES_Clr();//复位
//	delay_ms(100);
//	LCD_RES_Set();
//	delay_ms(100);
//	LCD_BLK_Clr();//打开背光
//	delay_ms(100);
//	LCD_WR_REG(0x11);     //Sleep out
//	delay_ms(120);                //Delay 120ms
//	LCD_WR_REG(0xB1);     //Normal mode
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_REG(0xB2);     //Idle mode
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_REG(0xB3);     //Partial mode
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_DATA8(0x3C);
//	LCD_WR_REG(0xB4);     //Dot inversion
//	LCD_WR_DATA8(0x03);
//	LCD_WR_REG(0xC0);     //AVDD GVDD
//	LCD_WR_DATA8(0xAB);
//	LCD_WR_DATA8(0x0B);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_REG(0xC1);     //VGH VGL
//	LCD_WR_DATA8(0xC5);   //C0
//	LCD_WR_REG(0xC2);     //Normal Mode
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_REG(0xC3);     //Idle
//	LCD_WR_DATA8(0x8D);
//	LCD_WR_DATA8(0x6A);
//	LCD_WR_REG(0xC4);     //Partial+Full
//	LCD_WR_DATA8(0x8D);
//	LCD_WR_DATA8(0xEE);
//	LCD_WR_REG(0xC5);     //VCOM
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_REG(0xE0);     //positive gamma
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x0E);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x10);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x02);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x09);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x25);
//	LCD_WR_DATA8(0x36);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x10);
//	LCD_WR_REG(0xE1);     //negative gamma
//	LCD_WR_DATA8(0x0A);
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x02);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x09);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x25);
//	LCD_WR_DATA8(0x35);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x09);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x10);
//	LCD_WR_REG(0xFC);
//	LCD_WR_DATA8(0x80);
//	LCD_WR_REG(0x3A);
//	LCD_WR_DATA8(0x05);
//	LCD_WR_REG(0x36);
//	if(USE_HORIZONTAL == 0)LCD_WR_DATA8(0x08);
//	else if(USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC8);
//	else if(USE_HORIZONTAL == 2)LCD_WR_DATA8(0x78);
//	else LCD_WR_DATA8(0xA8);
//	LCD_WR_REG(0x21);     //Display inversion
//	LCD_WR_REG(0x29);     //Display on
//	LCD_WR_REG(0x2A);     //Set Column Address
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x1A);  //26
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x69);   //105
//	LCD_WR_REG(0x2B);     //Set Page Address
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x01);    //1
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0xA0);    //160
//	LCD_WR_REG(0x2C);

//	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);	//初始化成白屏
//	screen_clear();
}

//清屏函数
//color:要清屏的填充色
void screen_clear(void) {
	uint16_t i, j ;
	memset(display_buff, 0x00, sizeof(display_buff));
	lcd_setwindows(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);	 //作用区域
	for(i = 0; i < DISPLAY_WIDTH; i++) {
		for (j = 0; j < DISPLAY_HEIGHT; j++) {
			LCD_WriteData_Color(display_buff[i + j]);
		}
	}
}


//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 xState, u16 yState, u16 xEnd, u16 yEnd, u16 color) {
	uint16_t temp;

	if((xState > xEnd) || (yState > yEnd)) {
		return;
	}
	lcd_setwindows(xState, yState, xEnd, yEnd);
	xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--) {
		temp = yState;
		while (temp--) {
			LCD_WriteData_Color(color);
		}
	}
}






SBITMAP_TABLE default_component[25];



uint8_t set_display_component(uint8_t index, uint8_t x, uint8_t y, const sBITMAP* bitmap) {
	uint8_t index_next = index + 1;
	default_component[index].x = x;
	default_component[index].y = y;
	default_component[index].bitmap = bitmap;
	return index_next;
}

//没有边界检测
uint8_t display_num(uint8_t index, uint8_t x, uint8_t y, sBITMAP* bitmap, uint16_t num) {
	uint8_t index_temp = index;
	if(num > 999) {
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 0, y, &bitmap[num / 1000 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 1, y, &bitmap[num / 100 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 2, y, &bitmap[num / 10 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 3, y, &bitmap[num % 10]);
	} else if(num > 99) {
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 0, y, &bitmap[num / 100 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 1, y, &bitmap[num / 10 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 2, y, &bitmap[num % 10]);
	} else if(num >= 0) {
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 0, y, &bitmap[num / 10 % 10]);
		index_temp = set_display_component(index_temp, x + (bitmap->w + 2) * 1, y, &bitmap[num % 10]);
	}

	return index_temp;
}


static uint8_t get_max(uint8_t a, uint8_t b) {
	return a > b ? a : b;
}

static uint8_t get_min(uint8_t a, uint8_t b) {
	return a < b ? a : b;
}

// 没有边界检测
// 背景图放最前面
void display_component(SBITMAP_TABLE* bitmap_table) {
	uint16_t current_screen_h_end = 0;
	uint16_t current_screen_h_start = 0;

	uint16_t picture_h_end = 0;
	uint16_t picture_h_start = 0;

	uint16_t draw_h_end = 0;
	uint16_t draw_h_start = 0;
	lcd_setwindows(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
	for(uint16_t j = 0; j < SPLIT_SCREEN; j++) {
		current_screen_h_start = j * DISPLAY_HEIGHT / SPLIT_SCREEN;
		current_screen_h_end += DISPLAY_HEIGHT / SPLIT_SCREEN; //display_buff 高度 0-4行


		memset(display_buff, 0x00, DISPLAY_BUF_SIZE);
		for(uint8_t i = 0; bitmap_table[i].bitmap != NULL; i++) {
			//在获取每张图片的数据

			//如果图片在屏幕内，则获取数据
			picture_h_start = bitmap_table[i].y;
			picture_h_end = bitmap_table[i].y + bitmap_table[i].bitmap->h;
			draw_h_start = get_max(current_screen_h_start, picture_h_start);
			draw_h_end = get_min(current_screen_h_end, picture_h_end);
			if(draw_h_start <= draw_h_end) {
				//获取这个图片一行数据并传入display_buff

				for(uint16_t k = 0; k < (draw_h_end - draw_h_start); k++) {
					// for(uint16_t x = 0; x < bitmap_table[i].bitmap->w; x++) {
					// 	// 计算源和目标索引
					// 	uint16_t src_index = k * bitmap_table[i].bitmap->w * 2 + x * 2;
					// 	uint16_t dst_index = (draw_h_start - current_screen_h_start + k) * DISPLAY_WIDTH * 2 + bitmap_table[i].x * 2 + x * 2;
					// 	// if (src_index + 1 >= DISPLAY_BUF_SIZE) {
					// 	// 	continue; // 跳过超出源缓冲区的像素
					// 	// }

					// 	// if (dst_index + 1 >= DISPLAY_BUF_SIZE) {
					// 	// 	continue; // 跳过超出目标缓冲区的像素
					// 	// }
					// 	// 读取源像素
					// 	// uint16_t pixel = rx_buff[src_index] | (rx_buff[src_index + 1] << 8);

					// 	// 检查是否为透明色
					// 	// if(pixel != 0x0000) {
					// 	// 复制像素到目标缓冲区
					// 	display_buff[dst_index] |= bitmap_table->bitmap->map[src_index];
					// 	display_buff[dst_index + 1] |= bitmap_table->bitmap->map[src_index + 1];
					// 	// }
					// }
				}

			}
		}
		for(uint16_t i = 0; i < DISPLAY_WIDTH; i++) {
			for (uint16_t j = 0; j < DISPLAY_HEIGHT; j++) {
				LCD_WriteData_Color(display_buff[i + j]);
			}
		}
	}
}
//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16* color) {
	u16 height, width;
	u16 i, j;
	width = ex - sx + 1; 			//得到填充的宽度
	height = ey - sy + 1;			//高度

	lcd_setwindows(sx, sy, ex, ey);
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			LCD_WriteData_Color(color[i * width + j]);
		}
	}
}
//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x, u16 y) {
	lcd_setwindows(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(FRONT_COLOR);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_DrawFRONT_COLOR(u16 x, u16 y, u16 color) {
	lcd_setwindows(x, y, x, y);
	LCD_WriteData_Color(color);
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2) {
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if(delta_x > 0)incx = 1; //设置单步方向
	else if(delta_x == 0)incx = 0; //垂直线
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if(delta_y > 0)incy = 1;
	else if(delta_y == 0)incy = 0; //水平线
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
	else distance = delta_y;
	for(t = 0; t <= distance + 1; t++ ) { //画线输出
		LCD_DrawPoint(uRow, uCol); //画点
		xerr += delta_x ;
		yerr += delta_y ;
		if(xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}

void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if(delta_x > 0)incx = 1; //设置单步方向
	else if(delta_x == 0)incx = 0; //垂直线
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if(delta_y > 0)incy = 1;
	else if(delta_y == 0)incy = 0; //水平线
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
	else distance = delta_y;
	for(t = 0; t <= distance + 1; t++ ) { //画线输出
		LCD_DrawFRONT_COLOR(uRow, uCol, color); //画点
		xerr += delta_x ;
		yerr += delta_y ;
		if(xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}


// 画一个十字的标记
// x：标记的X坐标
// y：标记的Y坐标
// color：标记的颜色
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color) {
	uint8_t i;

	/* 画点 */
	lcd_setwindows(x - 1, y - 1, x + 1, y + 1);
	for(i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}

	/* 画竖 */
	lcd_setwindows(x - 4, y, x + 4, y);
	for(i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}

	/* 画横 */
	lcd_setwindows(x, y - 4, x, y + 4);
	for(i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2) {
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);       //判断下个点位置的标志
	while(a <= b) {
		LCD_DrawPoint(x0 + a, y0 - b);        //5
		LCD_DrawPoint(x0 + b, y0 - a);        //0
		LCD_DrawPoint(x0 + b, y0 + a);        //4
		LCD_DrawPoint(x0 + a, y0 + b);        //6
		LCD_DrawPoint(x0 - a, y0 + b);        //1
		LCD_DrawPoint(x0 - b, y0 + a);
		LCD_DrawPoint(x0 - a, y0 - b);        //2
		LCD_DrawPoint(x0 - b, y0 - a);        //7
		a++;
		//使用Bresenham算法画圆
		if(di < 0)di += 4 * a + 6;
		else {
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}



//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode) {
	u8 temp, t1, t;
	u16 y0 = y;
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//得到字体一个字符对应点阵集所占的字节数
	num = num - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t = 0; t < csize; t++) {
		if(size == 12)temp = ascii_1206[num][t]; 	 	//调用1206字体
		else if(size == 16)temp = ascii_1608[num][t];	//调用1608字体
		else if(size == 24)temp = ascii_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1 = 0; t1 < 8; t1++) {
			if(temp & 0x80)LCD_DrawFRONT_COLOR(x, y, FRONT_COLOR);
			else if(mode == 0)LCD_DrawFRONT_COLOR(x, y, BACK_COLOR);
			temp <<= 1;
			y++;
			if(y >= tftlcd_data.height)return;		//超区域了
			if((y - y0) == size) {
				y = y0;
				x++;
				if(x >= tftlcd_data.width)return;	//超区域了
				break;
			}
		}
	}
}
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m, u8 n) {
	u32 result = 1;
	while(n--)result *= m;
	return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size) {
	u8 t, temp;
	u8 enshow = 0;
	for(t = 0; t < len; t++) {
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1)) {
			if(temp == 0) {
				LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0);
				continue;
			} else enshow = 1;

		}
		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0);
	}
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode) {
	u8 t, temp;
	u8 enshow = 0;
	for(t = 0; t < len; t++) {
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1)) {
			if(temp == 0) {
				if(mode & 0X80)LCD_ShowChar(x + (size / 2)*t, y, '0', size, mode & 0X01);
				else LCD_ShowChar(x + (size / 2)*t, y, ' ', size, mode & 0X01);
				continue;
			} else enshow = 1;

		}
		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, mode & 0X01);
	}
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8* p) {
	u8 x0 = x;
	width += x;
	height += y;
	while((*p <= '~') && (*p >= ' ')) { //判断是不是非法字符!
		if(x >= width) {
			x = x0;
			y += size;
		}
		if(y >= height)break; //退出
		LCD_ShowChar(x, y, *p, size, 0);
		x += size / 2;
		p++;
	}
}

/****************************************************************************
*函数名：LCD_ShowFontHZ
*输  入：x：汉字显示的X坐标
*      * y：汉字显示的Y坐标
*      * cn：要显示的汉字
*      * wordColor：文字的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写二号楷体汉字
****************************************************************************/
#if 0
void LCD_ShowFontHZ(u16 x, u16 y, u8* cn) {
	u8 i, j, wordNum;
	u16 color;
	while (*cn != '\0') {
		lcd_setwindows(x, y, x + 31, y + 28);
		for (wordNum = 0; wordNum < 20; wordNum++) {
			//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0] == *cn)
					&& (CnChar32x29[wordNum].Index[1] == *(cn + 1))) {
				for(i = 0; i < 116; i++) {
					//MSK的位数
					color = CnChar32x29[wordNum].Msk[i];
					for(j = 0; j < 8; j++) {
						if((color & 0x80) == 0x80) {
							LCD_WriteData_Color(FRONT_COLOR);
						} else {
							LCD_WriteData_Color(BACK_COLOR);
						}
						color <<= 1;
					}//for(j=0;j<8;j++)结束
				}
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 32;
	}
}
#endif


#if 1
void LCD_ShowFontHZ(u16 x, u16 y, u8* cn) {
	u8 i, j, wordNum;
	u16 color;
	u16 x0 = x;
	u16 y0 = y;
	while (*cn != '\0') {
		for (wordNum = 0; wordNum < 20; wordNum++) {
			//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0] == *cn)
					&& (CnChar32x29[wordNum].Index[1] == *(cn + 1))) {
				for(i = 0; i < 116; i++) {
					//MSK的位数
					color = CnChar32x29[wordNum].Msk[i];
					for(j = 0; j < 8; j++) {
						if((color & 0x80) == 0x80) {
							LCD_DrawFRONT_COLOR(x, y, FRONT_COLOR);
						} else {
							LCD_DrawFRONT_COLOR(x, y, BACK_COLOR);
						}
						color <<= 1;
						x++;
						if((x - x0) == 32) {
							x = x0;
							y++;
							if((y - y0) == 29) {
								y = y0;
							}
						}
					}//for(j=0;j<8;j++)结束
				}
			}

		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 32;
		x0 = x;
	}
}
#endif

void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high, u8* pic) {
	u16 i, j;
	u16 temp = 0;
	long tmp = 0, num = 0;
	lcd_setwindows(x, y, x + wide - 1, y + high - 1);
	num = wide * high * 2 ;
	//方法1：提高显示速度，太快可能导致图像显示不全
	//	do
	//	{
	//		temp = pic[tmp + 1];
	//		temp = temp << 8;
	//		temp = temp | pic[tmp];
	//		LCD_WriteData_Color(temp);//逐点显示
	//		tmp += 2;
	//	}
	//	while(tmp < num);

	//方法2：可有效消除LCD显示速度过快导致显示不全问题
	for(i = 0; i < high; i++) {
		for(j = 0; j < wide; j++) {
			temp = pic[tmp + 1];
			temp = temp << 8;
			temp = temp | pic[tmp];
			LCD_DrawFRONT_COLOR(x + j, y + i, temp);
			tmp += 2;
		}
	}
}

void LCD_2ShowPicture(u16 x, u16 y,  const sBITMAP* bitmap) {
	u16 i, j;
	u16 temp = 0;
	long tmp = 0, num = 0;
	lcd_setwindows(x, y, x + bitmap->w - 1, y + bitmap->h - 1);
	num = bitmap->w * bitmap->h * 2 ;
	//方法1：提高显示速度，太快可能导致图像显示不全
	do {
		temp = bitmap->map[tmp + 1];
		temp = temp << 8;
		temp = temp | bitmap->map[tmp];
		LCD_WriteData_Color(temp);//逐点显示
		tmp += 2;
	} while(tmp < num);

	//方法2：可有效消除LCD显示速度过快导致显示不全问题
	// for(i = 0; i < high; i++) {
	// 	for(j = 0; j < (wide*2); j++) {
	// 		LCD_WriteData(bitmap.map[i + j]);
	// 	}
	// }
}

