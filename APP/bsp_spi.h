#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "AllHeader.h"



void spi2_init(void);
u8 SPI_ReadWriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SpeedSet);
void SPI2_DMA_TransmitReceive(uint8_t* txbuf, uint8_t* rxbuf, uint16_t len);
void SPI2_ReceiveData_DMA(uint8_t* rxbuf, uint16_t len);
#define DMA_Channel_IsEnabled(DMA_Channel) (((DMA_Channel)->CCR & DMA_CCR1_EN) != 0)



#endif
