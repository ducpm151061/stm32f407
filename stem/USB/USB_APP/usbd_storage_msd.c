#include "usbd_msc_mem.h"
#include "usb_conf.h"
#include "w25q64.h"
#include "sdio_sdcard.h"

//���֧�ֵ��豸��,2��
#define STORAGE_LUN_NBR 2

////////////////////////////�Լ������һ�����USB״̬�ļĴ���///////////////////
//bit0:��ʾ����������SD��д������
//bit1:��ʾ��������SD����������
//bit2:SD��д���ݴ����־λ
//bit3:SD�������ݴ����־λ
//bit4:1,��ʾ��������ѯ����(�������ӻ�������)
vu8 USB_STATUS_REG = 0;
////////////////////////////////////////////////////////////////////////////////

//USB Mass storage ��׼��ѯ����(ÿ��lunռ36�ֽ�)
const int8_t STORAGE_Inquirydata[] = {

	/* LUN 0 */
	0x00,
	0x80,
	0x02,
	0x02,
	(USBD_STD_INQUIRY_LENGTH - 4),
	0x00,
	0x00,
	0x00,
	/* Vendor Identification */
	'F', '4', '0', '7', 'V', 'x', 'T', '6', ' ', //9�ֽ�
	/* Product Identification */
	'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h', //15�ֽ�
	' ', 'D', 'i', 's', 'k', ' ',
	/* Product Revision Level */
	'1', '.', '0', ' ', //4�ֽ�

	/* LUN 1 */
	0x00,
	0x80,
	0x02,
	0x02,
	(USBD_STD_INQUIRY_LENGTH - 4),
	0x00,
	0x00,
	0x00,
	/* Vendor Identification */
	'F', '4', '0', '7', 'V', 'x', 'T', '6', ' ', //9�ֽ�
	/* Product Identification */
	'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ', //15�ֽ�
	'D', 'i', 's', 'k', ' ', ' ',
	/* Product Revision Level */
	'1', '.', '0', ' ', //4�ֽ�
};
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

//USB Device �û��ص������ӿ�
USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
	{
		STORAGE_Init,
		STORAGE_GetCapacity,
		STORAGE_IsReady,
		STORAGE_IsWriteProtected,
		STORAGE_Read,
		STORAGE_Write,
		STORAGE_GetMaxLun,
		(int8_t *)STORAGE_Inquirydata,
};
USBD_STORAGE_cb_TypeDef *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops; //ָ��USBD_MICRO_SDIO_fops�ṹ��.

//��ʼ���洢�豸
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_Init(uint8_t lun)
{
	SD_Init();
	W25Q64_Init();
	return 0;
}

//��ȡ�洢�豸�������Ϳ��С
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//block_num:������(������)
//block_size:���С(������С)
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
	if (lun == 1)
	{
		*block_size = 512;
		*block_num = SDCardInfo.CardCapacity / 512;
	}
	else
	{
		*block_size = 512;
		*block_num = 1024 * 1024 * 12 / 512; //SPI FLASH��ǰ��12M�ֽ�,�ļ�ϵͳ��
	}
	return 0;
}

//�鿴�洢�豸�Ƿ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,����;
//    ����,δ����
int8_t STORAGE_IsReady(uint8_t lun)
{
	USB_STATUS_REG |= 0X10; //�����ѯ
	return 0;
}

//�鿴�洢�豸�Ƿ�д����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,û��д����;
//    ����,д����(ֻ��)
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
	return 0;
}

//�Ӵ洢�豸��ȡ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫ��ȡ�ĵ�ַ(������ַ)
//blk_len:Ҫ��ȡ�Ŀ���(������)
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	int8_t res = 0;
	USB_STATUS_REG |= 0X02; //������ڶ�����
	if (lun == 1)
	{
		res = SD_ReadDisk(buf, blk_addr, blk_len);
		if (res)
			USB_STATUS_REG |= 0X08; //SD��������!
	}
	else
	{
		W25Q64_Read(buf, blk_addr * 512, blk_len * 512);
	}
	return res;
}
//��洢�豸д����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫд��ĵ�ַ(������ַ)
//blk_len:Ҫд��Ŀ���(������)
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	int8_t res = 0;
	USB_STATUS_REG |= 0X01; //�������д����
	if (lun == 1)
	{
		res = SD_WriteDisk(buf, blk_addr, blk_len);
		if (res)
			USB_STATUS_REG |= 0X04; //SD��д����!
	}
	else
	{
		W25Q64_Write(buf, blk_addr * 512, blk_len * 512);
	}
	return res;
}
//��ȡ֧�ֵ�����߼���Ԫ����
//����ֵ:֧�ֵ��߼���Ԫ����-1
int8_t STORAGE_GetMaxLun(void)
{
	return (STORAGE_LUN_NBR - 1);
}
