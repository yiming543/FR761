/*
 * File:   PWM.c
 * Author: T00904
 *
 * Created on August 26, 2025, 2:27 PM
 */
// #include "mcc_generated_files/mcc.h"
#include "timer.h"
#include <pic.h>
#include <stdint.h>

enum COLOR { COLOR_YELLOW = 0, COLOR_WHITE = 1 };
uint16_t i=0;
// 382ns
#define DATA_0() CCPR5L = 3
// 758ns
#define DATA_1() CCPR5L = 6
// PWM OFF
#define PWM_STOP() CCPR5L = 0
#define TIMER_FLAG PIR3bits.TMR6IF
// #define WS2811_NUM 18

// test WS2811 8LED by RGB
#define WS2811_SEG1_NUM 4
#define SEG1_LED_NUM (WS2811_SEG1_NUM * 3)
#define WS2811_SEG2_NUM 2
#define SEG2_LED_NUM (WS2811_SEG2_NUM * 3)
#define TOTAL_WS2811_NUM (WS2811_SEG1_NUM + WS2811_SEG2_NUM)

#define TOTAL_LED_NUM (SEG1_LED_NUM + SEG2_LED_NUM)
// #define Streamer1_DELAY() Delay_ms(75)
#define Streamer1_DELAY() Delay_ms(75)
#define Streamer1_DELAY_TEST() Delay_ms(500)
#define Streamer2_DELAY() Delay_ms(75)
#define Streamer2_DELAY_TEST() Delay_ms(500)
#define Gradually_DELAY() Delay_ms(12)

// 1byte 41.6us,18byte 750us
void PWM_SendData(uint8_t InputData) {
  uint8_t bitmask = 0x80; // 1000 0000
  uint8_t data = InputData;

  // 縮短PWM等待時間
  TMR6 = PR6 - 10;
  // send data
  do {
    if (data & bitmask) {
      DATA_1();
    } else {
      DATA_0();
    }
    while (!TIMER_FLAG) {
    };
    TIMER_FLAG = 0;
    bitmask = bitmask >> 1;
  } while (bitmask);

  PWM_STOP();
}

// 1byte 41.6us,18byte 750us
void LED_SET(uint8_t LED_num, uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = LED_num;

  for (uint8_t i = 0; i < num; i++) {
    PWM_SendData(data);
  }
}

// all led off
void LED_OFF(void) {
  DELAY_2ms();
  LED_SET(TOTAL_LED_NUM, 0x00);
}

// all led on
void LED_ON(uint8_t InputData) {
    uint8_t data = InputData;
    LED_SET(TOTAL_LED_NUM, data);
}

// Gradually brighten and fade out
// 呼吸燈--逐漸變亮
void LED_SET_Gradually_ON(uint8_t InputData) {
  uint8_t data = InputData;
  int temp;
  // uint8_t brighten = 0;
  for (int j = 0; j < 250; j += 1) {
    LED_SET(TOTAL_LED_NUM, (uint8_t)(j & 0xFF));
    // Gradually_DELAY();
    i=12;
    while(i--&& !fTurn){
      DELAY_1ms();
    }
  }
}

// 流光1
// Streamer LED ON 500us(753us)
void Streamer_SEG1_ON(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;
  
  for (uint8_t j = 0; j < SEG1_LED_NUM; j++) {
    num += 1;
    // SEG1
    LED_SET(num, data);                // LED ON
    LED_SET(SEG1_LED_NUM - num, 0x00); // LED OFF
    // SEG2 OFF
    LED_SET(SEG2_LED_NUM, 0x00); // LED OFF
    // Streamer1_DELAY();
    i=75;
    while(i--&& !fTurn){
      DELAY_1ms();
    }
  }
}

void Streamer_SEG1_TEST(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;

  for (uint8_t i = 0; i < SEG1_LED_NUM; i++) {
    num += 1;
    // SEG1
    LED_SET(num, data);                // LED ON
    LED_SET(SEG1_LED_NUM - num, 0x00); // LED OFF
    // SEG2 OFF
    LED_SET(SEG2_LED_NUM, 0x00); // LED OFF
    Streamer1_DELAY_TEST();
  }
}

// Streamer LED OFF
void Streamer_SEG1_OFF(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;

  for (uint8_t j = 0; j < SEG1_LED_NUM; j++) {
    num += 1;
    // SEG1
    LED_SET(SEG1_LED_NUM - num, data); // LED ON
    LED_SET(num, 0x00);                // LED OFF
    // SEG2 OFF
    LED_SET(SEG2_LED_NUM, 0x00); // LED OFF
    // Streamer1_DELAY();
    i=75;
    while(i--&& !fTurn){
      DELAY_1ms();
    }
  }
}

// 流光2
void Streamer_SEG2_ON(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;

  for (uint8_t j = 0; j < SEG2_LED_NUM; j++) {
    // SEG1
    LED_SET(SEG1_LED_NUM, 0x00); // SEG1 LED OFF
    // SEG2
    num += 1;
    LED_SET(num, data);                // LED ON
    LED_SET(SEG2_LED_NUM - num, 0x00); // LED OFF
    // Streamer2_DELAY();                // for GM666
    i=75;
    while(i--&& !fTurn){
      DELAY_1ms();
    }
  }
}

void Streamer_SEG2_TEST(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;

  for (uint8_t i = 0; i < SEG2_LED_NUM; i++) {
    // SEG1
    LED_SET(SEG1_LED_NUM, data); // SEG1 LED OFF
    // SEG2
    num += 1;
    LED_SET(num, data);                // LED ON
    LED_SET(SEG2_LED_NUM - num, 0x00); // LED OFF
    Streamer2_DELAY_TEST();                // for GM666
  }
}

//迎賓燈 760ms
void WelcomeLight(uint8_t InputData){
  fWelcome=0;
  // 黃光
  YL_en = 1;
  WL_en = 0;

  // LED_OFF();
  DELAY_2ms();
  // 流光1
  if(!fTurn){
    Streamer_SEG1_ON(PWM_DUTY);
  }
  else{
    YL_en = 0;
  }
  //DELAY_250ms();
  i=250;
  while(i--&& !fTurn){
    DELAY_1ms();
  }
  if(!fTurn){
    Streamer_SEG1_OFF(PWM_DUTY);
  }else{
    YL_en = 0;
  }
  // DELAY_250ms();
  i=250;
  while(i--&& !fTurn){
    DELAY_1ms();
  }
  if(!fTurn){
    Streamer_SEG2_ON(PWM_DUTY);
  }else{
    YL_en = 0;
  }
  // DELAY_250ms();
  i=250;
  while(i--&& !fTurn){
    DELAY_1ms();
  }

  LED_OFF();
  DELAY_2ms();
  
  if(!fTurn){
    LED_SET_Gradually_ON(PWM_DUTY);
  }else{
    YL_en = 0;
  }
  LED_OFF();
  DELAY_2ms();
  
  // 白光
  if(!fTurn){
    YL_en = 0;
    WL_en = 1;
    LED_ON(PWM_DUTY);
  }else{
    YL_en = 0;
    WL_en = 0;
  }
  // DELAY_1000ms(); //15秒
  i=15000;
  while(i--&& !fTurn){
    DELAY_1ms();
  }

  LED_OFF();
  DELAY_1ms();
  T1MS_disable_cnt=600;
}

//方向燈 TrunLight 350ms ON/350ms OFF
void TrunLight(uint8_t InputData) {
  // uint16_t i=0;
  uint8_t j=0;
  uint8_t data = InputData;
  uint8_t seg1_num = 0;
  uint8_t seg2_num = 1;
  // fColorSeletDisable = 1;
  // 黃光
  YL_en = 1;
  WL_en = 0;

  LED_OFF();
  DELAY_1ms();

  //SETP1
  if(fTurn==1){
    LED_SET(seg1_num, data);                // SEG1 LED ON
    LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
    LED_SET(seg2_num, data);                // SEG2 LED ON
    LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF
    i=40;
    while(i-->0 && fTurn==1){
      DELAY_1ms();
    }
  }

  //STEP2-5
  if(fTurn==1){
    for (j = 0; j < SEG2_LED_NUM - 2; j++) {
      seg1_num += 1;
      seg2_num += 1;
      LED_SET(seg1_num, data);                // SEG1 LED ON
      LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
      LED_SET(seg2_num, data);                // SEG2 LED ON
      LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF
      i=40;
      while(i-->0 && fTurn==1){
        DELAY_1ms();
      }
    }
  }
  
  //STEP6
  if(fTurn==1){
    seg1_num = 12;
    seg2_num = 6;
    LED_SET(seg1_num, data);                // SEG1 LED ON
    LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
    LED_SET(seg2_num, data);                // SEG2 LED ON
    LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF
    i=400;
    while(i-->1 && fTurn==1){
      DELAY_1ms();
    }
  }
  if(i==0) fWelcome=1;
  //等待迎賓信號結束
  while(fTurn==1){
    DELAY_1ms();
  }
  LED_OFF();
  DELAY_1ms();
  T1MS_disable_cnt = 600;
}

void DRL_Light(void){
  // check switch
  if (ColorSelect == COLOR_YELLOW) {
    // 黃光
    YL_en = 1;
    WL_en = 0;
  } else {
    // 白光
    YL_en = 0;
    WL_en = 1;
  }

  DELAY_2ms();
  LED_ON(PWM_DUTY);
}

void POS_Light(void){
  // check switch
  if (ColorSelect == COLOR_YELLOW) {
    // 黃光
    YL_en = 1;
    WL_en = 0;
  } else {
    // 白光
    YL_en = 0;
    WL_en = 1;
  }

  DELAY_2ms();
  LED_ON(PWM_DUTY_10);
}