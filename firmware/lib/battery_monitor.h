/**
 * @file battery_monitor.h
 * @brief バッテリー電圧監視ライブラリ（XIAO ESP32C3用）
 * 
 * M2 Tracker（受信機）のバッテリー電圧監視機能を提供します。
 * msfujino氏の方式を参考に、200kΩ分圧回路でバッテリー電圧を監視します。
 * 
 * @see https://forum.seeedstudio.com/t/battery-voltage-monitor-and-ad-conversion-for-xiao-esp32c/267535
 */

#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

// ピン定義
#define BATTERY_ADC_PIN A0  // D0/GPIO2

// 電圧閾値（V）
#define VBAT_FULL           4.2f    // フル充電
#define VBAT_NOMINAL        3.7f    // 公称電圧
#define VBAT_LOW_WARNING    3.3f    // 低電圧警告
#define VBAT_CRITICAL       3.1f    // 過放電閾値（即スリープ）
#define VBAT_MINIMUM        3.0f    // 最低電圧（危険）

// 分圧比
#define VOLTAGE_DIVIDER_RATIO 2.0f  // 1/2分圧 → 実電圧は2倍

// サンプリング設定
#define BATTERY_SAMPLE_COUNT 16     // 平均化サンプル数

/**
 * @brief バッテリー状態
 */
typedef enum {
  BATTERY_FULL,       // 100% (4.0V以上)
  BATTERY_HIGH,       // 75%以上
  BATTERY_MEDIUM,     // 50%以上
  BATTERY_LOW,        // 25%以上
  BATTERY_CRITICAL,   // 25%未満（要警告）
  BATTERY_SHUTDOWN    // 過放電（即スリープ）
} BatteryState;

/**
 * @brief バッテリー監視クラス
 */
class BatteryMonitor {
public:
  /**
   * @brief コンストラクタ
   */
  BatteryMonitor();

  /**
   * @brief 初期化
   */
  void begin();

  /**
   * @brief バッテリー電圧を取得（V）
   * @return バッテリー電圧（ボルト）
   */
  float getVoltage();

  /**
   * @brief バッテリー残量を取得（%）
   * @return バッテリー残量（0-100%）
   */
  int getPercentage();

  /**
   * @brief バッテリー状態を取得
   * @return BatteryState 列挙値
   */
  BatteryState getState();

  /**
   * @brief 過放電かどうかをチェック
   * @return true: 過放電（要シャットダウン）
   */
  bool isCritical();

  /**
   * @brief 低電圧警告が必要かどうかをチェック
   * @return true: 低電圧警告
   */
  bool isLowWarning();

  /**
   * @brief デバッグ情報をシリアルに出力
   */
  void printDebug();

private:
  float lastVoltage;
  unsigned long lastReadTime;
  
  /**
   * @brief ADCから電圧を読み取る（内部用）
   * @return 分圧後の電圧（mV）
   */
  uint32_t readADCMilliVolts();
};

#endif // BATTERY_MONITOR_H
