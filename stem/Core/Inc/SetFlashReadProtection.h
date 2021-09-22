#pragma one
#ifndef __FLASH_PROTECTION_H
#define __FLASH_PROTECTION_H

#include <stdint.h>
#include "stm32f4xx_flash.h"

/* Read protect the flash.  NEVER EVER set this to level 2.  You can't
 * write to the chip ever again after that. */
/*
   Function: Set Flash Read Protection
   Parameters: func
            0 unlock
            1 lock
            2 lock forever
   Returns:
      void

*/
void setFlashReadProtection(uint8_t func);

#endif