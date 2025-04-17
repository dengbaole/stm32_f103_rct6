#ifndef _bsp_spi_flash_H
#define _bsp_spi_flash_H
#include "stm32f10x.h"

#define EN25Q80 	0XEF13
#define EN25Q16 	0XEF14
#define EN25Q32 	0XEF15
#define EN25Q64 	0XC216		//MXIC
#define EN25Q128	0XC817


extern u32 EN25QXX_TYPE;					//定义EN25QXX芯片型号

#define	EN25QXX_CS 		PBout(12)  		//EN25QXX的片选信号

#define TEXT_LEN sizeof(text_buf)



//指令表
#define EN25X_WRITEENABLE		0x06
#define EN25X_WRITEDISABLE		0x04
#define EN25X_READSTATUSREG		0x05
#define EN25X_WRITESTATUSREG		0x01
#define EN25X_READDATA			0x03
#define EN25X_FastReadData		0x0B
#define EN25X_FastReadDual		0x3B
#define EN25X_PageProgram		0x02
#define EN25X_BlockErase			0xD8
#define EN25X_SectorErase		0x20
#define EN25X_ChipErase			0xC7
#define EN25X_PowerDown			0xB9
#define EN25X_ReleasePowerDown	0xAB
#define EN25X_DeviceID			0xAB
#define EN25X_ManufactDeviceID	0x90
#define EN25X_JedecDeviceID		0x9F



void en25qxx_init(void);
uint16_t  en25qxx_readid(void);  	    		//读取FLASH ID
uint8_t	 en25qxx_readsr(void);        		//读取状态寄存器
void en25qxx_write_sr(uint8_t sr);  			//写状态寄存器
void en25qxx_write_enable(void);  		//写使能
void en25qxx_write_disable(void);		//写保护
void en25qxx_write_nocheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void en25qxx_read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead); //读取flash
void en25qxx_write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写入flash
void en25qxx_erase_chip(void);    	  	//整片擦除
void en25qxx_erase_sector(uint32_t Dst_Addr);	//扇区擦除
void en25qxx_wait_busy(void);           	//等待空闲
void en25qxx_powerdown(void);        	//进入掉电模式
void en25qxx_wakeup(void);				//唤醒


#endif
