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


// TODO Insert appropriate #include <>
#include <stdint.h>
// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations
extern void PWM_SendData(uint8_t InputData);
extern void LED_SET(uint8_t LED_num,uint8_t InputData );

extern void LED_ON(uint8_t InputData);
extern void LED_OFF(void);
extern void Streamer_SEG1_ON(uint8_t InputData);
extern void Streamer_SEG1_TEST(uint8_t InputData);
extern void Streamer_SEG1_OFF(uint8_t InputData);
extern void Streamer_SEG2_ON(uint8_t InputData);
extern void Streamer_SEG2_TEST(uint8_t InputData);
extern void LED_SET_Gradually_ON(uint8_t InputData);
extern void WelcomeLight(uint8_t InputData);
extern void TrunLight(uint8_t InputData);


// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PWM_H */

