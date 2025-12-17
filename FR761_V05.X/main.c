/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules
  selected in the GUI. Generation Information : Product Revision  :  PIC10 /
  PIC12 / PIC16 / PIC18 MCUs - 1.81.8 Device            :  PIC16F1936 Driver
  Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software
   and any derivatives exclusively with Microchip products. It is your
   responsibility to comply with third party license terms applicable to your
   use of third party software (including open source software) that may
   accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
 */
// 20251022 V06 CS:99BC
// modify AutoBaud_Detect function，只在開機時做auto baud偵測
// 不在每次接收同步信號時做auto baud偵測，因為這樣會導致無法正確接收LIN訊號
// 在每次接收同步信號時,如果沒有接收到同步信號,則會重置UART,重新開始接收同步信號.
// timer out 15ms
// LIN接收加入註解
// 修改程式消失所有編譯警告
// 使用控制模式點亮燈號，預留bit模式點亮燈號功能

// 20251107 V01 CS:BE2A
// 新增TESTMODE
// 當送電時選擇白光(5V)，進入TESTMODE 遠光恆亮.
// LoBeam Pin 5V:LoBeam亮
// LoBeam Pin 0V:LoBeam滅
// Turn Pin 0V:黃光逐一亮起，最終停在全亮.
// Turn Pin 5V:白光逐一亮起，最終停在全亮.
// 選擇黃光(0V)，離開TESTMODE

// 新增AutoBaud之前要等待TX Hi 1ms，等待時LoBeam亮，直到AutoBaud完成
// 避免AutoBaud出錯.
// 行車 10%，晝行100%，方向 100%
// 切換顏色: 行車/晝行

// 20251110 V02 CS:34BE
// PWM無法用中斷，5us中斷一次太平繁，程式無法使用.
// 在PWM中斷程序修改duty也不可行，超過5us.
// PWM每送出24bit要停280us
// LIN收到後只設定旗標，不做處理。
// LIN接收會干援PWM，需用另一個MCU接收LIN。
// 方向燈用GPIO接收 (無需分左右邊燈具)
// LoBeam用GPIO控制
// DRL，POS，HiBeam 由另一顆MCU解碼後經GPIO提供.
// 方向燈結束1秒後才可亮DRL或POS.
// 方向超過600ms就是迎賓信號，一直等到迎賓信號結束，600ms沒有其它信號再跑迎賓。

// 20211113 V03 CS:6EBD
//  方向燈用GPIO接收 (無需分左右邊燈具)
//  LoBeam用GPIO控制
//  DRL，POS，HiBeam 由另一顆MCU解碼後經GPIO提供.
//  方向燈結束600ms後才可亮DRL或POS.
//  方向超過600ms就是迎賓信號，一直等到迎賓信號結束，600ms沒有其它信號再跑迎賓。

// 20251114 V04 CS:45F7
// 迎賓燈完成
// 方向燈完成
// DRL完成
// POS完成
// HIBEAM完成
// LOBEAM完成
// TESTMODE REMARK

// 20251217 V05 CS:9639
// 新增RA0 控制風扇，Lobeam啟動時 風扇才啟動.
// 新增Lobeam點燈.
// 修正接收信號
// 新增LED OFF信號

#include "mcc_generated_files/mcc.h"
#include <pic16f1936.h>

#include "PWM.h"
#include "timer.h"

void LoBeam_Enable(void);
void LoBeam_Disable(void);
//

// main

/*
                         Main application
 */

void LoBeam_Enable(void) {
  LoBeam_en = 1;
  FAN_en = 1;
}

void LoBeam_Disable(void) {
  LoBeam_en = 0;
  FAN_en = 0;
}

void HiBeam_Enable(void){
  HiBeam_en1 = 1;
  HiBeam_en2 = 1;
}

void HiBeam_Disable(void){
  HiBeam_en1 = 0;
  HiBeam_en2 = 0;
}

void testmode(void) {
  bool fTurn_old = 0;
  uint8_t mode_old = 0;
  // TEST GPIO & LED
  // fColorSeletDisable = 1;
  HiBeam_en1 = 1;
  HiBeam_en2 = 1;
  // ftest = 1;

  while (SW_IN_PORT == 1) {
    Timer_TESTMODE();
    // 換模式

    // 換顏色
    if (Turn_IN == 1) {
      YL_en = 1;
      WL_en = 0;
    } else {
      YL_en = 0;
      WL_en = 1;
    }

    // 模式切換
    if ((mode == 0) && (mode != mode_old)) {
      mode_old = 0;
      HiBeam_en1 = 1;
      HiBeam_en2 = 1;
      LoBeam_en = 0;
      LED_OFF();
      T250MS_cnt = 0;
      while (T250MS_cnt > 1) {
        Timer_TESTMODE();
      }
    } else if ((mode == 1) && (mode != mode_old)) {
      mode_old = 1;
      HiBeam_en1 = 1;
      HiBeam_en2 = 1;
      LoBeam_Enable();
      LED_OFF();
      T250MS_cnt = 0;
      while (T250MS_cnt > 1) {
        Timer_TESTMODE();
      }
    } else if ((mode == 2) && (mode != mode_old)) {
      mode_old = 2;
      HiBeam_en1 = 0;
      HiBeam_en2 = 0;
      LoBeam_en = 0;
      LED_ON(PWM_DUTY_10);
      T250MS_cnt = 0;
      while (T250MS_cnt > 1) {
        Timer_TESTMODE();
      }
    } else if ((mode == 3) && (mode != mode_old)) {
      mode_old = 3;
      HiBeam_en1 = 0;
      HiBeam_en2 = 0;
      LoBeam_en = 0;
      LED_ON(PWM_DUTY);
      T250MS_cnt = 0;
      while (T250MS_cnt > 1) {
        Timer_TESTMODE();
      }
    }

    if ((Turn_IN == 1) && (fTurn_old == 0)) {
      fTurn_old = 1; // 黃光
      YL_en = 1;
      WL_en = 0;
      Streamer_SEG1_TEST(5);
      Streamer_SEG2_TEST(5);
    } else if ((Turn_IN == 0) && (fTurn_old == 1)) {
      fTurn_old = 0; // 白光
      YL_en = 0;
      WL_en = 1;
      Streamer_SEG1_TEST(5);
      Streamer_SEG2_TEST(5);
    }
  }
  HiBeam_en1 = 0;
  HiBeam_en2 = 0;
  LoBeam_en = 0;
  LED_OFF();
  DELAY_1ms();
}

// 消除未呼叫警告 假裝有使用這些函數
void Clear_No_Call_Warning(void) {
  uint8_t i = 0;
  if (i == 1) {
    LED_SET_Gradually_ON(0);
  } else if (i == 2) {
    WelcomeLight(0);
  } else if (i == 3) {
    TrunLight(0);
  } else if (i == 4) {
    testmode();
  } else if (i == 5) {
    PIN_MANAGER_IOC();
  }
}

int main(void) {
  // initialize the device
  SYSTEM_Initialize();

  DELAY_2ms();
  LED_OFF();
  // if (SW_IN_PORT == 1) {
  //   testmode();
  // }
  Clear_No_Call_Warning();
  while (1) {
    Timer();
    if (fTurn == 1) {
      TrunLight(PWM_DUTY_10);
    }

    if (fLoBeam == 1) {
      LoBeam_Enable();
    } else {
      LoBeam_Disable();
    }

    if (fHiBeam == 1) {
      HiBeam_Enable();
    } else {
      HiBeam_Disable();
    }

    if (T1MS_disable_cnt == 0) {
      if (fWelcome == 1) {
        WelcomeLight(PWM_DUTY);
      }
    }

    if (T1MS_disable_cnt == 0) {
      CheckGPIO();
      if (fDRL == 1) {
        DRL_Light();
      } else if (fPOS == 1) {
        POS_Light();
      } else {
        LED_OFF();
      }
    }
}

return 0;
}

/**
 End of File
 */