#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include <inttypes.h>
#include "platform.h"

#define EEPROM_I2C_WR 0 /* д����bit */
#define EEPROM_I2C_RD 1 /* ������bit */

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define EEPROM_GPIO_PORT_I2C GPIOB				 /* GPIO�˿� */
#define EEPROM_RCC_I2C_PORT RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define EEPROM_I2C_SCL_PIN GPIO_Pin_6			 /* ���ӵ�SCLʱ���ߵ�GPIO */
#define EEPROM_I2C_SDA_PIN GPIO_Pin_7			 /* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0																		 /* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define SCL_OUT_H() GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)	 /* SCL = 1 */
	#define SCL_OUT_L() GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN) /* SCL = 0 */

	#define SDA_OUT_H() GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)	 /* SDA = 1 */
	#define SDA_OUT_L() GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN) /* SDA = 0 */

	#define SDA_IN() GPIO_ReadInputDataBit(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN) /* ��SDA����״̬ */
#else																			 /* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
	/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define SCL_OUT_H() EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN				 /* SCL = 1 */
	#define SCL_OUT_L() EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN				 /* SCL = 0 */

	#define SDA_OUT_H() EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN /* SDA = 1 */
	#define SDA_OUT_L() EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN	/* SDA = 0 */

	#define SDA_IN() ((EEPROM_GPIO_PORT_I2C->IDR & EEPROM_I2C_SDA_PIN) != 0) /* ��SDA����״̬ */
#endif

void si2c_start(void);
void si2c_stop(void);
bool si2c_sendbyte(uint8_t date);
uint8_t si2c_readbyte(bool isAck);
uint8_t si2c_wait_ack(void);
void si2c_ack(void);
void si2c_nack(void);
uint8_t i2c_checkdevice(uint8_t _Address);
void si2c_master_init(void);
#endif
