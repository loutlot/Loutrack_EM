/**
 * @file m2_tracker.ino
 * @brief M2 Tracker（受信機）メインスケッチ
 * 
 * バッテリー駆動対応の受信機ファームウェア。
 * - バッテリー電圧監視
 * - 過放電防止（自動DEEP SLEEP）
 * - Wake-upボタンによる復帰
 * 
 * @note GPIO割り当て:
 *   D0 (GPIO2): バッテリー電圧監視 (A0)
 *   D1 (GPIO3): Wake-upボタン
 *   D7-D10: SPI (MCP3008)
 */

#include "../lib/battery_monitor.h"
#include "../lib/power_manager.h"

// インスタンス生成
BatteryMonitor batteryMonitor;
PowerManager powerManager(batteryMonitor);

// バッテリー監視間隔（ms）
const unsigned long BATTERY_CHECK_INTERVAL = 1000;
unsigned long lastBatteryCheck = 0;

// ステータス表示間隔（ms）
const unsigned long STATUS_INTERVAL = 5000;
unsigned long lastStatus = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);  // シリアルモニタを開く時間
  
  Serial.println();
  Serial.println("========================================");
  Serial.println("  M2 Tracker - Battery Powered Receiver");
  Serial.println("========================================");
  
  // バッテリーモニタ初期化
  batteryMonitor.begin();
  
  // 電源マネージャ初期化（過放電時はここでスリープに入る）
  powerManager.begin();
  
  // 起動情報を表示
  powerManager.printDebug();
  
  Serial.println();
  Serial.println("System ready.");
  Serial.println("----------------------------------------");
}

void loop() {
  unsigned long now = millis();
  
  // バッテリー監視（1秒間隔）
  if (now - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
    lastBatteryCheck = now;
    powerManager.update();  // 過放電チェック＆警告処理
  }
  
  // ステータス表示（5秒間隔）
  if (now - lastStatus >= STATUS_INTERVAL) {
    lastStatus = now;
    batteryMonitor.printDebug();
  }
  
  // --- ここに受信処理を追加 ---
  // TODO: MCP3008からのADC読み取り
  // TODO: I/Q処理
  // TODO: データ送信
  
  delay(10);  // CPU負荷軽減
}

/**
 * @brief ボタン押下でDEEP SLEEPに入るテスト用関数
 * 
 * シリアルから 's' を送信するとDEEP SLEEPに入ります。
 * Wake-upボタン（D1）を押すと復帰します。
 */
void checkSerialCommand() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 's' || cmd == 'S') {
      Serial.println("Manual sleep requested.");
      powerManager.enterDeepSleep();
    }
  }
}
