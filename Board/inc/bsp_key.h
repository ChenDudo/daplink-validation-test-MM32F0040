////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_KEY.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE KEY
///           BSP LAYER.
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
#ifndef __BSP_KEY_H
#define __BSP_KEY_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_BSP
/// @brief KEY BSP modules
/// @{

typedef enum {
    KEY_RELEASE,
    KEY_DOWN,
    KEY_DOWNED,
    KEY_PRESSING,
    KEY_PRESSED
} KEY_STATUS;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_Exported_Types
/// @{

typedef void (*KEY_DOWN_FUNC)(void);
typedef void (*KEY_PRESSING_FUNC)(void);

/* Key Port Macro Definition */
#define KEY1_PORT GPIOA
#define KEY2_PORT GPIOB
#define KEY3_PORT GPIOB
#define KEY4_PORT GPIOD

/* Key Pin Macro Definition */
#define KEY1_BIT GPIO_Pin_0
#define KEY2_BIT GPIO_Pin_7
#define KEY3_BIT GPIO_Pin_14
#define KEY4_BIT GPIO_Pin_6

/* Read key level macro definition */
#define KEY1_VAL (GPIO_ReadInputDataBit(KEY1_PORT, KEY1_BIT))
#define KEY2_VAL (!GPIO_ReadInputDataBit(KEY2_PORT, KEY2_BIT))
#define KEY3_VAL (!GPIO_ReadInputDataBit(KEY3_PORT, KEY3_BIT))
#define KEY4_VAL (!GPIO_ReadInputDataBit(KEY4_PORT, KEY4_BIT))

/* Key initialization macro definition */
#define KEY1_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOA;    GPIO_Mode_IPD_Init(KEY1_PORT, KEY1_BIT, NO_REMAP, GPIO_AF_0);}
#define KEY2_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOB;    GPIO_Mode_IPU_Init(KEY2_PORT, KEY2_BIT, NO_REMAP, GPIO_AF_0);}
#define KEY3_CONFIG {                                    GPIO_Mode_IPU_Init(KEY3_PORT, KEY3_BIT, NO_REMAP, GPIO_AF_0);}
#define KEY4_CONFIG {RCC->AHBENR |= RCC_AHBENR_GPIOD;    GPIO_Mode_IPU_Init(KEY4_PORT, KEY4_BIT, NO_REMAP, GPIO_AF_0);}


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_Exported_Variables
/// @{
#ifdef _BSP_KEY_C_
#define GLOBAL

GLOBAL u16               keyDelayCnt[4]      = {0, 0, 0, 0};
GLOBAL bool              keyDelayFlag[4]     = {false, false, false, false};
GLOBAL KEY_STATUS        KeyCurrentStatus[4] = {KEY_RELEASE, KEY_RELEASE, KEY_RELEASE, KEY_RELEASE};
GLOBAL KEY_DOWN_FUNC     keyDownFunc[4]      = {NULL, NULL, NULL, NULL};
GLOBAL KEY_PRESSING_FUNC keyPressingFunc[4]  = {NULL, NULL, NULL, NULL};

#else
#define GLOBAL extern
#endif

GLOBAL u16               keyDelayCnt[4];
GLOBAL bool              keyDelayFlag[4];
GLOBAL KEY_STATUS        KeyCurrentStatus[4];
GLOBAL KEY_DOWN_FUNC     keyDownFunc[4];
GLOBAL KEY_PRESSING_FUNC keyPressingFunc[4];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_Exported_Functions
/// @{

bool Key1(void);
bool Key2(void);
bool Key3(void);
bool Key4(void);

void BSP_KEY_Configure(void);

void BSP_KeyTick(void);
void BSP_KeyFuncSet(u8 key, void (*down)(void), void (*pressing)(void));
void BSP_Key(u8 key);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __BSP_KEY_H */
////////////////////////////////////////////////////////////////////////////////
