/* 
 * File:   gpio.h
 * Author: T00904
 *
 * Created on 2026年2月23日, 下午 3:19
 */

#ifndef GPIO_H
#define	GPIO_H

// input
#define POS_IN POS_IN_PORT
#define DRL_IN DRL_IN_PORT
#define Turn_IN TURN_IN_PORT
#define LoBeam_IN LoBeam_IN_PORT
#define HiBeam_IN HiBeam_IN_PORT
#define ACC_IN ACC_IN_PORT
#define ColorSelect SW_IN_PORT

//test 之後改回gpio.c
// output
#define FAN_en FAN_EN_LAT
#define LoBeam_en LoBeam_en_LAT
#define HiBeam_en1 HiBeam_en1_LAT
#define HiBeam_en2 HiBeam_en2_LAT


extern void LoBeam_ON(void);
extern void LoBeam_OFF(void);
extern void HiBeam_ON(void);
extern void HiBeam_OFF(void);
extern void Color_set(void);
extern void Color_Yellow(void);
extern void Color_White(void);
extern void Color_OFF(void);

#endif	/* GPIO_H */

