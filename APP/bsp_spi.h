#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "AllHeader.h"



void spi2_init(void);
u8 SPI_ReadWriteByte(u8 TxData);
void SPI1_SetSpeed(u8 SpeedSet);

#endif
