/*
 * File:   PWM.c
 * Author: T00904
 ws2811 LED control by PWM 200k(5us)
 ws2811 datasheet Send data at speed of 800Kbps
 此操作不符合WS2811的時序要求，但實際測試中WS2811仍能正常工作，可能是因為WS2811對於數據的接收有一定的容錯能力。
 每2.75ms刷新一次，刷新時間約750us，剩餘時間約2ms可用於其他操作。
 每次送出18顆LED的數據。每個bit 5us
 data 0: 375ns(400ns) high + 5us(4.64us) low=total 5.04us
 data 1: 750ns(720ns) high + 5us(4.32us) low=total 5.04us
 ws2811 datasheet T0H 220~380ns  T0L 580~1000ns
 ws2811 datasheet T1H 580~1000ns T1L 580~1000ns
 * Created on August 26, 2025, 2:27 PM
 */

#include <xc.h>
#include "PWM.h"

void PWM_SendData(uint8_t InputData);
void LED_SET(uint8_t LED_num, uint8_t InputData);

// 382ns
#define DATA_0() CCPR5L = 3
// 758ns
#define DATA_1() CCPR5L = 6
// PWM OFF
#define PWM_STOP() CCPR5L = 0
#define TIMER_FLAG PIR3bits.TMR6IF
// #define WS2811_NUM 18

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
