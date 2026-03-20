/*
 * File:   PWM.h
 * Author: T00904
 *
 * Created on August 26, 2025, 2:27 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#include <stdint.h>

// TODO Insert appropriate #include <>
extern void PWM_SendData(uint8_t InputData);
extern void LED_SET(uint8_t LED_num,uint8_t InputData );

//duty 100%
#define PWM_DUTY_100 255
//duty 10%
#define PWM_DUTY_10 25

#endif	/* PWM_H */

