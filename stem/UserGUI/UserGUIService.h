#ifndef __USERGUISERVICE_H__
#define __USERGUISERVICE_H__

#include "DataDefine.h"
#include "GUI.h"
#include "PROGBAR.h"
#include "TEXT.h"
#include "DIALOG.h"
#include "GUI_Text_Define.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmIconHome;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconBack;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconNext;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconPrint;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconBackSpace;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconEnter;

void UserGUI_WMInit(void);

void UserGUI_EndWindow(WM_HWIN WindowHandle);
void GUI_Manager_Change(void);
void GUI_WD_Change(WindowMenu_e Window);
void UserGUI_UpdateProgressBar(uint8_t value);
void DoThuDong_UpdateValueView(uint16_t Value);
void DoThuDong_UpdateStrView(uint8_t *Value);
void DoThuDong_UpdateADCValueView(uint16_t Value);

void DoChuDong_UpdateValueView(uint16_t Value);
void DoChuDong_UpdateStrView(uint8_t *Value);
void DoChuDong_UpdateProgessBar(uint8_t Value);
void DoChuDong_UpdateADCValueView(uint16_t Value);
void DoADC_UpdateProgessBar(uint8_t Value);

void WD_RP_Process_Enter(void);
void BTP_DoChuDong(void);
void BTP_DoThuDong(void);
void BTP_CaiDat(void);
void BTP_Back(void);
void BTP_Next(void);
void BTP_Home(void);
void BTP_Save(void);
void BTP_Print(void);
void BTP_ListRecord(void);
uint8_t BTP_CheckSetupSercurity(void);
void MultiPage_SetPage(uint8_t PageIndex);
void MultiPage_SetPage1(uint8_t PageIndex);
void GUI_WD_Noti(void);
void BTP_Do(void);
void BTP_Xa(void);
void BTP_DoADC(void);
void BT_XOADL(void);
void BT_TruyenDL(void);
void BT_HUY(void);
void BT_HIEUCHINH(void);
void NHAP_MK(void);
void WD_CDC_Process_Enter(void);
void BTP_TimeAuto(void);
void Set_TimeAuto(void);
void BT_SaveConfig(void);
void BTP_SaveCalib(void);

extern void CreateMainMenu_Window(void);
extern void CreateDoChuDong_Window(void);
extern void CreateDoThuDong_Window(void);
extern void CreateInfoEnter_Window(void);
extern void CreateMTABCalib_Window(void);
extern void CreateKeyInput_Window(void);
extern void CreateSetup_Window(void);
extern void ReloadCalibTable(void);
extern void CreateHeader_Window(void);
extern WM_HWIN CreateWD_ThoiChuDong(void);
extern WM_HWIN CreateWD_DangKhoiTaoCamBien(void);
extern WM_HWIN CreateWD_KetQuaDoChuDong(void);
extern WM_HWIN CreateWD_Setup(void);
extern WM_HWIN CreateWD_KeyNum(void);
extern WM_HWIN CreateWD_Report(void);
extern WM_HWIN CreateWD_Key(void);
extern WM_HWIN CreateWD_RecordList(void);
extern WM_HWIN CreateWD_Notification(void);
extern WM_HWIN CreateWD_NotiSimple(void);
extern WM_HWIN CreateWD_DoADC(void);
extern WM_HWIN CreateWD_KetQuaDoADC(void);
extern WM_HWIN CreateWD_SETTING(void);
extern WM_HWIN CreateWD_NOTIOK(void);
extern WM_HWIN CreateWD_CaiDatChung(void);
extern WM_HWIN CreateWD_HIEUCHINH(void);
extern System_t xSystem;

#endif
