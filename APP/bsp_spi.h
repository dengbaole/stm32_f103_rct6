#ifndef _bsp_spi_H
#define _bsp_spi_H
#include "platform.h"
#include "stm32f10x_spi.h"

void spi2_init(void);			 //初始化SPI2口
void spi2_setspeed(u8 SpeedSet); //设置SPI2速度
uint8_t spi2_readwritebyte(uint8_t TxData);//SPI2总线读写一个字节
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);


#endif
