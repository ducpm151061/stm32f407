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
#include "GUI.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_EDIT_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_1 (GUI_ID_USER + 0x03)
#define ID_BUTTON_2 (GUI_ID_USER + 0x04)
#define ID_BUTTON_3 (GUI_ID_USER + 0x05)
#define ID_BUTTON_4 (GUI_ID_USER + 0x06)
#define ID_BUTTON_5 (GUI_ID_USER + 0x07)
#define ID_BUTTON_6 (GUI_ID_USER + 0x08)
#define ID_BUTTON_7 (GUI_ID_USER + 0x09)
#define ID_BUTTON_8 (GUI_ID_USER + 0x0A)
#define ID_BUTTON_9 (GUI_ID_USER + 0x0B)
#define ID_BUTTON_10 (GUI_ID_USER + 0x0C)
#define ID_BUTTON_11 (GUI_ID_USER + 0x0D)
#define ID_TEXT_0 (GUI_ID_USER + 0x0E)
#define ID_IMAGE_0 (GUI_ID_USER + 0x0F)
#define ID_IMAGE_1 (GUI_ID_USER + 0x10)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const int useRelease = 1;
// USER START (Optionally insert additional static data)
extern void MTAB_CLIB_Update_Fr_Keypad(void);
extern void Info_Enter_Update_Fr_Keypad(void);
extern void DeleteRecord(void);
extern void ChangeModeData(void);
extern GUI_CONST_STORAGE GUI_BITMAP bmIconBack1;
extern void CAIDATCHUNG_Update_Fr_Keypad(void);
extern void HieuChinh_CLIB_Update_Fr_Keypad(void);
extern void HIEUCHINH_CLIB_CHECK(void);
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
    {
        {WINDOW_CreateIndirect, "WD_KeyNum", ID_WINDOW_0, 0, 20, 320, 460, 0, 0x0, 0},
        {EDIT_CreateIndirect, "Edit", ID_EDIT_0, 21, 62, 278, 45, 0, 0x32, 0},
        {BUTTON_CreateIndirect, "1", ID_BUTTON_0, 26, 157, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "3", ID_BUTTON_1, 206, 157, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "2", ID_BUTTON_2, 116, 157, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "4", ID_BUTTON_3, 26, 203, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "5", ID_BUTTON_4, 116, 203, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "6", ID_BUTTON_5, 206, 203, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "0", ID_BUTTON_6, 116, 295, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "7", ID_BUTTON_7, 26, 249, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "8", ID_BUTTON_8, 116, 249, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "9", ID_BUTTON_9, 206, 249, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "", ID_BUTTON_10, 206, 295, 87, 42, 0, 0x0, 0},
        {BUTTON_CreateIndirect, "", ID_BUTTON_11, 26, 295, 87, 42, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtHeader", ID_TEXT_0, 0, 0, 320, 30, 0, 0x0, 0},
        {IMAGE_CreateIndirect, "BmpBack", ID_IMAGE_1, 13, 410, 42, 42, 0, 0, 0},
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

static WM_HWIN ID_Text_handle;

const uint8_t Key0[] = {2, '0', ' '};
const uint8_t Key1[] = {5, '1', '.', ',', '+', '-'};
const uint8_t Key2[] = {4, '2', 'A', 'B', 'C'};
const uint8_t Key3[] = {4, '3', 'D', 'E', 'F'};
const uint8_t Key4[] = {4, '4', 'G', 'H', 'I'};
const uint8_t Key5[] = {4, '5', 'J', 'K', 'L'};
const uint8_t Key6[] = {4, '6', 'M', 'N', 'O'};
const uint8_t Key7[] = {5, '7', 'P', 'Q', 'R', 'S'};
const uint8_t Key8[] = {4, '8', 'T', 'U', 'V'};
const uint8_t Key9[] = {5, '9', 'X', 'Y', 'Z', 'W'};

const uint8_t *KeyPad[] =
    {
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
};

uint8_t OldKey = 1;

static void ProgressEnterKey(void);
static uint32_t lastKeyPressTime = 0;

static void ProgressKey(char Character)
{
    uint32_t currentKeyPressTime = xSystem.TickCount1Ms;
    if ((currentKeyPressTime < lastKeyPressTime + KPI_DELAY_MS))
    {
        return;
    }
    xSystem.Keypad.Str[xSystem.Keypad.StrLen] = KeyPad[Character - '0'][1];
    xSystem.Keypad.StrLen++;
    xSystem.Keypad.Str[xSystem.Keypad.StrLen] = 0;
    BeepKeypad();
    EDIT_SetText(ID_Text_handle, (char *)xSystem.Keypad.Str);
    lastKeyPressTime = xSystem.TickCount1Ms;
}
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
    GUI_CONST_STORAGE GUI_FONT *FontKeyPad = &GUI_FontArial24;

    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'WD_KeyNum'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEFDC));
        //
        // Initialization of 'TxtHeader'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetBkColor(hItem, GUI_MAKE_COLOR(0xFFEBC0));
        // USER START (Optionally insert additional code for further widget initialization)

        TEXT_SetText(hItem, (const char *)xSystem.Keypad.Header);

        ID_Text_handle = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
        EDIT_SetFont(ID_Text_handle, &GUI_FontArial24);
        EDIT_SetTextAlign(ID_Text_handle, GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetText(ID_Text_handle, (const char *)xSystem.Keypad.Str);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_7);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_8);
        BUTTON_SetFont(hItem, FontKeyPad);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9);
        BUTTON_SetFont(hItem, FontKeyPad);
        //
        // Initialization of 'E'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_10);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_MAKE_COLOR(0x00D5CECB));
        BUTTON_SetFrameColor(hItem, GUI_MAKE_COLOR(0x00D5CECB));
        BUTTON_SetFocusColor(hItem, GUI_MAKE_COLOR(0x00D5CECB));
        BUTTON_SetBitmapEx(hItem, BUTTON_BI_UNPRESSED, &bmIconEnter, 35, 16);
        //
        // Initialization of 'B'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_11);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_MAKE_COLOR(0x00E9731A));
        BUTTON_SetBitmapEx(hItem, BUTTON_BI_UNPRESSED, &bmIconBackSpace, 30, 12);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
        IMAGE_SetBitmap(hItem, &bmIconBack1);

        // USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_EDIT_0: // Notifications sent by 'Edit'
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
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_0: // Notifications sent by '1'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('1');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('1');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_1: // Notifications sent by '3'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('3');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('3');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_2: // Notifications sent by '2'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('2');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('2');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_3: // Notifications sent by '4'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('4');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('4');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_4: // Notifications sent by '5'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('5');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('5');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_5: // Notifications sent by '6'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('6');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('6');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_6: // Notifications sent by '0'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('0');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('0');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_7: // Notifications sent by '7'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('7');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('7');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_8: // Notifications sent by '8'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('8');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('8');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_9: // Notifications sent by '9'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    ProgressKey('9');
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    ProgressKey('9');
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_10: // Notifications sent by 'E'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    BeepKeypad();
                    xSystem.Keypad.Status = KP_ENTER;
                    EDIT_GetText(ID_Text_handle, (char *)xSystem.Keypad.Str, 40);
                    ProgressEnterKey();
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    BeepKeypad();
                    xSystem.Keypad.Status = KP_ENTER;
                    EDIT_GetText(ID_Text_handle, (char *)xSystem.Keypad.Str, 40);
                    ProgressEnterKey();
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_11: // Notifications sent by 'B'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                if (!useRelease)
                {
                    uint32_t currentKeyPressTime = xSystem.TickCount1Ms;
                    if (currentKeyPressTime < lastKeyPressTime + KPI_DELAY_MS)
                    {
                        break;
                    }
                    BeepKeypad();
                    if (xSystem.Keypad.StrLen > 0)
                    {
                        xSystem.Keypad.Str[xSystem.Keypad.StrLen - 1] = 0;
                        xSystem.Keypad.StrLen--;
                    }
                    else
                    {
                        xSystem.Keypad.Str[0] = 0;
                        OldKey = 1;
                    }
                    EDIT_SetText(ID_Text_handle, (char *)xSystem.Keypad.Str);
                    lastKeyPressTime = xSystem.TickCount1Ms;
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                if (useRelease)
                {
                    uint32_t currentKeyPressTime = xSystem.TickCount1Ms;
                    if (currentKeyPressTime < lastKeyPressTime + KPI_DELAY_MS)
                    {
                        break;
                    }
                    BeepKeypad();
                    if (xSystem.Keypad.StrLen > 0)
                    {
                        xSystem.Keypad.Str[xSystem.Keypad.StrLen - 1] = 0;
                        xSystem.Keypad.StrLen--;
                    }
                    else
                    {
                        xSystem.Keypad.Str[0] = 0;
                        OldKey = 1;
                    }
                    EDIT_SetText(ID_Text_handle, (char *)xSystem.Keypad.Str);
                    lastKeyPressTime = xSystem.TickCount1Ms;
                }
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        // USER START (Optionally insert additional code for further Ids)
        case ID_IMAGE_0: //
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                //BTP_Home();
                // GUI_WD_Change(xSystem.Keypad.WD_Req);
                break;
            }
            break;

        case ID_IMAGE_1: // Notifications sent by 'back'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                BTP_Back();
                break;
            }
            break;
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
*       CreateWD_KeyNum
*/
WM_HWIN CreateWD_KeyNum(void);
WM_HWIN CreateWD_KeyNum(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

// USER START (Optionally insert additional public code)

static void ProgressEnterKey(void)
{
    if (xSystem.Keypad.Status == KP_ENTER)
    {
        switch (xSystem.Keypad.WD_Req)
        {
        case WD_XOADULIEU:
            DeleteRecord();
            break;
        case WD_TRUYENDULIEU:
            ChangeModeData();
            break;
        case WD_CaiDatChung:
            CAIDATCHUNG_Update_Fr_Keypad();
            break;
        case WD_HIEUCHINH:
            HIEUCHINH_CLIB_CHECK();
            break;
        case WD_HIEUCHINH_CALIB:
            HieuChinh_CLIB_Update_Fr_Keypad();
            break;
        default:
            break;
        }
    }
}

// USER END

/*************************** End of file ****************************/
