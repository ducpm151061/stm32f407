/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "DataDefine.h"
#include "UserGUIService.h"
#include "AlcoholMeter.h"
#include "GUI_Text_Define.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)
#define ID_TEXT_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_4 (GUI_ID_USER + 0x05)
#define ID_IMAGE_0 (GUI_ID_USER + 0x06)
#define ID_ICONVIEW_1 (GUI_ID_USER + 0x07)
#define ID_ICONVIEW_2 (GUI_ID_USER + 0x08)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static WM_HWIN TxtHeader, TxtKQLabel, TxtKQTDValue, TxtKQString, TxtKQUnit;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
    {
        {WINDOW_CreateIndirect, "WD_KetQuaDoChuDong", ID_WINDOW_0, 0, 20, 320, 460, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtHeader", ID_TEXT_0, 0, 0, 320, 30, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtKetQuaString", ID_TEXT_1, 16, 300, 287, 50, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtKetQuaLabel", ID_TEXT_2, 13, 68, 150, 22, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtKetQuaValue", ID_TEXT_3, 0, 141, 320, 60, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtKetQuaUnit", ID_TEXT_4, 86, 223, 149, 50, 0, 0x0, 0},
        {IMAGE_CreateIndirect, "IconHome", ID_IMAGE_0, 139, 410, 42, 42, 0, 0x00200020, 0},
        // { ICONVIEW_CreateIndirect, "IconBack", ID_ICONVIEW_1, 20, 370, 42, 42, 0, 0x00200020, 0 },
        // { ICONVIEW_CreateIndirect, "IconNext", ID_ICONVIEW_2, 258, 370, 42, 42, 0, 0x00200020, 0 },
        // USER START (Optionally insert additional widgets)
        // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
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
        // Initialization of 'WD_KetQuaDoChuDong'
        //
        DebugPrint("\rWM_INIT_DIALOG WD_DoThuDong\r\n");
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEFDC));
        // USER START (Optionally insert additional code for further widget initialization)
        //
        // Initialization of 'TxtHeader'
        //
        TxtHeader = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetTextAlign(TxtHeader, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(TxtHeader, &GUI_FontArial21);
        TEXT_SetText(TxtHeader, "ĐO THỤ ĐỘNG");
        TEXT_SetBkColor(TxtHeader, GUI_MAKE_COLOR(0xFFEBC0));
        //
        // Initialization of 'TxtKQLabel'
        //
        TxtKQLabel = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetTextAlign(TxtKQLabel, GUI_TA_LEFT | GUI_TA_VCENTER);
        TEXT_SetFont(TxtKQLabel, &GUI_FontArial24);
        TEXT_SetText(TxtKQLabel, Txt_KetQuaKiemTra);
        //
        // Initialization of 'TxtKQValue'
        //
        TxtKQTDValue = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetTextAlign(TxtKQTDValue, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(TxtKQTDValue, &GUI_FontArial96_B_Lite);
        if (xSystem.Record.Name.KetQua.value <= 0.05)
        {
            xSystem.Record.Name.KetQua.value = 0.00;
        }
        char Buff[20];
        snprintf(Buff, sizeof(Buff), "%.2f", xSystem.Record.Name.KetQua.value);
        if (xSystem.Record.Name.KetQua.value <= 0.05)
        {
            //TEXT_SetText(TxtKQTDValue,Txt_KetQuaThuDongKoCo);
            TEXT_SetText(TxtKQTDValue, Buff);
            TEXT_SetTextColor(TxtKQTDValue, GUI_MAKE_COLOR(0x00389710));
        }
        else
        {
            //TEXT_SetText(TxtKQTDValue,Txt_KetQuaThuDongCo);
            TEXT_SetText(TxtKQTDValue, Buff);
            TEXT_SetTextColor(TxtKQTDValue, GUI_RED);
        }

        //
        // Initialization of 'TxtKQUnit'
        //
        TxtKQUnit = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
        TEXT_SetTextAlign(TxtKQUnit, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(TxtKQUnit, &GUI_FontArial32);
        TEXT_SetText(TxtKQUnit, "mg/L");

        TxtKQString = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetTextAlign(TxtKQString, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(TxtKQString, &GUI_FontArial24);
        if (xSystem.Record.Name.KetQua.value >= 0.05)
        {
            TEXT_SetTextColor(TxtKQString, GUI_RED);
            TEXT_SetText(TxtKQString, Txt_StringThuDongCo);
        }
        else
        {
            TEXT_SetTextColor(TxtKQString, GUI_MAKE_COLOR(0x00389710));
            TEXT_SetText(TxtKQString, Txt_StringThuDongKoCo);
        }
        //
        // Initialization of 'TxtKQValue'
        //

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        IMAGE_SetBitmap(hItem, &bmIconHome);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_2);
        IMAGE_SetBitmap(hItem, &bmIconNext);

        xSystem.Record.Name.Time = xSystem.Rtc->GetDateTime();
        xSystem.Record.Name.KinhDo.value = xSystem.GPS->GetPosition().KinhDo.value;
        xSystem.Record.Name.ViDo.value = xSystem.GPS->GetPosition().ViDo.value;

        // USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_IMAGE_0: // Notifications sent by 'IconHome'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                BTP_Home();
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SCROLL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_ICONVIEW_1: // Notifications sent by 'IconBack'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SCROLL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_ICONVIEW_2: // Notifications sent by 'IconNext'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // BTP_DoChuDong();
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                BTP_Next();
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SCROLL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
            // USER START (Optionally insert additional code for further Ids)
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

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWD_KetQuaDoChuDong
*/
//WM_HWIN CreateWD_KetQuaDoChuDong(void);
void CreateDoThuDong_Window(void)
{
    xSystem.GUI_Window[WD_DO_THU_DONG] = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)

// USER END

/*************************** End of file ****************************/
