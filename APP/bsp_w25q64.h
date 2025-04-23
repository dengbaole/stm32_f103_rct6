#ifndef _BSP_W25Q64_H
#define _BSP_W25Q64_H

#include "AllHeader.h"

#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define RANDOM_BYTE 0XFF


extern u16 SPI_FLASH_TYPE;		//定义我们使用的flash芯片型号

#define	SPI_FLASH_CS PBout(12)  //选中FLASH					 


extern u8 SPI_FLASH_BUF[4096];
//W25X64读写
#define FLASH_ID 0XEF16
//指令表
#define W25X_WRITE_ENABLE		 0x06
#define W25X_WRITE_DISABLE		 0x04
#define W25X_READ_STATU_TEG		 0x05
#define W25X_WRITE_STATUS_REG	 0x01
#define W25X_READ_DATA			 0x03
#define W25X_FAST_READ_DATA		 0x0B
#define W25X_FAST_READ_DUAL		 0x3B
#define W25X_PAGE_PROGRAM		 0x02
#define W25X_BLOCK_ERASE		 0xD8
#define W25X_SECTOR_ERASE		 0x20
#define W25X_CHIP_ERASE			 0xC7
#define W25X_POWER_DOWN			 0xB9
#define W25X_RELEASE_POWER_DOWN	 0xAB
#define W25X_DEVICE_ID			 0xAB
#define W25X_MANUFACT_DEVICE_ID	 0x90
#define W25X_JEDEC_DEVICE_ID	 0x9F

void SpiFlashInit(void);
uint16_t flash_reas_id(void);    //读取FLASH ID
uint8_t	flash_read_sr(void);     //读取状态寄存器
void SpiFlashWriteSR(uint8_t sr); //写状态寄存器
void flsh_write_enable(void);   //写使能
void SpiFlashWriteDisable(void);  //写保护
void SpiFlashRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead); //读取flash
void SpiFlashWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写入flash
void SpiFlashEraseChip(void);    	           //整片擦除
void SpiFlashEraseSector(uint32_t Dst_Addr);   //扇区擦除
void flash_wait_busy(void);                   //等待空闲
void SpiFlashPowerDown(void);                  //进入掉电模式
void SpiFlashWAKEUP(void);			           //唤醒



#endif
