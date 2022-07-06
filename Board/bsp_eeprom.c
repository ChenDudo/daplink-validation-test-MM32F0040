////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_EEPROM.C
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
#define _BSP_EEPROM_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_eeprom.h"
#include "bsp_uart.h"
#include "bsp.h"

#include "bsp_delay.h"

////////////////////////////////////////////////////////////////////////////////
void initGPIO_I2C(I2C_TypeDef *I2Cx)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
	
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}


////////////////////////////////////////////////////////////////////////////////
void NVIC_I2C(I2C_TypeDef* I2Cx)
{

}

////////////////////////////////////////////////////////////////////////////////
void i2cInitMaster(I2C_TypeDef *I2Cx, u8 sla, u32 uiI2C_speed)
{
    I2C_InitTypeDef I2C_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C1, ENABLE);

    I2C_StructInit(&I2C_InitStruct);

    I2C_InitStruct.Mode = I2C_CR_MASTER;
    I2C_InitStruct.OwnAddress = 0;
    I2C_InitStruct.Speed = I2C_CR_FAST;
    I2C_InitStruct.ClockSpeed = uiI2C_speed;
    I2C_Init(I2Cx, &I2C_InitStruct);

    I2C_Send7bitAddress(I2C1, SLAVE_ADDR, WR);
    I2C_Cmd(I2Cx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief  Wait for EEPROM getting ready.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WaitEEready(void)
{
    //eeprom operation interval delay
    u32 i = 10000;
    while(i--);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WriteByte(u8 dat)
{
    //Send data
    I2C_SendData(I2C1, dat);
    //Checks whether transmit FIFO completely empty or not
    while(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadBuff(void)
{
    u8 i, flag = 0, _cnt = 0;
    for (i = 0; i < i2c.cnt; i++) {
        while(1) {
            //Write command is sent when RX FIFO is not full
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                //Configure to read
                I2C_ReadCmd(I2C1);
                _cnt++;
                //When flag is set, receive complete
                if (_cnt == i2c.cnt)
                    flag = 1;
            }
            //Check receive FIFO not empty
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                //read data to gEeprom.ptr
                i2c.ptr[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Send bytes
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt)
{
    //Send sub address
    EEPROM_WriteByte(sub);
    while (cnt --) {
        //Send data
        EEPROM_WriteByte(*ptr);
        //Point to the next data
        ptr++;
    }
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
    i2c.ack = true;
    //I2C operation stops
    i2c.busy = false;
    //Wait for EEPROM getting ready.
    EEPROM_WaitEEready();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : The state of this transmission.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_SendPacket(u8 sub, u8* ptr, u16 cnt)
{
    u8 i;
    //i2c option flag set to write
    i2c.opt = WR;
    //number to send
    i2c.cnt = cnt;
    //sub address
    i2c.sub = sub;
    //I2C operation starts
    i2c.busy = true;
    i2c.ack = false;

    if ((sub % PAGESIZE) > 0) {
        //Need temp number of data, just right to the page address
        u8 temp = MIN((PAGESIZE - sub % PAGESIZE), i2c.cnt);
        //If WRITE successful
        if(EEPROM_WriteBuff(sub, ptr, temp)) {
            //Point to the next page
            ptr +=  temp;
            i2c.cnt -=  temp;
            sub += temp;
        }
        //gEeprom.cnt = 0 means transmition complete
        if (i2c.cnt == 0) return true;
    }
    for (i = 0; i < (i2c.cnt / PAGESIZE); i++) {
        //Full page write
        if (EEPROM_WriteBuff(sub, ptr, PAGESIZE)) {
            //Point to the next page
            ptr += PAGESIZE;
            sub += PAGESIZE;
            i2c.cnt -= PAGESIZE;
        }
        if (i2c.cnt == 0) return true;
    }
    if (i2c.cnt > 0) {
        if (EEPROM_WriteBuff(sub, ptr, i2c.cnt)) return true;
    }
    //I2C operation ends
    i2c.busy = false;
    i2c.ack = true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadPacket(u8 sub, u8* ptr, u16 cnt)
{
    //I2C operation starts
    i2c.busy = true;
    i2c.ack = false;
    i2c.sub = sub;
    i2c.ptr = ptr;
    i2c.cnt = cnt;

    //Send sub address
    EEPROM_WriteByte(i2c.sub);
    //receive bytes
    EEPROM_ReadBuff();
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);

    //I2C operation ends
    i2c.busy = false;
    i2c.ack = true;
    EEPROM_WaitEEready();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Write
/// @note   Write a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Data in the buffer)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Write(u8 sub, u8* ptr, u16 len)
{
    do {
        EEPROM_SendPacket(sub, ptr, len);
        while(i2c.busy);
    } while(!i2c.ack);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Read
/// @note   Receive a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Buffer to storage data)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Read(u8 sub, u8* ptr, u16 len)
{
    do {
        //read data
        EEPROM_ReadPacket(sub, ptr, len);
        //till I2C is not work
        while(i2c.busy);
    } while(!i2c.ack);
}

////////////////////////////////////////////////////////////////////////////////
u8 checkE2prom()
{
	EEPROM_Read(0x00, i2cRx, 8);
    printf("--- I2C-E2prom Read:");
	for(u32 i = 0; i < 8 ; i++){
	printf("0x%x ",i2cRx[i]);
	}
    printf("---\r\n");
	return ((memcmp(i2cRx, i2cTx, 8) == 0) ? true : false);
}

////////////////////////////////////////////////////////////////////////////////
void BSP_EEPROM_Configure()
{
    initGPIO_I2C(I2C1);
    i2cInitMaster(I2C1, SLAVE_ADDR, 100000);

    EEPROM_Read(0x00, i2cRx, 8);

    if (memcmp(i2cRx, i2cTx, 8) != 0) {
        EEPROM_Write(0x00, i2cTx, 8);
	}

}
