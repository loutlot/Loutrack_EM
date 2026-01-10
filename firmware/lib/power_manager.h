/**
 * @file power_manager.h
 * @brief 電源管理ライブラリ（XIAO ESP32C3用）
 * 
 * DEEP SLEEPとWake-up機能を提供します。
 * バッテリー電圧監視と連携し、過放電防止を実現します。
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "battery_monitor.h"

// Wake-upピン定義
#define WAKEUP_PIN        D1        // GPIO3
#define WAKEUP_GPIO       GPIO_NUM_3

// 低電圧時のスリープまでの待機時間（ms）
#define LOW_BATTERY_TIMEOUT_MS  30000  // 30秒

/**
 * @brief Wake-up理由
 */
typedef enum {
  WAKEUP_REASON_RESET,      // 通常起動（リセット）
  WAKEUP_REASON_BUTTON,     // ボタン押下によるWake-up
  WAKEUP_REASON_TIMER,      // タイマーによるWake-up
  WAKEUP_REASON_UNKNOWN     // 不明
} WakeupReason;

/**
 * @brief 電源管理クラス
 */
class PowerManager {
public:
  /**
   * @brief コンストラクタ
   * @param batteryMonitor バッテリーモニタへの参照
   */
  PowerManager(BatteryMonitor& batteryMonitor);

  /**
   * @brief 初期化
   */
  void begin();

  /**
   * @brief Wake-up理由を取得
   * @return WakeupReason 列挙値
   */
  WakeupReason getWakeupReason();

  /**
   * @brief Wake-up理由を文字列で取得
   * @return 理由の文字列
   */
  const char* getWakeupReasonString();

  /**
   * @brief DEEP SLEEPに入る
   */
  void enterDeepSleep();

  /**
   * @brief タイマー付きDEEP SLEEPに入る
   * @param sleepTimeSeconds スリープ時間（秒）
   */
  void enterDeepSleep(uint32_t sleepTimeSeconds);

  /**
   * @brief バッテリー状態をチェックし、必要ならスリープ
   * @return true: スリープに入った（この関数からは戻らない）
   */
  bool checkBatteryAndSleep();

  /**
   * @brief 電源管理のメインループ処理（定期的に呼び出す）
   */
  void update();

  /**
   * @brief 起動回数を取得（RTC RAMに保存）
   * @return 起動回数
   */
  uint32_t getBootCount();

  /**
   * @brief デバッグ情報をシリアルに出力
   */
  void printDebug();

private:
  BatteryMonitor& battery;
  unsigned long lowBatteryStartTime;
  bool lowBatteryWarningActive;
  
  /**
   * @brief Wake-upピンを設定
   */
  void configureWakeup();
};

#endif // POWER_MANAGER_H
