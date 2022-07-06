////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_EEPROM.H
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
////////////////////////////////////////////////////////////////////////////////
#ifndef _BSP_EEPROM_H_
#define _BSP_EEPROM_H_

#define PAGESIZE    16


#define SLAVE_ADDR  0xA0


typedef struct {
	u8 busy;
	u8 ack;
	u8 error;
	u8 opt;
	u8 sub;
	u8 cnt;
	u8 *ptr;
	u8 rev;
	u8 sadd;
}I2C_def;

////////////////////////////////////////////////////////////////////////////////
#ifdef _BSP_EEPROM_C_
#define GLOBAL

enum{WR, RD};

u8 i2cTx[] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};
u8 i2cRx[] = {0,0,0,0,0,0,0,0};


#else
#define GLOBAL extern

extern u8 i2cTx[8];
extern u8 i2cRx[8];

#endif

GLOBAL I2C_def i2c;


#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void BSP_EEPROM_Configure(void);

void i2cInitMaster(I2C_TypeDef *I2Cx, u8 sla, u32 speed);
void i2cSendPacket(u8 sub, u8* ptr, u16 cnt);
void i2cRcvPacket(u8 sub, u8* ptr, u16 cnt);

void initGPIO_I2C(I2C_TypeDef *I2Cx);
void NVIC_I2C(I2C_TypeDef *I2Cx);
void initI2C(void);

void EEPROM_Write(u8 sub, u8* ptr, u16 len);
void EEPROM_Read(u8 sub, u8* ptr, u16 len);
void EEPROM_WaitEEready(void);
void EEPROM_WriteByte(u8 dat);
u8   EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt);
u8   EEPROM_SendPacket(u8 sub, u8* ptr, u16 cnt);
void EEPROM_ReadBuff(void);
void EEPROM_ReadPacket(u8 sub, u8* ptr, u16 cnt);
void I2C_WR_EepromInit(void);
void I2C_WR_EepromTest(void);
u8 checkE2prom(void);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////


