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
#include "GUI.h"
#include "GUI_Text_Define.h"
#include "Parameters.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_MULTIPAGE_0 (GUI_ID_USER + 0x01)
#define ID_IMAGE_0 (GUI_ID_USER + 0x03)
#define ID_IMAGE_1 (GUI_ID_USER + 0x04)
#define ID_IMAGE_2 (GUI_ID_USER + 0x05)
#define ID_IMAGE_3 (GUI_ID_USER + 0x06)
#define ID_TEXT_40 (GUI_ID_USER + 0x028)

// USER START (Optionally insert additional defines)
#define ID_WPAGE_1 (GUI_ID_USER + 0x10)

#define ID_TEXT_0 (ID_WPAGE_1 + 0x01)
#define ID_SLIDER_0 (ID_WPAGE_1 + 0x02)
#define ID_TEXT_1 (ID_WPAGE_1 + 0x03)
#define ID_SLIDER_1 (ID_WPAGE_1 + 0x04)

////////////////////////////////////////////////

#define ID_WPAGE_2 (GUI_ID_USER + 0x20)

#define ID_TEXT_20 (ID_WPAGE_2 + 0x01)
#define ID_TEXT_21 (ID_WPAGE_2 + 0x02)
#define ID_TEXT_22 (ID_WPAGE_2 + 0x03)
#define ID_TEXT_23 (ID_WPAGE_2 + 0x04)

#define ID_EDIT_21 (ID_WPAGE_2 + 0x05)
#define ID_EDIT_22 (ID_WPAGE_2 + 0x06)
#define ID_TEXT_24 (ID_WPAGE_2 + 0x07)

#define ID_EDIT_23 (ID_WPAGE_2 + 0x08)
#define ID_EDIT_24 (ID_WPAGE_2 + 0x09)
#define ID_TEXT_25 (ID_WPAGE_2 + 0x0A)

#define ID_EDIT_25 (ID_WPAGE_2 + 0x0B)
#define ID_EDIT_26 (ID_WPAGE_2 + 0x0C)
#define ID_TEXT_26 (ID_WPAGE_2 + 0x0D)

#define ID_EDIT_27 (ID_WPAGE_2 + 0x0E)
#define ID_EDIT_28 (ID_WPAGE_2 + 0x0F)
#define ID_TEXT_27 (ID_WPAGE_2 + 0x10)

#define ID_EDIT_29 (ID_WPAGE_2 + 0x11)
#define ID_EDIT_30 (ID_WPAGE_2 + 0x12)
#define ID_TEXT_28 (ID_WPAGE_2 + 0x13)

#define ID_EDIT_31 (ID_WPAGE_2 + 0x014)
#define ID_EDIT_32 (ID_WPAGE_2 + 0x15)
#define ID_TEXT_29 (ID_WPAGE_2 + 0x016)

#define ID_EDIT_33 (ID_WPAGE_2 + 0x17)
#define ID_EDIT_34 (ID_WPAGE_2 + 0x18)
#define ID_TEXT_30 (ID_WPAGE_2 + 0x19)

#define ID_EDIT_35 (ID_WPAGE_2 + 0x1A)
#define ID_EDIT_36 (ID_WPAGE_2 + 0x1B)
#define ID_TEXT_31 (ID_WPAGE_2 + 0x1C)

#define ID_EDIT_37 (ID_WPAGE_2 + 0x1D)
#define ID_EDIT_38 (ID_WPAGE_2 + 0x1E)
#define ID_TEXT_32 (ID_WPAGE_2 + 0x1F)

#define ID_EDIT_39 (ID_WPAGE_2 + 0x20)
#define ID_EDIT_40 (ID_WPAGE_2 + 0x21)
#define ID_TEXT_33 (ID_WPAGE_2 + 0x22)

#define ID_EDIT_41 (ID_WPAGE_2 + 0x23)
#define ID_EDIT_42 (ID_WPAGE_2 + 0x24)
#define ID_TEXT_34 (ID_WPAGE_2 + 0x25)

#define ID_TEXT_35 (ID_WPAGE_2 + 0x26)
#define ID_TEXT_36 (ID_WPAGE_2 + 0x27)

#define IsID_EDIT_TEXT(ID) (((ID) == ID_EDIT_21) || \
                            ((ID) == ID_EDIT_22) || \
                            ((ID) == ID_EDIT_23) || \
                            ((ID) == ID_EDIT_24) || \
                            ((ID) == ID_EDIT_25) || \
                            ((ID) == ID_EDIT_26) || \
                            ((ID) == ID_EDIT_27) || \
                            ((ID) == ID_EDIT_28) || \
                            ((ID) == ID_EDIT_29) || \
                            ((ID) == ID_EDIT_30) || \
                            ((ID) == ID_EDIT_31) || \
                            ((ID) == ID_EDIT_32) || \
                            ((ID) == ID_EDIT_33) || \
                            ((ID) == ID_EDIT_34) || \
                            ((ID) == ID_EDIT_35) || \
                            ((ID) == ID_EDIT_36) || \
                            ((ID) == ID_EDIT_37) || \
                            ((ID) == ID_EDIT_38) || \
                            ((ID) == ID_EDIT_39) || \
                            ((ID) == ID_EDIT_40) || \
                            ((ID) == ID_EDIT_41) || \
                            ((ID) == ID_EDIT_42))

/////////////////////////////////////////////////////////////////

// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)

static uint16_t ID_EDIT_FOCUS = 0;
static uint8_t SelectedTabIndex = 0;
extern uint32_t GetNumberFromString(uint16_t BeginAddress, char *Buffer);
extern GUI_CONST_STORAGE GUI_BITMAP bmIconADC;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconDFU;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconBack1;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconSave;
extern void MultiPage_SetPage(uint8_t PageIndex);
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
    {
        {WINDOW_CreateIndirect, "WD_Setup", ID_WINDOW_0, 0, 20, 320, 460, 0, 0x0, 0},
        {MULTIPAGE_CreateIndirect, "MultiPage", ID_MULTIPAGE_0, 0, 30, 320, 370, 0, 0x0, 0},
        {TEXT_CreateIndirect, "HEADER", ID_TEXT_40, 0, 0, 320, 30, 0, 0x0, 0},
        {IMAGE_CreateIndirect, "BmpBack", ID_IMAGE_0, 12, 410, 42, 42, 0, 0, 0},
        {IMAGE_CreateIndirect, "BmpSave", ID_IMAGE_1, 97, 410, 42, 42, 0, 0, 0},
        {IMAGE_CreateIndirect, "BmpDFU", ID_IMAGE_2, 181, 410, 42, 42, 0, 0, 0},
        {IMAGE_CreateIndirect, "BmpADC", ID_IMAGE_3, 266, 410, 42, 42, 0, 0, 0},

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

static const GUI_WIDGET_CREATE_INFO _aDialogCreate_Page1[] =
    {

        {WINDOW_CreateIndirect, "Page 1", ID_WPAGE_1, 0, 50, 320, 440, FRAMEWIN_CF_MOVEABLE},
        {SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 125, 34, 182, 35, 0, 0x0, 0},
        {TEXT_CreateIndirect, "TxtBlowTime", ID_TEXT_0, 13, 44, 80, 30, 0, 0x64, 0},
        {TEXT_CreateIndirect, "TxtBlowLevel", ID_TEXT_1, 13, 94, 80, 30, 0, 0x64, 0},
        {SLIDER_CreateIndirect, "Slider", ID_SLIDER_1, 125, 84, 182, 35, 0, 0x0, 0},

};

/*********************************************************************
*
*       _cbDialog_Page1
*/
static void _cbDialog_Page1(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'TxtBlowTime'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEBDF));

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetText(hItem, Txt_ThoiGian);
        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetText(hItem, Txt_BlowApSuat);
        TEXT_SetFont(hItem, &GUI_FontArial18);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
        SLIDER_SetRange(hItem, 1, 5);
        SLIDER_SetNumTicks(hItem, 5);
        SLIDER_SetFocusColor(hItem, GUI_RED);
        SLIDER_SetWidth(hItem, 20);
        SLIDER_SetValue(hItem, xSystem.Parameters.BlowTime);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
        SLIDER_SetRange(hItem, 1, 5);
        SLIDER_SetNumTicks(hItem, 5);
        SLIDER_SetFocusColor(hItem, GUI_RED);
        SLIDER_SetWidth(hItem, 20);
        SLIDER_SetValue(hItem, xSystem.Parameters.BlowPressure);

        break;

    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_SLIDER_0: // Notifications sent by 'Slider'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
                xSystem.Parameters.BlowTime = SLIDER_GetValue(hItem);
                break;
            }
            break;
        case ID_SLIDER_1: // Notifications sent by 'Slider'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
                xSystem.Parameters.BlowPressure = SLIDER_GetValue(hItem);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

//=====================================================================

static const GUI_WIDGET_CREATE_INFO _aDialogCreate_Page2[] =
    {

        {WINDOW_CreateIndirect, "Page 2", ID_WPAGE_2, 0, 0, 320, 440, FRAMEWIN_CF_MOVEABLE},
        {TEXT_CreateIndirect, "ID_TEXT_0", ID_TEXT_20, 40, 10, 50, 20, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_1", ID_TEXT_21, 100, 10, 50, 20, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_2", ID_TEXT_22, 0, 10, 40, 20, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_3", ID_TEXT_23, 160, 10, 40, 20, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_4", ID_TEXT_35, 200, 10, 50, 20, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_5", ID_TEXT_36, 260, 10, 50, 20, 0, 0x0, 0},

        {TEXT_CreateIndirect, "ID_TEXT_6", ID_TEXT_24, 0, 40, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_7", ID_TEXT_25, 0, 85, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_8", ID_TEXT_26, 0, 130, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_9", ID_TEXT_27, 0, 175, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_10", ID_TEXT_28, 0, 220, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_11", ID_TEXT_29, 0, 265, 30, 40, 0, 0x0, 0},

        {TEXT_CreateIndirect, "ID_TEXT_12", ID_TEXT_30, 160, 40, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_13", ID_TEXT_31, 160, 85, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_14", ID_TEXT_32, 160, 130, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_15", ID_TEXT_33, 160, 175, 30, 40, 0, 0x0, 0},
        {TEXT_CreateIndirect, "ID_TEXT_16", ID_TEXT_34, 160, 220, 30, 40, 0, 0x0, 0},

        {EDIT_CreateIndirect, "ID_EDIT_21", ID_EDIT_21, 40, 40, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_22", ID_EDIT_22, 100, 40, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_1", ID_BUTTON_21, 100, 30, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_23", ID_EDIT_23, 40, 85, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_24", ID_EDIT_24, 100, 85, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_2", ID_BUTTON_22, 100, 55, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_25", ID_EDIT_25, 40, 130, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_26", ID_EDIT_26, 100, 130, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_3", ID_BUTTON_23, 100, 80, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_27", ID_EDIT_27, 40, 175, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_28", ID_EDIT_28, 100, 175, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_4", ID_BUTTON_24, 100, 105, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_29", ID_EDIT_29, 40, 220, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_30", ID_EDIT_30, 100, 220, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_5", ID_BUTTON_25, 100, 130, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_31", ID_EDIT_31, 40, 265, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_32", ID_EDIT_32, 100, 265, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_6", ID_BUTTON_26, 100, 155, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_33", ID_EDIT_33, 195, 40, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_34", ID_EDIT_34, 255, 40, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_7", ID_BUTTON_27, 100, 205, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_35", ID_EDIT_35, 195, 85, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_36", ID_EDIT_36, 255, 85, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_8", ID_BUTTON_28, 100, 235, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_37", ID_EDIT_37, 195, 130, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_38", ID_EDIT_38, 255, 130, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_9", ID_BUTTON_29, 100, 265, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_39", ID_EDIT_39, 195, 175, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_40", ID_EDIT_40, 255, 175, 60, 40, 0, 0x0, 0},
        //{ BUTTON_CreateIndirect, "ID_BUTTON_9", ID_BUTTON_29, 100, 295, 80, 25, 0, 0x0, 0 },

        {EDIT_CreateIndirect, "ID_EDIT_41", ID_EDIT_41, 195, 220, 60, 40, 0, 0x0, 0},
        {EDIT_CreateIndirect, "ID_EDIT_42", ID_EDIT_42, 255, 220, 60, 40, 0, 0x0, 0},
        // { BUTTON_CreateIndirect, "ID_BUTTON_0", ID_BUTTON_35, 195, 270, 100, 40, 0, 0x0, 0 },

        //{SCROLLBAR_CreateIndirect,"ScrollBar", ID_SCROLLBAR_1, 310, 50, 5,260,0,0x0,0  },
};

/*********************************************************************
*
*       _cbDialog_Page2
*/
static void _cbDialog_Page2(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    char Buff[10];
    Al_CalibTable_t *AlMCalibData = &xSystem.Parameters.AlMCalibData;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:

        //
        // Initialization of TEXT
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEBDF));

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_20);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "ADC");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_21);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "Mg/L");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_22);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "TT");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_23);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "TT");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_35);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "ADC");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_36);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "Mg/L");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_24);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "1");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_25);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "2");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_26);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "3");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_27);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "4");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_28);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "5");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_29);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "6");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_30);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "7");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_31);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "8");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_32);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "9");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_33);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "10");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_34);
        TEXT_SetFont(hItem, &GUI_FontArial21);
        TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        TEXT_SetText(hItem, "11");

        //
        // Initialization of 'Value1_Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_21);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[0].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_22);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[0].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);
        //
        // Initialization of
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_23);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[1].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_24);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[1].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);
        //
        // Initialization of
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_25);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[2].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_26);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[2].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        //
        // Initialization of
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_27);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[3].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_28);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[3].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_29);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[4].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_30);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[4].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_31);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[5].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_32);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[5].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_33);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[6].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_34);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[6].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_35);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[7].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_36);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[7].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_37);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[8].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_38);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[8].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_39);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[9].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_40);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[9].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_41);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[10].ValueADC);
        EDIT_SetText(hItem, (const char *)Buff);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_42);
        EDIT_SetFont(hItem, &GUI_FontArial21);
        EDIT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
        sprintf((char *)Buff, "%04d", AlMCalibData->s.Data[10].ValueMgL);
        EDIT_SetText(hItem, (const char *)Buff);

        /* hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);
        BUTTON_SetFont(hItem, &GUI_FontArial21);
        BUTTON_SetText(hItem, MTAB_CLIB_BT1); */

        if (IsID_EDIT_TEXT(ID_EDIT_FOCUS))
        {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FOCUS);
            EDIT_SetFocussable(hItem, ENABLE);
        }
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {

        // ================================================================================
        // ================================================================================
        /*case ID_BUTTON_35:
              switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                      UpdateParametersToMemory();
          case WM_NOTIFICATION_RELEASED:
              break;
        }
        break;*/
        case ID_EDIT_21:
        case ID_EDIT_22:
        case ID_EDIT_23:
        case ID_EDIT_24:
        case ID_EDIT_25:
        case ID_EDIT_26:
        case ID_EDIT_27:
        case ID_EDIT_28:
        case ID_EDIT_29:
        case ID_EDIT_30:
        case ID_EDIT_31:
        case ID_EDIT_32:
        case ID_EDIT_33:
        case ID_EDIT_34:
        case ID_EDIT_35:
        case ID_EDIT_36:
        case ID_EDIT_37:
        case ID_EDIT_38:
        case ID_EDIT_39:
        case ID_EDIT_40:
        case ID_EDIT_41:
        case ID_EDIT_42:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                ID_EDIT_FOCUS = Id;
                strcpy((char *)xSystem.Keypad.Header, Txt_Nhap);
                xSystem.Keypad.WD_Req = WD_HIEUCHINH_CALIB;
                xSystem.Keypad.Type_Req = KP_REQ_VALUE;
                hItem = WM_GetDialogItem(pMsg->hWin, Id);
                xSystem.Keypad.StrLen = KPI_CONTENTSIZE;
                EDIT_GetText(hItem, (char *)xSystem.Keypad.Str, xSystem.Keypad.StrLen);
                xSystem.Keypad.StrLen = strlen((const char *)xSystem.Keypad.Str);
                GUI_WD_Change(WD_KEY_NUM);
                break;
            }
            break;
            // ================================================================================
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void HieuChinh_CLIB_Update_Fr_Keypad(void)
{
    WM_HWIN hItem;
    Al_CalibTable_t *AlMCalibData = &xSystem.Parameters.AlMCalibData;
    if (xSystem.Keypad.Status == KP_ENTER)
    {
        switch (ID_EDIT_FOCUS)
        {
        case ID_EDIT_21:
            AlMCalibData->s.Data[0].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_23:
            AlMCalibData->s.Data[1].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_25:
            AlMCalibData->s.Data[2].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_27:
            AlMCalibData->s.Data[3].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_29:
            AlMCalibData->s.Data[4].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_31:
            AlMCalibData->s.Data[5].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_33:
            AlMCalibData->s.Data[6].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_35:
            AlMCalibData->s.Data[7].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_37:
            AlMCalibData->s.Data[8].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_39:
            AlMCalibData->s.Data[9].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_41:
            AlMCalibData->s.Data[10].ValueADC = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_22:
            AlMCalibData->s.Data[0].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_24:
            AlMCalibData->s.Data[1].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_26:
            AlMCalibData->s.Data[2].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_28:
            AlMCalibData->s.Data[3].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_30:
            AlMCalibData->s.Data[4].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_32:
            AlMCalibData->s.Data[5].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_34:
            AlMCalibData->s.Data[6].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_36:
            AlMCalibData->s.Data[7].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_38:
            AlMCalibData->s.Data[8].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_40:
            AlMCalibData->s.Data[9].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;

        case ID_EDIT_42:
            AlMCalibData->s.Data[10].ValueMgL = GetNumberFromString(0, (char *)xSystem.Keypad.Str);
            break;
        }
    }
    GUI_WD_Change(WD_HIEUCHINH);
}

//=====================================================================

/*********************************************************************
*
*       _cbDialog_Page2
*/
void MultiPage_SetPage1(uint8_t PageIndex)
{
    SelectedTabIndex = PageIndex;
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
    WM_HWIN hMultiPage;
    WM_HWIN hDialog;
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'WD_Setup'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFEFDC));
        // USER START (Optionally insert additional code for further widget initialization)

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        IMAGE_SetBitmap(hItem, &bmIconBack1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
        IMAGE_SetBitmap(hItem, &bmIconSave);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2);
        IMAGE_SetBitmap(hItem, &bmIconDFU);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_3);
        IMAGE_SetBitmap(hItem, &bmIconADC);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_40);
        TEXT_SetText(hItem, Txt_HeaderHieuChinh);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(hItem, &GUI_FontArial21);

        hMultiPage = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
        MULTIPAGE_SetTabHeight(hMultiPage, 30);
        MULTIPAGE_SetFont(hMultiPage, &GUI_FontArial21);

        hDialog = GUI_CreateDialogBox(_aDialogCreate_Page1, GUI_COUNTOF(_aDialogCreate_Page1), _cbDialog_Page1, WM_UNATTACHED, 0, 0);
        MULTIPAGE_AddPage(hMultiPage, hDialog, Txt_CoBan);
        MULTIPAGE_SetTabWidth(hMultiPage, 158, 0);

        hDialog = GUI_CreateDialogBox(_aDialogCreate_Page2, GUI_COUNTOF(_aDialogCreate_Page2), _cbDialog_Page2, WM_UNATTACHED, 0, 0);
        MULTIPAGE_AddPage(hMultiPage, hDialog, Txt_NangCao);
        MULTIPAGE_SetTabWidth(hMultiPage, 158, 1);

        MULTIPAGE_SelectPage(hMultiPage, SelectedTabIndex);
        // USER END
        break;
    // USER START (Optionally insert additional message handling)
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_MULTIPAGE_0: // Notifications sent by 'MultiPage'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
                hMultiPage = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
                SelectedTabIndex = MULTIPAGE_GetSelection(hMultiPage);

                break;
            }
            break;
        // Notifications sent by 'Button Back'
        case ID_IMAGE_0:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                GUI_WD_Change(WD_SETTING);
                break;
            }
            break;
        // Notifications sent by 'Button Save'
        case ID_IMAGE_1:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                BTP_SaveCalib();
                break;
            }
            break;
        // Notifications sent by 'Button DFU'
        case ID_IMAGE_2:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
            {
                xSystem.TimeOut.Clk1s_JumpToDFU = 10;
            }
            break;
            }
            break;
        case ID_IMAGE_3:
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                BTP_DoADC();
                break;
            }
            break;
        }
        break;
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
*       CreateWD_Setup
*/
WM_HWIN CreateWD_HIEUCHINH(void);
WM_HWIN CreateWD_HIEUCHINH(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/