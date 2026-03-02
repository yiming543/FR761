/*
 * File:   func_ws2811.c
 * Author: T00904
 *
 * Created on 2026年2月24日, 上午 9:14
 */

#include "PWM.h"
#include "gpio.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#define WS2811_SEG1_NUM 4
#define SEG1_LED_NUM (WS2811_SEG1_NUM * 3)
#define WS2811_SEG2_NUM 2
#define SEG2_LED_NUM (WS2811_SEG2_NUM * 3)
#define TOTAL_WS2811_NUM (WS2811_SEG1_NUM + WS2811_SEG2_NUM)

#define TOTAL_LED_NUM (SEG1_LED_NUM + SEG2_LED_NUM)


#define TURN_STEP_TIME (40)      // 方向燈流光時間 40ms (total 200ms)
#define TURN_STEP_TIME2 (400)    // 200+400ms(迎賓燈成立時間)
#define WELCOME_TIME (600)       // 迎賓燈成立時間 600ms
#define GRADUALLY_TIME (12)      // 呼吸燈時間 12ms
#define DELAY_250MS (250)        // 延遲250ms
#define DELAY_15_SECOND (15000U) // 延遲15秒

// all led off
void LED_OFF(void) {
  // DELAY_2ms();
  LED_SET(TOTAL_LED_NUM, 0x00);
}

// all led on
void LED_ON(uint8_t InputData) {
  uint8_t data = InputData;
  LED_SET(TOTAL_LED_NUM, data);
}



// 流光1
// Streamer LED ON 500us(753us)
void Streamer_SEG1_ON(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;
  // uint8_t i = 0;

  for (uint8_t j = 0; j < SEG1_LED_NUM; j++) {
    num += 1;
    // SEG1
    LED_SET(num, data);                // LED ON
    LED_SET(SEG1_LED_NUM - num, 0x00); // LED OFF
    // SEG2 OFF
    LED_SET(SEG2_LED_NUM, 0x00); // LED OFF
    // Streamer1_DELAY();
    // i = 75;
    T1MS_cnt = 0;
    while (T1MS_cnt < 75) {
      // DELAY_1ms();
      Timer();
      if (ORB.fTurn) {
        Color_OFF();
        break;
      }
    }
  }
}

// test 似乎不需要使用了
//  void Streamer_SEG1_TEST(uint8_t InputData) {
//    uint8_t data = InputData;
//    uint8_t num = 0;

//   for (uint8_t i = 0; i < SEG1_LED_NUM; i++) {
//     num += 1;
//     // SEG1
//     LED_SET(num, data);                // LED ON
//     LED_SET(SEG1_LED_NUM - num, 0x00); // LED OFF
//     // SEG2 OFF
//     LED_SET(SEG2_LED_NUM, 0x00); // LED OFF
//     Streamer1_DELAY_TEST();
//   }
// }

// Streamer LED OFF
void Streamer_SEG1_OFF(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;
  // uint8_t i = 0;

  for (uint8_t j = 0; j < SEG1_LED_NUM; j++) {
    num++;
    // SEG1
    LED_SET(SEG1_LED_NUM - num, data); // LED ON
    LED_SET(num, 0x00);                // LED OFF
    // SEG2 OFF
    LED_SET(SEG2_LED_NUM, 0x00); // LED OFF

    // Streamer1_DELAY();
    T1MS_cnt = 0;
    while (T1MS_cnt < 75) {
      // DELAY_1ms();
      Timer();
      if (ORB.fTurn) {
        Color_OFF();
        break;
      }
    }
  }
}

// 流光2
void Streamer_SEG2_ON(uint8_t InputData) {
  uint8_t data = InputData;
  uint8_t num = 0;
  // uint8_t i = 0;

  for (uint8_t j = 0; j < SEG2_LED_NUM; j++) {
    // SEG1
    LED_SET(SEG1_LED_NUM, 0x00); // SEG1 LED OFF
    // SEG2
    num++;
    LED_SET(num, data);                // LED ON
    LED_SET(SEG2_LED_NUM - num, 0x00); // LED OFF

    // Streamer2_DELAY();                // for GM666
    T1MS_cnt = 0;
    while (T1MS_cnt < 75) {
      // DELAY_1ms();
      Timer();
      if (ORB.fTurn) {
        Color_OFF();
        break;
      }
    }
  }
}

// Gradually brighten and fade out
// 呼吸燈--逐漸變亮
void LED_SET_Gradually_ON(uint8_t InputData) {
  uint8_t data = InputData;

  for (uint8_t j = 1; j < 255; j++) {
    if (ORB.fTurn) {
      Color_OFF();
      break;
    }
    LED_SET(TOTAL_LED_NUM, j);

    T1MS_cnt = 0;
    while (T1MS_cnt < GRADUALLY_TIME) { // 12ms
      Timer();
    }
  }
}

// test 似乎不需要使用了
//  void Streamer_SEG2_TEST(uint8_t InputData) {
//    uint8_t data = InputData;
//    uint8_t num = 0;

//   for (uint8_t i = 0; i < SEG2_LED_NUM; i++) {
//     // SEG1
//     LED_SET(SEG1_LED_NUM, data); // SEG1 LED OFF
//     // SEG2
//     num += 1;
//     LED_SET(num, data);                // LED ON
//     LED_SET(SEG2_LED_NUM - num, 0x00); // LED OFF
//     Streamer2_DELAY_TEST();                // for GM666
//   }
// }

// 迎賓燈 760ms
void WelcomeLight_ON(uint8_t InputData) {
  uint16_t i = 0;
  flag1.fWelcome = 0;
  // 迎賓燈時間重置
  T1MS_welcome_cnt = 0;
  // 黃光
  Color_Yellow();

  // 流光1
  if (!ORB.fTurn) {
    Streamer_SEG1_ON(PWM_DUTY_100);
  } else {
    Color_OFF();
  }

  T1MS_cnt = 0;
  while (T1MS_cnt < DELAY_250MS) {
    Timer();
    if (ORB.fTurn) {
      Color_OFF();
      break;
    }
  }

  if (!ORB.fTurn) {
    Streamer_SEG1_OFF(PWM_DUTY_100);
  } else {
    Color_OFF();
  }

  T1MS_cnt = 0;
  while (T1MS_cnt < DELAY_250MS) {
    Timer();
    if (ORB.fTurn) {
      Color_OFF();
      break;
    }
  }

  if (!ORB.fTurn) {
    Streamer_SEG2_ON(PWM_DUTY_100);
  } else {
    Color_OFF();
  }

  T1MS_cnt = 0;
  while (T1MS_cnt < DELAY_250MS) {
    Timer();
    if (ORB.fTurn) {
      Color_OFF();
      break;
    }
  }

  if (!ORB.fTurn) {
    LED_SET_Gradually_ON(PWM_DUTY_100);
  } else {
    Color_OFF();
  }

  Color_OFF();

  // 白光
  if (!ORB.fTurn) {
    Color_White();
    LED_ON(PWM_DUTY_100);
  } else {
    Color_OFF();
  }

  // 迎賓模式最多持續15秒，期間有TURN/DRL/POS信號就離開迎賓模式
  while (T1MS_welcome_cnt < DELAY_15_SECOND) {
    Timer();
    if (ORB.fTurn || ORB.fDRL || ORB.fPOS) {
      Color_OFF();
      break;
    }
  }

  Color_OFF();
  T1MS_disable_cnt = 600;
}

// 方向燈 TrunLight 350ms ON/350ms OFF
void TrunLight(uint8_t InputData) {
  uint8_t data = InputData;
  // SEG1 不亮
  // SEG2 亮第一段
  uint8_t seg1_num = 0;
  uint8_t seg2_num = 1;

  // 迎賓燈成立時間重置
  T1MS_welcome_cnt = 0;
  // 黃光
  Color_Yellow();

  // SETP1
  if (ORB.fTurn) {
    LED_SET(seg1_num, data);                // SEG1 LED ON
    LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
    LED_SET(seg2_num, data);                // SEG2 LED ON
    LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF

    T1MS_cnt = 0;
    while (T1MS_cnt < TURN_STEP_TIME) { // 40ms
      // DELAY_1ms();
      Timer();
      if (!ORB.fTurn) {
        Color_OFF();
        break;
      }
    }
  }

  // STEP2-5
  if (ORB.fTurn) {
    for (uint8_t j = 0; j < SEG2_LED_NUM - 2; j++) {
      seg1_num++;
      seg2_num++;
      LED_SET(seg1_num, data);                // SEG1 LED ON
      LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
      LED_SET(seg2_num, data);                // SEG2 LED ON
      LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF

      T1MS_cnt = 0;
      while (T1MS_cnt < TURN_STEP_TIME) { // 40ms
        // DELAY_1ms();
        Timer();
        if (!ORB.fTurn) {
          Color_OFF();
          break;
        }
      }
    }
  }

  // STEP6
  if (ORB.fTurn) {
    // SEG1/SEG2 全亮
    seg1_num = 12;
    seg2_num = 6;
    LED_SET(seg1_num, data);                // SEG1 LED ON
    LED_SET(SEG1_LED_NUM - seg1_num, 0x00); // LED OFF
    LED_SET(seg2_num, data);                // SEG2 LED ON
    LED_SET(SEG2_LED_NUM - seg2_num, 0x00); // LED OFF
    T1MS_cnt = 0;
    while (T1MS_cnt < TURN_STEP_TIME2) { // 400ms
      Timer();
      if (!ORB.fTurn) {
        Color_OFF();
        break;
      }
    }
  }

  if (T1MS_welcome_cnt >= WELCOME_TIME) { // 600ms
    flag1.fWelcome = 1;
    // 等待方向燈信號結束
    while (1) {
      Timer();
      if (!ORB.fTurn) {
        Color_OFF();
        T1MS_disable_cnt = 600;
        break;
      }
    }
  }
}

void DRL_ON(void) {
  Color_set();
  LED_ON(PWM_DUTY_100);
}

void POS_ON(void) {
  Color_set();
  LED_ON(PWM_DUTY_10);
}
