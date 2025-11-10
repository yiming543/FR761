/**
  LIN Slave Application

  Company:
    Microchip Technology Inc.

  File Name:
    lin_app.c

  Summary:
    LIN Slave Application

  Description:
    This source file provides the interface between the user and
    the LIN drivers.

 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

#include "lin_app.h"
#include "../pin_manager.h"
#include "lin_slave.h"
#include <xc.h>

#include "../../PWM.h"
#include "../../timer.h"

// #define TEST_PIN TEST_LAT
//  20251008 CS:8864  V01
//  模式和旗標都可以判斷信號

//  D1 bit7 超車
//  D1 bit6 遠燈
//  D1 bit5 右方向
//  D1 bit4 左方向
// D0    D1    D2    D3    D4    D5    D6    D7
// 0x24  0x00  0x00  0x00  0x00  0x00  0x00  0x00

// D0
// 0x01 UNLOCK
// 0x11 LOCK
// 0x24 引擎啟動

// D1
// 0x80 超車 bit7
// 0x40 遠燈 bit6
// 0x20 右方向 bit5
// 0x10 左方向 bit4

// D2
// 0x51 OFF mode 晝行
// 0x14 POS mode 位置燈(行車)
// 0x4C AUTO mode 白天 晝行
// 0x94 AUTO mode 夜間 近燈+行車
// 0x13 近燈 mode 近燈
// bit6 晝行
// bit3 行車

// Const 啟動碼 As Integer = &H24  'D0
// Const UNLOCK碼 As Integer = &H1  'D0
// Const LOCK碼 As Integer = &H11  'D0

// Const 超車碼 As Integer = &H80  'D1
// Const 遠燈碼 As Integer = &H40  'D1
// Const 右方向碼 As Integer = &H20    'D1
// Const 左方向碼 As Integer = &H10    'D1

// Const OFF模式碼_24 As Integer = &H51   'D2 2024
// Const 行車模式碼_24 As Integer = &H14   'D2 2024
// Const 行車模式碼_24_2 As Integer = &H12   'D2 2024
// Const AUTO晚上模式碼_24 As Integer = &H94  'D2 2024
// Const AUTO晚上模式碼_24_2 As Integer = &HAC  'D2 2024
// Const 近燈模式碼_24 As Integer = &H13   'D2 2024

// Const OFF模式碼 As Integer = &H49   'D2 2023
// Const 行車模式碼 As Integer = &HA   'D2 2023
// Const AUTO白天模式碼 As Integer = &H4C  'D2 2023
// Const AUTO晚上模式碼 As Integer = &H8C  'D2 2023
// Const 近燈模式碼 As Integer = &HB   'D2 2023
// Const 晝行碼 As Integer = &H40   'D2

// Const 近燈碼 As Integer = &H8   'D4
// Const 行車碼 As Integer = &H3   'D4

extern bool fLoBeam;
LampState_t lampState;
bool fLock = 0; // 1:lock 0:unlock
LampFlags_t lampFlags;
bool fRight_old=0;
bool fLeft_old=0;

// ford ranger 2024
#define MODE_OFF_DRL_24 0x51
#define MODE_POS_24 0x14
#define MODE_POS_24_2 0x12
#define MODE_AUTO_NIGHT_24 0x94
#define MODE_AUTO_NIGHT_24_2 0xAC
#define MODE_LOW_BEAM_24 0x13
// ford ranger 2023
#define MODE_OFF_DRL 0x49
#define MODE_POS 0x0A
#define MODE_AUTO_DAY 0x4C
#define MODE_AUTO_NIGHT 0x8C
#define MODE_LOW_BEAM 0x0B
// 超車
#define mOVER_TAKING 0B10000000 // 0x80 data[1] MASK
// 遠燈
#define mHIBEAM 0B01000000 // 0x40 data[1]
// 左方向
#define mLEFT_TURN_SIGNAL 0B00010000 // 0x10 data[1]
// 右方向
#define mRIGHT_TURN_SIGNAL 0B00100000 // 0x20 data[1]
// 晝行
#define mDRL 0B01000000 // 0x40 data[2]
// 近燈
#define mLOBEAM 0B00001000 // 0x08 data[4]
// 行車
#define mPOS 0B00000011 // 0x03 data[4]

void POS_ON(void) {}
void POS_OFF(void) {}
void DRL_ON(void) {}
void DRL_OFF(void) {}
void LOBEAM_ON(void) { LoBeam_en_SetHigh(); }
void LOBEAM_OFF(void) { LoBeam_en_SetLow(); }

void HIBEAM_ON(void) {
  HiBeam_en1_SetHigh();
  HiBeam_en2_SetHigh();
}
void HIBEAM_OFF(void) {
  HiBeam_en1_SetLow();
  HiBeam_en2_SetLow();
}
void LEFT_ON(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  TrunLight(PWM_DUTY);
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}
void LEFT_ON2(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  LED_ON(PWM_DUTY);
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}

void LEFT_OFF(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  DELAY_1ms();
  DELAY_1ms();
  LED_OFF();
  DELAY_1ms();
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}
void RIGHT_ON(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  TrunLight(PWM_DUTY);
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}
void RIGHT_ON2(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  LED_ON(PWM_DUTY);
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}
void RIGHT_OFF(void) {
  // Disable the Global Interrupts
  INTERRUPT_GlobalInterruptDisable();
  DELAY_1ms();
  DELAY_1ms();
  LED_OFF();
  DELAY_1ms();
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
}

// 準備LIN接收所需要的變數和函式
void LIN_Slave_Initialize(void) {

  LIN_init(TABLE_SIZE, scheduleTable, processLIN);
}

// LIN資料處理()
void processLIN(void) {
  uint8_t tempRxData[8];
  uint8_t cmd;

  lampState.Byte = 0;
  lampFlags.Byte = 0;
  // TEST_PIN =1;

  cmd = LIN_getPacket(tempRxData);
  // TEST_PIN =0;

  switch (cmd) {
  case SW_STATE:
    // TEST_Toggle();
    // Lamp mode judgment
    if ((tempRxData[2] == MODE_OFF_DRL) || (tempRxData[2] == MODE_OFF_DRL_24)) {
      lampState.mode = eOFF_DRL;
      lampFlags.fDRL = 1;
    } else if ((tempRxData[2] == MODE_POS) || (tempRxData[2] == MODE_POS_24) ||
               (tempRxData[2] == MODE_POS_24_2)) {
      lampState.mode = ePOS;
      lampFlags.fPOS = 1;
    } else if ((tempRxData[2] == MODE_AUTO_NIGHT) ||
               (tempRxData[2] == MODE_AUTO_NIGHT_24) ||
               (tempRxData[2] == MODE_AUTO_NIGHT_24_2)) {
      lampState.mode = eAUTO_LOBEAM_POS;
      lampFlags.fLOBEAM = 1;
      lampFlags.fPOS = 1;
    } else if (tempRxData[2] == MODE_AUTO_DAY) {
      lampState.mode = eAUTO_DRL;
      lampFlags.fDRL = 1;
    } else if ((tempRxData[2] == MODE_LOW_BEAM) ||
               (tempRxData[2] == MODE_LOW_BEAM_24)) {
      lampState.mode = eLOBEAM;
      lampFlags.fLOBEAM = 1;
      lampFlags.fPOS = 1;
    }

    // 超車
    if ((tempRxData[1] & mOVER_TAKING) == mOVER_TAKING) {
      lampState.OverTaking = 1;
      lampFlags.fHIBEAM = 1;
    }

    // 遠燈
    if ((tempRxData[1] & mHIBEAM) == mHIBEAM) {
      lampState.HighBeam = 1;
      lampFlags.fHIBEAM = 1;
    }

    // 左方向
    if ((tempRxData[1] & mLEFT_TURN_SIGNAL) == mLEFT_TURN_SIGNAL) {
      lampState.TurnLeft = 1;
      lampFlags.fLEFT = 1;
    }

    // 右方向
    if ((tempRxData[1] & mRIGHT_TURN_SIGNAL) == mRIGHT_TURN_SIGNAL) {
      lampState.TurnRight = 1;
      lampFlags.fRIGHT = 1;
    }

    // 晝行
    // if ((tempRxData[2] & mDRL) == mDRL) {
    //   lampFlags.fDRL = 1;
    // }

    // 近燈
    // if ((tempRxData[4] & mLOBEAM) == mLOBEAM) {
    //   lampFlags.fLOBEAM = 1;
    // }

    // 行車
    // if ((tempRxData[4] & mPOS) == mPOS) {
    //   lampFlags.fPOS = 1;
    // }

    break;

  default:
    lampState.Byte = 0;
    lampFlags.Byte = 0;
    break;
  }

  // Set lamp output
  // DRL
  if (lampFlags.fDRL) {
    DRL_ON();
  } else {
    DRL_OFF();
  }
  // POS
  if (lampFlags.fPOS) {
    POS_ON();
  } else {
    POS_OFF();
  }
  // LOBEAM
  if (lampFlags.fLOBEAM) {
    LOBEAM_ON();
  } else if(fLoBeam==0){
    LOBEAM_OFF();
  } 
  
  // HIBEAM
  if (lampFlags.fHIBEAM) {
    HIBEAM_ON();
  } else {
    HIBEAM_OFF();
  }

  //turn signal
  if(fLampSide==LAMP_R_SIDE)
  {
    if ((lampFlags.fLEFT) ){
      if(fLeft_old==0)
      {
        fLeft_old=1;
        LEFT_ON();
      }
      else {
        LEFT_ON2();
      }
    }
    else{
      if(fLeft_old==1){
        fLeft_old=0;
        LEFT_OFF();
      }
    }

  }
  else {
  {
    if ((lampFlags.fRIGHT)) {
      if(fRight_old==0){
        fRight_old=1;
        RIGHT_ON();
      }
      else {
       RIGHT_ON2();
      }
    }
    else {
      if(fRight_old==1){
        fRight_old=0;
        RIGHT_OFF();
      }
    }
  }
  }
}
