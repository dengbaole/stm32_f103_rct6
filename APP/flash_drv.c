#include "flash_drv.h"


uint16_t SPI_FLASH_TYPE = 0; //默认就是25Q64


void flash_gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd (SPI_CLOCK, ENABLE );
	RCC_APB2PeriphClockCmd (FLASH_SPI_GPIO, ENABLE );

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

	SPI_FLASH_CS_HIGH();
}


uint16_t flash_reas_id(void) {
	uint16_t Temp = 0;
	SPI_FLASH_CS_LOW();
	SPI_ReadWriteByte(0x90);//发送读取ID命令
	SPI_ReadWriteByte(0x00);
	SPI_ReadWriteByte(0x00);
	SPI_ReadWriteByte(0x00);
	Temp |= SPI_ReadWriteByte(0xFF) << 8;
	Temp |= SPI_ReadWriteByte(0xFF);
	SPI_FLASH_CS_HIGH();
	return Temp;
}

uint8_t flash_read_sr(void) {
	uint8_t byte = 0;
	SPI_FLASH_CS_LOW();                          //使能器件
	SPI_ReadWriteByte(W25X_READ_STATU_TEG);    //发送读取状态寄存器命令
	byte = SPI_ReadWriteByte(RANDOM_BYTE);           //读取一个字节
	SPI_FLASH_CS_HIGH();                          //取消片选
	return byte;
}

//等待空闲
void flash_wait_busy(void) {
	while((flash_read_sr() & 0x01) == 0x01); // 等待BUSY位清空
}

//SPI_FLASH写使能
//将WEL置位
void flsh_write_enable(void) {
	SPI_FLASH_CS_LOW();                          //使能器件
	SPI_ReadWriteByte(W25X_WRITE_ENABLE);      //发送写使能
	SPI_FLASH_CS_HIGH();                          //取消片选
}

void flash_erase(void) {
	flsh_write_enable();                  //SET WEL
	flash_wait_busy();
	SPI_FLASH_CS_LOW();                          //使能器件
	SPI_ReadWriteByte(W25X_CHIP_ERASE);        //发送片擦除命令
	SPI_FLASH_CS_HIGH();                          //取消片选
	flash_wait_busy();   				   //等待芯片擦除结束
}



//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25Q128_ReadData(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
	uint16_t i;
	uint8_t txBuffer[4];
	SPI_FLASH_CS_LOW();                          //使能器件
	txBuffer[0] = W25X_READ_DATA;         //发送读取命令
	txBuffer[1] = (uint8_t)((ReadAddr) >> 16); //发送24bit地址
	txBuffer[2] = (uint8_t)((ReadAddr) >> 8);
	txBuffer[3] = (uint8_t)ReadAddr;

	// SPI_ReadWriteByte(W25X_READ_DATA);         //发送读取命令
	// SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
	// SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
	// SPI_ReadWriteByte((uint8_t)ReadAddr);
	// SPI2_SendData_DMA(txBuffer, 4);
	SPI2_DMA_TransmitReceive(txBuffer, pBuffer, 4);
	// delay_ms(1);
	SPI2_DMA_TransmitReceive(tx_buff, pBuffer, NumByteToRead + 1);
	// delay_ms(2);
	// memset(pBuffer, 0x17, 1600);
	// memset(pBuffer, 0x08, 1600);
	SPI_FLASH_CS_HIGH();                          //取消片选
}






