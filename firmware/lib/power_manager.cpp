/**
 * @file power_manager.cpp
 * @brief 電源管理ライブラリ実装
 */

#include "power_manager.h"

// RTC RAMに保存される変数（DEEP SLEEP後も保持）
RTC_DATA_ATTR uint32_t bootCount = 0;

PowerManager::PowerManager(BatteryMonitor& batteryMonitor)
  : battery(batteryMonitor),
    lowBatteryStartTime(0),
    lowBatteryWarningActive(false) {
}

void PowerManager::begin() {
  // 起動回数をインクリメント
  bootCount++;
  
  // Wake-upピンを設定
  configureWakeup();
  
  // 起動時にバッテリーチェック
  if (battery.isCritical()) {
    Serial.println("Battery critically low on boot. Entering deep sleep...");
    delay(100);  // シリアル出力を待つ
    enterDeepSleep();
  }
}

void PowerManager::configureWakeup() {
  // Wake-upピンを入力（内部プルアップ）に設定
  pinMode(WAKEUP_PIN, INPUT_PULLUP);
}

WakeupReason PowerManager::getWakeupReason() {
  esp_sleep_wakeup_cause_t wakeupCause = esp_sleep_get_wakeup_cause();
  
  switch (wakeupCause) {
    case ESP_SLEEP_WAKEUP_EXT0:
    case ESP_SLEEP_WAKEUP_EXT1:
    case ESP_SLEEP_WAKEUP_GPIO:
      return WAKEUP_REASON_BUTTON;
    case ESP_SLEEP_WAKEUP_TIMER:
      return WAKEUP_REASON_TIMER;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
    default:
      // DEEP SLEEPからではない（通常起動/リセット）
      return WAKEUP_REASON_RESET;
  }
}

const char* PowerManager::getWakeupReasonString() {
  switch (getWakeupReason()) {
    case WAKEUP_REASON_BUTTON:
      return "Button press";
    case WAKEUP_REASON_TIMER:
      return "Timer";
    case WAKEUP_REASON_RESET:
      return "Reset/Power on";
    default:
      return "Unknown";
  }
}

void PowerManager::enterDeepSleep() {
  Serial.println("Entering deep sleep...");
  Serial.flush();
  
  // GPIO Wake-upを有効化（D1/GPIO3、LOWで起動）
  esp_deep_sleep_enable_gpio_wakeup(BIT(WAKEUP_GPIO), ESP_GPIO_WAKEUP_GPIO_LOW);
  
  // DEEP SLEEPに入る（この関数からは戻らない）
  esp_deep_sleep_start();
}

void PowerManager::enterDeepSleep(uint32_t sleepTimeSeconds) {
  Serial.print("Entering deep sleep for ");
  Serial.print(sleepTimeSeconds);
  Serial.println(" seconds...");
  Serial.flush();
  
  // GPIO Wake-upも有効化（ボタンでも起きられるように）
  esp_deep_sleep_enable_gpio_wakeup(BIT(WAKEUP_GPIO), ESP_GPIO_WAKEUP_GPIO_LOW);
  
  // タイマーWake-upを設定
  esp_sleep_enable_timer_wakeup(sleepTimeSeconds * 1000000ULL);
  
  // DEEP SLEEPに入る
  esp_deep_sleep_start();
}

bool PowerManager::checkBatteryAndSleep() {
  // 過放電チェック（即座にスリープ）
  if (battery.isCritical()) {
    Serial.println("Battery critically low! Shutting down...");
    battery.printDebug();
    delay(100);
    enterDeepSleep();
    return true;  // ここには到達しない
  }
  
  return false;
}

void PowerManager::update() {
  // 過放電チェック
  if (battery.isCritical()) {
    checkBatteryAndSleep();
    return;
  }
  
  // 低電圧警告チェック
  if (battery.isLowWarning()) {
    if (!lowBatteryWarningActive) {
      // 警告開始
      lowBatteryWarningActive = true;
      lowBatteryStartTime = millis();
      Serial.println("Low battery warning! Will sleep in 30 seconds if not charged.");
    } else {
      // タイムアウトチェック
      if (millis() - lowBatteryStartTime >= LOW_BATTERY_TIMEOUT_MS) {
        Serial.println("Low battery timeout. Entering deep sleep...");
        enterDeepSleep();
      }
    }
  } else {
    // 電圧が回復した
    if (lowBatteryWarningActive) {
      lowBatteryWarningActive = false;
      Serial.println("Battery voltage recovered.");
    }
  }
}

uint32_t PowerManager::getBootCount() {
  return bootCount;
}

void PowerManager::printDebug() {
  Serial.println("=== Power Manager Debug ===");
  Serial.print("Boot count: ");
  Serial.println(bootCount);
  Serial.print("Wake-up reason: ");
  Serial.println(getWakeupReasonString());
  Serial.print("Low battery warning: ");
  Serial.println(lowBatteryWarningActive ? "ACTIVE" : "inactive");
  battery.printDebug();
  Serial.println("===========================");
}
