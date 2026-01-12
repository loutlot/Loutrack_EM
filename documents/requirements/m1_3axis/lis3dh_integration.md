# M1 LIS3DH統合仕様書

## 概要

LIS3DH（3軸加速度センサー）をTrackerに統合し、Hemispheric ambiguityを解決する。

**参考**: [Kirb EM Tracking講義](../../outside_sources/Electromagnetic%206-DOF%20Tracking%20for%20VR%20Applications/reformatted_kirb_lecture.md)

---

## 1. 目的

### 1.1 Hemispheric Ambiguity問題

磁界測定では、送信コイルからの距離と角度だけで位置を推定するため、原点に対して対称な2つの位置（+X+Y+Zと-X-Y-Z）が同じ測定値を示す。

```
           Beacon
              │
    ┌─────────┴─────────┐
    │                   │
 Tracker A          Tracker A'
 (+X,+Y,+Z)        (-X,-Y,-Z)
    │                   │
    └───── 同じA値 ─────┘
```

### 1.2 解決アプローチ

Kirbの実装を参考に、**IMU（加速度センサー）で重力方向を検出**し、どちらの半球にいるかを判別する。

- **Beaconの「下」方向を固定**（設置時に定義）
- **TrackerのIMUで重力方向を検出**
- **比較して半球を判定**

---

## 2. LIS3DH仕様

### 2.1 基本仕様

| 項目 | 値 |
|------|-----|
| メーカー | STMicroelectronics |
| 型番 | LIS3DH / LIS3DHTR |
| 機能 | 3軸加速度センサー |
| 電源電圧 | 1.71V〜3.6V |
| インターフェース | I2C / SPI |
| I2Cアドレス | 0x18 (SA0=GND) / 0x19 (SA0=VDD) |
| 測定レンジ | ±2g / ±4g / ±8g / ±16g |
| 消費電流 | 2µA (1Hz) 〜 11µA (10Hz) |
| パッケージ | LGA-16 (3x3x1mm) |

**データシート**: [`documents/requirements/lis3dh/lis3dh.pdf`](../lis3dh/lis3dh.pdf)

### 2.2 選定理由

| 項目 | 評価 |
|------|------|
| コスト | ⭐⭐⭐ 低コスト（$1-2） |
| 消費電力 | ⭐⭐⭐ 超低消費電力 |
| 精度 | ⭐⭐ 重力方向検出には十分 |
| 入手性 | ⭐⭐⭐ 秋月、Adafruit、AliExpress |
| ライブラリ | ⭐⭐⭐ Adafruit LIS3DH、Arduino標準 |

**ジャイロは不要**: 重力方向検出だけなので、6軸IMU（BMI160など）より低コストなLIS3DHで十分。

---

## 3. ハードウェア接続

### 3.1 M1（ブレークアウトボード使用）

**推奨ボード**:
- 秋月電子 LIS3DHモジュール
- Adafruit LIS3DH Breakout
- AliExpress GY-LIS3DH

**接続図**:

```
XIAO ESP32C3         LIS3DH Breakout
─────────────        ───────────────
3.3V ────────────── VDD
GND  ────────────── GND
D4 (GPIO6) ──────── SDA
D5 (GPIO7) ──────── SCL
(GND) ───────────── SA0 (アドレス=0x18)
```

**I2Cプルアップ**: ブレークアウトボードに内蔵されていれば不要。無ければ4.7kΩを追加。

### 3.2 M2（SMD直付け）

→ [`documents/schematics/m2_smd/tracker/lis3dh_schematic.md`](../../schematics/m2_smd/tracker/lis3dh_schematic.md)を参照

### 3.3 XIAO ESP32C3 GPIO割り当て（M1/M2共通）

| 機能 | ピン | GPIO | プロトコル |
|------|------|------|-----------|
| I2C_SDA | D4 | GPIO6 | I2C |
| I2C_SCL | D5 | GPIO7 | I2C |

**ピン競合チェック**:
- SPI（MCP3008）: D7, D8, D9, D10 → OK（別ピン）
- バッテリー監視: D0 → OK（別ピン）
- 空きピン: D1, D2, D3, D6 → OK

---

## 4. ファームウェア実装

### 4.1 依存ライブラリ

```cpp
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
```

**インストール** (Arduino IDE):
- `Adafruit LIS3DH` ライブラリ
- `Adafruit Unified Sensor` ライブラリ

### 4.2 初期化コード

```cpp
// LIS3DH設定
#define I2C_SDA 6  // GPIO6 (D4)
#define I2C_SCL 7  // GPIO7 (D5)
#define LIS3DH_ADDR 0x18  // SA0=GND

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setupLIS3DH() {
  // I2C初期化（ESP32C3のカスタムピン）
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // LIS3DH初期化
  if (!lis.begin(LIS3DH_ADDR)) {
    Serial.println("ERROR: LIS3DH not found!");
    while (1) yield();
  }
  
  // レンジ設定（±2g: 重力検出に十分）
  lis.setRange(LIS3DH_RANGE_2_G);
  
  // データレート（10Hz: 低消費電力、レスポンス十分）
  lis.setDataRate(LIS3DH_DATARATE_10_HZ);
  
  Serial.println("LIS3DH initialized!");
}
```

### 4.3 重力方向取得

```cpp
// 重力ベクトルを取得（正規化済み）
void getGravityDirection(float &gx, float &gy, float &gz) {
  lis.read();
  
  // 生値（±2gレンジ、1g ≈ 1.0）
  gx = lis.x_g;
  gy = lis.y_g;
  gz = lis.z_g;
  
  // 正規化（オプション）
  float mag = sqrt(gx*gx + gy*gy + gz*gz);
  if (mag > 0.1) {  // ゼロ除算防止
    gx /= mag;
    gy /= mag;
    gz /= mag;
  }
}
```

### 4.4 Hemispheric Ambiguity解決

```cpp
// ===== 設定 =====
// Beaconの「下」方向を定義（設置時に固定）
// 例: Beaconを水平に置いた場合、重力は+Z方向
const float BEACON_DOWN_X = 0.0;
const float BEACON_DOWN_Y = 0.0;
const float BEACON_DOWN_Z = 1.0;  // +Z = 下

// ===== Hemispheric判定 =====
// true: 正半球（Beaconと同じ側）
// false: 負半球（Beaconと反対側）
bool isPositiveHemisphere() {
  float gx, gy, gz;
  getGravityDirection(gx, gy, gz);
  
  // TrackerとBeaconの重力方向の内積
  // 内積 > 0 なら同じ向き（正半球）
  float dot = gx * BEACON_DOWN_X + gy * BEACON_DOWN_Y + gz * BEACON_DOWN_Z;
  
  return dot > 0;
}

// ===== 磁界測定値の符号補正 =====
void applyHemisphereCorrection(float &Ax, float &Ay, float &Az) {
  if (!isPositiveHemisphere()) {
    // 負半球の場合、符号を反転
    Ax = -Ax;
    Ay = -Ay;
    Az = -Az;
  }
}
```

### 4.5 メインループ統合

```cpp
void loop() {
  // ===== 1. 磁界測定（I/Q同期検波） =====
  float Ix, Qx, Ax, phix;
  computeIQ(Ix, Qx, Ax, phix);  // X軸
  
  // 同様にY軸、Z軸（TDM使用時）
  
  // ===== 2. Hemispheric Ambiguity解決 =====
  float Ay = 0, Az = 0;  // TDM使用時は実測値
  applyHemisphereCorrection(Ax, Ay, Az);
  
  // ===== 3. 結果出力 =====
  Serial.printf("A = (%.3f, %.3f, %.3f)\n", Ax, Ay, Az);
}
```

---

## 5. キャリブレーション手順

### 5.1 Beacon設置キャリブレーション

1. **Beaconを設置**（通常は動かさない）
2. **「下」方向を定義**:
   - Beaconを水平に置く → `BEACON_DOWN_Z = 1.0`
   - Beaconを壁に立てかける → `BEACON_DOWN_X = 1.0` など
3. **ファームウェアに定数を設定**

### 5.2 Trackerキャリブレーション（将来拡張）

必要に応じて以下を実装:
- オフセットキャリブレーション（静止状態で1gになるよう補正）
- 軸整列キャリブレーション（LIS3DHとTrackerコイルの軸を合わせる）

---

## 6. 検証項目

### 6.1 M1タスク一覧

| # | タスク | 確認項目 |
|---|--------|----------|
| 1.7 | LIS3DH購入・接続 | I2C通信成功、加速度値読み取り |
| 1.8 | FW実装（重力方向検出） | 静止状態で約1g、傾けると軸ごとに変化 |
| 1.9 | Hemispheric ambiguity検証 | Beaconの上下で磁界符号が正しく補正される |

### 6.2 テストシナリオ

1. **静止テスト**: 
   - Trackerを水平に置く → gz ≈ 1.0
   - Trackerを垂直に立てる → gx or gy ≈ 1.0

2. **半球判定テスト**:
   - TrackerをBeaconの「上」に配置 → 正半球
   - TrackerをBeaconの「下」に配置 → 負半球
   - 符号補正後のAxが一貫していることを確認

---

## 7. 参考資料

- [LIS3DHデータシート](../lis3dh/lis3dh.pdf)
- [Adafruit LIS3DHライブラリ](https://github.com/adafruit/Adafruit_LIS3DH)
- [Kirb EM Tracking講義](../../outside_sources/Electromagnetic%206-DOF%20Tracking%20for%20VR%20Applications/reformatted_kirb_lecture.md)
- [M1 BOM](../../bom/m1_3axis/bom.md)
- [M2 LIS3DH回路図](../../schematics/m2_smd/tracker/lis3dh_schematic.md)
