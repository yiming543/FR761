/*
 * File:   func_main.c
 * Author: T00904
 *
 * Created on 2026年2月23日, 下午 2:19
 */

 #include "PWM.h"
#include "mcc_generated_files/mcc.h"
#include "func_main.h"
#include "timer.h"
#include "gpio.h"
#include "func_ws2811.h"


void Clear_No_Call_Warning(void);
void software_Initialize(void);
void func_main (void);

#define DELAY_100MS (100)

// 消除未呼叫警告 假裝有使用這些函數
void Clear_No_Call_Warning(void) {
  uint8_t i = 0;
  if (i == 1) {
    PIN_MANAGER_IOC();
  } else if (i == 2) {
    LoBeam_ON();
    LoBeam_OFF();
    HiBeam_ON();
    HiBeam_OFF();
    LED_OFF();
  } else if (i == 3) {
    TrunLight(0);
  } else if (i == 4) {
    TMR4_StartTimer();
    TMR4_StopTimer();
    TMR4_ReadTimer();
    TMR4_WriteTimer(0);
    TMR4_LoadPeriodRegister(0);
    TMR4_HasOverflowOccured();
  } else if (i == 5) {
    TMR2_StartTimer();
    TMR2_StopTimer();
    TMR2_ReadTimer();
    TMR2_WriteTimer(0);
    TMR2_LoadPeriodRegister(0);
    TMR2_HasOverflowOccured();
  } else if (i == 6) {
    TMR6_StartTimer();
    TMR6_StopTimer();
    TMR6_ReadTimer();
    TMR6_WriteTimer(0);
    TMR6_LoadPeriodRegister(0);
    TMR6_HasOverflowOccured();
  } else if (i == 7) {
  } else if (i == 8) {
//    Timer_TESTMODE();
//    Streamer_SEG1_TEST(0);
//    Streamer_SEG2_TEST(0);
  }
}

// 初始化軟體相關的變數和狀態
void software_Initialize(void) {
  Color_OFF();
  // if (SW_IN_PORT == 1) {
  //   testmode();
  // }
  Clear_No_Call_Warning();
}

void func_main (void) {
    Timer();
    if (ORB.fTurn) {
      TrunLight(PWM_DUTY_100); //test
      T1MS_cnt =0;
    }

    //test Timer裡面有處理了
    // if (ORB.fLoBeam) {
    //   LoBeam_ON();
    // } else {
    //   LoBeam_OFF();
    // }

    // if (ORB.fHiBeam) {
    //   HiBeam_ON();
    // } else {
    //   HiBeam_OFF();
    // }

    if (flag1.fLight_en){
      if (flag1.fWelcome) {
        WelcomeLight_ON(PWM_DUTY_100);
        T1MS_cnt =0;
      }
    }

    if (flag1.fLight_en){
      //固定時間送WS2811的信號 100ms
      if(T1MS_cnt >= DELAY_100MS){
        T1MS_cnt = 0;

        if (ORB.fDRL) {
          DRL_ON();
        } else if (ORB.fPOS) {
          POS_ON();
        } else {
          Color_OFF();
        }
      }
    }
}