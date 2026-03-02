/* 
 * File:   func_ws2811.h
 * Author: T00904
 *
 * Created on 2026年2月24日, 上午 9:15
 */

#ifndef FUNC_WS2812_H
#define	FUNC_WS2812_H

#include <stdint.h>
#include <stdbool.h>
#include "PWM.h"

extern void LED_ON(uint8_t InputData);
extern void LED_OFF(void);
extern void Color_OFF(void);
extern void Streamer_SEG1_ON(uint8_t InputData);
extern void Streamer_SEG1_OFF(uint8_t InputData);
extern void Streamer_SEG2_ON(uint8_t InputData);
// extern void Streamer_SEG1_TEST(uint8_t InputData);
// extern void Streamer_SEG2_TEST(uint8_t InputData);
extern void LED_SET_Gradually_ON(uint8_t InputData);
extern void WelcomeLight_ON(uint8_t InputData);
extern void TrunLight(uint8_t InputData);
extern void DRL_ON(void);
extern void POS_ON(void);

#endif	/* FUNC_WS2812_H */

