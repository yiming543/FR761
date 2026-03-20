/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules
  selected in the GUI. Generation Information : Product Revision  :  PIC10 /
  PIC12 / PIC16 / PIC18 MCUs - 1.81.8 Device            :  PIC16F1936 Driver
  Version    :  2.00
 */

// 20251022 V06 CS:99BC
// modify AutoBaud_Detect function，只在開機時做auto baud偵測
// 不在每次接收同步信號時做auto baud偵測，因為這樣會導致無法正確接收LIN訊號
// 在每次接收同步信號時,如果沒有接收到同步信號,則會重置UART,重新開始接收同步信號.
// timer out 15ms
// LIN接收加入註解
// 修改程式消失所有編譯警告
// 使用控制模式點亮燈號，預留bit模式點亮燈號功能

// 20251107 V01 CS:BE2A
// 新增TESTMODE
// 當送電時選擇白光(5V)，進入TESTMODE 遠光恆亮.
// LoBeam Pin 5V:LoBeam亮
// LoBeam Pin 0V:LoBeam滅
// Turn Pin 0V:黃光逐一亮起，最終停在全亮.
// Turn Pin 5V:白光逐一亮起，最終停在全亮.
// 選擇黃光(0V)，離開TESTMODE

// 新增AutoBaud之前要等待TX Hi 1ms，等待時LoBeam亮，直到AutoBaud完成
// 避免AutoBaud出錯.
// 行車 10%，晝行100%，方向 100%
// 切換顏色: 行車/晝行

// 20251110 V02 CS:34BE
// PWM無法用中斷，5us中斷一次太平繁，程式無法使用.
// 在PWM中斷程序修改duty也不可行，超過5us.
// PWM每送出24bit要停280us
// LIN收到後只設定旗標，不做處理。
// LIN接收會干援PWM，需用另一個MCU接收LIN。
// 方向燈用GPIO接收 (無需分左右邊燈具)
// LoBeam用GPIO控制
// DRL，POS，HiBeam 由另一顆MCU解碼後經GPIO提供.
// 方向燈結束1秒後才可亮DRL或POS.
// 方向超過600ms就是迎賓信號，一直等到迎賓信號結束，600ms沒有其它信號再跑迎賓。

// 20211113 V03 CS:6EBD
//  方向燈用GPIO接收 (無需分左右邊燈具)
//  LoBeam用GPIO控制
//  DRL，POS，HiBeam 由另一顆MCU解碼後經GPIO提供.
//  方向燈結束600ms後才可亮DRL或POS.
//  方向超過600ms就是迎賓信號，一直等到迎賓信號結束，600ms沒有其它信號再跑迎賓。

// 20251114 V04 CS:45F7
// 迎賓燈完成
// 方向燈完成
// DRL完成
// POS完成
// HIBEAM完成
// LOBEAM完成
// TESTMODE REMARK

// 20251217 V05 CS:9639
// 新增RA0 控制風扇，Lobeam啟動時 風扇才啟動.
// 新增Lobeam點燈.
// 修正接收信號
// 新增LED OFF信號

// 20260223 V06 CS:6583
// 1.新增TESTMODE獨立檔案.C和.H
// 2.新增FUNC_MAIN獨立檔案.C和.H
// 3.新增GPIO獨立檔案.C和.H
// 4.新增FUNC_WS2811獨立檔案.C和.H
// 5.新增TIMER獨立檔案.C和.H
// 6.新增IO_debounce 5ms 使用tmr4
// 7.新增delay 1ms 使用tmr2
// 8.LED OFF不用設定WS2811改用關閉LED電源.(加快速度)
// 9.方向燈修正
// 11.迎賓燈修正(開迎賓模式時，有TURN信號就離開迎賓模式))
// 10.迎賓燈修正(最後亮白光時，DRL/POS/TURN有信號就離開迎賓模式)

//20260320 V07 CS:D96E
//修正方向燈OFF 白光沒有延遲600ms再亮起.


#include "mcc_generated_files/mcc.h"
#include "func_main.h"

/*
  Main application
 */

int main(void) {
  // initialize the device
  SYSTEM_Initialize();
  software_Initialize();

  while (1) {
   func_main();
  }

  return 0;
}
/**
 End of File
 */