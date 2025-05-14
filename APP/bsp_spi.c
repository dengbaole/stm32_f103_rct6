#include "bsp_spi.h"


volatile uint8_t dma_tx_complete = 0;
volatile uint8_t dma_rx_complete = 0;

void DMA1_Channel5_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_IT_TC5)) {
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		DMA_Cmd(DMA1_Channel5, DISABLE);
		dma_tx_complete = 1;  // 发送完成标志
	}
}

void DMA1_Channel4_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_IT_TC4)) {
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_Cmd(DMA1_Channel4, DISABLE);
		dma_rx_complete = 1;  // 接收完成标志
	}
}



void spi2_init(void) {
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;

	/* 使能SPI2时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* 使能GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* 使能DMA1时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* SPI2 GPIO配置: PB13-SCK, PB14-MISO, PB15-MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;  // SCK, MOSI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI2配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* 使能SPI2 */
	SPI_Cmd(SPI2, ENABLE);

	/* DMA1 Channel5 配置 - SPI2_TX */
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = 0; // 发送缓冲区地址，后面函数设置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 内存到外设
	DMA_InitStructure.DMA_BufferSize = 0; // 传输大小，后面函数设置
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	/* DMA1 Channel4 配置 - SPI2_RX */
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设到内存
	DMA_InitStructure.DMA_MemoryBaseAddr = 0; // 接收缓冲区地址，后面函数设置
	DMA_InitStructure.DMA_BufferSize = 0; // 传输大小，后面函数设置
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	/* 使能SPI DMA请求 */
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

	//中断配置
	/* 使能DMA传输完成中断 */
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);  // 接收完成中断
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);  // 发送完成中断
	

	/* 使能DMA中断通道NVIC */
	  NVIC_InitTypeDef NVIC_InitStructure;

    // DMA1_Channel4 - RX
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // DMA1_Channel5 - TX
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_Init(&NVIC_InitStructure);

}

void SPI2_DMA_TransmitReceive(uint8_t* txbuf, uint8_t* rxbuf, uint16_t len) {
    // 等待 DMA 通道空闲
    while (DMA1_Channel5->CCR & DMA_CCR1_EN);
    while (DMA1_Channel4->CCR & DMA_CCR1_EN);

    // 配置 DMA 发送
    DMA1_Channel5->CMAR = (uint32_t)txbuf;
    DMA1_Channel5->CNDTR = len;

    // 配置 DMA 接收
    DMA1_Channel4->CMAR = (uint32_t)rxbuf;
    DMA1_Channel4->CNDTR = len;

    // 清除 DMA 标志位
    DMA_ClearFlag(DMA1_FLAG_TC5);
    DMA_ClearFlag(DMA1_FLAG_TC4);
    dma_tx_complete = 0;
    dma_rx_complete = 0;

    // 使能 DMA 通道
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // 等待传输完成
    while (!dma_tx_complete);
    while (!dma_rx_complete);

    // 等待 SPI 完成最后的数据传输
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
}


// void SPI2_DMA_TransmitReceive(uint8_t* txbuf, uint8_t* rxbuf, uint16_t len) {
// 	/* 等待DMA通道空闲 */
// 	while (DMA_Channel_IsEnabled(DMA1_Channel5));
// 	while (DMA_Channel_IsEnabled(DMA1_Channel4));

	

// 	/* 配置DMA发送 */
// 	DMA1_Channel5->CMAR = (uint32_t)txbuf;
// 	DMA1_Channel5->CNDTR = len;

// 	/* 配置DMA接收 */
// 	DMA1_Channel4->CMAR = (uint32_t)rxbuf;
// 	DMA1_Channel4->CNDTR = len;

// 	/* 清除DMA传输完成标志 */
// 	DMA_ClearFlag(DMA1_FLAG_TC5);
// 	DMA_ClearFlag(DMA1_FLAG_TC4);
//     dma_tx_complete = 0;
// 	dma_rx_complete = 0;
// 	/* 使能DMA通道 */
// 	DMA_Cmd(DMA1_Channel4, ENABLE);
// 	DMA_Cmd(DMA1_Channel5, ENABLE);

// 	/* 等待DMA传输完成（中断方式） */
// 	while (!dma_tx_complete);
// 	while (!dma_rx_complete);
//     while(DMA_GetCurrDataCounter(DMA1_Channel5)){
//         ;
//     }
//      while(DMA_GetCurrDataCounter(DMA1_Channel4)){
//         ;
//     }
//     DMA_Cmd(DMA1_Channel5, DISABLE);
//     DMA_Cmd(DMA1_Channel4, DISABLE);
// }

/**
 * @brief SPI2 发送一个字节（阻塞模式）
 * @param TxData 发送数据
 * @return 接收数据
 */
uint8_t SPI_ReadWriteByte(uint8_t TxData) {
	uint8_t RxData = 0;
	SPI2_DMA_TransmitReceive(&TxData, &RxData, 1);
	return RxData;
}

/**
 * @brief SPI2 发送数据（阻塞模式）
 * @param txbuf 发送缓冲区
 * @param len 发送长度
 */

void SPI2_SendData_DMA(uint8_t* txbuf, uint16_t len) {
	// 等待DMA通道空闲
	// while (DMA1_Channel5->CCR & DMA_CCR1_EN);
     while (DMA_Channel_IsEnabled(DMA1_Channel5) && DMA_Channel_IsEnabled(DMA1_Channel4));

	// 配置DMA通道
	DMA1_Channel5->CCR &= ~DMA_CCR1_EN;           // 关闭DMA通道
	DMA1_Channel5->CPAR = (uint32_t) & (SPI2->DR); // 外设地址
	DMA1_Channel5->CMAR = (uint32_t)txbuf;       // 内存地址
	DMA1_Channel5->CNDTR = len;                   // 传输长度

	DMA_ClearFlag(DMA1_FLAG_TC5);                 // 清除传输完成标志
    DMA_ClearFlag(DMA1_FLAG_TC4);
    dma_tx_complete = 0;
	dma_rx_complete = 0;

	DMA_Cmd(DMA1_Channel5, ENABLE);
    // DMA_Cmd(DMA1_Channel4, ENABLE);

	// 等待DMA传输完成（中断方式）
	while (!dma_tx_complete);
    while(DMA_GetCurrDataCounter(DMA1_Channel5)){
        ;
    }
    //  while(DMA_GetCurrDataCounter(DMA1_Channel4)){
    //     ;
    // }
    // while (!dma_rx_complete);

	DMA_Cmd(DMA1_Channel5, DISABLE);
    // DMA_Cmd(DMA1_Channel4, DISABLE);
}





