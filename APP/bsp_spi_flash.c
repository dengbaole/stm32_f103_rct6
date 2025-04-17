#include "bsp_spi_flash.h"
#include "platform.h"
#include "bsp_i2c_gpio.h"


uint32_t EN25QXX_TYPE = EN25Q128;	//默认是EN25Q128

//4Kbytes为一个Sector
//16个扇区为1个Block
//EN25Q128
//容量为16M字节,共有128个Block,4096个Sector

//初始化SPI FLASH的IO口
void en25qxx_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	/* FLASH_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	/* NRF24L01_CS PG7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOG, GPIO_Pin_7); //输出1,防止NRF干扰SPI FLASH的通信

	EN25QXX_CS = 1;			       //SPI FLASH不选中
	spi2_init();		   			//初始化SPI
	spi2_setspeed(SPI_BaudRatePrescaler_4);
	EN25QXX_TYPE = en25qxx_readid();	//读取FLASH ID.
	printf("使用的FLASH芯片ID是:%X\r\n", EN25QXX_TYPE);
}

//读取EN25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t en25qxx_readsr(void) {
	uint8_t byte = 0;
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_READSTATUSREG);    //发送读取状态寄存器命令
	byte = spi2_readwritebyte(0Xff);           //读取一个字节
	EN25QXX_CS = 1;                          //取消片选
	return byte;
}
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void en25qxx_write_sr(uint8_t sr) {
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_WRITESTATUSREG);//发送写取状态寄存器命令
	spi2_readwritebyte(sr);               	//写入一个字节
	EN25QXX_CS = 1;                          //取消片选
}
//EN25QXX写使能
//将WEL置位
void en25qxx_write_enable(void) {
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_WRITEENABLE);      //发送写使能
	EN25QXX_CS = 1;                          //取消片选
}
//EN25QXX写禁止
//将WEL清零
void en25qxx_write_disable(void) {
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_WRITEDISABLE);     //发送写禁止指令
	EN25QXX_CS = 1;                          //取消片选
}
//读取芯片ID
//返回值如下:
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XC216,表示芯片型号为W25Q64
//0XC817,表示芯片型号为W25Q128
uint16_t en25qxx_readid(void) {
	uint16_t Temp = 0;
	EN25QXX_CS = 0;
	spi2_readwritebyte(0x90);//发送读取ID命令
	spi2_readwritebyte(0x00);
	spi2_readwritebyte(0x00);
	spi2_readwritebyte(0x00);
	Temp |= spi2_readwritebyte(0xFF) << 8;
	Temp |= spi2_readwritebyte(0xFF);
	EN25QXX_CS = 1;
	return Temp;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void en25qxx_read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
	uint16_t i;
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_READDATA);         //发送读取命令
	spi2_readwritebyte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
	spi2_readwritebyte((uint8_t)((ReadAddr) >> 8));
	spi2_readwritebyte((uint8_t)ReadAddr);
	for(i = 0; i < NumByteToRead; i++) {
		pBuffer[i] = spi2_readwritebyte(0XFF); //循环读数
	}
	EN25QXX_CS = 1;
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void en25qxx_write_page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
	uint16_t i;
	en25qxx_write_enable();                  //SET WEL
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_PageProgram);      //发送写页命令
	spi2_readwritebyte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
	spi2_readwritebyte((uint8_t)((WriteAddr) >> 8));
	spi2_readwritebyte((uint8_t)WriteAddr);
	for(i = 0; i < NumByteToWrite; i++)spi2_readwritebyte(pBuffer[i]); //循环写数
	EN25QXX_CS = 1;                          //取消片选
	en25qxx_wait_busy();					   //等待写入结束
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void en25qxx_write_nocheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
	uint16_t pageremain;
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //不大于256个字节
	while(1) {
		en25qxx_write_page(pBuffer, WriteAddr, pageremain);
		if(NumByteToWrite == pageremain)break; //写入结束了
		else { //NumByteToWrite>pageremain
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain;			 //减去已经写入了的字节数
			if(NumByteToWrite > 256)pageremain = 256; //一次可以写入256个字节
			else pageremain = NumByteToWrite; 	 //不够256个字节了
		}
	}
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
uint8_t EN25QXX_BUFFER[4096];
void en25qxx_write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t* EN25QXX_BUF;
	EN25QXX_BUF = EN25QXX_BUFFER;
	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
	if(NumByteToWrite <= secremain)secremain = NumByteToWrite; //不大于4096个字节
	while(1) {
		en25qxx_read(EN25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
		for(i = 0; i < secremain; i++) { //校验数据
			if(EN25QXX_BUF[secoff + i] != 0XFF)break; //需要擦除
		}
		if(i < secremain) { //需要擦除
			en25qxx_erase_sector(secpos);//擦除这个扇区
			for(i = 0; i < secremain; i++) { //复制
				EN25QXX_BUF[i + secoff] = pBuffer[i];
			}
			en25qxx_write_nocheck(EN25QXX_BUF, secpos * 4096, 4096); //写入整个扇区

		} else en25qxx_write_nocheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		if(NumByteToWrite == secremain)break; //写入结束了
		else { //写入未结束
			secpos++;//扇区地址增1
			secoff = 0; //偏移位置为0

			pBuffer += secremain; //指针偏移
			WriteAddr += secremain; //写地址偏移
			NumByteToWrite -= secremain;				//字节数递减
			if(NumByteToWrite > 4096)secremain = 4096;	//下一个扇区还是写不完
			else secremain = NumByteToWrite;			//下一个扇区可以写完了
		}
	}
}

//擦除整个芯片
//等待时间超长...
void en25qxx_erase_chip(void) {
	en25qxx_write_enable();                  //SET WEL
	en25qxx_wait_busy();
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_ChipErase);        //发送片擦除命令
	EN25QXX_CS = 1;                          //取消片选
	en25qxx_wait_busy();   				   //等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void en25qxx_erase_sector(u32 Dst_Addr) {
	//监视falsh擦除情况,测试用
	printf("fe:%x\r\n", Dst_Addr);
	Dst_Addr *= 4096;
	en25qxx_write_enable();                  //SET WEL
	en25qxx_wait_busy();
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_SectorErase);      //发送扇区擦除指令
	spi2_readwritebyte((u8)((Dst_Addr) >> 16)); //发送24bit地址
	spi2_readwritebyte((u8)((Dst_Addr) >> 8));
	spi2_readwritebyte((u8)Dst_Addr);
	EN25QXX_CS = 1;                          //取消片选
	en25qxx_wait_busy();   				   //等待擦除完成
}

//等待空闲
void en25qxx_wait_busy(void) {
	while((en25qxx_readsr() & 0x01) == 0x01); // 等待BUSY位清空
}

//进入掉电模式
void en25qxx_powerdown(void) {
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_PowerDown);        //发送掉电命令
	EN25QXX_CS = 1;                          //取消片选
	delay_us(3);                               //等待TPD
}

//唤醒
void en25qxx_wakeup(void) {
	EN25QXX_CS = 0;                          //使能器件
	spi2_readwritebyte(EN25X_ReleasePowerDown);   //  send EN25X_PowerDown command 0xAB
	EN25QXX_CS = 1;                          //取消片选
	delay_us(3);                               //等待TRES1
}
