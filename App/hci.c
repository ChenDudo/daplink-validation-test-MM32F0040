////////////////////////////////////////////////////////////////////////////////
/// @file     UID.C
/// @author   D CHEN
/// @version  v2.0.0
/// @date     2019-03-13
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
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _HCI_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "hci.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_key.h"
#include "bsp_led.h"
#include "music.h"
#include "bsp_adc.h"


//#include "core_cm3.h"
//#include "intrinsics.h"
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
void hci_task()
{
    BSP_Key(1);
    BSP_Key(2);
    BSP_Key(3);
    BSP_Key(4);
    
    ledStatus[0] ? LD1_on() : LD1_off();
    ledStatus[1] ? LD2_on() : LD2_off();
    ledStatus[2] ? LD3_on() : LD3_off();
    ledStatus[3] ? LD4_on() : LD4_off();
    if(adcFlag){
        adcFlag = false;
        for(u8 i = 1; i < 4; i++){
            LDFreq[i] = rv[i - 1] * 500 / 4095 + 1;
        }
    }
    
}

////////////////////////////////////////////////////////////////////////////////
void ledTick()
{
    if(blinkFlag){
        for(u8 i = 1; i < 4; i++){
            if(LDCnt[i]++ >= LDFreq[i]){
                LDCnt[i] = 0;
                ledStatus[i] = !ledStatus[i];
            }
        }
    }
    else
        memset(ledStatus, 0x00, sizeof(ledStatus));
    
}

////////////////////////////////////////////////////////////////////////////////
void Key1Down()
{
    ledStatus[0] = !ledStatus[0];
    blinkFlag = !blinkFlag;
    
    if(sPlayMusic.MusicNum != 0){
        memset(&sPlayMusic, 0x00, sizeof(sPlayMusic));
        sPlayMusic.MusicNum = 0;
        sPlayMusic.PlayFlag = true;
    }
    else if(sPlayMusic.PlayFlag){
        sPlayMusic.PlayFlag = false;
        setBuzzerEn(DISABLE);
    }
    else sPlayMusic.PlayFlag = true;
}

////////////////////////////////////////////////////////////////////////////////
void Key2Down()
{
    blinkFlag = !blinkFlag;
    
    if(sPlayMusic.MusicNum != 1){
        memset(&sPlayMusic, 0x00, sizeof(sPlayMusic));
        sPlayMusic.MusicNum = 1;
        sPlayMusic.PlayFlag = true;
    }
    else if(sPlayMusic.PlayFlag){
        sPlayMusic.PlayFlag = false;
        setBuzzerEn(DISABLE);
    }
    else sPlayMusic.PlayFlag = true;
}

////////////////////////////////////////////////////////////////////////////////
void Key3Down()
{
    blinkFlag = !blinkFlag;
    ledStatus[2] = !ledStatus[2];
}

////////////////////////////////////////////////////////////////////////////////
void Key4Down()
{
    blinkFlag = !blinkFlag;
    ledStatus[3] = !ledStatus[3];
}

void resetTest()
{
    void (*fp)(void);
    fp = (void (*)(void))(* (vu32 *)(0x8000004));
    (*fp)(); 
}
////////////////////////////////////////////////////////////////////////////////
void Key1Pressing()
{
    ledStatus[0] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key2Pressing()
{
    ledStatus[1] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key3Pressing()
{
    ledStatus[2] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key4Pressing()
{
    ledStatus[3] = 1;
    resetTest();
}

/// @}

/// @}

/// @}
