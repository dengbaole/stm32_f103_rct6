/**
 ******************************************************************************
 * @file    bsp_i2c_ee.c
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   ��gpioģ��i2c����, ������STM32ϵ��CPU����ģ�鲻����Ӧ�ò�����֡��������I2C���߻�������������
 ******************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :https://fire-stm32.taobao.com
 *
 ******************************************************************************
 */

/*
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� i2c_checkdevice() ���I2C�豸�Ƿ��������ú���������GPIO

*/
#include "bsp_i2c_gpio.h"
#include "stm32f10x.h"
static uint8_t i2c_temp_buf[32];
static uint8_t i2c_addr;
static uint8_t* i2c_transbuf;
static uint16_t i2c_translens;
static volatile uint16_t i2c_transptr;

/*
*********************************************************************************************************
*	�� �� ��: si2c_master_init
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void si2c_master_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(EEPROM_RCC_I2C_PORT, ENABLE); /* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; /* ��©��� */
	GPIO_Init(EEPROM_GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	si2c_stop();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_delay(void) {
	uint8_t i;

	/*��
		�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
	����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�

		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
		ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us
	*/
	for (i = 0; i < 10; i++)
		;
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void si2c_start(void) {
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	SDA_OUT_H();
	SCL_OUT_H();
	i2c_delay();
	SDA_OUT_L();
	i2c_delay();
	SCL_OUT_L();
	i2c_delay();
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void si2c_stop(void) {
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SDA_OUT_L();
	SCL_OUT_H();
	i2c_delay();
	SDA_OUT_H();
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_sendbyte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
bool si2c_sendbyte(uint8_t date) {
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++) {
		if (date & 0x80) {
			SDA_OUT_H();
		} else {
			SDA_OUT_L();
		}
		i2c_delay();
		SCL_OUT_H();
		i2c_delay();
		SCL_OUT_L();
		if (i == 7) {
			SDA_OUT_H(); // �ͷ�����
		}
		date <<= 1; /* ����һ��bit */
		i2c_delay();
	}
	return si2c_wait_ack();
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_readbyte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t si2c_readbyte(bool isAck) {
	uint8_t i;
	uint8_t receive;

	/* ������1��bitΪ���ݵ�bit7 */
	receive = 0;
	for (i = 0; i < 8; i++) {
		receive <<= 1;
		SCL_OUT_H();
		i2c_delay();
		if (SDA_IN()) {
			receive++;
		}
		SCL_OUT_L();
		i2c_delay();
	}
	if(!isAck) {
		si2c_nack();
	} else {
		si2c_ack();
	}
	return receive;
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_wait_ack
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t si2c_wait_ack(void) {
	uint8_t re;

	SDA_OUT_H(); /* CPU�ͷ�SDA���� */
	i2c_delay();
	SCL_OUT_H(); /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_delay();
	if (SDA_IN()) { /* CPU��ȡSDA����״̬ */
		re = 1;
	} else {
		re = 0;
	}
	SCL_OUT_L();
	i2c_delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void si2c_ack(void) {
	SDA_OUT_L(); /* CPU����SDA = 0 */
	i2c_delay();
	SCL_OUT_H(); /* CPU����1��ʱ�� */
	i2c_delay();
	SCL_OUT_L();
	i2c_delay();
	SDA_OUT_H(); /* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: si2c_nack
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void si2c_nack(void) {
	SDA_OUT_H(); /* CPU����SDA = 1 */
	i2c_delay();
	SCL_OUT_H(); /* CPU����1��ʱ�� */
	i2c_delay();
	SCL_OUT_L();
	i2c_delay();
}

void si2c_write_to(uint8_t addr, uint8_t* array, uint16_t len) {
	uint16_t i = 0;
	i2c_addr = addr;
	i2c_transptr = 0;
	i2c_translens = len;
	i2c_transbuf = array;

	si2c_start();
	if(si2c_sendbyte(i2c_addr & 0xFE) == false) {
		return;
	}
	for(i = 0; i < i2c_translens; i++) {
		if(si2c_sendbyte(i2c_transbuf[i2c_transptr++]) == false) {
			return;
		}
	}
	si2c_stop();
}

void si2c_read_from(uint8_t addr, uint8_t* array, uint16_t len) {
	uint16_t i = 0;
	i2c_addr = addr;
	i2c_transptr = 0;
	i2c_translens = len;
	i2c_transbuf = array;

	si2c_start();
	if(si2c_sendbyte(i2c_addr & 0xFE) == false) {
		return;
	}
	if(si2c_sendbyte(i2c_transbuf[i2c_transptr++]) == false) {
		return;
	}
	si2c_start();
	if(si2c_sendbyte(i2c_addr | 0x01) == false) {
		return;
	}
	for(i = 0; i < i2c_translens - 2; i++) {
		i2c_transbuf[i2c_transptr++] = si2c_readbyte(true);
	}
	i2c_transbuf[i2c_transptr++] = si2c_readbyte(false);
	si2c_stop();
}

void si2c_write_buf_to(uint8_t addr, uint8_t* array, uint16_t len) {
	memcpy(i2c_temp_buf, array, len);
	si2c_write_to(addr, i2c_temp_buf, len);
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_checkdevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_checkdevice(uint8_t _Address) {
	uint8_t ucAck;

	si2c_start(); /* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	ucAck = si2c_sendbyte(_Address | EEPROM_I2C_WR); /* ����豸��ACKӦ��(0��ʾӦ��1��ʾ��Ӧ��) */

	si2c_stop(); /* ����ֹͣ�ź� */

	return ucAck;
}
