////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ADC.C
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
#define _BSP_ADC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_adc.h"
#include "bsp.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
void adcTick()
{
    if(++adcCnt > 5){
        adcCnt = 0;
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)){
            adcValue[0] = ADC1->CH8DR;
            adcValue[1] = ADC1->CH9DR;
            adcValue[2] = ADC1->CH10DR;
        }
        for(u8 i = 0; i < 3; i++){
            rv[i] = (u16)((float)(rv[i] * 8 + adcValue[i] * 2) / 10);
        }
        adcFlag = true;
    }
}
void ADCxChannelEnable(ADC_TypeDef* ADCn, ADCCHANNEL_TypeDef channel)
{
    ADCn->CHSR &= ~(1 << channel);
    ADCn->CHSR |=  (1 << channel);
}
////////////////////////////////////////////////////////////////////////////////
void BSP_ADC_Configure()
{
		ADC_InitTypeDef  ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);

		RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1, ENABLE);                           //Enable ADC clock

    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;                        //ADC prescale factor
    ADC_InitStruct.ADC_Mode = ADC_Mode_Continue;                                //ADC continue scan convert mode
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                         //AD data right-justified
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

    ADC_Init(ADC1, &ADC_InitStruct);		
	
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  1, ADC_SampleTime_239_5Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  2, ADC_SampleTime_239_5Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10,  3, ADC_SampleTime_239_5Cycles);
    
    ADC_Cmd(ADC1, ENABLE);
		
		RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);														//Enable GPIOB clock
		
		GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
    GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AIN;                              //GPIO mode
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/// @}

/// @}

/// @}
