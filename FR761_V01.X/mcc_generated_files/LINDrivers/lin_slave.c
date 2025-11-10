/**
  LIN Slave Driver

  Company:
    Microchip Technology Inc.

  File Name:
    lin_slave.c

  Summary:
    LIN Slave Driver

  Description:
    This source file provides the driver for LIN slave nodes

 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "lin_slave.h"
#include "../eusart.h"
#include "../tmr0.h"
#include <string.h>

#define READ_TIMEOUT 20 // ms

// 加static變數避免外部存取(無法使用extern存取)
static void (*LIN_processData)(void);

lin_packet_t LIN_packet;           // LIN封包變數
bool LIN_rxInProgress = false;     // 1:LIN接收進行中旗標
const lin_rx_cmd_t *LIN_rxCommand; // 指向LIN接收命令表的指標
uint8_t LIN_rxCommandLength;       // LIN接收命令表長度

// 加static變數避免外部存取(無法使用extern存取)
static uint8_t LIN_timeout = 15;           // TODO: Make dependent on Baudrate
static bool LIN_timerRunning = false;      // 1:計時器運行中
static volatile uint8_t CountCallBack = 0; // 計時器 1ms回呼計數器

// 準備LIN接收所需要的變數和函式
void LIN_init(uint8_t tableLength, const lin_rx_cmd_t *const command,
              void (*processData)(void)) {
  LIN_rxCommand = command;
  LIN_rxCommandLength = tableLength;
  LIN_processData = processData;
  LIN_stopTimer();
  LIN_enableRx();
  LIN_setTimerHandler();
}

// 將LIN要傳送的資料放到封包中，並傳送出去
void LIN_queuePacket(uint8_t cmd) {
  const lin_rx_cmd_t *tempSchedule =
      LIN_rxCommand; // copy table pointer so we can modify it

  cmd &= 0x3F; // clear possible parity bits
  for (uint8_t i = 0; i < LIN_rxCommandLength; i++) {
    if (cmd == tempSchedule->cmd) {
      break;
    }
    tempSchedule++; // go to next entry
  }

  LIN_packet.type = tempSchedule->type;
  LIN_packet.length = tempSchedule->length;

  // Build Packet - User defined data
  // add data
  memcpy(LIN_packet.data, tempSchedule->data, LIN_packet.length);

  // Add Checksum
  LIN_packet.checksum =
      LIN_getChecksum(LIN_packet.length, LIN_packet.PID, LIN_packet.data);
  LIN_sendPacket(LIN_packet.length, LIN_packet.PID, LIN_packet.data);
}

// LIN狀態機
lin_rx_state_t LIN_handler(void) {
  // 加static 資料不會隨著函式執行結束而消失
  static lin_rx_state_t LIN_rxState = LIN_RX_IDLE;
  static uint8_t rxDataIndex = 0;

  // 只要接收時間超時就變成error state
  if (LIN_rxInProgress == true) {
    if (LIN_timerRunning == false) {
      // Timeout
      LIN_rxState = LIN_RX_ERROR;
    }
  }

  switch (LIN_rxState) {
  case LIN_RX_IDLE:
    // buffer裡有資料，進入下一個state
    if (EUSART_is_rx_ready() > 0) {
      // 開始計時
      LIN_startTimer(READ_TIMEOUT);
      LIN_rxInProgress = true;
      LIN_rxState = LIN_RX_BREAK;
    }
    break;
  case LIN_RX_BREAK:
    // 如果收到break信號，進入下一個state
    if (EUSART_is_rx_ready() > 0) {
      if (LIN_breakCheck() == true) { // Read Break
        LIN_rxState = LIN_RX_SYNC;
      } else {
        LIN_rxState = LIN_RX_ERROR;
      }
    }
    break;
  case LIN_RX_SYNC:
    // 如果收到同步信號，進入下一個state
    if (EUSART_is_rx_ready() > 0) {
      if (EUSART_Read() == 0x55) { // Read sync - discard
        LIN_rxState = LIN_RX_PID;
      } else {
        LIN_rxState = LIN_RX_ERROR;
      }
    }
    break;
  case LIN_RX_PID:
    // Read PID,取得狀態,取得資料長度
    // 這裡就要檢查PID，如果不是要接收的ID就進入error state
    if (EUSART_is_rx_ready() > 0) {
      LIN_packet.PID = EUSART_Read();

      // PID檢查，錯誤進入error state
      if (LIN_checkPID(LIN_packet.PID) == false) {
        LIN_rxState = LIN_RX_ERROR;
        break;
      }
      // 取得狀態: RX or TX
      LIN_packet.type = LIN_getFromTable(LIN_packet.PID, TYPE);
      if (LIN_packet.type == RECEIVE) {
        // 取得資料長度，進入data接收state
        LIN_packet.length = LIN_getFromTable(LIN_packet.PID, LENGTH);
        LIN_rxState = LIN_RX_DATA;
      } else {
        LIN_disableRx();
        LIN_rxState = LIN_RX_TX_DATA;
      }
    }
    break;
  case LIN_RX_DATA:
    // 接收資料
    if (EUSART_is_rx_ready() > 0) {
      LIN_packet.data[rxDataIndex] = EUSART_Read();
      if (++rxDataIndex >= LIN_packet.length) {
        // received all data bytes
        rxDataIndex = 0;
        LIN_rxState = LIN_RX_CHECKSUM;
      }
    }
    break;
  case LIN_RX_CHECKSUM:
    // 讀取checksum並比對
    if (EUSART_is_rx_ready() > 0) {
      LIN_packet.checksum = EUSART_Read();
      if (LIN_packet.checksum !=
          LIN_getChecksum(LIN_packet.length, LIN_packet.PID, LIN_packet.data)) {
        LIN_rxState = LIN_RX_ERROR;
      } else {
        // 資料正確，進入ready state
        LIN_rxState = LIN_RX_RDY;
      }
    }
    break;
  case LIN_RX_TX_DATA:
    LIN_queuePacket(LIN_packet.PID); // Send response automatically
    LIN_rxState = LIN_RX_RDY;
  case LIN_RX_RDY:
    LIN_processData();
  case LIN_RX_ERROR:
    LIN_stopTimer();
    rxDataIndex = 0;
    LIN_rxInProgress = false;
    memset(LIN_packet.rawPacket, 0,
           sizeof(LIN_packet.rawPacket)); // clear receive data
  case LIN_RX_WAIT:
    if (TXSTAbits.TRMT) { // 等待傳送完成，但目前只收不傳
      LIN_enableRx();
      LIN_rxState = LIN_RX_IDLE;
    } else {
      LIN_rxState = LIN_RX_WAIT;
    }
    break;
  }
  return LIN_rxState;
}

// 將LIN資料傳送出去
void LIN_sendPacket(uint8_t length, uint8_t pid, uint8_t *data) {

  // Write data
  for (uint8_t i = 0; i < length; i++) {
    EUSART_Write(*(data + i));
  }
  // Add Checksum
  EUSART_Write(LIN_getChecksum(length, pid, data));
}

// 將LIN接收到的資料存放到tempRxData陣列中，清除LIN_packet所有資料，並回傳ID
uint8_t LIN_getPacket(uint8_t *data) {
  uint8_t cmd = LIN_packet.PID & 0x3F;

  memcpy(data, LIN_packet.data, sizeof(LIN_packet.data));
  memset(LIN_packet.rawPacket, 0, sizeof(LIN_packet.rawPacket));

  return cmd;
}

// 比對ID 回傳ID, type, length(ID找不到，回傳ERROR)
uint8_t LIN_getFromTable(uint8_t cmd, lin_sch_param_t param) {
  const lin_rx_cmd_t *rxCommand =
      LIN_rxCommand; // copy table pointer so we can modify it

  cmd &= 0x3F; // clear possible parity bits
  // check table
  for (uint8_t length = 0; length < LIN_rxCommandLength; length++) {
    // ID match就回傳資料
    if (cmd == rxCommand->cmd) {
      break;
    }
    rxCommand++; // go to next entry

    // ID找不到，回傳ERROR
    if (length == (LIN_rxCommandLength - 1)) {
      return ERROR; // command not in schedule table
    }
  }

  switch (param) {
  case CMD:
    return rxCommand->cmd;
  case TYPE:
    return rxCommand->type;
  case LENGTH:
    return rxCommand->length;
  default:
    break;
  }

  return ERROR;
}

bool LIN_checkPID(uint8_t pid) {
  // ID比對失敗，回傳false
  if (LIN_getFromTable(pid, TYPE) == ERROR)
    return false; // PID not in schedule table

  // pid(接收到的) 與 計算出的PID 比對
  if (pid == LIN_calcParity(pid & 0x3F))
    return true;

  return false; // Parity Error
}

// 傳入ID:計算奇偶校验位，回傳PID
uint8_t LIN_calcParity(uint8_t CMD) {
  lin_pid_t PID;
  PID.rawPID = CMD;

  // Workaround for compiler bug - CAE_MCU8-200:
  //    PID.P0 = PID.ID0 ^ PID.ID1 ^ PID.ID2 ^ PID.ID4;
  //    PID.P1 = ~(PID.ID1 ^ PID.ID3 ^ PID.ID4 ^ PID.ID5);
  PID.P0 = PID.ID0 ^ PID.ID1;
  PID.P0 = PID.P0 ^ PID.ID2;
  PID.P0 = PID.P0 ^ PID.ID4;
  PID.P1 = PID.ID1 ^ PID.ID3;
  PID.P1 = PID.P1 ^ PID.ID4;
  PID.P1 = PID.P1 ^ PID.ID5;
  PID.P1 = ~PID.P1;

  return PID.rawPID;
}

uint8_t LIN_getChecksum(uint8_t length, uint8_t pid, uint8_t *data) {
  uint16_t checksum = pid; // enhanced checksum includes PID

  for (uint8_t i = 0; i < length; i++) {
    checksum = checksum + *data++;
    if (checksum > 0xFF)
      checksum -= 0xFF;
  }
  checksum = ~checksum;

  return (uint8_t)checksum;
}

// 開始計時器
void LIN_startTimer(uint8_t timeout) {
  LIN_timeout = timeout;
  // TMR0_WriteTimer(0);
  TMR0_Reload();
  LIN_timerRunning = true;
}

void LIN_timerHandler(void) {

  // callback function
  if (++CountCallBack >= LIN_timeout) {
    // Stop timer directly here to avoid calling a non-reentrant function
    // from interrupt context which can cause the compiler to duplicate it.
    CountCallBack = 0;
    LIN_timerRunning = false;
  }
}

//TMR0中斷服務程式會呼叫這個函式來設定LIN計時器處理函式
void LIN_setTimerHandler(void) { TMR0_SetInterruptHandler(LIN_timerHandler); }

// 停止計時器
void LIN_stopTimer(void) {
  // reset ticker counter
  CountCallBack = 0;
  LIN_timerRunning = false;
}

// 使能LIN接收
void LIN_enableRx(void) {
  RCSTAbits.CREN = 1;
  PIE1bits.RCIE = 1;
}

// 禁用LIN接收
void LIN_disableRx(void) {
  RCSTAbits.CREN = 0;
  PIE1bits.RCIE = 0;
}

// 檢查是否收到break信號
bool LIN_breakCheck(void) {

  if ((EUSART_Read() == 0x00) && (EUSART_get_last_status().ferr == 1)) {
    return true;
  }

  return false;
}