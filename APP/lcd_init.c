#include "lcd_init.h"


void LCD_GPIO_Init(void) {
	 GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;

    // ����GPIOʱ��
    RCC_APB2PeriphClockCmd(LCD_SPI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    // ����SPI���ţ�SCK��MOSI��
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_PIN_SCK | LCD_SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_SPI_GPIO, &GPIO_InitStructure);

    // �����������ã�CS��DC��RES��BLK��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    LCD_BL_OFF();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // ��ʼ��SPI
    RCC_APB1PeriphClockCmd(LCD_SPI_CLK, ENABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       // ����LCD������
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;     // ����LCD������
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // ����ʱ���ٶ�
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(LCD_SPI, &SPI_InitStructure);
    SPI_Cmd(LCD_SPI, ENABLE);
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) {
	LCD_CS_Clr();
    while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET); // �ȴ����ͻ�������
    SPI_I2S_SendData(LCD_SPI, dat);                                    // ��������
    while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) == SET);   // �ȴ��������
    LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat) {
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat) {
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat) {
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2) {
	if(USE_HORIZONTAL == 0) {
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1 + 26);
		LCD_WR_DATA(x2 + 26);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1 + 1);
		LCD_WR_DATA(y2 + 1);
		LCD_WR_REG(0x2c);//������д
	} else if(USE_HORIZONTAL == 1) {
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1 + 26);
		LCD_WR_DATA(x2 + 26);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1 + 1);
		LCD_WR_DATA(y2 + 1);
		LCD_WR_REG(0x2c);//������д
	} else if(USE_HORIZONTAL == 2) {
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1 + 1);
		LCD_WR_DATA(x2 + 1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1 + 26);
		LCD_WR_DATA(y2 + 26);
		LCD_WR_REG(0x2c);//������д
	} else {
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1 + 1);
		LCD_WR_DATA(x2 + 1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1 + 26);
		LCD_WR_DATA(y2 + 26);
		LCD_WR_REG(0x2c);//������д
	}
}

void LCD_Init(void) {
	LCD_GPIO_Init();//��ʼ��GPIO
	LCD_RES_Clr();//��λ
	delay_ms(10);
	LCD_RES_Set();
	delay_ms(10);
	delay_ms(10);
	LCD_WR_REG(0x11);     //Sleep out
	delay_ms(12);                //Delay 120ms
	LCD_WR_REG(0xB1);     //Normal mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);     //Idle mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);     //Partial mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB4);     //Dot inversion
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xC0);     //AVDD GVDD
	LCD_WR_DATA8(0xAB);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);     //VGH VGL
	LCD_WR_DATA8(0xC5);   //C0
	LCD_WR_REG(0xC2);     //Normal Mode
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);     //Idle
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x6A);
	LCD_WR_REG(0xC4);     //Partial+Full
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	LCD_WR_REG(0xC5);     //VCOM
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0xE0);     //positive gamma
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x10);
	LCD_WR_REG(0xE1);     //negative gamma
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x10);
	LCD_WR_REG(0xFC);
	LCD_WR_DATA8(0x80);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL == 0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL == 2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	LCD_WR_REG(0x21);     //Display inversion
	LCD_WR_REG(0x29);     //Display on
	LCD_WR_REG(0x2A);     //Set Column Address
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x1A);  //26
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x69);   //105
	LCD_WR_REG(0x2B);     //Set Page Address
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);    //1
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xA0);    //160
	LCD_WR_REG(0x2C);
}








