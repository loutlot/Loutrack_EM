# M2 LIS3DH SMD回路図

## 概要

LIS3DH（3軸加速度センサー）のSMD実装回路図。Hemispheric ambiguity解決用。

**データシート**: [`documents/requirements/lis3dh/lis3dh.pdf`](../../../requirements/lis3dh/lis3dh.pdf)

---

## LIS3DH仕様

| 項目 | 値 |
|------|-----|
| メーカー | STMicroelectronics |
| 型番 | LIS3DHTR |
| パッケージ | LGA-16 (3x3x1mm) |
| 電源電圧 | 1.71V〜3.6V |
| IO電源 | 1.8V（独立供給可能、VDD共用可） |
| インターフェース | I2C / SPI |
| I2Cアドレス | 0x18（SA0=GND）/ 0x19（SA0=VDD） |
| 消費電力 | 2µA @ 1Hz（Ultra-low-power mode） |

---

## ピンアウト（LGA-16）

```
        ┌─────────────────────┐
        │     LIS3DH (TOP)    │
        │      LGA-16         │
        │                     │
    1 ──┤ VDD_IO        GND  ├── 5
    2 ──┤ NC           INT1  ├── 6
    3 ──┤ NC           INT2  ├── 7
    4 ──┤ SDO/SA0      GND   ├── 8
        │                     │
   16 ──┤ VDD          SCL   ├── 9
   15 ──┤ NC           SDA   ├── 10
   14 ──┤ NC           ADC3  ├── 11
   13 ──┤ CS           ADC2  ├── 12
        │                     │
        └─────────────────────┘
```

**ピン説明**:

| ピン | 名称 | 機能 | 接続先 |
|------|------|------|--------|
| 1 | VDD_IO | IO電源 (1.8V〜VDD) | 3.3V（VDD共用可） |
| 4 | SDO/SA0 | I2Cアドレス選択 / SPI MISO | GND（アドレス=0x18） |
| 5, 8 | GND | グランド | GND |
| 6 | INT1 | 割り込み出力1 | NC（将来拡張用） |
| 7 | INT2 | 割り込み出力2 | NC（将来拡張用） |
| 9 | SCL | I2Cクロック / SPI CLK | GPIO7 (D5) + 4.7kΩプルアップ |
| 10 | SDA | I2Cデータ / SPI MOSI | GPIO6 (D4) + 4.7kΩプルアップ |
| 11 | ADC3 | 外部ADC入力3 | NC |
| 12 | ADC2 | 外部ADC入力2 | NC |
| 13 | CS | SPI CS（I2Cモード時はVDD） | 3.3V（I2Cモード） |
| 16 | VDD | 電源 | 3.3V |
| 2, 3, 14, 15 | NC | 未接続 | NC |

---

## 回路図

### I2C接続（推奨）

```
                         3.3V
                          │
                    ┌─────┴─────┐
                    │           │
                   4.7kΩ      4.7kΩ
                    │           │
                    │           │
    GPIO6 (D4) ─────┼───────────┼──────────┐
    (SDA)           │           │          │
                    │           │          │
    GPIO7 (D5) ─────┼───────────┘          │
    (SCL)           │                      │
                    │                      │
                    │     ┌────────────────┼────────────────┐
                    │     │                │                │
                    │     │   LIS3DH       │                │
                    │     │   (LGA-16)     │                │
                    │     │                │                │
              3.3V ─┼─────┼── VDD (16)     │                │
                    │     │                │                │
              3.3V ─┼─────┼── VDD_IO (1)   │                │
                    │     │                │                │
              3.3V ─┼─────┼── CS (13)      │ (I2Cモード)    │
                    │     │                │                │
               GND ─┼─────┼── GND (5, 8)   │                │
                    │     │                │                │
               GND ─┼─────┼── SA0 (4)      │ (アドレス=0x18)│
                    │     │                │                │
                    └─────┼── SCL (9) ─────┘                │
                          │                                 │
                          └── SDA (10) ────────────────────┘
                          │
                          ├── INT1 (6) ──── NC (将来拡張用)
                          │
                          └── INT2 (7) ──── NC (将来拡張用)


    電源デカップリング:

              3.3V
               │
           ┌───┴───┐
           │       │
         100nF   10µF
           │       │
           └───┬───┘
               │
              GND
```

---

## 部品リスト

| 参照番号 | 部品 | 値 | パッケージ | 数量 | 備考 |
|----------|------|-----|----------|------|------|
| U1 | LIS3DHTR | - | LGA-16 | 1 | 3軸加速度センサー |
| C1 | デカップリング | 100nF | 0402/0603 | 1 | VDD-GND |
| C2 | バルク | 10µF | 0603/0805 | 1 | VDD-GND |
| R1 | プルアップ | 4.7kΩ | 0402/0603 | 1 | SDA-3.3V |
| R2 | プルアップ | 4.7kΩ | 0402/0603 | 1 | SCL-3.3V |

---

## PCBレイアウトガイドライン

### フットプリント

データシート（lis3dh.pdf）のFigure 12参照。

| 項目 | 値 |
|------|-----|
| パッケージサイズ | 3.0 × 3.0 × 1.0 mm |
| パッドピッチ | 0.5mm |
| パッドサイズ | データシート推奨値に従う |

### 配置推奨

```
┌─────────────────────────────────────────────┐
│                                             │
│   ┌───────┐                                 │
│   │ C1    │  ← VDD近傍（デカップリング）     │
│   │100nF  │                                 │
│   └───────┘                                 │
│                                             │
│   ┌───────────────┐                         │
│   │               │                         │
│   │    LIS3DH     │  ← センサー配置          │
│   │               │                         │
│   └───────────────┘                         │
│                                             │
│   ┌───────┐                                 │
│   │ C2    │  ← バルクコンデンサ             │
│   │ 10µF  │                                 │
│   └───────┘                                 │
│                                             │
│   ┌───┐ ┌───┐                               │
│   │R1 │ │R2 │  ← I2Cプルアップ              │
│   │4.7k│ │4.7k│                             │
│   └───┘ └───┘                               │
│                                             │
└─────────────────────────────────────────────┘
```

### 配線ガイドライン

1. **デカップリング**: C1 (100nF) はVDDピンの直近に配置
2. **I2Cライン**: できるだけ短く配線、GNDプレーンから離す
3. **GNDプレーン**: センサー周辺にベタGNDを配置
4. **ノイズ対策**: デジタル回路（MCU）とは離して配置

---

## ファームウェア設定

### I2C初期化

```cpp
#include <Wire.h>
#include <Adafruit_LIS3DH.h>

// XIAO ESP32C3のI2Cピン
#define I2C_SDA 6  // GPIO6 (D4)
#define I2C_SCL 7  // GPIO7 (D5)

// I2Cアドレス（SA0=GND）
#define LIS3DH_ADDR 0x18

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setupLIS3DH() {
  // I2C初期化
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // LIS3DH初期化
  if (!lis.begin(LIS3DH_ADDR)) {
    Serial.println("LIS3DH not found!");
    while (1);
  }
  
  // レンジ設定（±2g、重力検出に十分）
  lis.setRange(LIS3DH_RANGE_2_G);
  
  // データレート設定（低消費電力モード）
  lis.setDataRate(LIS3DH_DATARATE_10_HZ);
  
  Serial.println("LIS3DH initialized!");
}
```

### 重力方向検出（Hemispheric Ambiguity解決）

```cpp
// 重力方向を取得
void getGravityDirection(float &gx, float &gy, float &gz) {
  lis.read();
  gx = lis.x_g;
  gy = lis.y_g;
  gz = lis.z_g;
}

// Hemispheric ambiguity解決
// Beaconの「下」方向を+Zと仮定
bool isPositiveHemisphere() {
  float gx, gy, gz;
  getGravityDirection(gx, gy, gz);
  
  // 重力方向がBeaconの「下」と同じ向きなら正半球
  return gz > 0;
}
```

---

## 参考資料

- [LIS3DHデータシート](../../../requirements/lis3dh/lis3dh.pdf)
- [Kirb EMトラッキング講義](../../../outside_sources/Electromagnetic%206-DOF%20Tracking%20for%20VR%20Applications/reformatted_kirb_lecture.md)（Hemispheric ambiguity解決の参考）
- [Adafruit LIS3DHライブラリ](https://github.com/adafruit/Adafruit_LIS3DH)
