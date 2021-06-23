//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ���������������ҵ��;
//����Ӳ������Ƭ��STM32F407VGT6,STM32F407VxT6��Сϵͳ������,��Ƶ168MHZ������8MHZ
//QDtech-TFTҺ������ for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//wiki����������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-21077707 
//�ֻ�: (����)18823372746 ������)15989313508
//����:(����/����) sales@qdtft.com  (�ۺ�/��������)service@qdtft.com
//QQ:(��ǰ��ѯ)3002706772 (����֧��)3002778157
//��������QQȺ:778679828
//��������:2020/06/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6��Сϵͳ�������ڲ�SD��ʾ��
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
//֧��ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD����ֱ�Ӳ���STM32F407VxT6��Сϵͳ������TFTLCD��ۻ���ͨ�����߽���TFTLCD����
//STM32����������ָTFTLCD��ۻ��߲��������ڲ����ӵ�STM32����
//=================================��Դ����=======================================//
//     LCDģ��                    STM32��������
//      VDD           --->           DC5V/3.3V          //��Դ
//      GND           --->             GND              //��Դ��
//=============================Һ���������߽���===================================//
//     LCDģ��                    STM32��������
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>Һ����16λ���������ź�
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================Һ���������߽���===================================//
//     LCDģ�� 				            STM32�������� 
//      WR            --->            PD5             //Һ����д���ݿ����ź�
//      RD            --->            PD4             //Һ���������ݿ����ź�
//      RS            --->            PD11            //Һ��������/��������ź�
//      RST           --->          ��λ���ţ�Ĭ�ϣ�  //Һ������λ�����źţ�Ҳ��ѡ��PD13��
//      CS            --->            PD7             //Һ����Ƭѡ�����ź�
//      BL            --->            PB15            //Һ������������ź�
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
	while(SD_Init())			//���SD��
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
	while(font_init()) 		//����ֿ�
	{	 
		key=KEY_Scan(0);
		LCD_ShowString(lcddev.width/2-44,90,16,"Font Error!",1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-44,90,lcddev.width/2+44,106,WHITE);//�����ʾ	     
		delay_ms(200);
		LCD_ShowString(lcddev.width/2-104,120,16,"press KEY0 to update Font!",1);
		if(key==KEY0_PRES)
		{
			LCD_Fill(lcddev.width/2-104,120,lcddev.width/2+104,136,WHITE);//�����ʾ	 
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
	Gui_StrCenter(0,20,RED,BLUE,"STM32F407VxT6��Сϵͳ������",16,1);//������ʾ
	Gui_StrCenter(0,45,RED,BLUE,"SDCARD ���Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,170,BLUE,BLUE,"KEY0:NEXT KEY_UP:PREV",16,1);//������ʾ	
	Gui_StrCenter(0,190,BLUE,BLUE,"2020-06-05",16,1);//������ʾ
	Gui_StrCenter(0,210,GREEN,BLUE,"http://www.lcdwiki.com",16,1);//������ʾ
}

u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ	
	u8 *fn;	 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	return rval;
}

void Show_Picture(void)
{
	u8 res;
 	DIR picdir;	 		//ͼƬĿ¼
	FILINFO picfileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totpicnum; 		//ͼƬ�ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ
	u8 pause=0;			//��ͣ���
	u8 t;
	u16 temp;
	u16 *picindextbl;	//ͼƬ������ ��
	POINT_COLOR=RED;
	while(f_opendir(&picdir,"0:/PICTURE"))//��ͼƬ�ļ���
 	{	    
		Show_Str(lcddev.width/2-72,145,lcddev.width,16,"PICTURE�ļ��д���!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-72,145,lcddev.width/2+72,161,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  
	totpicnum=pic_get_tnum("0:/PICTURE"); //�õ�����Ч�ļ���
  while(totpicnum==NULL)//ͼƬ�ļ�Ϊ0		
 	{	    
		Show_Str(lcddev.width/2-52,145,lcddev.width,16,"û��ͼƬ�ļ�!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-52,145,lcddev.width/2+52,161,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}
  picfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	picfileinfo.lfname=mymalloc(SRAMIN,picfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
 	pname=mymalloc(SRAMIN,picfileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
 	picindextbl=mymalloc(SRAMIN,2*totpicnum);				//����2*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
 	while(picfileinfo.lfname==NULL||pname==NULL||picindextbl==NULL)//�ڴ�������
 	{	    
		Show_Str(lcddev.width/2-52,145,lcddev.width,16,"�ڴ����ʧ��!",16,1);
		delay_ms(200);				  
		LCD_Fill(lcddev.width/2-52,145,lcddev.width/2+52,161,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  
	//��¼����
    res=f_opendir(&picdir,"0:/PICTURE"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=picdir.index;								//��¼��ǰindex
	        res=f_readdir(&picdir,&picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				picindextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}  
	POINT_COLOR=BLUE;
	Show_Str(lcddev.width/2-44,145,lcddev.width,16,"��ʼ��ʾ...",16,1); 
	delay_ms(1500);
	delay_ms(500);
	piclib_init();										//��ʼ����ͼ	   	   
	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&picdir,picindextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&picdir,&picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
		strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		LCD_Clear(BLACK);
 		ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//��ʾͼƬ    
		Show_Str(2,2,240,16,pname,16,1); 				//��ʾͼƬ����
		t=0;
		while(1) 
		{
			key=KEY_Scan(0);		//ɨ�谴��
			if(t>250)key=1;			//ģ��һ�ΰ���KEY0    
			if((t%20)==0)LED0=!LED0;//LED0��˸,��ʾ������������.
			if(key==KEY_UP_PRES)		//��һ��
			{
				if(curindex)curindex--;
				else curindex=totpicnum-1;
				break;
			}else if(key==KEY0_PRES)//��һ��
			{
				curindex++;		   	
				if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
				break;
			}
			if(pause==0)t++;
			delay_ms(10); 
		}					    
		res=0;  
	} 											  
	myfree(SRAMIN,picfileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,picindextbl);			//�ͷ��ڴ�	
}

