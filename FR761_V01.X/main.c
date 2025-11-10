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
// 20250908 V05 CS:E04B
// 車輛實測 ford ranger 2023
// #define MODE_OFF_DRL 0x49
// #define MODE_POS 0x0A
// #define MODE_AUTO_DAY 0x4C
// #define MODE_AUTO_NIGHT 0x8C
// #define MODE_LOW_BEAM 0x0B
// #define MODE_OFF_DRL_24 0x51
// #define MODE_POS_24 0x14
// #define MODE_POS_24_2 0x12
// #define MODE_AUTO_NIGHT_24 0x94
// #define MODE_AUTO_NIGHT_24_2 0xAC
// #define MODE_LOW_BEAM_24 0x13
// // 超車
// #define OVER_TAKING 0B10000000 // 0x80 data[1]
// // 遠燈
// #define HIGH_BEAM 0B01000000 // 0x40 data[1]
// // 左方向
// #define LEFT_TURN_SIGNAL 0B00010000 // 0x10 data[1]
// // 右方向
// #define RIGHT_TURN_SIGNAL 0B00100000 // 0x20 data[1]
// //晝行
// #define DRL 0B01000000 // 0x40 data[2]
// //近燈
// #define LOBEAM 0B00001000 // 0x08 data[4]
// //行車
// #define POS 0B00000011 // 0x03 data[4]
// 收到LIN訊號後，設定燈號旗標

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



#include "mcc_generated_files/LINDrivers/lin_slave.h"
#include "mcc_generated_files/mcc.h"
// #include <pic.h>
#include <pic16f1936.h>

#include "PWM.h"
#include "timer.h"

extern LampFlags_t lampFlags;
bool fLampSide = 0; // 0:右邊燈具
bool fTurn = 0;
bool ftest = 0;
uint8_t mode = 0;
uint8_t mode_old = 0;

// 啟動UART自動波特率偵測
void AutoBaud_Detect_ON(void) {
  BAUDCONbits.ABDOVF = 0;
  BAUDCONbits.ABDEN = 1;
  BAUDCONbits.WUE = 1;
}

// 設定通信波特率
void AutoBaud_Detect(void) {
  while (1) {
    AutoBaud_Detect_ON();
    while (!BAUDCONbits.ABDOVF) // wait for auto baud to complete
    {
      if (!BAUDCONbits.ABDEN) // if auto baud is disabled, break the loop
      {
        break;
      }
    }

    // 找到正確波特率,沒有超時離開無限迴圈
    if (!BAUDCONbits.ABDOVF) // if auto baud overflowed, try again
    {
      break;
    }
  }
}

/*
                         Main application
 */

void testmode(void) {
  // TEST GPIO & LED
  fColorSeletDisable = 1;
  HiBeam_en1 = 1;
  HiBeam_en2 = 1;
  ftest =1;
  while (SW_IN_PORT == 1) {
    Timer_TESTMODE();
    // 換模式


    // 換顏色
    if (Turn_IN == 1)
    {
        YL_en = 1;
        WL_en = 0;
      }
    else {
      YL_en = 0;
      WL_en = 1;
    }

    // 模式切換
    if ((mode == 0) && (mode != mode_old)) {
      mode_old = 0;
      HiBeam_en1 = 1;
      HiBeam_en2 = 1;
      LoBeam_en =0;
      LED_OFF();
      T250MS_cnt =0;
      while(T250MS_cnt >1)
      {
        Timer_TESTMODE();
      }
    } else if ((mode == 1) && (mode != mode_old)) {
      mode_old = 1;
      HiBeam_en1 = 1;
      HiBeam_en2 = 1;
      LoBeam_en =1;
      LED_OFF();
      T250MS_cnt =0;
      while(T250MS_cnt >1)
      {
        Timer_TESTMODE();
      }      
    } else if ((mode == 2) && (mode != mode_old)) {
      mode_old = 2;
      HiBeam_en1 = 0;
      HiBeam_en2 = 0;
      LoBeam_en =0;
      LED_ON(PWM_DUTY_10);
      T250MS_cnt =0;
      while(T250MS_cnt >1)
      {
        Timer_TESTMODE();
      }      
    } else if ((mode == 3) && (mode != mode_old)) {
      mode_old = 3;
      HiBeam_en1 = 0;
      HiBeam_en2 = 0;
      LoBeam_en =0;
      LED_ON(PWM_DUTY);
      T250MS_cnt =0;
      while(T250MS_cnt >1)
      {
        Timer_TESTMODE();
      }      
    }

    if((Turn_IN==1)&&(fTurn==0))
    {
      fTurn=1;
      YL_en=1;
      WL_en=0;
      Streamer_SEG1_TEST(5);
      Streamer_SEG2_TEST(5);
    }
    else if((Turn_IN==0)&&(fTurn==1))
    {
      fTurn=0;
      YL_en=0;
      WL_en=1;
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

int main(void) {
  uint8_t UART_CNT = 0;
  // initialize the device
  SYSTEM_Initialize();
  // When using interrupts, you need to set the Global and Peripheral Interrupt
  // Enable bits Use the following macros to:
  DELAY_2ms();
  if (SW_IN_PORT == 1) {
    testmode();
  }

  DELAY_2ms();
  LoBeam_en = 1;
  ftest = 1;
  while (1) {
    Timer();
    if (RC7_GetValue() == 1) {
      if (UART_CNT < 10)
        UART_CNT++;
      else
        break;
    } else
      UART_CNT = 0;
  }
  AutoBaud_Detect();
  LoBeam_en = 0;
  ftest = 0;
  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();

  // Enable the Peripheral Interrupts
  INTERRUPT_PeripheralInterruptEnable();

  // Disable the Global Interrupts
  // INTERRUPT_GlobalInterruptDisable();

  // Disable the Peripheral Interrupts
  // INTERRUPT_PeripheralInterruptDisable();

  // 左邊燈具選擇
  if (LampSideSelect() == LAMP_R_SIDE)
    fLampSide = LAMP_R_SIDE;
  else
    fLampSide = LAMP_L_SIDE;

  while (1) {
    // Add your application code
    LIN_handler();
    Timer(); // 測試:之後移除
  }

  return 0;
}
/**
 End of File
 */