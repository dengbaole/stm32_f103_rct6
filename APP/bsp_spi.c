#include "bsp_spi.h"

void spi2_init(void) {
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��SPIʱ�� */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2, ENABLE );

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}



void SPI1_SetSpeed(u8 SpeedSet) {
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}


u8 SPI_ReadWriteByte(u8 TxData) {
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) { //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		retry++;
		if(retry > 200)return 0;
	}
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) { //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		retry++;
		if(retry > 200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}

