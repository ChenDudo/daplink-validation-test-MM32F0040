////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN.C
/// @author   Y Xu
/// @version  v1.0.0
/// @date     2022-03-17
/// @brief    THIS FILE PROVIDES ALL THE UID EXAMPLE.
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
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2022 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _MAIN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_adc.h"
#include "bsp_eeprom.h"
#include "bsp_flash.h"
#include "bsp_uart.h"
#include "bsp_delay.h"

#include "music.h"
#include "hci.h"

#include <string.h>
#include <stdlib.h>

#define SIZEBUF 1024
uint8_t localbuf[SIZEBUF];
uint32_t val;

////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint32_t head;
	uint32_t tail;
	size_t size;
	uint8_t* buffer;
} ByteBuffer;
ByteBuffer buf;

////////////////////////////////////////////////////////////////////////////////
void deInitByteBuffer()
{
	memset(localbuf, 0x00, sizeof(localbuf));
}

////////////////////////////////////////////////////////////////////////////////
void initByteBuffer(size_t size)
{
	deInitByteBuffer();
	buf.head = 0;
	buf.tail = 0;
	buf.size = size;
	buf.buffer = localbuf;	//(uint8_t*)malloc(size);
}

////////////////////////////////////////////////////////////////////////////////
bool empty()
{
	return (buf.head == buf.tail);
}

////////////////////////////////////////////////////////////////////////////////
bool full()
{
	uint32_t new_tail = buf.tail + 1;
	if (new_tail >= buf.size) {
		new_tail = new_tail - buf.size;
	}
	return (buf.head == new_tail);
}

////////////////////////////////////////////////////////////////////////////////
void enqueue(uint8_t data)
{
	if (full()) {
		printf("Queue full\n");
	}
	buf.buffer[buf.tail] = data;
	buf.tail ++;
	if (buf.tail >= buf.size) {
		buf.tail -= buf.size;
	}
}

////////////////////////////////////////////////////////////////////////////////
uint8_t dequeue()
{
	uint8_t data;
	if (empty()) {
		printf("Queue empty\n");
	}
	data = buf.buffer[buf.head];
	buf.head ++;
	if ( buf.head >= buf.size) {
		buf.head = buf.head - buf.size;
	}
	return data;
}

////////////////////////////////////////////////////////////////////////////////
bool readable(void)
{
//	if (UART_GetFlagStatus(UART1, UART_CSR_RXAVL) != RESET){
//		readValue = UART1->RDR;
//		return 1;
//	}
//	else
//		return 0;
	return UART_GetFlagStatus(UART1, UART_CSR_RXAVL);
}

////////////////////////////////////////////////////////////////////////////////
bool writeable(void)
{
	return UART_GetFlagStatus(UART1, UART_CSR_TXEPT);
}

//////////////////////////////////////////////////////////////////////////////////
//uint32_t getc(void)
//{
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////////////
//void putc(uint32_t dat)
//{
//	
//}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializing System Peripherals.
/// @param  None.
/// @note   None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void initPeri()
{		
	DELAY_Init();
	BSP_KEY_Configure();
	BSP_KeyFuncSet(1, Key1Down, Key1Pressing);
	BSP_KeyFuncSet(2, Key2Down, Key2Pressing);
	BSP_KeyFuncSet(3, Key3Down, Key3Pressing);
	BSP_KeyFuncSet(4, Key4Down, Key4Pressing);

	BSP_LED_Configure();
	BSP_BEEP_Configure(1000);

	for(u8 i = 0; i < 2; i++){
		OpenLED();
		BEEP_on(1500);
		while(!delay(50));
		CloseLED();
		BEEP_off();
		while(!delay(200));
	}
}

///////////////////////////////////////////////////////////////////////////////
void setbaud(uint32_t baud)
{
	UART_InitTypeDef UART_InitStruct;
	
	UART_StructInit(&UART_InitStruct);
	UART_InitStruct.BaudRate = baud;
	UART_Init(UART1, &UART_InitStruct);
}

///////////////////////////////////////////////////////////////////////////////
// @brief  This function handles App SysTick Handler.
// @param  None.
// @retval None.
///////////////////////////////////////////////////////////////////////////////
void SysTick_Handler()
{
	sysTickFlag1mS = true;

}

///////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)
{
	if (UART_GetITStatus(UART1, UART_IER_RX) != RESET)
	{
		UART_ClearITPendingBit(UART1, UART_IER_RX);
		val = UART1->RDR;	//val = getc();
		enqueue(val);
	}
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	initPeri();
	
	uint32_t baud;
    uint32_t count;
    uint32_t index;
    uint8_t str[64];
    
    count = 0;
    index = 0;
	initByteBuffer(SIZEBUF);
	
	BSP_UART_Configure(115200);
	printf("{init}");
	
	while (1) {
		// Enqueue data as it arrives
		//while (readable() && !full()) {
		//if (UART_GetFlagStatus(UART1, UART_CSR_RXAVL)){
		//	val = UART1->RDR;	//val = getc();
		//	enqueue(val);
		//}
		
		// Process and send data
        //if (!empty() && writeable()) {
		if (!empty() && UART_GetFlagStatus(UART1, UART_CSR_TXEPT)){
            val = dequeue();
            /* Check for overflow. Leave space for a null terminating character */
            if (index >= sizeof(str) - 1) {
                index = 0;
            }
 
            /* Check for start of frame */
            if ('{' == val) {
                index = 0;
            }
 
            /* Check for end of frame */
            str[index] = val;
            index++;
 
            /* Check for end of frame */
            if ('}' == val && index > 0) {
                str[index] = 0;
                count = sscanf((char*)str, "{baud:%d}", &baud);
            }
 
            /* Echo back character */
            //putc(val);
			UART_SendData(UART1, val);
			while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT)){;}
 
            /* Set baud if there is a valid command */
            if (count == 1) {
                //wait(0.01f);
				while(!delay(1)){;}
				setbaud(baud);
                /* Make sure pc has enough time LCP11u35 requires ~0.1us while K20D requires ~0.01us */
                //wait(0.1f);
				while(!delay(1)){;}
                printf("{change}");
                count = 0;
            }
        }
		
	}
}
