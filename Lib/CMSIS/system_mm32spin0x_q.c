////////////////////////////////////////////////////////////////////////////////
/// @file     system_MM32.c
/// @author   AE Team
/// @brief    CMSIS Cortex-M0 Device Peripheral Access Layer System Source File.
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
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion

/// @addtogroup CMSIS
/// @{

#include "mm32_device.h"


/// @}



/// @}


/// Uncomment the line corresponding to the desired System clock (SYSCLK)
/// frequency (after reset the HSI is used as SYSCLK source)
///
/// IMPORTANT NOTE:
/// ==============
/// 1. After each device reset the HSI is used as System clock source.
///
/// 2. Please make sure that the selected System clock doesn't exceed your device's
/// maximum frequency.
///
/// 3. If none of the define below is enabled, the HSI is used as System clock
/// source.
///
/// 4. The System clock configuration functions provided within this file assume that:
/// - For Low, Medium and High density Value line devices an external 8MHz
/// crystal is used to drive the System clock.
/// - For Low, Medium and High density devices an external 8MHz crystal is
/// used to drive the System clock.
/// - For Connectivity line devices an external 25MHz crystal is used to drive
/// the System clock.
/// If you are using different crystal you have to adapt those functions accordingly.


/// @}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the microcontroller system
///         Initialize the Embedded Flash Interface and update the
///         SystemCoreClock variable.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void SystemInit(void)
{
//	RCC->CR |= 0x01;
//	while(!(RCC->CR & (1 << 1)));
	
	RCC->CR |= 1 << 16;
	while(!(RCC->CR & (1 << 17)));
	
	// FLASH Latency
	FLASH->ACR &= ~0x07;
	FLASH->ACR |= 0x10;
	FLASH->ACR |= 0x2;
	
	RCC->PLLCFGR |= 1;
	RCC->PLLCFGR &= ~0x00FF0000;
	RCC->PLLCFGR |= 8 << 16;
	
	RCC->CR |= 1 << 24;
	while(!(RCC->CR & (1 << 25)));
	
	RCC->CFGR |= 2;
	//while(!(RCC->CFGR & 0x4) & (RCC->CFGR |= ~8) & 0x80);
}


/// @}



/// @}



/// @}

