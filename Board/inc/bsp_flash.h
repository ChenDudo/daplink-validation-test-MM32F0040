////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_I2C.H
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE I2C
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
#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H



#define READ        	0x03
#define FAST_READ   	0x0B
#define RDID        	0x9F
#define WREN 					0x06
#define WRDI 					0x04
#define SE 						0xD8
#define BE 						0xC7
#define PP 						0x02
#define RDSR 					0x05
#define WRSR 					0x01
#define DP 						0xB9
#define RES 					0xAB


////////////////////////////////////////////////////////////////////////////////
#ifdef _BSP_FLASH_C_

#define GLOBAL


#else

#define GLOBAL extern
	
#endif

GLOBAL u8 spiId[3];
#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////
u8 checkSPIFlashId(void);
void BSP_FLASH_Configure(void);
void SWDIO_Resume(void);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////


