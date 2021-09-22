#include "AlcoholMeter.h"
#include "UserGUIService.h"
#include "GUI_Text_Define.h"
#include "Parameters.h"
#include "TPRT.h"
#include "TPRT.h"
#include "Utilities.h"

extern System_t xSystem;
extern void RTC_User_Init(void);
extern void AlM_SetTask(ALM_Task task);
uint16_t check = 0;

void BTP_DoChuDong(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    memset(xSystem.Record.Buff, 0, RECORD_SIZE - 4);
    AlM_SetTask(ALM_DOCHUDONG);
}

void BTP_DoThuDong(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    AlM_SetTask(ALM_DOTHUDONG);
}
void BTP_DoADC(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    //memset(xSystem.Record.Buff,0,RECORD_SIZE-4);
    AlM_SetTask(ALM_DOADC);
}

void BTP_Do(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    //memset(xSystem.Record.Buff,0,RECORD_SIZE-4);
    //AlM_SetTask(ALM_DO);
    AlM_SetTask(ALM_DOADC);
}
void BTP_Xa(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    //memset(xSystem.Record.Buff,0,RECORD_SIZE-4);
    AlM_SetTask(ALM_XA);
}

uint8_t BTP_CheckSetupSercurity(void)
{
    uint8_t i;
    DebugPrint("\r%s-%s\r\n", xSystem.Keypad.Str, xSystem.Parameters.SetupKey);
    for (i = 0; i < DEVICE_PASSKEY_LEN; i++)
        if (xSystem.Keypad.Str[i] != xSystem.Parameters.SetupKey[i])
        {
            return 0;
        }
    return 1;
}

void BTP_CaiDat(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    GUI_WD_Change(WD_SETTING);
}
void setReportFrom(int from);
void BTP_Next(void)
{
    DebugPrint("\rBTP_Next: %d\r\n", xSystem.GUI_Manager.CurrentWindowID);
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    switch (xSystem.GUI_Manager.CurrentWindowID)
    {
    case WD_DO_CHU_DONG:
        AlM_SetTask(ALM_IDLE);
        xSystem.Record.Name.Epoch = xSystem.Rtc->GetCounter();
        xSystem.Record.Name.Time = xSystem.Rtc->GetDateTime();
        xSystem.Record.Name.KinhDo.value = xSystem.GPS->GetPosition().KinhDo.value;
        xSystem.Record.Name.ViDo.value = xSystem.GPS->GetPosition().ViDo.value;
        setReportFrom(0);
        GUI_WD_Change(WD_KQ_REPORT);
        break;

    case WD_KQ_DO_CHU_DONG:
        setReportFrom(0);
        GUI_WD_Change(WD_KQ_REPORT);
        break;

        // case WD_CaiDatChung:
        //  // xSystem.TimeOut.Clk1s_UpdateCFGToFlash = 1;
        //  if (ValidDateTime(xSystem.Time)){
        //      xSystem.Rtc->SetDateTime(xSystem.Time, 0);
        //      DebugPrint("\rSet time sucessfull!!!\r\n");
        //  }
        //  DebugPrint("\rDang save config\r\n");
        //  strcpy((char *)xSystem.Keypad.Str, Txt_DaLuuThongTin);
        //  strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
        //  xSystem.Keypad.Status = 1;
        //  xSystem.Keypad.WD_Req = WD_CaiDatChung;
        //  GUI_WD_Change(WD_NOTIFI);
        //  break;

    default:
        GUI_WD_Change(WD_MAIN_MENU);
        break;
    }
}

void BTP_Back(void)
{
    DebugPrint("\rBTP_Back\r\n");
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    switch (xSystem.Keypad.WD_Req)
    {
    case WD_RECORDLIST:
        GUI_WD_Change(WD_RECORDLIST);
        break;
    case WD_KQDCD:
        GUI_WD_Change(WD_KQ_DO_CHU_DONG);
        break;
    case WD_CDC:
    case WD_XOADULIEU:
    case WD_TRUYENDULIEU:
    case WD_HIEUCHINH:
        GUI_WD_Change(WD_SETTING);
        break;
    case WD_HIEUCHINH_CALIB:
        GUI_WD_Change(WD_HIEUCHINH);
        break;
    case WD_CaiDatChung:
        GUI_WD_Change(WD_CaiDatChung);
        break;
    case WD_HC:
        GUI_WD_Change(WD_SETTING);
        break;

    default:
        GUI_WD_Change(WD_MAIN_MENU);
        break;
    }
}

void BTP_Home(void)
{
    DebugPrint("\rBTP_Home\r\n");
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    xSystem.GLStatus.Al_Step = NO_MEASURE;
    AlM_SetTask(ALM_IDLE);
    GUI_WD_Change(WD_MAIN_MENU);
}

void BTP_Save(void)
{
    //DebugPrint("\rSave:%d-%d-%d\r\n", xSystem.Parameters.BlowTime, xSystem.Parameters.BlowPressure, xSystem.Parameters.TimeAutoTurnOff);
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    GUI_WD_Change(WD_MAIN_MENU);
}

void BTP_ListRecord(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    //DebugPrint("IndexStore:  %d \n",xSystem.Record.Name.IndexStore);

    GUI_WD_Change(WD_RECORDLIST);
}

uint8_t GetVNFont(unsigned short Character);
uint8_t GetVNFont(unsigned short Character)
{
    switch (Character)
    {
    case 0x00C0:
        return 127; // AF
    case 0x00C1:
        return 128; // AS
    case 0x1EA2:
        return 129; // AR
    case 0x00C3:
        return 130; // AX
    case 0x1EA0:
        return 131; // AJ
    case 0x0102:
        return 132; // AW
    case 0x1EB0:
        return 133; // AWF
    case 0x1EAE:
        return 134; // AWS
    case 0x1EB2:
        return 135; // AWR
    case 0x1EB4:
        return 136; // AWX
    case 0x1EB6:
        return 137; // AWJ
    case 0x00C2:
        return 138; // AA
    case 0x1EA6:
        return 139; // AAF
    case 0x1EA4:
        return 140; // AAS
    case 0x1EA8:
        return 141; // AAR
    case 0x1EAA:
        return 142; // AAX
    case 0x1EAC:
        return 143; // AAJ
    case 0x0110:
        return 144; // DD
    case 0x00C8:
        return 145; // EF
    case 0x00C9:
        return 146; // ES
    case 0x1EBA:
        return 147; // ER
    case 0x1EBC:
        return 148; // EX
    case 0x1EB8:
        return 149; // EJ
    case 0x00CA:
        return 150; // EE
    case 0x1EC0:
        return 151; // EEF
    case 0x1EBE:
        return 152; // EES
    case 0x1EC2:
        return 153; // EER
    case 0x1EC4:
        return 154; // EEX
    case 0x1EC6:
        return 155; // EEJ
    case 0x00CC:
        return 156; // IF
    case 0x00CD:
        return 157; // IS
    case 0x1EC8:
        return 158; // IR
    case 0x0128:
        return 159; // IX
    case 0x1ECA:
        return 160; // IJ
    case 0x00D2:
        return 161; // OF
    case 0x00D3:
        return 162; // OS
    case 0x1ECE:
        return 163; // OR
    case 0x00D5:
        return 164; // OX
    case 0x1ECC:
        return 165; // OJ
    case 0x00D4:
        return 166; // OO
    case 0x1ED2:
        return 167; // OOF
    case 0x1ED0:
        return 168; // OOS
    case 0x1ED4:
        return 169; // OOR
    case 0x1ED6:
        return 170; // OOX
    case 0x1ED8:
        return 171; // OOJ
    case 0x01A0:
        return 172; // OW
    case 0x1EDC:
        return 173; // OWF
    case 0x1EDA:
        return 174; // OWS
    case 0x1EDE:
        return 175; // OWR
    case 0x1EE0:
        return 176; // OWX
    case 0x1EE2:
        return 177; // OWJ
    case 0x00D9:
        return 178; // UF
    case 0x00DA:
        return 179; // US
    case 0x1EE6:
        return 180; // UR
    case 0x0168:
        return 181; // UX
    case 0x1EE4:
        return 182; // UJ
    case 0x01AF:
        return 183; // UW
    case 0x1EEA:
        return 184; // UWF
    case 0x1EE8:
        return 185; // UWS
    case 0x1EEC:
        return 186; // UWR
    case 0x1EEE:
        return 187; // UWX
    case 0x1EF0:
        return 188; // UWJ
    case 0x1EF2:
        return 189; // YF
    case 0x00DD:
        return 190; // YS
    case 0x1EF6:
        return 191; // YR
    case 0x1EF8:
        return 192; // YX
    case 0x1EF4:
        return 193; // YJ

        // chu thuong

    case 0x00E0:
        return 194; // af
    case 0x00E1:
        return 195; // as
    case 0x1EA3:
        return 196; // ar
    case 0x00E3:
        return 197; // ax
    case 0x1EA1:
        return 198; // aj
    case 0x0103:
        return 199; // aw
    case 0x1EB1:
        return 200; // awf
    case 0x1EAF:
        return 201; // aws
    case 0x1EB3:
        return 202; // awr
    case 0x1EB5:
        return 203; // awx
    case 0x1EB7:
        return 204; // awj
    case 0x00E2:
        return 205; // aa
    case 0x1EA7:
        return 206; // aaf
    case 0x1EA5:
        return 207; // aas
    case 0x1EA9:
        return 208; // aar
    case 0x1EAB:
        return 209; // aax
    case 0x1EAD:
        return 210; // aaj
    case 0x0111:
        return 211; // dd
    case 0x00E8:
        return 212; // ef
    case 0x00E9:
        return 213; // es
    case 0x1EBB:
        return 214; // er
    case 0x1EBD:
        return 215; // ex
    case 0x1EB9:
        return 216; // ej
    case 0x00EA:
        return 217; // ee
    case 0x1EC1:
        return 218; // eef
    case 0x1EBF:
        return 219; // ees
    case 0x1EC3:
        return 220; // eer
    case 0x1EC5:
        return 221; // eex
    case 0x1EC7:
        return 222; // eej
    case 0x00EC:
        return 223; // if
    case 0x00ED:
        return 224; // is
    case 0x1EC9:
        return 225; // ir
    case 0x0129:
        return 226; // ix
    case 0x1ECB:
        return 227; // ij
    case 0x00F2:
        return 228; // of
    case 0x00F3:
        return 229; // os
    case 0x1ECF:
        return 230; // or
    case 0x00F5a:
        return 231; // ox
    case 0x1ECD:
        return 232; // oj
    case 0x00F4:
        return 233; // oo
    case 0x1ED3:
        return 234; // oof
    case 0x1ED1:
        return 235; // oos
    case 0x1ED5:
        return 236; // oor
    case 0x1ED7:
        return 237; // oox
    case 0x1ED9:
        return 238; // ooj
    case 0x01A1:
        return 239; // ow
    case 0x1EDD:
        return 240; // owf
    case 0x1EDB:
        return 241; // ows
    case 0x1EDF:
        return 242; // owr
    case 0x1EE1:
        return 243; // owx
    case 0x1EE3:
        return 244; // owj
    case 0x00F9:
        return 245; // uf
    case 0x00FA:
        return 246; // us
    case 0x1EE7:
        return 247; // ur
    case 0x0169:
        return 248; // ux
    case 0x1EE5:
        return 249; // uj
    case 0x01B0:
        return 250; // uw
    case 0x1EEB:
        return 251; // uwf
    case 0x1EE9:
        return 252; // uws
    case 0x1EED:
        return 253; // uwr
    case 0x1EEF:
        return 254; // uwx
    case 0x1EF1:
        return 255; // uwj

    case 0x1EF3:
        return 36; // y huyen
    case 0x00FD:
        return 91; // y sac
    case 0x1EF7:
        return 93; // y hoi
    case 0x1EF9:
        return 94; // y nga
    case 0x1EF5:
        return 260; // y nang

    default:
        return 32;
    }
}

uint8_t UnicodeToAscii(unsigned char *Unicode, char *Ascii)
{
    uint8_t i = 0, j = 0, Index;
    uint16_t CharCode, CharSize;

    Index = 0;
    for (i = 0; i < 30; i++)
    {
        CharSize = GUI_UC_GetCharSize((const char *)&Unicode[Index]);
        CharCode = GUI_UC_GetCharCode((const char *)&Unicode[Index]);
        Ascii[i] = CharCode < 0x00C0 ? CharCode : GetVNFont(CharCode);
        //DebugPrint("\r[%d-%d]:%x-%x-%c ",i,Index,CharCode,Ascii[i],Ascii[i]);
        //Ascii[i] = GetVNFont(CharCode)+1;
        Index += CharSize;
        if (CharCode == 0)
            break;
    }
    return i;
}

void AlM_PrintResult(void)
{
    uint8_t Buff[32], Index = 0;
    char Tmp[32];

    TPRT_AlM_ClearData();

    DebugPrint("\r%s", "=================================\r\n");

    TPRT_AlM_AddLine((uint8_t *)"-----------------------------------");
    UnicodeToAscii((unsigned char *)Txt_BienBan, (char *)Buff);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine((uint8_t *)Buff);
    TPRT_AlM_AddLine((uint8_t *)"                   ");

    sprintf((char *)Buff, "Serial: %s", xSystem.Parameters.DeviceID);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    UnicodeToAscii((unsigned char *)Txt_ThoiGian, (char *)Tmp);
    Index = sprintf((char *)Buff, "%s: ", Tmp);
    DateTimeToString(&xSystem.Record.Name.Time, (char *)Tmp);
    Index += sprintf((char *)&Buff[Index], "%s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine((uint8_t *)Buff);

    UnicodeToAscii((unsigned char *)Txt_CheDoKiemTra, (char *)Tmp);
    Index = sprintf((char *)Buff, "%s ", Tmp);
    UnicodeToAscii((unsigned char *)Txt_DoChuDong, (char *)Tmp);
    Index += sprintf((char *)&Buff[Index], "%s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine((uint8_t *)Buff);

    TPRT_AlM_AddLine((uint8_t *)"-----------------------------------");
    Index = UnicodeToAscii((unsigned char *)Txt_KetQua, (char *)Buff);
    sprintf((char *)&Buff[Index], "    %.3f mg/L", xSystem.Record.Name.KetQua.value);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);
    TPRT_AlM_AddLine((uint8_t *)"-----------------------------------");

    Index = UnicodeToAscii((unsigned char *)Txt_NguoiVanHanh, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.NguoiThucHien, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_DonViCongTac, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.DonViCongTac, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_ChuKyNguoiVanHanh, (char *)Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_TenLaiXe, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.TenLaiXe, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_BienSoXe, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.BienSo, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_DiaDiem, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.DiaDiemKiemTra, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_ChuKyLaiXe, (char *)Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_NguoiLamChung, (char *)Buff);
    UnicodeToAscii((unsigned char *)xSystem.Record.Name.NguoiLamChung, (char *)Tmp);
    sprintf((char *)&Buff[Index], " %s", Tmp);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);

    Index = UnicodeToAscii((unsigned char *)Txt_ToaDo, (char *)Buff);
    sprintf((char *)&Buff[Index], " %.4f-%.4f", xSystem.Record.Name.ViDo.value, xSystem.Record.Name.KinhDo.value);
    DebugPrint("\r%s", Buff);
    TPRT_AlM_AddLine(Buff);
    DebugPrint("\r%s", "=================================\r\n");
    TPRT_AlM_AddLine((uint8_t *)"                   ");
    TPRT_AlM_AddLine((uint8_t *)"=================================");
    //snprintf((char*)Tmp,sizeof(Tmp),"%s %s", __DATE__, __TIME__);
    //TPRT_AlM_AddLine((uint8_t*)Tmp);
    TPRT_AlM_AddLine((uint8_t *)"                   ");
    TPRT_AlM_AddLine((uint8_t *)"                   ");

    TPRT_AlM_StartPrint();
}

extern void Record_Store(void);
extern uint8_t TPRT_IsPrintg(void);

void BTP_Print(void)
{
    //if (RTC_ReadBackupRegister(RTC_BKP_DR0) == 0x32F2)
    //{
    // Thong bao
    xSystem.CTP.CTP_Sens = 65535;
    //Record_Store();

    DebugPrint("\rBTP_Print");

    strcpy((char *)xSystem.Keypad.Str, Txt_DangIn);
    xSystem.Keypad.Status = 1;
    GUI_WD_Change(WD_NOTIFISIMPLE);

    AlM_PrintResult();

    while (TPRT_IsPrintg())
    {
        xSystem.Utility->WdtRst();
    }
    GUI_WD_Change(xSystem.Keypad.WD_Req);
    xSystem.CTP.CTP_Sens = 5;
    //}
    //else
    //{
    //}
}
void BT_XOADL(void)
{
    xSystem.CTP.CTP_Sens = 1;
    strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
    strcpy((char *)xSystem.Keypad.Str, Txt_XacNhan);
    xSystem.Keypad.Status = 1;
    xSystem.Keypad.WD_Req = WD_XOADULIEU;
    GUI_WD_Change(WD_NOTIFI);
}
void NHAP_MK(void)
{
    xSystem.Keypad.WD_Req = WD_XOADULIEU;
    xSystem.Keypad.Type_Req = KP_REQ_PASS;
    sprintf((char *)xSystem.Keypad.Header, Txt_NhapMaBaoMat);
    xSystem.Keypad.StrLen = sprintf((char *)xSystem.Keypad.Str, "");
    xSystem.Keypad.Index = 0;
    GUI_WD_Change(WD_KEY_NUM);
}
void DeleteRecord(void)
{
    if (xSystem.Keypad.Status == KP_ENTER)
    {
        if (xSystem.Keypad.Type_Req == KP_REQ_PASS)
        {
            if (BTP_CheckSetupSercurity() == 1)
            {
                FullClear_Record();
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                strcpy((char *)xSystem.Keypad.Str, Txt_Xoaxong);
                xSystem.Keypad.Status = 0;
                xSystem.Keypad.WD_Req = WD_SETTING;
                GUI_WD_Change(WD_NOTIOK);
            }
            else
            {
                xSystem.CTP.CTP_Sens = 1;
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                strcpy((char *)xSystem.Keypad.Str, Txt_SaiMK);
                xSystem.Keypad.Status = 1;
                xSystem.Keypad.WD_Req = WD_SAI_MK;
                GUI_WD_Change(WD_NOTIFI);
            }
        }
    }
}
void BTP_TimeAuto(void)
{
    strcpy((char *)xSystem.Keypad.Str, Txt_Time);
    strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
    xSystem.Keypad.Status = 0;
    GUI_WD_Change(WD_NOTIFISIMPLE);
    // code update GPS Time , define 1 bien x�c nhan cap nhat thoi gian thanh cong
}
void Set_TimeAuto(void)
{
    if (xSystem.GLStatus.DoneSync == 1) // bien cap nhat thoi gian thanh cong
    {
        strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
        strcpy((char *)xSystem.Keypad.Str, Txt_CapNhatThanhCong);
        xSystem.Keypad.Status = 0;
        xSystem.Keypad.WD_Req = WD_CaiDatChung;
        GUI_WD_Change(WD_NOTIOK);
    }
    else
    {
        strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
        strcpy((char *)xSystem.Keypad.Str, Txt_CapNhatKhongThanhCong);
        xSystem.Keypad.Status = 0;
        xSystem.Keypad.WD_Req = WD_CaiDatChung;
        GUI_WD_Change(WD_NOTIOK);
    }
}

void BT_SaveConfig(void)
{
    uint8_t result = 1;

    if (ValidDateTime(xSystem.TmpTime))
    {
        if (xSystem.TmpTime.Year != xSystem.Time.Year || xSystem.TmpTime.Month != xSystem.Time.Month || xSystem.TmpTime.Day != xSystem.Time.Day || xSystem.TmpTime.Hour != xSystem.Time.Hour || xSystem.TmpTime.Minute != xSystem.Time.Minute || strcmp((uint8_t *)xSystem.TmpParameters.NguoiThucHien, (uint8_t *)xSystem.Parameters.NguoiThucHien) != 0 || strcmp((uint8_t *)xSystem.TmpParameters.DonVi, (uint8_t *)xSystem.Parameters.DonVi) != 0 || xSystem.TmpParameters.TimeAutoTurnOff != xSystem.Parameters.TimeAutoTurnOff || xSystem.TmpParameters.TimeoutKeypad != xSystem.Parameters.TimeoutKeypad)
        {
            xSystem.Parameters = xSystem.TmpParameters;
            xSystem.TimeOut.Clk1s_UpdateCFGToFlash = 1;
            xSystem.Rtc->SetDateTime(xSystem.TmpTime, 0);

            DebugPrint("\rSet time sucessfull!!!\r\n");

            strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
            strcpy((char *)xSystem.Keypad.Str, Txt_SaveConfig);
            xSystem.Keypad.Status = 0;
            xSystem.Keypad.WD_Req = WD_CaiDatChung;
            GUI_WD_Change(WD_NOTIOK);
            DebugPrint("\rDang save config\r\n");
        }
        else
        {
            strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
            strcpy((char *)xSystem.Keypad.Str, Txt_NotSaveConfig);
            xSystem.Keypad.Status = 0;
            xSystem.Keypad.WD_Req = WD_CaiDatChung;
            GUI_WD_Change(WD_NOTIOK);
        }
    }
}

void BTP_SaveCalib(void)
{
    // code lưu calib
    SaveConfigToFlash();
    strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
    strcpy((char *)xSystem.Keypad.Str, Txt_SaveCalib);
    xSystem.Keypad.Status = 0;
    xSystem.Keypad.WD_Req = WD_HIEUCHINH;
    GUI_WD_Change(WD_NOTIOK);
}