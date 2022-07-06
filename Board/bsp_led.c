////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_LED.C
/// @author   Y Xu
/// @version  v1.0.0
/// @date     2022-03-17
/// @brief    THIS FILE PROVIDES ALL THE LED BSP LAYER FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _BSP_LED_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_led.h"
#include "bsp.h"
/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize LED.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_LED_Configure(void)
{
    LED1_CONFIG;
    LED2_CONFIG;
    LED3_CONFIG;
    LED4_CONFIG;
}


/* -----------------------------------------------------------------------------
------------------   F u n c t i o n     I m p l e m e n t  --------------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  LED on/off function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void LD1_on(void)	{ GPIO_ResetBits(LED1_PORT, LED1_BIT); ledStatus[0] = true;  }
void LD1_off(void)	{ GPIO_SetBits  (LED1_PORT, LED1_BIT); ledStatus[0] = false; }
void LD2_on(void)	{ GPIO_ResetBits(LED2_PORT, LED2_BIT); ledStatus[1] = true;  }
void LD2_off(void)	{ GPIO_SetBits  (LED2_PORT, LED2_BIT); ledStatus[1] = false; }

void LD3_on(void)	{ GPIO_ResetBits(LED3_PORT, LED3_BIT); ledStatus[2] = true;  }
void LD3_off(void)	{ GPIO_SetBits  (LED3_PORT, LED3_BIT); ledStatus[2] = false; }

void LD4_on(void)	{ GPIO_ResetBits(LED4_PORT, LED4_BIT); ledStatus[3] = true;  }
void LD4_off(void)	{ GPIO_SetBits  (LED4_PORT, LED4_BIT); ledStatus[3] = false; }

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn off all LEDs.
/// @param  None.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CloseLED(void)	{ 
	LD1_off();
	LD2_off();
	LD3_off();
	LD4_off();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Light up all LEDs.
/// @param  None.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void OpenLED(void)	{ 
	LD1_on();
	LD2_on();
	LD3_on();
	LD4_on();
}

/// @}

/// @}

/// @}

