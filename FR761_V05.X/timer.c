/*
 * File:   timer.c
 * Author: T00904
 *
 * Created on 2025年10月29日, 上午 8:56
 */

#include "timer.h"
#include <pic16f1936.h>

// variable declaration
uint8_t T100us_cnt = 0;
uint8_t T100us_cnt1 = 0;
uint16_t T1MS_cnt = 0;
uint16_t T1MS_disable_cnt = 0;
uint8_t T5MS_cnt = 0;
uint8_t T250MS_cnt = 0;
bool fLoBeam = 0;
bool fHiBeam = 0;
bool fACC = 0;
bool fTurn = 0;
bool fPOS = 0;
bool fDRL = 0;
bool fWelcome = 0;
// testmode
uint8_t mode = 0;

void Timer(void) {
  while (!TMR2IF) {
  };
  TMR2IF = 0;
  T100us_cnt++;
  if (T100us_cnt >= 10) { // 1ms
    T100us_cnt = 0;
    T1MS_cnt++;
    if(T1MS_disable_cnt>=1){
      T1MS_disable_cnt--;
    }
  }

  // check LoBeam
  if (LoBeam_IN == 1) {
    fLoBeam = 1;
    LoBeam_en = 1;
  } else {
    fLoBeam = 0;
    LoBeam_en = 0;
  }

  // check ACC
  if (ACC_IN == 1) {
    fACC = 1;
  } else {
    fACC = 0;
  }

  // check turn signal
  if (Turn_IN == 1) {
    fTurn = 1;
  } else {
    fTurn = 0;
  }

  //HiBeam_IN
  if(HiBeam_IN==1)
  {
    HiBeam_en1=1;
    HiBeam_en2=1;
  }
  else {
    HiBeam_en1=0;
    HiBeam_en2=0;
  }
}

void Timer_TESTMODE(void) {
  while (!TMR2IF) {
  };
  TMR2IF = 0;
  T100us_cnt++;
  T100us_cnt1++;
  if (T100us_cnt >= 10) { // 1ms
    T100us_cnt = 0;
    T1MS_cnt++;
  }

  if (T100us_cnt1 >= 50) { // 5ms
    T100us_cnt1 = 0;
    T5MS_cnt++;
  }
  if (T5MS_cnt >= 50) { // 250ms
    T5MS_cnt = 0;
    T250MS_cnt++;
  }

  if ((LoBeam_IN == 1) && (fLoBeam == 0)) {
    if (mode < 3) {
      mode++;
    } else {
      mode = 0;
    }
  }

  if (LoBeam_IN == 1) {
    fLoBeam = 1;
  } else {
    fLoBeam = 0;
  }
}

void Delay_ms(uint16_t input_ms) {
  uint16_t ms = input_ms;
  T100us_cnt = 0;
  T1MS_cnt = 0;
  do {
    Timer();
  } while (T1MS_cnt < ms);
}

void CheckGPIO(void) {
  // check POS
  if(POS_IN==1)
    fPOS=1;
  else
    fPOS=0;

  // check DRL
  if(DRL_IN==1)
    fDRL=1;
  else
    fDRL=0; 

  //check Hibeam
  if(HiBeam_IN==1)
    fHiBeam=1;
  else
    fHiBeam=0; 

}