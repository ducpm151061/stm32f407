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
//STM32F407VxT6最小系统开发板内部SD卡示例
//     LED0           --->            PA1
//     KEY0           --->            PE4
//     KEY_UP         --->            PA0
//     SD_D0          --->            PC8
//     SD_D1          --->            PC9
//     SD_D2          --->            PC10
//     SD_D3          --->            PC11
//     SD_CLK         --->            PC12
//     SD_CMD         --->            PD2
//     SPI_CLK        --->            PB3
//     SPI_MOSI       --->            PB5
//     SPI_MISO       --->            PB4
//     USART1_TX      --->            PA9
//     USART1_RX      --->            PA10
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
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "led.h"
#include "key.h"
#include "ff.h" 
#include "malloc.h" 
#include "exfuns.h"  
#include "piclib.h"	
#include "usart.h" 
#include "w25q64.h"
#include "text.h"
#include "fontupd.h"
#include "sdio_sdcard.h"

void Check_SDCard(void)
{
	POINT_COLOR=RED;
	BACK_COLOR=WHITE;
	while(SD_Init())			//检测SD卡
	{
		LCD_ShowString(lcddev.width/2-54,70,16,"SDCard Failed!",1);
		delay_ms(200);
		LCD_Fill(lcddev.width/2-60,70,lcddev.width/2+60,86,WHITE);
		delay_ms(200);		    
	}	
	POINT_COLOR=BLUE;
	LCD_ShowString(lcddev.width/2-36,70,16,"SDCard OK",0);
}

void Check_Font(void)
{
	u8 key=0;
	POINT_COLOR=RED;
	BACK_COLOR=WHITE;
	while(font_init()) 		//检查字库
	{	 
		key=KEY_Scan(0);
		LCD_ShowString(lcddev.width/2-44,90,16,"Font Error!",1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-44,90,lcddev.width/2+44,106,WHITE);//清除显示	     
		delay_ms(200);
		LCD_ShowString(lcddev.width/2-104,120,16,"press KEY0 to update Font!",1);
		if(key==KEY0_PRES)
		{
			LCD_Fill(lcddev.width/2-104,120,lcddev.width/2+104,136,WHITE);//清除显示	 
			while(update_font(lcddev.width/2-100,120,16,"0:"))
			{
				LCD_ShowString(lcddev.width/2-76,120,16,"Font Update Failed!",1);
				delay_ms(200);
				LCD_Fill(lcddev.width/2-76,120,lcddev.width/2+76,136,WHITE);
				delay_ms(200);	
			}
			POINT_COLOR=BLUE;
			LCD_ShowString(lcddev.width/2-100,120,16,"  Font Update Success!  ",0);
		}
	}
	POINT_COLOR=BLUE;
	LCD_ShowString(lcddev.width/2-28,90,16,"Font OK",0);
}


/*****************************************************************************
 * @name       :void SDCard_test(void)
 * @date       :2018-08-09 
 * @function   :show SDCARD test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SDCard_test(void)
{
	printf("sdcard test\r\n");
	Gui_StrCenter(0,20,RED,BLUE,"STM32F407VxT6最小系统开发板",16,1);//居中显示
	Gui_StrCenter(0,45,RED,BLUE,"SDCARD 测试程序",16,1);//居中显示	
	Gui_StrCenter(0,170,BLUE,BLUE,"KEY0:NEXT KEY_UP:PREV",16,1);//居中显示	
	Gui_StrCenter(0,190,BLUE,BLUE,"2020-06-05",16,1);//居中显示
	Gui_StrCenter(0,210,GREEN,BLUE,"http://www.lcdwiki.com",16,1);//居中显示
}

u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息	
	u8 *fn;	 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//长文件名最大长度
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);//为长文件缓存区分配内存
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	} 
	return rval;
}

void Show_Picture(void)
{
	u8 res;
 	DIR picdir;	 		//图片目录
	FILINFO picfileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	u16 totpicnum; 		//图片文件总数
	u16 curindex;		//图片当前索引
	u8 key;				//键值
	u8 pause=0;			//暂停标记
	u8 t;
	u16 temp;
	u16 *picindextbl;	//图片索引表 　
	POINT_COLOR=RED;
	while(f_opendir(&picdir,"0:/PICTURE"))//打开图片文件夹
 	{	    
		Show_Str(lcddev.width/2-72,145,lcddev.width,16,"PICTURE文件夹错误!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-72,145,lcddev.width/2+72,161,WHITE);//清除显示	     
		delay_ms(200);				  
	}  
	totpicnum=pic_get_tnum("0:/PICTURE"); //得到总有效文件数
  while(totpicnum==NULL)//图片文件为0		
 	{	    
		Show_Str(lcddev.width/2-52,145,lcddev.width,16,"没有图片文件!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-52,145,lcddev.width/2+52,161,WHITE);//清除显示	     
		delay_ms(200);				  
	}
  picfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	picfileinfo.lfname=mymalloc(SRAMIN,picfileinfo.lfsize);	//为长文件缓存区分配内存
 	pname=mymalloc(SRAMIN,picfileinfo.lfsize);				//为带路径的文件名分配内存
 	picindextbl=mymalloc(SRAMIN,2*totpicnum);				//申请2*totpicnum个字节的内存,用于存放图片索引
 	while(picfileinfo.lfname==NULL||pname==NULL||picindextbl==NULL)//内存分配出错
 	{	    
		Show_Str(lcddev.width/2-52,145,lcddev.width,16,"内存分配失败!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-52,145,lcddev.width/2+52,161,WHITE);//清除显示	     
		delay_ms(200);				  
	}  
	//记录索引
    res=f_opendir(&picdir,"0:/PICTURE"); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
			temp=picdir.index;								//记录当前index
	        res=f_readdir(&picdir,&picfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				picindextbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}  
	POINT_COLOR=BLUE;
	Show_Str(lcddev.width/2-44,145,lcddev.width,16,"开始显示...",16,1); 
	delay_ms(1500);
	delay_ms(500);
	piclib_init();										//初始化画图	   	   
	curindex=0;											//从0开始显示
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//打开目录
	while(res==FR_OK)//打开成功
	{	
		dir_sdi(&picdir,picindextbl[curindex]);			//改变当前目录索引	   
        res=f_readdir(&picdir,&picfileinfo);       		//读取目录下的一个文件
        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出
     	fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
		strcpy((char*)pname,"0:/PICTURE/");				//复制路径(目录)
		strcat((char*)pname,(const char*)fn);  			//将文件名接在后面
 		LCD_Clear(BLACK);
 		ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//显示图片    
		Show_Str(2,2,240,16,pname,16,1); 				//显示图片名字
		t=0;
		while(1) 
		{
			key=KEY_Scan(0);		//扫描按键
			if(t>250)key=1;			//模拟一次按下KEY0    
			if((t%20)==0)LED0=!LED0;//LED0闪烁,提示程序正在运行.
			if(key==KEY_UP_PRES)		//上一张
			{
				if(curindex)curindex--;
				else curindex=totpicnum-1;
				break;
			}else if(key==KEY0_PRES)//下一张
			{
				curindex++;		   	
				if(curindex>=totpicnum)curindex=0;//到末尾的时候,自动从头开始
				break;
			}
			if(pause==0)t++;
			delay_ms(10); 
		}					    
		res=0;  
	} 											  
	myfree(SRAMIN,picfileinfo.lfname);	//释放内存			    
	myfree(SRAMIN,pname);				//释放内存			    
	myfree(SRAMIN,picindextbl);			//释放内存	
}

