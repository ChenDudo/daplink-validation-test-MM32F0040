////////////////////////////////////////////////////////////////////////////////
/// @file    bsp_uart.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _BSP_UART_C_

// Files includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp_uart.h"
#include "bsp_led.h"
#include "hal_conf.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

//#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)

u8 UartTxData[16] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99};


void _sys_exit(s32 x)
{
    x = x;
}
//redefine fputcfunction
s32 fputc(s32 ch, FILE* f)
{
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure NVIC.
/// @param  None
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NVIC_UART(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void BSP_UART_Configure(u32 baudrate)
{
    //GPIO port set
    GPIO_InitTypeDef GPIO_InitStruct;
    UART_InitTypeDef UART_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART1, ENABLE);  								 					//Enable UART1 clock
	//RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART2, ENABLE);  								 					//Enable UART2 clock
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);    											 	    //Enable GPIOB clock
	//RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);    											 	    //Enable GPIOB clock

    //GPIO initialset
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_3);

	//UART1_TX   GPIOB.6
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //UART1_RX    GPIOB.7
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

//	//UART2_TX   GPIOD.4
//    GPIO_StructInit(&GPIO_InitStruct);
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOD, &GPIO_InitStruct);

//    //UART1_RX    GPIOD.6
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOD, &GPIO_InitStruct);

    //UART1 initialset
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.UART_BaudRate = baudrate;
    UART_InitStruct.UART_WordLength = UART_WordLength_8b;
    UART_InitStruct.UART_StopBits = UART_StopBits_1;													//one stopbit
    UART_InitStruct.UART_Parity = UART_Parity_No;														//none odd-even  verify bit
    UART_InitStruct.UART_HardwareFlowControl = UART_HardwareFlowControl_None;							//No hardware flow control
    UART_InitStruct.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; 											// receive and sent  mode
    UART_Init(UART1, &UART_InitStruct); 																//initial uart 1
    UART_Cmd(UART1, ENABLE);                    														//enable uart 1
	UART_ITConfig(UART1, UART_IER_RX, ENABLE);
	
//	//UART2 initialset
//    UART_StructInit(&UART_InitStruct);
//    UART_InitStruct.UART_BaudRate = baudrate;
//    UART_InitStruct.UART_WordLength = UART_WordLength_8b;
//    UART_InitStruct.UART_StopBits = UART_StopBits_1;													//one stopbit
//    UART_InitStruct.UART_Parity = UART_Parity_No;														//none odd-even  verify bit
//    UART_InitStruct.UART_HardwareFlowControl = UART_HardwareFlowControl_None;							//No hardware flow control
//    UART_InitStruct.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; 											// receive and sent  mode
//    UART_Init(UART2, &UART_InitStruct); 																//initial uart 2
//	UART_ITConfig(UART2, UART_IER_RX, ENABLE);
//    UART_Cmd(UART2, ENABLE);                    														//enable uart 2

	//NVIC initialset
	NVIC_UART();																				
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles interrupt request.
/// @param  None.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART2_IRQHandler(void)
{
	if (UART_GetITStatus(UART2, UART_IER_RX) != RESET)
	{
		//LD2_on();
		UART_ClearITPendingBit(UART2, UART_IER_RX);
		memmove(&UartRxData[0], &UartRxData[1], 15);
		UartRxData[15] = UART_ReceiveData(UART2);
	}
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  dat(A byte data).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART2_Send_Byte(u8 dat)
{
    UART_SendData(UART2, dat);
    while(!UART_GetFlagStatus(UART2, UART_FLAG_TXEPT));
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  test Uart
/// @note   
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 checkUart()
{	
	printf("--- Uart2 Transmit£º");
	for(u32 i = 0; i < 16 ; i++){
    UART2_Send_Byte(UartTxData[i]);
	printf("0x%x ",UartTxData[i]);
	}
	printf("---\r\n");
	printf("--- Uart2 Received£º");
	for(u32 i = 0; i < 16; i++) {
		printf("0x%x ",UartRxData[i]);
		if(UartTxData[i] != UartRxData[i]){
		printf("---\r\n--- Uart2 Received data Error ---\r\n");
		return 0;
    }
    }
	printf("---\r\n");
	printf("--- UART2 test success ---\r\n");
	return 1;
}
/// @}

/// @}

/// @}
