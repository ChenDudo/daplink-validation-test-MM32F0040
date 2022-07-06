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
#define _BSP_BEEP_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_beep.h"
#include "bsp.h"

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize BEEP.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_BEEP_Configure(u32 freq)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
  GPIO_InitTypeDef 				GPIO_InitStruct;
	
        /* Enable GPIO */
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_6);

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	u16 arr = 2000000 / freq - 1;
         
        /* Enable TIM1 Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1, ENABLE);
	TIM_TimeBaseStructure.TIM_Period        = arr;
	TIM_TimeBaseStructure.TIM_Prescaler     = 23;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(BEEP_TIMER, &TIM_TimeBaseStructure);

        /* Enable CHANNE0L */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse           = arr >> 1;
	TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High;
	TIM_OC2Init(BEEP_TIMER, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(BEEP_TIMER, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);      
        /* Enable GPIO */

	BEEP_off();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn on the buzzer.
/// @param  Specifies the Capture Compare register new value.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BEEP_on(u32 val)
{
    TIM_SetAutoreload(BEEP_TIMER, val << 1);
        
		TIM_SetCompare2(BEEP_TIMER, val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn off the buzzer.
/// @param  None.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BEEP_off(void)
{

    TIM_SetCompare2(BEEP_TIMER, 0);
}

////////////////////////////////////////////////////////////////////////////////
void BEEP_freq(u32 *val)
{
    if (*val < 200) {
        *val = 200;
    }
    
    TIM_SetAutoreload(BEEP_TIMER, *val);
}

/// @}

/// @}

/// @}

