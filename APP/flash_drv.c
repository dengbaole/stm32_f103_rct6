#include "flash_drv.h"

uint16_t SPI_FLASH_TYPE = W25Q64; //默认就是25Q64


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


uint16_t SpiFlashReadID(void) {
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