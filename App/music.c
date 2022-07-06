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
#define _MUSIC_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "mm32_device.h"
#include "hal_conf.h"

#include "bsp_beep.h"
#include "bsp.h"

#include "music.h"

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
u8 music[]={
    25,26,      1,    1,26, 1,2,
    2,         2,3,  5,   5,3,5,3,
    3,         2,3,  1,   1,26,1,2,
    2,         2,1,26
};

u8 time[] = {
    2,2,        4,    1,1,1,1,
    4,          2,2,  4,   1,1,1,1,
    4,          2,2,  4,   1,1,1,1,
    4,          2,1,1 
};

//////////////////////////////////////////////////////////////////////////////////
u8 music1[]={
    6,13,15,            13,6,11,            6,11,6,11,13,
    13,12,13,11,6,      13,15,16,16,        16,13,15,
    13,15,13,15,16,16,  16,13,15,           15,6,15,6,
    13,12,13,11,        12,12,              12,11,12,13,15,
    16,33,              13,33,              13,33,13,33,
    13,12,13,11,        12,12,              12,11,12,13,15,
    16    
};

u8 time1[] = {
    4,3,1,              2,2,4,              1,1,1,1,4,
    1,1,1,1,4,          3,1,2,2,            2,2,4,
    1,1,1,1,2,2,        2,2,4,              2,2,2,2,
    2,1,1,4,            4,4,                2,1,1,2,2,
    4,4,                4,4,                2,2,2,2,
    2,1,1,4,            4,4,                2,1,1,2,2,
    4
};

////////////////////////////////////////////////////////////////////////////////
const u16 tonetime[] = {
    1000,
    // 1: do
    7633,   6802,   6060,   5076,   5102,   4545,   4048,1000,1000,1000,
    //11: `do
    3824,   3407,   3034,   2865,   2550,   2273,   2024,1000,1000,1000,
    //21: do`
    15267,  13605,  12121,  11429,  10204,  9090,   8097,1000,1000,1000,
    //31: ``do
    1910,   1702,   1516,   1432,   1276,   1136,   1012,1000,1000,1000,
};

////////////////////////////////////////////////////////////////////////////////
void setBuzzerFreq(u16 Period)
{
    TIM_SetAutoreload(BEEP_TIMER, Period);
    if (2 == BEEP_PWMCH) {
        TIM_SetCompare2(BEEP_TIMER, Period / 2);
    }
    else
        TIM_SetCompare1(BEEP_TIMER, Period / 2);    
}

////////////////////////////////////////////////////////////////////////////////
void setBuzzerEn(FunctionalState NewState)
{
    TIM_Cmd(BEEP_TIMER, NewState);
}

////////////////////////////////////////////////////////////////////////////////
void musicTick()
{
    u16 tonetemp;
    
    switch(sPlayMusic.PlayStep){
        ////////////////////////////////////////////////////////////////////////    
        case PLAYSTEP1:
        setBuzzerEn(DISABLE);
        if(true == sPlayMusic.PlayFlag){
            setBuzzerFreq(tonetime[music[sPlayMusic.ToneNumCount]]);
            sPlayMusic.PlayStep = PLAYSTEP2;         
        }
        break;
        ////////////////////////////////////////////////////////////////////////
        case PLAYSTEP2:
        if(true == sPlayMusic.PlayFlag){
            // play music 0
            if(sPlayMusic.MusicNum == 0) {
                if(sPlayMusic.ToneNumCount < sizeof(music)/sizeof(u8)){
                    sPlayMusic.ToneCount++;
                    if(sPlayMusic.ToneCount <= (time[sPlayMusic.ToneNumCount] * 10 / 2)){
                        setBuzzerEn(ENABLE);
                    }
                    else {
                        setBuzzerEn(DISABLE);
                        if(sPlayMusic.ToneCount >= (time[sPlayMusic.ToneNumCount] * 10)){
                            sPlayMusic.ToneCount =0;
                            sPlayMusic.ToneNumCount ++;
                            tonetemp = tonetime[music[sPlayMusic.ToneNumCount]];
                            if(tonetemp <= 1000) 
                                setBuzzerEn(DISABLE);
                            else {
                                setBuzzerFreq(tonetemp);
                                setBuzzerEn(ENABLE);
                            }
                        }
                    }
                }
                else{
                    sPlayMusic.PlayStep = PLAYSTEP3;
                    setBuzzerEn(DISABLE);
                }
            }
            else if(sPlayMusic.MusicNum == 1) {
                // play music 1
                if(sPlayMusic.ToneNumCount < sizeof(music1)/sizeof(u8)){
                    sPlayMusic.ToneCount++;
                    if(sPlayMusic.ToneCount <= (time1[sPlayMusic.ToneNumCount] * 10 / 2)){
                        setBuzzerEn(ENABLE);
                    }
                    else {
                        setBuzzerEn(DISABLE);
                        if(sPlayMusic.ToneCount >= (time1[sPlayMusic.ToneNumCount] * 10)){
                            sPlayMusic.ToneCount =0;
                            sPlayMusic.ToneNumCount ++;
                            tonetemp = tonetime[music1[sPlayMusic.ToneNumCount]];
                            if(tonetemp <= 1000) 
                                setBuzzerEn(DISABLE);
                            else {
                                setBuzzerFreq(tonetemp);
                                setBuzzerEn(ENABLE);
                            }
                        }
                    }
                }
                else{
                    sPlayMusic.PlayStep = PLAYSTEP3;
                    setBuzzerEn(DISABLE);
                }
            }
            
            
            ///
            else{
                setBuzzerEn(DISABLE);
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////
        case PLAYSTEP3:         
        sPlayMusic.PlayFlag = false;
        sPlayMusic.CurrentPlayFlag = false;
        sPlayMusic.MusicNum = 0;
        sPlayMusic.ToneCount = 0;
        sPlayMusic.ToneNumCount = 0;
        sPlayMusic.PlayStep = PLAYSTEP1;
        break; 
    }
}

/// @}

/// @}

/// @}
