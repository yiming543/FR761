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

#include <stdbool.h>
#include <stdint.h>

extern void Timer(void);

extern uint16_t T1MS_cnt;
extern uint16_t T1MS_disable_cnt;
extern uint16_t T1MS_welcome_cnt;

//testmode
// extern uint8_t mode;

// define
typedef enum {
    eTurn = 0,
    ePOS = 1,
    eDRL = 2,
    eACC = 3,
    eLoBeam = 4,
    eHiBeam = 5,
}ORB_ORDER_t;

typedef union {
    struct {
        unsigned fTurn : 1;
        unsigned fPOS : 1;
        unsigned fDRL : 1;
        unsigned fACC : 1;
        unsigned fLoBeam : 1;
        unsigned fHiBeam : 1;
        unsigned reserved : 2; // 保留位，填充到8位
    };
    uint8_t all_flags; // 可以用來一次性訪問所有旗標
}ORB_t;

typedef union {
    struct {
        unsigned fWelcome : 1;
        unsigned fLight_en : 1;
        unsigned reserved : 6; // 保留位，填充到8位
    };
    uint8_t all_flags; // 可以用來一次性訪問所有旗標
}flag_t;

extern ORB_t ORB;
extern flag_t flag1;

#endif	/* TIMER_H */

