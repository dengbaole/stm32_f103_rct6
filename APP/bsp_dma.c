#include "bsp_dma.h"


uint8_t ReceiveBuff[RECEIVEBUFF_SIZE];
void dma_config(void) {
	DMA_InitTypeDef DMA_InitStructure;
	// 开启DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// 外设
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)ReceiveBuff;
	// 内存
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ReceiveBuff;
	// 传输方向		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// 传输数量
	DMA_InitStructure.DMA_BufferSize = RECEIVEBUFF_SIZE;
	// 外设地址增量模式
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 存储器地址增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 外设数据宽度
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	// 存储器数据宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// 通道优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 存储器到存储器模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// 初始化
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	// 使能初始化
	DMA_Cmd (DMA1_Channel4, ENABLE);
}


