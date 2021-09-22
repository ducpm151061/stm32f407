#include "DataDefine.h"
#include "UserGUIService.h"
#include "AlcoholMeter.h"
#include "GUI.h"
#include "GUI_Text_Define.h"
#include "Parameters.h"
#include "InitSystem.h"
// USB define
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
// USER END

#include "DIALOG.h"

#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_ICONVIEW_0 (GUI_ID_USER + 0x01)
#define ID_ICONVIEW_1 (GUI_ID_USER + 0x02)
#define ID_ICONVIEW_2 (GUI_ID_USER + 0x03)
#define ID_ICONVIEW_3 (GUI_ID_USER + 0x04)
//#define ID_IMAGE_0    (GUI_ID_USER + 0x05)
#define ID_IMAGE_1 (GUI_ID_USER + 0x06)
//#define ID_IMAGE_2    (GUI_ID_USER + 0x07)
#define ID_TEXT_0 (GUI_ID_USER + 0x08)
#define ID_TEXT_1 (GUI_ID_USER + 0x09)
#define ID_TEXT_2 (GUI_ID_USER + 0x0A)
#define ID_TEXT_3 (GUI_ID_USER + 0x0B)
#define ID_TEXT_4 (GUI_ID_USER + 0x0C)

extern GUI_CONST_STORAGE GUI_BITMAP bmHieuChinh;
extern GUI_CONST_STORAGE GUI_BITMAP bmXoaDuLieu;
extern GUI_CONST_STORAGE GUI_BITMAP bmCaiDatChung;
extern GUI_CONST_STORAGE GUI_BITMAP bmBatTruyenDuLieu;
extern GUI_CONST_STORAGE GUI_BITMAP bmTatTruyenDuLieu;
extern USB_OTG_CORE_HANDLE USB_OTG_dev;

static uint8_t state = 0;
static uint8_t isLogined = 0;
static uint8_t SelectedTabIndex = 0;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
    {
        {WINDOW_CreateIndirect, "WD_Setup", ID_WINDOW_0, 0, 20, 320, 460, 0, 0x0, 0},
        {ICONVIEW_CreateIndirect, "CaiDatChung", ID_ICONVIEW_0, 30, 55, 110, 110, 0, 0x00640064, 0},
        {ICONVIEW_CreateIndirect, "XoaDuLieu", ID_ICONVIEW_1, 30, 232, 110, 110, 0, 0x00640064, 0},
        {ICONVIEW_CreateIndirect, "TruyenDuLieu", ID_ICONVIEW_2, 180, 232, 110, 110, 0, 0x00640064, 0},
        {ICONVIEW_CreateIndirect, "HieuChinh", ID_ICONVIEW_3, 180, 55, 110, 110, 0, 0x00640064, 0},
        //{IMAGE_CreateIndirect, "BmpBack", ID_IMAGE_0, 50, 410, 42, 42, 0, 0, 0},
        {IMAGE_CreateIndirect, "BmpHome", ID_IMAGE_1, 138, 410, 42, 42, 0, 0, 0},
        //{IMAGE_CreateIndirect, "BmpSave", ID_IMAGE_2, 228, 410, 42, 42, 0, 0, 0},

        {TEXT_CreateIndirect, "Header", ID_TEXT_0, 0, 0, 320, 30, 0, 0x0, 0},
        {TEXT_CreateIndirect, "CAIDATCHUNG", ID_TEXT_1, 18, 179, 128, 18, 0, 0x0, 0},
        {TEXT_CreateIndirect, "HIEUCHINH", ID_TEXT_2, 189, 179, 94, 18, 0, 0x0, 0},
        {TEXT_CreateIndirect, "XOADULIEU", ID_TEXT_3, 30, 356, 105, 18, 0, 0x0, 0},
        {TEXT_CreateIndirect, "BATTRUYENDULEU", ID_TEXT_4, 172, 356, 129, 18, 0, 0x0, 0},
};

static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'WD_ThoiChuDong'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEBDF));
        //
        // Initialization of 'TxtLabel'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_0);
        ICONVIEW_AddBitmapItem(hItem, &bmCaiDatChung, "");
        //ICONVIEW_SetTextColor(hItem, 1, GUI_WHITE);
        //ICONVIEW_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_1);
        ICONVIEW_AddBitmapItem(hItem, &bmXoaDuLieu, "");
        //ICONVIEW_SetTextColor(hItem, 1, GUI_WHITE);
        //ICONVIEW_SetFont(hItem, &GUI_FontArial18);
        //ICONVIEW_SetBkColor(hItem,1, GUI_MAKE_COLOR(0x00FFEBDF));

        hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_2);
        //BUTTON_SetFont(hItem, &GUI_FontArial18);
        if (state == 1)
            ICONVIEW_AddBitmapItem(hItem, &bmBatTruyenDuLieu, "");
        else if (state == 0)
            ICONVIEW_AddBitmapItem(hItem, &bmTatTruyenDuLieu, "");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_3);
        ICONVIEW_AddBitmapItem(hItem, &bmHieuChinh, "");
        //ICONVIEW_SetTextColor(hItem, 1, GUI_WHITE);
        //ICONVIEW_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetText(hItem, Txt_SettingHeader);
        TEXT_SetBkColor(hItem, GUI_MAKE_COLOR(0xFFEBC0));

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        //TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, Txt_CAIDATCHUNG);
        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        //TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, Txt_BT3);
        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        //TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, Txt_BT1);
        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
        if (state == 1)
            TEXT_SetText(hItem, Txt_BT2);
        else if (state == 0)
            TEXT_SetText(hItem, Txt_BT2_1);

        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
        IMAGE_SetBitmap(hItem, &bmIconHome);

        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_ICONVIEW_0: // Notifications sent by 'IconNext'
            if (NCode == WM_NOTIFICATION_CLICKED)
            {
                xSystem.Keypad.WD_Req = WD_CDC;
                GUI_WD_Change(WD_CaiDatChung);
            }
            break;

        case ID_ICONVIEW_1: // Notifications sent by 'IconNext'
            if (NCode == WM_NOTIFICATION_CLICKED)
            {
                BT_XOADL();
            }
            break;

        case ID_ICONVIEW_2: // Notifications sent by 'IconNext'
            if (NCode == WM_NOTIFICATION_CLICKED)
            {
                if (state == 1)
                {
                    state = 0;
                    DeInitUSB();
                    DebugPrint("\rTat che do truyen du lieu\r\n");
                    GUI_WD_Change(WD_SETTING);
                }
                else
                {
                    BT_TruyenDL();
                }
                break;
            }
            break;

        case ID_ICONVIEW_3: // Notifications sent by 'IconNext'
            if (NCode == WM_NOTIFICATION_CLICKED)
            {
                BT_HIEUCHINH();
            }
            break;

        // Notifications sent by 'Button Home'
        case ID_IMAGE_1:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                BTP_Home();
                break;
            }
            break;
            // Notifications sent by 'Button Next'

            // USER END
        }
        break;
    // USER START (Optionally insert additional message handling)
    // USER END
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void BT_TruyenDL(void)
{
    xSystem.Keypad.WD_Req = WD_TRUYENDULIEU;
    xSystem.Keypad.Type_Req = KP_REQ_PASS;
    sprintf((char *)xSystem.Keypad.Header, Txt_NhapMaBaoMat);
    xSystem.Keypad.StrLen = sprintf((char *)xSystem.Keypad.Str, "");
    xSystem.Keypad.Index = 0;
    GUI_WD_Change(WD_KEY_NUM);
}
void ChangeModeData(void)
{
    if (xSystem.Keypad.Status == KP_ENTER)
    {
        if (xSystem.Keypad.Type_Req == KP_REQ_PASS)
        {
            if (BTP_CheckSetupSercurity() == 1)
            {
                state = 1;
                InitUSB();
                DebugPrint("\rBat che do truyen du lieu\r\n");
                GUI_WD_Change(WD_SETTING);
            }
            else
            {
                xSystem.CTP.CTP_Sens = 1;
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                strcpy((char *)xSystem.Keypad.Str, Txt_SaiMK);
                xSystem.Keypad.Status = 1;
                xSystem.Keypad.WD_Req = WD_SAI_MK_TDL;
                GUI_WD_Change(WD_NOTIFI);
            }
        }
    }
}
void BT_HIEUCHINH(void)
{
    if (isLogined == 0)
    {
        xSystem.Keypad.WD_Req = WD_HIEUCHINH;
        xSystem.Keypad.Type_Req = KP_REQ_PASS;
        sprintf((char *)xSystem.Keypad.Header, Txt_NhapMaBaoMat);
        xSystem.Keypad.StrLen = sprintf((char *)xSystem.Keypad.Str, "");
        xSystem.Keypad.Index = 0;
        GUI_WD_Change(WD_KEY_NUM);
    }
    else
    {
        GUI_WD_Change(WD_HIEUCHINH);
    }
}

void HIEUCHINH_CLIB_CHECK(void)
{
    if (xSystem.Keypad.Status == KP_ENTER)
    {
        if (xSystem.Keypad.Type_Req == KP_REQ_PASS)
        {
            if (BTP_CheckSetupSercurity() == 1)
            {
                GUI_WD_Change(WD_HIEUCHINH);
                isLogined = 1;
            }
            else
            {
                xSystem.CTP.CTP_Sens = 1;
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                strcpy((char *)xSystem.Keypad.Str, Txt_SaiMK);
                xSystem.Keypad.Status = 1;
                xSystem.Keypad.WD_Req = WD_SAI_MK_HC;
                GUI_WD_Change(WD_NOTIFI);
            }
        }
    }
}

WM_HWIN CreateWD_SETTING(void);
WM_HWIN CreateWD_SETTING(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}