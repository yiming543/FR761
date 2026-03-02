/*
 * File:   timer.c
 * Author: T00904
 *
 * Created on 2025年10月29日, 上午 8:56
 */

#include "mcc_generated_files/mcc.h"
#include "timer.h"
#include "gpio.h"



// variable declaration
uint16_t T1MS_disable_cnt = 0;
// uint8_t T5MS_cnt = 0;
uint16_t T1MS_cnt = 0;
uint16_t T1MS_welcome_cnt = 0;
// uint8_t T250MS_cnt = 0;

// bool fWelcome = 0;


// debounce後的IO狀態
ORB_t ORB = {0};
flag_t flag1 = {0};

void Timer(void) {
  static uint8_t T1ms_cnt = 0;
  static ORB_t TRB = {0};
  uint8_t temp = 0;

  // 等待TMR4溢出 (1ms)
  while (!PIR3bits.TMR4IF);
  PIR3bits.TMR4IF = 0;

  T1ms_cnt++;
  T1MS_cnt++;
  T1MS_welcome_cnt++;

  if (T1MS_disable_cnt > 0) {
    T1MS_disable_cnt--;
    flag1.fLight_en = 0;
  }
  else{
    flag1.fLight_en = 1;
  }

  // IO debounce 5ms
  if (T1ms_cnt >= 5) {
    T1ms_cnt = 0;
    temp = 0;
    if (POS_IN)
      temp |= 1 << ePOS;
    if (DRL_IN)
      temp |= 1 << eDRL;
    if (Turn_IN)
      temp |= 1 << eTurn;
    if (HiBeam_IN)
      temp |= 1 << eHiBeam;
    if (LoBeam_IN)
      temp |= 1 << eLoBeam;
    if (ACC_IN)
      temp |= 1 << eACC;

    if (temp ^ TRB.all_flags) {
      // IO狀態改變
      TRB.all_flags = temp;
    } else {
      // IO狀態穩定
      ORB.all_flags = temp;
    }
  }

  // test
  if (ORB.fLoBeam) {
    LoBeam_en = 1;
    FAN_en = 1;
    // HiBeam_IN(有近燈才能開遠燈)
    if (ORB.fHiBeam) {
      HiBeam_en1 = 1;
      HiBeam_en2 = 1;
    } else {
      HiBeam_en1 = 0;
      HiBeam_en2 = 0;
    }
  } else {
    LoBeam_en = 0;
    FAN_en = 0;
    //(沒有近燈則遠燈關閉)
    HiBeam_en1 = 0;
    HiBeam_en2 = 0;
  }
}

// void Timer_TESTMODE(void) {
//   while (!TMR2IF) {
//   };
//   TMR2IF = 0;
//   T100us_cnt++;
//   T100us_cnt1++;
//   if (T100us_cnt >= 10) { // 1ms
//     T100us_cnt = 0;
//     T1MS_cnt++;
//   }

//   if (T100us_cnt1 >= 50) { // 5ms
//     T100us_cnt1 = 0;
//     T5MS_cnt++;
//   }

//   if (T5MS_cnt >= 50) { // 250ms
//     T5MS_cnt = 0;
//     T250MS_cnt++;
//   }

//   //rise edge detect LoBeam_IN
//   if ((LoBeam_IN == 1) && (fLoBeam == 0)) {
//     if (mode < 3) {
//       mode++;
//     } else {
//       mode = 0;
//     }
//   }

//   if (LoBeam_IN == 1) {
//     fLoBeam = 1;
//   } else {
//     fLoBeam = 0;
//   }
// }

// void Delay_ms(uint16_t input_ms) {
//   uint16_t delay_1ms_cnt = 0;

//   do {
//     // 等待TMR2溢出 (1ms)
//     while (!PIR1bits.TMR2IF);
//     PIR1bits.TMR2IF = 0;
//     delay_1ms_cnt++;

//     // check turn signal
//     if (Turn_IN == 1) {
//       bTurn = 1;
//     } else {
//       bTurn = 0;
//     }

//   } while (delay_1ms_cnt < input_ms);
// }
