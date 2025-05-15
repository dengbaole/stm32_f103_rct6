#ifndef __FLASH_DRV_H
#define __FLASH_DRV_H

#include "platform.h"


#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define RANDOM_BYTE 0XFF


#define W25X_WRITE_ENABLE		 0x06
#define W25X_WRITE_DISABLE		 0x04
#define W25X_READ_STATU_TEG		 0x05
#define W25X_WRITE_STATUS_REG	 0x01
#define W25X_READ_DATA			 0x03
#define W25X_FAST_READ_DATA		 0x0B
#define W25X_FAST_READ_DUAL		 0x3B
#define W25X_PAGE_PROGRAM		 0x02
#define W25X_BLOCK_ERASE		 0xD8
#define W25X_SECTOR_ERASE		 0x20
#define W25X_CHIP_ERASE			 0xC7
#define W25X_POWER_DOWN			 0xB9
#define W25X_RELEASE_POWER_DOWN	 0xAB
#define W25X_DEVICE_ID			 0xAB
#define W25X_MANUFACT_DEVICE_ID	 0x90
#define W25X_JEDEC_DEVICE_ID	 0x9F


#define SPI_CLOCK RCC_APB1Periph_SPI2
#define USE_SPI SPI2

#define FLASH_SPI_GPIO RCC_APB2Periph_GPIOB

#define FLASH_SPI_CS_PIN 		GPIO_Pin_12
#define FLASH_SPI_CS_PORT 		GPIOB

#define FLASH_SPI_SCK_PIN 		GPIO_Pin_13
#define FLASH_SPI_SCK_PORT 		GPIOB

#define FLASH_SPI_MISO_PIN 		GPIO_Pin_14
#define FLASH_SPI_MISO_PORT 	GPIOB

#define FLASH_SPI_MOSI_PIN 		GPIO_Pin_15
#define FLASH_SPI_MOSI_PORT 	GPIOB

#define SPI_FLASH_CS_HIGH() GPIO_SetBits(GPIOB,FLASH_SPI_CS_PIN);
#define SPI_FLASH_CS_LOW()  GPIO_ResetBits(GPIOB,FLASH_SPI_CS_PIN);


extern uint16_t SPI_FLASH_TYPE;




void flash_gpio_init(void);
uint16_t flash_reas_id(void);
void flash_erase(void);
void W25Q128_ReadData(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

#endif




