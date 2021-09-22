#include "UserGUIService.h"
#include "DIALOG.h"

//===========================================================================================================
//===========================================================================================================
//===========================================================================================================

//===========================================================================================================
//===========================================================================================================
//===========================================================================================================

//===========================================================================================================
//===========================================================================================================
//===========================================================================================================

void UserGUI_StartWindow(uint8_t WindowID);

static void Callback_BackgroundWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
    //=====================================================================================================
    //=====================================================================================================
    case WM_PAINT:
    {
        //==================================================================================================
        GUI_Clear();
        //==================================================================================================
        break;
        //==================================================================================================
    }
    //=====================================================================================================
    //=====================================================================================================
    default:
    {
        WM_DefaultProc(pMsg);
    }
        //=====================================================================================================
        //=====================================================================================================
    }
}

void UserGUI_WMInit(void)
{

    //Activate the use of memory device feature
    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_SetCallback(WM_HBKWIN, Callback_BackgroundWindow);
    //========================================================================================================
    //========================================================================================================
    //========================================================================================================
    //Activates UTF-8
    GUI_UC_SetEncodeUTF8();
    GUI_SetDefaultFont(&GUI_FontArial21);
    //=======================

    xSystem.GUI_Manager.NextWindowID = xSystem.GUI_Manager.CurrentWindowID = WD_MAIN_MENU;
    CreateHeader_Window();
    UserGUI_StartWindow(xSystem.GUI_Manager.NextWindowID);
    xSystem.Keypad.CAP_State = 1;
}

void UserGUI_StartWindow(uint8_t WindowID)
{

    switch (WindowID)
    {
    case WD_MAIN_MENU:
        CreateMainMenu_Window();
        DebugPrint("\rCreateMainMenu_Window.\r\n");
        break;
    case WD_DO_THU_DONG:
        CreateDoThuDong_Window();
        DebugPrint("\rCreateDoThuDong_Window.\r\n");
        break;

    case WD_KHOITAO_CAMBIEN:
        xSystem.GUI_Window[WD_KHOITAO_CAMBIEN] = CreateWD_DangKhoiTaoCamBien();
        DebugPrint("\rCreateWD_DangKhoiTaoCamBien.\r\n");
        break;

    case WD_DO_CHU_DONG:
        xSystem.GUI_Window[WD_DO_CHU_DONG] = CreateWD_ThoiChuDong();
        DebugPrint("\rCreateDoChuDong_Window.\r\n");
        break;

    case WD_KQ_DO_CHU_DONG:
        xSystem.GUI_Window[WD_KQ_DO_CHU_DONG] = CreateWD_KetQuaDoChuDong();
        DebugPrint("\rCreateWD_KetQuaDoChuDong.\r\n");
        break;

    case WD_KQ_REPORT:
        xSystem.GUI_Window[WD_KQ_REPORT] = CreateWD_Report();
        DebugPrint("\rCreateWD_Report.\r\n");
        break;

    case WD_KEY_NUM:
        xSystem.GUI_Window[WD_KEY_NUM] = CreateWD_KeyNum();
        DebugPrint("\rCreateWD_KeyNum.\r\n");
        break;

    case WD_KEYPAD:
        xSystem.GUI_Window[WD_KEYPAD] = CreateWD_Key();
        DebugPrint("\rCreateWD_Key.\r\n");
        break;

    case WD_MTAB_SETUP:
        xSystem.GUI_Window[WD_MTAB_SETUP] = CreateWD_Setup();
        DebugPrint("\rCreateWD_Setup.\r\n");
        break;

    case WD_RECORDLIST:
        //xSystem.GUI_Window[WD_RECORDLIST]=CreateWD_Report();
        xSystem.GUI_Window[WD_RECORDLIST] = CreateWD_RecordList();
        DebugPrint("\rCreateWD_RecordList.\r\n");
        break;

    case WD_NOTIFI:
        xSystem.GUI_Window[WD_NOTIFI] = CreateWD_Notification();
        DebugPrint("\rCreateWD_Notification.\r\n");
        break;

    case WD_NOTIFISIMPLE:
        xSystem.GUI_Window[WD_NOTIFISIMPLE] = CreateWD_NotiSimple();
        DebugPrint("\rCreateWD_NotiSimple.\r\n");
        break;

    case WD_DOADC:
        xSystem.GUI_Window[WD_DOADC] = CreateWD_DoADC();
        DebugPrint("\rCreateWD_DoADC.\r\n");
        break;

    case WD_SETTING:
        xSystem.GUI_Window[WD_SETTING] = CreateWD_SETTING();
        DebugPrint("\rCreateWD_SETTING.\r\n");
        break;

    case WD_NOTIOK:
        xSystem.GUI_Window[WD_NOTIOK] = CreateWD_NOTIOK();
        DebugPrint("\rCreateWD_NOTIOK.\r\n");
        break;
    case WD_CaiDatChung:
        xSystem.GUI_Window[WD_CaiDatChung] = CreateWD_CaiDatChung();
        DebugPrint("\rCreateWD_CaiDatChung.\r\n");
        break;
    case WD_HIEUCHINH:
        xSystem.GUI_Window[WD_HIEUCHINH] = CreateWD_HIEUCHINH();
        DebugPrint("\rCreateWD_HIEUCHINH.\r\n");
        break;
    }

    GUI_Exec();
}

void UserGUI_EndWindow(WM_HWIN WindowHandle)
{
    GUI_EndDialog(WindowHandle, 0);
    GUI_Exec();
}

void GUI_Manager_Change(void)
{
    /*Dong Window hien tai*/
    UserGUI_EndWindow(xSystem.GUI_Window[xSystem.GUI_Manager.CurrentWindowID]);
    xSystem.GUI_Manager.CurrentWindowID = xSystem.GUI_Manager.NextWindowID;
    /*Mo Window moi*/
    UserGUI_StartWindow(xSystem.GUI_Manager.NextWindowID);
    /*Thuc hien function neu co*/
    if (xSystem.GUI_Manager.Function != NULL)
        xSystem.GUI_Manager.Function();
}

void GUI_WD_Change(WindowMenu_e Window)
{
    /*Dong Window hien tai*/
    UserGUI_EndWindow(xSystem.GUI_Window[xSystem.GUI_Manager.CurrentWindowID]);
    xSystem.GUI_Manager.CurrentWindowID = Window;
    /*Mo Window moi*/
    UserGUI_StartWindow(Window);
    /*Thuc hien function neu co*/
    if (xSystem.GUI_Manager.Function != NULL)
        xSystem.GUI_Manager.Function();
}