////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_LED.H
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

// Define to prevent recursive inclusion  --------------------------------------
#ifndef __BSP_LED_H
#define __BSP_LED_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_BSP
/// @brief LED BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Types
/// @{

/* LED Port Macro Definition */
#define LED1_PORT GPIOA
#define LED2_PORT GPIOA
#define LED3_PORT GPIOB
#define LED4_PORT GPIOD

/* LED Pin Macro Definition */
#define LED1_BIT GPIO_Pin_4
#define LED2_BIT GPIO_Pin_5
#define LED3_BIT GPIO_Pin_6
#define LED4_BIT GPIO_Pin_4

/* LED initialization macro definition */
#define LED1_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOA;    GPIO_SetBits(LED1_PORT, LED1_BIT); GPIO_Mode_OUT_PP_Init(LED1_PORT, LED1_BIT);}
#define LED2_CONFIG {                                    GPIO_SetBits(LED2_PORT, LED2_BIT); GPIO_Mode_OUT_PP_Init(LED2_PORT, LED2_BIT);}
#define LED3_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOB;    GPIO_SetBits(LED3_PORT, LED3_BIT); GPIO_Mode_OUT_PP_Init(LED3_PORT, LED3_BIT);}
#define LED4_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOD;    GPIO_SetBits(LED4_PORT, LED4_BIT); GPIO_Mode_OUT_PP_Init(LED4_PORT, LED4_BIT);}      

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Variables
/// @{
#ifdef _BSP_LED_C_

#define GLOBAL

GLOBAL bool ledStatus[6] = {false, false, false, false, false, false};

#else
#define GLOBAL extern
#endif

GLOBAL bool ledStatus[6];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Functions
/// @{

void LD1_on(void);
void LD2_on(void);
void LD3_on(void);
void LD4_on(void);
void OpenLED(void);

void LD1_off(void);
void LD2_off(void);
void LD3_off(void);
void LD4_off(void);
void CloseLED(void);


void BSP_LED_Configure(void);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_LED_H */
////////////////////////////////////////////////////////////////////////////////
