/**
 * @file battery_monitor.cpp
 * @brief バッテリー電圧監視ライブラリ実装
 */

#include "battery_monitor.h"

BatteryMonitor::BatteryMonitor() : lastVoltage(0), lastReadTime(0) {
}

void BatteryMonitor::begin() {
  pinMode(BATTERY_ADC_PIN, INPUT);
  // 初回読み取り
  lastVoltage = getVoltage();
}

uint32_t BatteryMonitor::readADCMilliVolts() {
  uint32_t sum = 0;
  
  // 16回サンプリングして平均化（ノイズ除去）
  for (int i = 0; i < BATTERY_SAMPLE_COUNT; i++) {
    sum += analogReadMilliVolts(BATTERY_ADC_PIN);
  }
  
  return sum / BATTERY_SAMPLE_COUNT;
}

float BatteryMonitor::getVoltage() {
  // ADCから分圧後の電圧を取得（mV）
  uint32_t adcMilliVolts = readADCMilliVolts();
  
  // 分圧比を考慮して実際のバッテリー電圧を計算
  // 200k/200kの1/2分圧なので、2倍する
  float voltage = VOLTAGE_DIVIDER_RATIO * adcMilliVolts / 1000.0f;
  
  lastVoltage = voltage;
  lastReadTime = millis();
  
  return voltage;
}

int BatteryMonitor::getPercentage() {
  float voltage = getVoltage();
  
  // リニア近似（Li-Po電圧特性を簡易的に）
  // 4.2V = 100%, 3.0V = 0%
  if (voltage >= VBAT_FULL) {
    return 100;
  } else if (voltage <= VBAT_MINIMUM) {
    return 0;
  } else {
    // (voltage - 3.0) / (4.2 - 3.0) * 100
    return (int)((voltage - VBAT_MINIMUM) / (VBAT_FULL - VBAT_MINIMUM) * 100);
  }
}

BatteryState BatteryMonitor::getState() {
  float voltage = getVoltage();
  
  if (voltage < VBAT_CRITICAL) {
    return BATTERY_SHUTDOWN;
  } else if (voltage < VBAT_LOW_WARNING) {
    return BATTERY_CRITICAL;
  } else if (voltage < 3.5f) {
    return BATTERY_LOW;
  } else if (voltage < 3.8f) {
    return BATTERY_MEDIUM;
  } else if (voltage < 4.0f) {
    return BATTERY_HIGH;
  } else {
    return BATTERY_FULL;
  }
}

bool BatteryMonitor::isCritical() {
  return getVoltage() < VBAT_CRITICAL;
}

bool BatteryMonitor::isLowWarning() {
  float voltage = getVoltage();
  return voltage < VBAT_LOW_WARNING && voltage >= VBAT_CRITICAL;
}

void BatteryMonitor::printDebug() {
  float voltage = getVoltage();
  int percentage = getPercentage();
  BatteryState state = getState();
  
  Serial.print("Battery: ");
  Serial.print(voltage, 3);
  Serial.print("V (");
  Serial.print(percentage);
  Serial.print("%) - ");
  
  switch (state) {
    case BATTERY_FULL:
      Serial.println("FULL");
      break;
    case BATTERY_HIGH:
      Serial.println("HIGH");
      break;
    case BATTERY_MEDIUM:
      Serial.println("MEDIUM");
      break;
    case BATTERY_LOW:
      Serial.println("LOW");
      break;
    case BATTERY_CRITICAL:
      Serial.println("CRITICAL - Low battery warning!");
      break;
    case BATTERY_SHUTDOWN:
      Serial.println("SHUTDOWN - Entering deep sleep!");
      break;
  }
}
