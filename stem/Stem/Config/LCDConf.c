/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    LCDConf_stm3210c_eval.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016
  * @brief   Driver for STM3210C-EVAL board LCD
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "ILI9488.h"
#include "ResistiveTouch.h"

extern System_t xSystem;

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
extern const GUI_DEVICE_API GUIDRV_Template_API;
//
// Physical display size
//
#define XSIZE_PHYS  320
#define YSIZE_PHYS  480


#define COLOR_CONVERSION		GUICC_565														//Color Conversion is RGB565
#define DISPLAY_DRIVER			GUIDRV_FLEXCOLOR												//Display Driver
#define DISPLAY_ORIENTATION 	(GUI_MIRROR_X)	
/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif


/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteCMD(U16 Data) {
  ILI9488_Write_Command(Data & 0xFF);
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
	ILI9488_Write_GRAM(Data);
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  
 while (NumItems--) 
	{
		ILI9488_Write_GRAM(*pData++);
	}
  
}



/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  
 while (NumItems--) 
	{
	 // ... TBD by user
	}
}


int GUI_TOUCH_X_MeasureX(void){
	return 0;//RTP_Get_X();
}

int GUI_TOUCH_X_MeasureY(void){
	return 0;//RTP_Get_X();
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //

  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
	//
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  Config.Orientation = DISPLAY_ORIENTATION;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
	PortAPI.pfWrite16_A0  = LcdWriteCMD;
	PortAPI.pfWrite16_A1  = LcdWriteData;
	PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
	PortAPI.pfReadM16_A1 = LcdReadDataMultiple;

  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
  
  
  /*Config RTP*/
//  uint16_t TouchOrientation = GUI_MIRROR_X; //(GUI_MIRROR_X * LCD_GetMirrorX()) | (GUI_MIRROR_Y * LCD_GetMirrorY()) | (GUI_SWAP_XY * LCD_GetSwapXY());
//  GUI_TOUCH_SetOrientation(TouchOrientation); 
//  
//  /*Default Calib*/
//  GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS, 0, 3584 );
//  GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS, 0, 3984 );
  
  
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: 
	/*Init LCD func*/

    return 0;
  
  case LCD_X_SETVRAMADDR:
	  
	return 0;
  case LCD_X_SETORG:
	  
	return 0;
 
  case LCD_X_SETLUTENTRY:
	  return 0;
  
  case LCD_X_ON:
	  
	return 0;
  case LCD_X_OFF:
	  
	return 0; /* Switching the display off */
case LCD_X_SETSIZE:
	  
	return 0; /* Setting the layer size */
case LCD_X_SETPOS:
	  
	return 0; /* Setting the layer position */
case LCD_X_SETVIS:
	  
	return 0; /* Setting the visibility of a layer */
case LCD_X_SETALPHA:
	  
	return 0; /* Setting the alpha value of the layer */
case LCD_X_SETALPHAMODE:
	  
	return 0; /* Setting the alpha blending mode */
case LCD_X_SETCHROMAMODE:
	  
	return 0; /* Setting the chroma blending mode */
case LCD_X_SETCHROMA:
	  
	return 0; /* Setting the chroma values */
case LCD_X_SHOWBUFFER:
	  
	return 0; /* Switching to the given buffer */
  
	  
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

