#include "SetFlashReadProtection.h"
#include "Debug.h"
void setFlashReadProtection(uint8_t func)
{
    // Unlock read
    if (func == 0)
    {
        FLASH_OB_Unlock();
        FLASH_OB_RDPConfig(OB_RDP_Level_0);
        if (FLASH_OB_Launch() != FLASH_COMPLETE)
        {
            DebugPrint("\rError enabling RDP\n");
        }
        FLASH_OB_Lock();
    }
    // Lock read
    if (func == 1)
    {
        FLASH_OB_Unlock();
        FLASH_OB_RDPConfig(OB_RDP_Level_1);
        if (FLASH_OB_Launch() != FLASH_COMPLETE)
        {
            DebugPrint("\rError enabling RDP\n");
        }
        FLASH_OB_Lock();
    }
    /* Lock read forever, cant recovery
    if (func==2)
    {
        FLASH_OB_Unlock();
        FLASH_OB_RDPConfig(OB_RDP_Level_2);
        if (FLASH_OB_Launch() != FLASH_COMPLETE)
        {
            DebugPrint("\rError enabling RDP\n");
        }
        FLASH_OB_Lock();
    }
    */
}