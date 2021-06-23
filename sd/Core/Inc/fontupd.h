#ifndef __FONTUPD_H__
#define __FONTUPD_H__	 
#include <stm32f4xx.h>



//������Ϣ�����ַ,ռ33���ֽ�,��1���ֽ����ڱ���ֿ��Ƿ����.����ÿ8���ֽ�һ��,�ֱ𱣴���ʼ��ַ���ļ���С														   
extern u32 FONTINFOADDR;	
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
 typedef struct
{
	u8 fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	u32 ugbkaddr; 			//unigbk�ĵ�ַ
	u32 ugbksize;			//unigbk�Ĵ�С	 
	u32 f12addr;			//gbk12��ַ	
	u32 gbk12size;			//gbk12�Ĵ�С	 
	u32 f16addr;			//gbk16��ַ
	u32 gbk16size;			//gbk16�Ĵ�С		 
	u32 f24addr;			//gbk24��ַ
	u32 gkb24size;			//gbk24�Ĵ�С 
}_font_info __attribute__((packed, aligned(1))); 


extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��

u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos);	//��ʾ���½���
u8 updata_fontx(u16 x,u16 y,u8 size,u8 *fxpath,u8 fx);	//����ָ���ֿ�
u8 update_font(u16 x,u16 y,u8 size,u8* src);			//����ȫ���ֿ�
u8 font_init(void);										//��ʼ���ֿ�
#endif





















