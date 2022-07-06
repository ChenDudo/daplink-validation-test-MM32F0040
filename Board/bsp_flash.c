////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ADC.C
/// @author   Y Xu
/// @version  v1.0.0
/// @date     2022-03-22
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
#define _BSP_FLASH_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_uart.h"
#include "bsp_flash.h"
#include "bsp_delay.h"
#include "bsp_uart.h"
#include "bsp.h"



////////////////////////////////////////////////////////////////////////////////
void SPIM_Init(SPI_TypeDef* SPIx,unsigned short spi_baud_div)
{
	SPI_InitTypeDef SPI_InitStructure;

	 RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI2, ENABLE);

	SPI_InitStructure.SPI_Mode		= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize	= SPI_DataSize_8b;
	SPI_InitStructure.SPI_DataWidth	= 8;
	SPI_InitStructure.SPI_CPOL		= SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA		= SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS		= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)spi_baud_div;
	SPI_InitStructure.SPI_FirstBit	= SPI_FirstBit_MSB;
	SPI_Init(SPIx, &SPI_InitStructure);

    if (SPI_InitStructure.SPI_BaudRatePrescaler <= 8) {
        exSPI_DataEdgeAdjust(SPIx, SPI_DataEdgeAdjust_FAST);
    }
		
  SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
	SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
	SPI_Cmd(SPIx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
u16 SPIMReadWriteByte(SPI_TypeDef* SPIx,u8 tx_data)
{
  u16 overTime = 10000;
	SPI_SendData(SPIx, tx_data);
	while (1){
		if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))	{
			return SPI_ReceiveData(SPIx);
		}
        if (overTime-- == 0)
            return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPIM_CSLow()
{
		//Spi cs assign to this pin,select
    SPI_CSInternalSelected(SPI2, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPIM_CSHigh()
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, DISABLE);
}

////////////////////////////////////////////////////////////////////////////////
void SPIM_ReadID(SPI_TypeDef* SPIx, u8* id)
{
	u16 i;

	SPIM_CSLow();
	SPIMReadWriteByte(SPIx, RDID);

	for(i = 0; i < 3; i++){
		*(id + i) = SPIMReadWriteByte(SPIx, i);
	}

	SPIM_CSHigh();
}


////////////////////////////////////////////////////////////////////////////////
u8 checkSPIFlashId()
{
    SPIM_ReadID(SPI2, &spiId[0]);
	if (((spiId[0] != 0x00) | (spiId[1] != 0x00) | (spiId[2] != 0x00)) &\
		((spiId[0] != 0xff) | (spiId[1] != 0xff) | (spiId[2] != 0xff))) {
			printf("--- SPI-Flash Read ID:0x%x 0x%x 0x%x ---\r\n",spiId[0],spiId[1],spiId[2]);
		return 1;
		}
		else{
		printf("--- SPI-Flash Read ID Error ---\r\n");
		return 0;
		}
	
}

////////////////////////////////////////////////////////////////////////////////
void initGPIO_SPI()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);	

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//spi2_cs  pb12
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
}

////////////////////////////////////////////////////////////////////////////////
void BSP_FLASH_Configure()
{
    initGPIO_SPI();
	SPIM_Init(SPI2, 0x6);			//div 8
}

void SWDIO_Resume()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_0);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
