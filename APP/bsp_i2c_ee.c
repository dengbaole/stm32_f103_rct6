#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "bsp_usart.h"

uint8_t write_buf[EEPROM_SIZE];
uint8_t read_buf[EEPROM_SIZE];

/*
*********************************************************************************************************
*	函 数 名: i2c_checkdevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/

uint8_t ee_checkok(void) {
	if (i2c_checkdevice(EEPROM_DEV_ADDR) == 0) {
		return 1;
	} else {
		return 0;
	}
}
void ee_init(void) {
	ee_checkok();
}
/*
*********************************************************************************************************
*	函 数 名: ee_readbytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_readbytes(uint8_t* _pReadBuf, uint16_t _usAddress, uint16_t _usSize) {
	uint16_t i;

	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

	/* 第1步：发起I2C总线启动信号 */
	si2c_start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	/* 此处是写指令 */

	/* 第3步：等待ACK */
	if (si2c_sendbyte(EEPROM_DEV_ADDR | EEPROM_I2C_WR) != 0) {
		goto cmd_fail; /* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */


	/* 第5步：等待ACK */
	if (si2c_sendbyte((uint8_t)_usAddress) != 0) {
		goto cmd_fail; /* EEPROM器件无应答 */
	}

	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	si2c_start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (si2c_sendbyte(EEPROM_DEV_ADDR | EEPROM_I2C_RD) != 0) {
		goto cmd_fail; /* EEPROM器件无应答 */
	}

	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++) {
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1) {
			///* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
			_pReadBuf[i] = si2c_readbyte(true);
		} else {
			///* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
			_pReadBuf[i] = si2c_readbyte(false);
		}
	}
	/* 发送I2C总线停止信号 */
	si2c_stop();
	return 1; /* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	si2c_stop();
	return 0;
}

/// @brief 
/// @param _pWriteBuf 数组
/// @param _usAddress eeprom地址
/// @param _usSize 写入长度
/// @return 
uint8_t ee_writebytes(uint8_t* _pWriteBuf, uint16_t _usAddress, uint16_t _usSize) {
	uint16_t i, m;
	uint16_t usAddr;

	/*
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8
		简单的处理方法为：按字节写操作模式，没写1个字节，都发送地址
		为了提高连续写的效率: 本函数采用page wirte操作。
	*/

	usAddr = _usAddress;
	// si2c_sendbyte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);
	// si2c_sendbyte((uint8_t)usAddr);
	// si2c_write_to(EEPROM_DEV_ADDR,_pWriteBuf,_usSize);
	for (i = 0; i < _usSize; i++) {
		// 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址
		if ((i == 0) || (usAddr % EEPROM_PAGE_SIZE == 0)) {
			// 发停止信号，启动内部写操作
			si2c_stop();

			// 检查器件应答，判断内部写操作是否完成
			for (m = 0; m <= 1000; m++) {
				si2c_start();
				if (si2c_sendbyte(EEPROM_DEV_ADDR | EEPROM_I2C_WR) == 0) {
					break; // 成功发送
				}
			}
			if (m > 1000) {
				goto cmd_fail; // EEPROM器件无应答
			}

			// 发送字节地址
			if (si2c_sendbyte((uint8_t)usAddr) != 0) {
				goto cmd_fail; // EEPROM器件无应答
			}
		}

		// 发送数据字节
		if (si2c_sendbyte(_pWriteBuf[i]) != 0) {
			goto cmd_fail; // EEPROM器件无应答
		}

		usAddr++; // 地址增1
	}

	// 	/* 命令执行成功，发送I2C总线停止信号 */
	si2c_stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	si2c_stop();
	return 0;
}

void ee_erase(void) {
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];

	/* 填充缓冲区 */
	for (i = 0; i < EEPROM_SIZE; i++) {
		buf[i] = 0xFF ;
	}

	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (ee_writebytes(buf, 0, EEPROM_SIZE) == 0) {
		printf("擦除eeprom出错!\r\n");
		return;
	} else {
		printf("擦除eeprom成功!\r\n");
	}
}

/*--------------------------------------------------------------------------------------------------*/
static void eeprom_delay(__IO uint32_t nCount) { // 简单的延时函数
	for (; nCount != 0; nCount--)
		;
}

/*
 * eeprom AT24C02 读写测试
 * 正常返回1，异常返回0
 */
uint8_t eeprom_test(void) {
	uint16_t i;
	uint8_t write_buf[EEPROM_SIZE];
	uint8_t read_buf[EEPROM_SIZE];

	/*-----------------------------------------------------------------------------------*/
	if (ee_checkok() == 0) {
		printf("没有检测到串行EEPROM!\r\n");
		return 0;
	}
	/*------------------------------------------------------------------------------------*/
	/* 填充测试缓冲区 */
	for (i = 0; i < EEPROM_SIZE; i++) {
		write_buf[i] = i;
	}
	// 写数据
	/*------------------------------------------------------------------------------------*/
	if (ee_writebytes(write_buf, 0, EEPROM_SIZE) == 0) {
		printf("写eeprom出错!\r\n");
		return 0;
	} else {
		printf("写eeprom成功!\r\n");
	}

	/*写完之后需要适当的延时再去读，不然会出错*/
	eeprom_delay(0x0FFFFF);
	/*-----------------------------------------------------------------------------------*/
	if (ee_readbytes(read_buf, 0, EEPROM_SIZE) == 0) {
		printf("读eeprom出错!\r\n");
		return 0;
	} else {
		printf("读eeprom成功,数据如下：\r\n");
	}
	/*-----------------------------------------------------------------------------------*/
	for (i = 0; i < EEPROM_SIZE; i++) {
		if (read_buf[i] != write_buf[i]) {
			printf("0x%02X ", read_buf[i]);
			return 0;
		}
		printf(" %02X", read_buf[i]);

		if ((i & 15) == 15) {
			printf("\r\n");
		}
	}
	printf("eeprom读写测试成功\r\n");
	return 1;
}
/*********************************************END OF FILE**********************/
