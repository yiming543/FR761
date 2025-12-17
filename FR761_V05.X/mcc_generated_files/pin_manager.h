/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set FAN_EN aliases
#define FAN_EN_TRIS                 TRISAbits.TRISA0
#define FAN_EN_LAT                  LATAbits.LATA0
#define FAN_EN_PORT                 PORTAbits.RA0
#define FAN_EN_ANS                  ANSELAbits.ANSA0
#define FAN_EN_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define FAN_EN_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define FAN_EN_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define FAN_EN_GetValue()           PORTAbits.RA0
#define FAN_EN_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define FAN_EN_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define FAN_EN_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define FAN_EN_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set LoBeam_IN aliases
#define LoBeam_IN_TRIS                 TRISAbits.TRISA3
#define LoBeam_IN_LAT                  LATAbits.LATA3
#define LoBeam_IN_PORT                 PORTAbits.RA3
#define LoBeam_IN_ANS                  ANSELAbits.ANSA3
#define LoBeam_IN_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define LoBeam_IN_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define LoBeam_IN_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define LoBeam_IN_GetValue()           PORTAbits.RA3
#define LoBeam_IN_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define LoBeam_IN_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define LoBeam_IN_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define LoBeam_IN_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set RA4 procedures
#define RA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define RA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define RA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define RA4_GetValue()              PORTAbits.RA4
#define RA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define RA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define RA4_SetAnalogMode()         do { ANSELAbits.ANSA4 = 1; } while(0)
#define RA4_SetDigitalMode()        do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set DRL_IN aliases
#define DRL_IN_TRIS                 TRISAbits.TRISA6
#define DRL_IN_LAT                  LATAbits.LATA6
#define DRL_IN_PORT                 PORTAbits.RA6
#define DRL_IN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define DRL_IN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define DRL_IN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define DRL_IN_GetValue()           PORTAbits.RA6
#define DRL_IN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define DRL_IN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set LoBeam_en aliases
#define LoBeam_en_TRIS                 TRISBbits.TRISB1
#define LoBeam_en_LAT                  LATBbits.LATB1
#define LoBeam_en_PORT                 PORTBbits.RB1
#define LoBeam_en_WPU                  WPUBbits.WPUB1
#define LoBeam_en_ANS                  ANSELBbits.ANSB1
#define LoBeam_en_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define LoBeam_en_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define LoBeam_en_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define LoBeam_en_GetValue()           PORTBbits.RB1
#define LoBeam_en_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define LoBeam_en_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define LoBeam_en_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define LoBeam_en_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define LoBeam_en_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define LoBeam_en_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set SW_IN aliases
#define SW_IN_TRIS                 TRISBbits.TRISB2
#define SW_IN_LAT                  LATBbits.LATB2
#define SW_IN_PORT                 PORTBbits.RB2
#define SW_IN_WPU                  WPUBbits.WPUB2
#define SW_IN_ANS                  ANSELBbits.ANSB2
#define SW_IN_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define SW_IN_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define SW_IN_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define SW_IN_GetValue()           PORTBbits.RB2
#define SW_IN_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define SW_IN_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define SW_IN_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define SW_IN_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define SW_IN_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define SW_IN_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set HiBeam_en1 aliases
#define HiBeam_en1_TRIS                 TRISBbits.TRISB4
#define HiBeam_en1_LAT                  LATBbits.LATB4
#define HiBeam_en1_PORT                 PORTBbits.RB4
#define HiBeam_en1_WPU                  WPUBbits.WPUB4
#define HiBeam_en1_ANS                  ANSELBbits.ANSB4
#define HiBeam_en1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define HiBeam_en1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define HiBeam_en1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define HiBeam_en1_GetValue()           PORTBbits.RB4
#define HiBeam_en1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define HiBeam_en1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define HiBeam_en1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define HiBeam_en1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define HiBeam_en1_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define HiBeam_en1_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set HiBeam_en2 aliases
#define HiBeam_en2_TRIS                 TRISBbits.TRISB5
#define HiBeam_en2_LAT                  LATBbits.LATB5
#define HiBeam_en2_PORT                 PORTBbits.RB5
#define HiBeam_en2_WPU                  WPUBbits.WPUB5
#define HiBeam_en2_ANS                  ANSELBbits.ANSB5
#define HiBeam_en2_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define HiBeam_en2_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define HiBeam_en2_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define HiBeam_en2_GetValue()           PORTBbits.RB5
#define HiBeam_en2_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define HiBeam_en2_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define HiBeam_en2_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define HiBeam_en2_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define HiBeam_en2_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define HiBeam_en2_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set ACC_IN aliases
#define ACC_IN_TRIS                 TRISCbits.TRISC0
#define ACC_IN_LAT                  LATCbits.LATC0
#define ACC_IN_PORT                 PORTCbits.RC0
#define ACC_IN_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define ACC_IN_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define ACC_IN_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define ACC_IN_GetValue()           PORTCbits.RC0
#define ACC_IN_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define ACC_IN_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

// get/set POS_IN aliases
#define POS_IN_TRIS                 TRISCbits.TRISC1
#define POS_IN_LAT                  LATCbits.LATC1
#define POS_IN_PORT                 PORTCbits.RC1
#define POS_IN_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define POS_IN_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define POS_IN_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define POS_IN_GetValue()           PORTCbits.RC1
#define POS_IN_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define POS_IN_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)

// get/set TURN_IN aliases
#define TURN_IN_TRIS                 TRISCbits.TRISC2
#define TURN_IN_LAT                  LATCbits.LATC2
#define TURN_IN_PORT                 PORTCbits.RC2
#define TURN_IN_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define TURN_IN_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define TURN_IN_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define TURN_IN_GetValue()           PORTCbits.RC2
#define TURN_IN_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define TURN_IN_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)

// get/set HiBeam_IN aliases
#define HiBeam_IN_TRIS                 TRISCbits.TRISC3
#define HiBeam_IN_LAT                  LATCbits.LATC3
#define HiBeam_IN_PORT                 PORTCbits.RC3
#define HiBeam_IN_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define HiBeam_IN_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define HiBeam_IN_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define HiBeam_IN_GetValue()           PORTCbits.RC3
#define HiBeam_IN_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define HiBeam_IN_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)

// get/set WL_en aliases
#define WL_en_TRIS                 TRISCbits.TRISC4
#define WL_en_LAT                  LATCbits.LATC4
#define WL_en_PORT                 PORTCbits.RC4
#define WL_en_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define WL_en_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define WL_en_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define WL_en_GetValue()           PORTCbits.RC4
#define WL_en_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define WL_en_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set YL_en aliases
#define YL_en_TRIS                 TRISCbits.TRISC5
#define YL_en_LAT                  LATCbits.LATC5
#define YL_en_PORT                 PORTCbits.RC5
#define YL_en_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define YL_en_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define YL_en_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define YL_en_GetValue()           PORTCbits.RC5
#define YL_en_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define YL_en_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/