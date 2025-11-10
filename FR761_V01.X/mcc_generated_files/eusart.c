/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.1.1
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

/**
  Section: Included Files
*/
#include "eusart.h"

/**
  Section: Macro Declarations
*/

#define EUSART_TX_BUFFER_SIZE 8
#define EUSART_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/
//傳輸緩衝區頭
volatile uint8_t eusartTxHead = 0;
//傳輸緩衝區尾巴
volatile uint8_t eusartTxTail = 0;
//傳輸緩衝區
volatile uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
//傳輸緩衝區剩餘空間
volatile uint8_t eusartTxBufferRemaining;
//接收緩衝區頭
volatile uint8_t eusartRxHead = 0;
//接收緩衝區尾巴
volatile uint8_t eusartRxTail = 0;
//接收緩衝區
volatile uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
//狀態記錄陣列
volatile eusart_status_t eusartRxStatusBuffer[EUSART_RX_BUFFER_SIZE];
//接收緩衝區計數
volatile uint8_t eusartRxCount;
//最後一次接收錯誤狀態
volatile eusart_status_t eusartRxLastError;

/**
  Section: EUSART APIs
*/
void (*EUSART_TxDefaultInterruptHandler)(void);
void (*EUSART_RxDefaultInterruptHandler)(void);

void (*EUSART_FramingErrorHandler)(void);
void (*EUSART_OverrunErrorHandler)(void);
void (*EUSART_ErrorHandler)(void);

void EUSART_DefaultFramingErrorHandler(void);
void EUSART_DefaultOverrunErrorHandler(void);
void EUSART_DefaultErrorHandler(void);

// EUART初始化
void EUSART_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RCIE = 0;
    EUSART_SetRxInterruptHandler(EUSART_Receive_ISR);
    PIE1bits.TXIE = 0;
    EUSART_SetTxInterruptHandler(EUSART_Transmit_ISR);
    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUDCON = 0x18;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN disabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TXSTA = 0x04;

    // SPBRGL 255; 
    SPBRGL = 0xFF;

    // SPBRGH 2; 
    SPBRGH = 0x02;


    EUSART_SetFramingErrorHandler(EUSART_DefaultFramingErrorHandler);
    EUSART_SetOverrunErrorHandler(EUSART_DefaultOverrunErrorHandler);
    EUSART_SetErrorHandler(EUSART_DefaultErrorHandler);

    eusartRxLastError.status = 0;

    // initializing the driver state
    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;

    // enable receive interrupt
    PIE1bits.RCIE = 1;
}

bool EUSART_is_tx_ready(void)
{
    return (eusartTxBufferRemaining ? true : false);
}

bool EUSART_is_rx_ready(void)
{
    return (eusartRxCount ? true : false);
}

bool EUSART_is_tx_done(void)
{
    return TXSTAbits.TRMT;
}

// 得到最後一次接收的錯誤狀態
eusart_status_t EUSART_get_last_status(void){
    return eusartRxLastError;
}

// 讀取一個字元
uint8_t EUSART_Read(void)
{
    uint8_t readValue  = 0;
    //等待有資料可讀
    while(0 == eusartRxCount)
    {
    }
    //最後一次接收錯誤狀態
    eusartRxLastError = eusartRxStatusBuffer[eusartRxTail];
    //從緩衝區讀取資料
    readValue = eusartRxBuffer[eusartRxTail++];
    //尾巴到達緩衝區末端，回到開頭
    if(sizeof(eusartRxBuffer) <= eusartRxTail)
    {
        eusartRxTail = 0;
    }
    //關閉接收中斷，更新計數，再開啟接收中斷
    PIE1bits.RCIE = 0;
    eusartRxCount--;
    PIE1bits.RCIE = 1;

    return readValue;
}

// 寫入一個字元
void EUSART_Write(uint8_t txData)
{
    //等待有空間可寫，0表示沒有空間
    while(0 == eusartTxBufferRemaining)
    {
    }

    //如果傳輸中斷沒啟用，直接寫入TXREG
    if(0 == PIE1bits.TXIE)
    {
        TXREG = txData;
    }
    else
    {   //關閉傳輸中斷，寫入緩衝區，更新頭尾和剩餘空間，再開啟傳輸中斷
        PIE1bits.TXIE = 0;
        eusartTxBuffer[eusartTxHead++] = txData;
        if(sizeof(eusartTxBuffer) <= eusartTxHead)
        {
            eusartTxHead = 0;
        }
        eusartTxBufferRemaining--;
    }
    PIE1bits.TXIE = 1;
}

//UART傳送中斷服務程式
void EUSART_Transmit_ISR(void)
{
    // add your EUSART interrupt custom code

    //計算緩衝區是否有資料可傳，剩餘空間小於緩衝區大小表示有資料可傳
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining)
    {
        TXREG = eusartTxBuffer[eusartTxTail++];
        //尾巴到達緩衝區末端，回到開頭
        if(sizeof(eusartTxBuffer) <= eusartTxTail)
        {
            eusartTxTail = 0;
        }
        eusartTxBufferRemaining++;
    }
    else
    {
        PIE1bits.TXIE = 0;
    }
}

//UART接收中斷服務程式
void EUSART_Receive_ISR(void)
{
    eusartRxStatusBuffer[eusartRxHead].status = 0;

    //接收訊框錯誤，break信號 不做處理
    if(RCSTAbits.FERR){
        eusartRxStatusBuffer[eusartRxHead].ferr = 1;
        EUSART_FramingErrorHandler();
    }

    //接收溢位錯誤，EUART重置
    if(RCSTAbits.OERR){
        eusartRxStatusBuffer[eusartRxHead].oerr = 1;
        EUSART_OverrunErrorHandler();
    }
    
    //其他錯誤，不做處理
    if(eusartRxStatusBuffer[eusartRxHead].status){
        EUSART_ErrorHandler();
    } else {
        // 沒有錯誤，正常接收資料
        EUSART_RxDataHandler();
    }
    
    // or set custom function using EUSART_SetRxInterruptHandler()
}

// UART接收資料處理
void EUSART_RxDataHandler(void){
    // use this default receive interrupt handler code

    //從RCREG讀取資料放入緩衝區
    eusartRxBuffer[eusartRxHead++] = RCREG;
    //頭到達緩衝區末端，回到開頭
    if(sizeof(eusartRxBuffer) <= eusartRxHead)
    {
        eusartRxHead = 0;
    }
    //更新接收計數器
    eusartRxCount++;
}

void EUSART_DefaultFramingErrorHandler(void){}

void EUSART_DefaultOverrunErrorHandler(void){
    // EUSART error - restart

    RCSTAbits.CREN = 0; //Disables receiver
    RCSTAbits.CREN = 1; //Enables receiver

}

void EUSART_DefaultErrorHandler(void){
    EUSART_RxDataHandler();
}

void EUSART_SetFramingErrorHandler(void (* interruptHandler)(void)){
    EUSART_FramingErrorHandler = interruptHandler;
}

void EUSART_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    EUSART_OverrunErrorHandler = interruptHandler;
}

void EUSART_SetErrorHandler(void (* interruptHandler)(void)){
    EUSART_ErrorHandler = interruptHandler;
}

void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_TxDefaultInterruptHandler = interruptHandler;
}

void EUSART_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_RxDefaultInterruptHandler = interruptHandler;
}
/**
  End of File
*/
