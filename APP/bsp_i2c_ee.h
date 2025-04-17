#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include "stm32f10x.h"
#include "platform.h"

/*
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1
 */

/* AT24C01/02ÿҳ��8���ֽ�
 * AT24C04/08A/16Aÿҳ��16���ֽ�
 */


#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02��ҳ���С */
#define EEPROM_SIZE				  256			  /* 24xx02������ */

extern uint8_t write_buf[EEPROM_SIZE];
extern uint8_t read_buf[EEPROM_SIZE];
uint8_t ee_checkok(void);
void ee_init(void);
uint8_t ee_readbytes(uint8_t* _pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_writebytes(uint8_t* _pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void ee_erase(void);
uint8_t eeprom_test(void);


#endif /* __I2C_EE_H */
