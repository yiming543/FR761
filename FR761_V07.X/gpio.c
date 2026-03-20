/*
 * File:   gpio.c
 * Author: T00904
 * 車燈輸入腳位(輸入) DRL/POS/TURN/HIBEAM/LOBEAM/ACC
 * 車燈LED腳位(輸出)  DRL/POS/TURN/HIBEAM/LOBEAM/P2
 * 散熱風扇腳位(輸出) FAN_EN
 * LED顏色選擇腳位(輸入) COLORSELECT
 * LED顏色控制腳位(輸出) YL_en WL_en
 * LED顏色控制腳位(輸出) WL_en WL_en
 * Created on 2026年2月23日, 下午 3:18
 */

#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include <stdbool.h>
// #include <stdint.h>
#include "gpio.h"

// output
#define YL_en YL_en_LAT
#define WL_en WL_en_LAT
// #define Fan_en FAN_en_LAT
#define FAN_en FAN_EN_LAT
#define LoBeam_en LoBeam_en_LAT
#define HiBeam_en1 HiBeam_en1_LAT
#define HiBeam_en2 HiBeam_en2_LAT


enum COLOR { COLOR_YELLOW = 0, COLOR_WHITE = 1 };

#define ON 1
#define OFF 0

// #define LampSideSelect() Lamp_R_EN_PORT
// #define LAMP_R_SIDE 0
#define LAMP_L_SIDE 1

void LoBeam_ON(void) {
  LoBeam_en = ON;
  FAN_en = ON;
}

void LoBeam_OFF(void) {
  LoBeam_en = OFF;
  FAN_en = OFF;
}

void HiBeam_ON(void){
  HiBeam_en1 = ON;
  HiBeam_en2 = ON;
}

void HiBeam_OFF(void){
  HiBeam_en1 = OFF;
  HiBeam_en2 = OFF;
}

// DRL/POS/TURN LED顏色設定
void Color_set(void){
  if(ColorSelect == COLOR_YELLOW){
    YL_en = 1;
    WL_en = 0;
  }else{
    YL_en = 0;
    WL_en = 1;
  }
}

void Color_Yellow(void){
    YL_en = 1;
    WL_en = 0;
}

void Color_White(void){
    YL_en = 0;
    WL_en = 1;
}

void Color_OFF(void){
    YL_en = 0;
    WL_en = 0;
}