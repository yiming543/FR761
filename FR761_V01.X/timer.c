/*
 * File:   timer.c
 * Author: T00904
 *
 * Created on 2025年10月29日, 上午 8:56
 */

#include "timer.h"
#include <pic16f1936.h>

extern LampFlags_t lampFlags;
extern bool ftest;
extern uint8_t mode;

enum COLOR { COLOR_YELLOW = 0, COLOR_WHITE = 1 };

// variable declaration
uint8_t T100us_cnt = 0;
uint8_t T100us_cnt1 = 0;
uint16_t T1MS_cnt = 0;
uint8_t T5MS_cnt = 0;
uint8_t T250MS_cnt = 0;
bool fY_W = 0; // 0: Yellow 1: White
bool fColorSeletDisable = 0;
bool fLoBeam = 0;

void Timer(void) {
  while (!TMR2IF) {
  };
  TMR2 += 57;
  TMR2IF = 0;
  T100us_cnt++;
  T100us_cnt1++;
  if (T100us_cnt1 >= 10) { // 1ms
    T100us_cnt1 = 0;
    T1MS_cnt++;
  }

  if (T100us_cnt >= 50) { // 5ms
    T100us_cnt = 0;
    T5MS_cnt++;
  }
  if (T5MS_cnt >= 50) { // 250ms
    T5MS_cnt = 0;
    T250MS_cnt++;
  }

  if (!fColorSeletDisable) {
    if (ColorSelect == COLOR_YELLOW) {
      // 黃光
      YL_en = 1;
      WL_en = 0;
    } else {
      // 白光
      YL_en = 0;
      WL_en = 1;
    }
  }

  // 計算Autobaud時，不做LoBeam控制
  if (ftest == 0) {
    if (LoBeam_IN == 1) {
      fLoBeam = 1;
      LoBeam_en = 1;
      // HiBeam_en1=1;
      // HiBeam_en2=1;
    } else if (lampFlags.fLOBEAM == 0) {
      fLoBeam = 0;
      LoBeam_en = 0;
      // HiBeam_en1=0;
      // HiBeam_en2=0;
    }
  }
}

void Timer_TESTMODE(void) {
  while (!TMR2IF) {
  };
  TMR2 += 57;
  TMR2IF = 0;
  T100us_cnt++;
  T100us_cnt1++;
  if (T100us_cnt1 >= 10) { // 1ms
    T100us_cnt1 = 0;
    T1MS_cnt++;
  }

  if (T100us_cnt >= 50) { // 5ms
    T100us_cnt = 0;
    T5MS_cnt++;
  }
  if (T5MS_cnt >= 50) { // 250ms
    T5MS_cnt = 0;
    T250MS_cnt++;
  }

  if((LoBeam_IN==1)&&(fLoBeam==0))
  {
    if(mode <3)
    {
      mode++;
    }
    else {
      mode =0;
    }
  }

  if (LoBeam_IN == 1) {
    fLoBeam = 1;
  } else{
    fLoBeam = 0;
  }
}

void Delay_ms(uint16_t input_ms) {
  uint16_t ms = input_ms - 1;
  TMR0 = 56;
  T100us_cnt1 = 0;
  T1MS_cnt = 0;
  do {
    Timer();
  } while (T1MS_cnt < ms);
}