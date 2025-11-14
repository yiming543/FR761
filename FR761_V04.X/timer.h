/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   timer.c
 * Author: T00904
 * Comments: 20251029
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

#include <pic.h>
#include <stdbool.h>
#include "mcc_generated_files/mcc.h"


extern void Timer(void);
extern void Timer_TESTMODE(void);
extern void Delay_ms(uint16_t input_ms);
extern void CheckGPIO(void);

extern bool fTurn;
extern bool fPOS;
extern bool fDRL;
extern bool fWelcome;
extern uint8_t T250MS_cnt;
extern uint16_t T1MS_disable_cnt;
//testmode
extern uint8_t mode;

// define
//main delay
// #define PWM_DUTY 250
#define PWM_DUTY 255
#define PWM_DUTY_10 25
#define DELAY_1ms() Delay_ms(1)
#define DELAY_2ms() Delay_ms(2)
#define DELAY_5ms() Delay_ms(5)
#define DELAY_50ms() Delay_ms(50)
#define DELAY_100ms() Delay_ms(100)
#define DELAY_250ms() Delay_ms(250)
#define DELAY_500ms() Delay_ms(500)
#define DELAY_1000ms() Delay_ms(1000)

#define DELAY5() Delay_ms(166)
#define DELAY6() Delay_ms(166)
// #define DELAY5() Delay_ms(10)
// #define DELAY2() Delay_ms(100)

// output
#define YL_en YL_en_LAT
#define WL_en WL_en_LAT
// #define Fan_en FAN_en_LAT
#define LoBeam_en LoBeam_en_LAT
#define HiBeam_en1 HiBeam_en1_LAT
#define HiBeam_en2 HiBeam_en2_LAT


// input
#define ColorSelect SW_IN_PORT
#define LoBeam_IN LoBeam_IN_PORT
#define ACC_IN ACC_IN_PORT
#define Turn_IN TURN_IN_PORT
#define POS_IN POS1_IN_PORT
#define DRL_IN DRL_IN_PORT
#define HiBeam_IN HiBeam_IN_PORT

// #define LampSideSelect() Lamp_R_EN_PORT
// #define LAMP_R_SIDE 0
// #define LAMP_L_SIDE 1



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

#endif	/* TIMER_H */

