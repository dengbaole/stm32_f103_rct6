#include "bsp_spi_lcd.h"


static void my_delay_ms(u16 time) {
	u16 i = 0;
	while(time--) {
		i = 12000;
		while(i--);
	}
}
//液晶IO初始化配置 //RST：复位 DC：数据命令选择 CS:片选选择
static void LCD_GPIO_Init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN | LCD_DC_PIN | LCD_CS_PIN | LCD_BLK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//向液晶屏写一个8位指令
void tft_write_cmd(uint8_t Index) {
	//SPI 写命令时序开始
	LCD_CS_PIN_CLR;
	LCD_DC_CLR;
	SPI1_ReadWriteByte(Index);
	LCD_CS_PIN_SET;
}

//向液晶屏写一个8位数据
void tft_write_data(u8 Data) {
	LCD_CS_PIN_CLR;
	LCD_DC_PIN_SET;
	SPI1_ReadWriteByte(Data);
	LCD_CS_PIN_SET;
}


//向液晶屏写一个16位数据
void LCD_WriteData_16Bit(uint16_t Data) {
	LCD_CS_PIN_CLR;
	LCD_DC_PIN_SET;
	SPI1_ReadWriteByte(Data >> 8); 	//写入高8位数据
	SPI1_ReadWriteByte(Data); 			//写入低8位数据
	LCD_CS_PIN_SET;
}


void Lcd_WriteReg(uint8_t Index, uint8_t Data) {
	tft_write_cmd(Index);
	tft_write_data(Data);
}

void Lcd_Reset(void) {
	LCD_RST_PIN_CLR;
	my_delay_ms(10);
	LCD_RST_PIN_SET;
	my_delay_ms(10);
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void) {
	LCD_GPIO_Init();
	SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	Lcd_Reset(); //Reset before LCD Init.
	LCD_BLK_PIN_SET;
	// //LCD Init For 1.44Inch LCD Panel with ST7735R.
	tft_write_cmd(0x11);//Sleep exit
	my_delay_ms (12);

	//ST7735R Frame Rate
	tft_write_cmd(0xB1);
	tft_write_data(0x01);
	tft_write_data(0x2C);
	tft_write_data(0x2D);

	tft_write_cmd(0xB2);
	tft_write_data(0x01);
	tft_write_data(0x2C);
	tft_write_data(0x2D);

	tft_write_cmd(0xB3);
	tft_write_data(0x01);
	tft_write_data(0x2C);
	tft_write_data(0x2D);
	tft_write_data(0x01);
	tft_write_data(0x2C);
	tft_write_data(0x2D);

	tft_write_cmd(0xB4); //Column inversion
	tft_write_data(0x07);

	//ST7735R Power Sequence
	tft_write_cmd(0xC0);
	tft_write_data(0xA2);
	tft_write_data(0x02);
	tft_write_data(0x84);
	tft_write_cmd(0xC1);
	tft_write_data(0xC5);

	tft_write_cmd(0xC2);
	tft_write_data(0x0A);
	tft_write_data(0x00);

	tft_write_cmd(0xC3);
	tft_write_data(0x8A);
	tft_write_data(0x2A);
	tft_write_cmd(0xC4);
	tft_write_data(0x8A);
	tft_write_data(0xEE);

	tft_write_cmd(0xC5); //VCOM
	tft_write_data(0x0E);

	tft_write_cmd(0x36); //MX, MY, RGB mode
	tft_write_data(0xC0);

	//ST7735R Gamma Sequence
	tft_write_cmd(0xe0);
	tft_write_data(0x0f);
	tft_write_data(0x1a);
	tft_write_data(0x0f);
	tft_write_data(0x18);
	tft_write_data(0x2f);
	tft_write_data(0x28);
	tft_write_data(0x20);
	tft_write_data(0x22);
	tft_write_data(0x1f);
	tft_write_data(0x1b);
	tft_write_data(0x23);
	tft_write_data(0x37);
	tft_write_data(0x00);
	tft_write_data(0x07);
	tft_write_data(0x02);
	tft_write_data(0x10);

	tft_write_cmd(0xe1);
	tft_write_data(0x0f);
	tft_write_data(0x1b);
	tft_write_data(0x0f);
	tft_write_data(0x17);
	tft_write_data(0x33);
	tft_write_data(0x2c);
	tft_write_data(0x29);
	tft_write_data(0x2e);
	tft_write_data(0x30);
	tft_write_data(0x30);
	tft_write_data(0x39);
	tft_write_data(0x3f);
	tft_write_data(0x00);
	tft_write_data(0x07);
	tft_write_data(0x03);
	tft_write_data(0x10);

	tft_write_cmd(0x2a);
	tft_write_data(0x00);
	tft_write_data(0x00);
	tft_write_data(0x00);
	tft_write_data(0x7f);

	tft_write_cmd(0x2b);
	tft_write_data(0x00);
	tft_write_data(0x00);
	tft_write_data(0x00);
	tft_write_data(0x9f);

	tft_write_cmd(0xF0); //Enable test command
	tft_write_data(0x01);
	tft_write_cmd(0xF6); //Disable ram power save mode
	tft_write_data(0x00);

	tft_write_cmd(0x3A); //65k mode
	tft_write_data(0x05);


	tft_write_cmd(0x29);//Display on
}

/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(u16 x_start, u16 y_start, u16 x_end, u16 y_end) {
	x_start = x_start + LCD_OFFSET_X;
	x_end = x_end + LCD_OFFSET_X;
	y_start = y_start + LCD_OFFSET_Y;
	y_end = y_end + LCD_OFFSET_Y;

	tft_write_cmd(0x2A);
	tft_write_data(x_start >> 8);
	tft_write_data(x_start);
	tft_write_data(x_end >> 8);
	tft_write_data(x_end);

	tft_write_cmd(0x2B);
	tft_write_data(y_start >> 8);
	tft_write_data(y_start);
	tft_write_data(y_end >> 8);
	tft_write_data(y_end);

	tft_write_cmd(0x2C);
}
/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(u16 x, u16 y) {
	Lcd_SetRegion(x, y, x, y);
}


/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x, u16 y, u16 Data) {
	Lcd_SetRegion(x, y, x + 1, y + 1);
	LCD_WriteData_16Bit(Data);

}

/*****************************************
 函数功能：读TFT某一点的颜色
 出口参数：color  点颜色值
******************************************/
unsigned int Lcd_ReadPoint(u16 x, u16 y) {
	unsigned int Data;
	Lcd_SetXY(x, y);

	//Lcd_ReadData();//丢掉无用字节
	//Data=Lcd_ReadData();
	tft_write_data(Data);
	return Data;
}
/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(u16 Color) {
	unsigned int i, m;
	Lcd_SetRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
	for(i = 0; i < X_MAX_PIXEL; i++)
		for(m = 0; m < Y_MAX_PIXEL; m++) {
			LCD_WriteData_16Bit(Color);
		}
}

void showimage(uint8_t x, uint8_t y, const uint8_t* p) { //显示40*40 QQ图片
	int i;
	unsigned char picH, picL;
	Lcd_SetRegion(x, y, x + 39, y + 39);		//坐标设置
	for(i = 0; i < 40 * 40; i++) {
		picL = *(p + i * 2);	//数据低位在前
		picH = *(p + i * 2 + 1);
		LCD_WriteData_16Bit(picH << 8 | picL);
	}
}

void my_showimage(uint8_t x, uint8_t y, const sBITMAP* p) { //显示40*40 QQ图片
	uint16_t i;
	uint8_t picH, picL;
	Lcd_SetRegion(x, y, x + p->w - 1, y + p->h - 1);		//坐标设置
	for(i = 0; i < p->w * p->h; i++) {
		picH = *(p->map + i * 2);	//数据低位在前
		picL = *(p->map + i * 2 + 1);
		LCD_WriteData_16Bit(picH << 8 | picL);
	}
}

