#ifndef __FLASH_DRV_H
#define __FLASH_DRV_H

#include "platform.h"


#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0


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
uint16_t SpiFlashReadID(void);

#endif




