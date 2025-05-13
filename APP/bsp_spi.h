#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "AllHeader.h"



void spi2_init(void);
u8 SPI_ReadWriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SpeedSet);

#define DMA_Channel_IsEnabled(DMA_Channel) (((DMA_Channel)->CCR & DMA_CCR1_EN) != 0)



#endif
