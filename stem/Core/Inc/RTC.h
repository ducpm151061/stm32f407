#ifndef __RTC_H__
#define __RTC_H__

#define BGT_LAIXE_SIGN RTC_BKP_DR5
#define BGT_LAIXE_ID RTC_BKP_DR1
#define BGT_LAIXE_TGLXLT RTC_BKP_DR2
#define BGT_LAIXE_TGLXTN RTC_BKP_DR3
#define BGT_LAIXE_TGSTOP RTC_BKP_DR4
#define BGT_LAIXE_TGTICK1 RTC_BKP_DR6
#define BGT_LAIXE_TGTICK2 RTC_BKP_DR7
#define BGT_TGIAN_XEDUNG RTC_BKP_DR8
#if (defined __BA4TX__) || (defined __BA2PTX__)
#define DEBUG_CONFIG RTC_BKP_DR10
#else
#define MAINAPP_FORCE_JUMP RTC_BKP_DR9
#define MAINAPP_PARTITION RTC_BKP_DR11
#define GOT_NEW_FW RTC_BKP_DR12
#define RESET_REASON_255_COUNT RTC_BKP_DR13
#define WARNING_STATUS RTC_BKP_DR14

#define DEBUG_CONFIG RTC_BKP_DR15
#define HARDFAULT_RST RTC_BKP_DR16
#define HARDFAULT_REASON RTC_BKP_DR17

#endif

#endif // __RTC_H__