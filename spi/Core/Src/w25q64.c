//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它商业用途
//测试硬件：单片机STM32F407VGT6,STM32F407VxT6最小系统开发板,主频168MHZ，晶振8MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//wiki技术资料网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-21077707 
//手机: (销售)18823372746 （技术)15989313508
//邮箱:(销售/订单) sales@qdtft.com  (售后/技术服务)service@qdtft.com
//QQ:(售前咨询)3002706772 (技术支持)3002778157
//技术交流QQ群:778679828
//创建日期:2020/06/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6最小系统开发板SPI示例
//     LED0           --->            PA1
//     KEY0           --->            PE4
//     KEY_UP         --->            PA0
//     SPI_CLK        --->            PB3
//     SPI_MOSI       --->            PB5
//     SPI_MISO       --->            PB4
//支持ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD可以直接插入STM32F407VxT6最小系统开发板TFTLCD插槽或者通过排线接入TFTLCD插座
//STM32连接引脚是指TFTLCD插槽或者插座引脚内部连接的STM32引脚
//=================================电源接线=======================================//
//     LCD模块                    STM32连接引脚
//      VDD           --->           DC5V/3.3V          //电源
//      GND           --->             GND              //电源地
//=============================液晶屏数据线接线===================================//
//     LCD模块                    STM32连接引脚
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>液晶屏16位并口数据信号
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================液晶屏控制线接线===================================//
//     LCD模块 				            STM32连接引脚 
//      WR            --->            PD5             //液晶屏写数据控制信号
//      RD            --->            PD4             //液晶屏读数据控制信号
//      RS            --->            PD11            //液晶屏数据/命令控制信号
//      RST           --->          复位引脚（默认）  //液晶屏复位控制信号（也可选择PD13）
//      CS            --->            PD7             //液晶屏片选控制信号
//      BL            --->            PB15            //液晶屏背光控制信号
*************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*************************************************************************************/
#include "w25q64.h" 
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
	
u16 W25QXX_TYPE=W25Q64;	//默认是W25Q64

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q64
//容量为8M字节,共有64个Block,2048个Sector 
										
/*****************************************************************************
 * @name       :void W25Q64_Init(void)
 * @date       :2020-05-08 
 * @function   :Initialization of SPI flash IO port
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 										
void W25Q64_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	W25Q64_CS=1;			//SPI FLASH不选中
	SPI1_Init();		   			//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//设置为21M时钟,高速模式 
	W25QXX_TYPE=W25QXX_ReadID();	//读取FLASH ID.
}  

/*****************************************************************************
 * @name       :u8 W25Q64_ReadSR(void) 
 * @date       :2020-05-08 
 * @function   :Read the status register of w25q64
								BIT7  6   5   4   3   2   1   0
							  SPR   RV  TB BP2 BP1 BP0 WEL BUSY
								SPR:Default 0, status register protection bit, used with WP
								TB,BP2,BP1,BP0:Flash area write protection settings
								WEL:Write enable lock
								BUSY:Busy flag bit (1, busy; 0, idle)
								Default:0x00
 * @parameters :None
 * @retvalue   :the value of the status register
******************************************************************************/	
u8 W25Q64_ReadSR(void)   
{  
	u8 byte=0;   
	W25Q64_CS=0;                            //使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	W25Q64_CS=1;                            //取消片选     
	return byte;   
}

/*****************************************************************************
 * @name       :void W25Q64_Write_SR(u8 sr)
 * @date       :2020-05-08 
 * @function   :Write the status register of w25q64
								(Only SPR,TB,bp2,BP1,bp0(bit 7,5,4,3,2)can be written!!!)
								BIT7  6   5   4   3   2   1   0
							  SPR   RV  TB BP2 BP1 BP0 WEL BUSY
								SPR:Default 0, status register protection bit, used with WP
								TB,BP2,BP1,BP0:Flash area write protection settings
								WEL:Write enable lock
								BUSY:Busy flag bit (1, busy; 0, idle)
								Default:0x00
 * @parameters :sr:the value of the status register to be written
 * @retvalue   :the value of the status register
******************************************************************************/	 
void W25Q64_Write_SR(u8 sr)   
{   
	W25Q64_CS=0;                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	W25Q64_CS=1;                            //取消片选     	      
} 

/*****************************************************************************
 * @name       :void W25Q64_Write_Enable(void) 
 * @date       :2020-05-08 
 * @function   :W25q64 write enable(Set wel)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	   
void W25Q64_Write_Enable(void)   
{
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_WriteEnable);    //发送写使能  
	W25Q64_CS=1;                            //取消片选     	      
} 

/*****************************************************************************
 * @name       :void W25Q64_Write_Disable(void) 
 * @date       :2020-05-08 
 * @function   :W25q64 write disable(Reset wel)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void W25Q64_Write_Disable(void)   
{  
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令    
	W25Q64_CS=1;                            //取消片选     	      
}

/*****************************************************************************
 * @name       :u16 W25QXX_ReadID(void)
 * @date       :2020-05-08 
 * @function   :Read chip ID							
 * @parameters :None
 * @retvalue   :0XEF13:W25Q80  
								0XEF14:W25Q16    
								0XEF15:W25Q32  
								0XEF16:W25Q64 
								0XEF17:W25Q128 
******************************************************************************/	 		  
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25Q64_CS=0;				    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25Q64_CS=1;				    
	return Temp;
}   

/*****************************************************************************
 * @name       :void W25Q64_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead) 
 * @date       :2020-05-08 
 * @function   :read spi flash
								Start reading data of specified length at the specified address							
 * @parameters :pBuffer:Data storage area
								ReadAddr:Address to start reading (24bit)
								NumByteToRead:Number of bytes to read (maximum 65535)
 * @retvalue   :None
******************************************************************************/			    
void W25Q64_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q64_CS=0;                               //使能器件   
  SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令   
  SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
  SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
  SPI1_ReadWriteByte((u8)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数  
  }
	W25Q64_CS=1;  				    	      
}

/*****************************************************************************
 * @name       :void W25Q64_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead) 
 * @date       :2020-05-08 
 * @function   :Write less than 256 bytes of data in a page(0~65535)
								Start writing data of specified length at the specified address							
 * @parameters :pBuffer:Data storage area
								WriteAddr:Address to start writing (24bit)
								NumByteToWrite:Number of bytes to write (maximum 256),This number 
								              should not exceed the number of bytes left on this page!!!
 * @retvalue   :None
******************************************************************************/	   
void W25Q64_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
  W25Q64_Write_Enable();                  //SET WEL 
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
  SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
  SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
  SPI1_ReadWriteByte((u8)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)
	{
		SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	}
	W25Q64_CS=1;                   //取消片选 
	W25Q64_Wait_Busy();					   //等待写入结束
} 

/*****************************************************************************
 * @name       :void W25Q64_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)  
 * @date       :2020-05-08 
 * @function   :Write SPI flash without check
								You must ensure that all the data in the address range written is 0xff, 
								otherwise the data written in non-0xff will fail!
								With automatic page change function	
								Start to write the specified length data at the specified address, 
								but make sure that the address does not cross the boundary!								
 * @parameters :pBuffer:Data storage area
								WriteAddr:Address to start writing (24bit)
								NumByteToWrite:Number of bytes to write (maximum 65535)
 * @retvalue   :None
******************************************************************************/	
void W25Q64_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q64_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)
		{
			break;//写入结束了
	 	}
		else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)
			{
				pageremain=256; //一次可以写入256个字节
			}
			else 
			{
				pageremain=NumByteToWrite; 	  //不够256个字节了
			}
		}
	}   
} 

u8 W25Q64_BUFFER[4096];	
/*****************************************************************************
 * @name       :void W25Q64_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)     
 * @date       :2020-05-08 
 * @function   :Write SPI flash 
								Start to write the specified length data at the specified address
								This function has erase operation!					
 * @parameters :pBuffer:Data storage area
								WriteAddr:Address to start writing (24bit)
								NumByteToWrite:Number of bytes to write (maximum 65535)
 * @retvalue   :None
******************************************************************************/	 
void W25Q64_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
  W25QXX_BUF=W25Q64_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q64_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)
			{
				break;//需要擦除  	  
			}
		}
		if(i<secremain)//需要擦除
		{
			W25Q64_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q64_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  
		}
		else 
		{
			W25Q64_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		}
		if(NumByteToWrite==secremain)
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
		  pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		  NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)
			{
				secremain=4096;	//下一个扇区还是写不完
			}
			else 
			{
				secremain=NumByteToWrite;			//下一个扇区可以写完了
			}
		}	 
	}	 
}

/*****************************************************************************
 * @name       :void W25Q64_Erase_Chip(void) 
 * @date       :2020-05-08 
 * @function   :Erase whole chip(Waiting time is too long)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void W25Q64_Erase_Chip(void)   
{                                   
  W25Q64_Write_Enable();                  //SET WEL 
  W25Q64_Wait_Busy();   
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	W25Q64_CS=1;                            //取消片选     	      
	W25Q64_Wait_Busy();   				   //等待芯片擦除结束
}

/*****************************************************************************
 * @name       :void W25Q64_Erase_Sector(u32 Dst_Addr)  
 * @date       :2020-05-08 
 * @function   :Erase a sector(Minimum time: 150ms)
 * @parameters :Dst_Addr:The sector address(Set according to actual capacity)
 * @retvalue   :None
******************************************************************************/	   
void W25Q64_Erase_Sector(u32 Dst_Addr)   
{    
 //	printf("fe:%x\r\n",Dst_Addr);	//监视falsh擦除情况,测试用   
 	Dst_Addr*=4096;
  W25Q64_Write_Enable();                  //SET WEL 	 
  W25Q64_Wait_Busy();   
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
  SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
  SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
  SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25Q64_CS=1;                            //取消片选     	      
  W25Q64_Wait_Busy();   				   //等待擦除完成
} 

/*****************************************************************************
 * @name       :void W25Q64_Wait_Busy(void)    
 * @date       :2020-05-08 
 * @function   :Waiting for idle
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void W25Q64_Wait_Busy(void)   
{   
	while((W25Q64_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
} 

/*****************************************************************************
 * @name       :void W25Q64_PowerDown(void)     
 * @date       :2020-05-08 
 * @function   :Enter power down mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void W25Q64_PowerDown(void)   
{ 
  W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	W25Q64_CS=1;                            //取消片选     	      
  delay_us(3);                               //等待TPD  
} 

/*****************************************************************************
 * @name       :void W25Q64_WAKEUP(void)       
 * @date       :2020-05-08 
 * @function   :Enter wake up mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void W25Q64_WAKEUP(void)   
{  
	W25Q64_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	W25Q64_CS=1;                            //取消片选     	      
  delay_us(3);                               //等待TRES1
}   

