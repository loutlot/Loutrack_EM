# モジュール3: ADC（MCP3008）

## 概要

このモジュールでは:
1. MCP3008をブレッドボードに配置
2. 電源とデカップリングを接続
3. XIAO ESP32C3とSPI接続
4. アンプ出力をADCに入力

---

## MCP3008について

MCP3008は**8チャンネル、10bit、SPIインターフェース**のADCです。

### ピン配置（DIP-16）

```
        ┌───∪───┐
   CH0 ─┤1   16├─ VDD (3.3V)
   CH1 ─┤2   15├─ VREF (3.3V)
   CH2 ─┤3   14├─ AGND
   CH3 ─┤4   13├─ CLK   ← SPI
   CH4 ─┤5   12├─ DOUT  ← SPI (MISO)
   CH5 ─┤6   11├─ DIN   ← SPI (MOSI)
   CH6 ─┤7   10├─ CS    ← SPI
   CH7 ─┤8    9├─ DGND
        └───────┘
```

⚠️ **ICの向き確認:** ピン1のマークを確認！

---

## 必要な部品

| 部品 | 秋月コード | 数量 | 用途 |
|------|------------|------|------|
| MCP3008-I/P | 109485 | 1 | ADC IC |
| コンデンサ 0.1µF | 117059 | 2 | VDD, VREFデカップリング |
| BAT43 | 113907 | 2 | 入力保護（オプション） |

---

## 手順

### Step 1: MCP3008の配置

ブレッドボードの溝をまたぐようにICを挿入します。

```
     列: 30 31 32 33 34 35 36 37 38
        ┌──┴──┴──┴──┴──┴──┴──┴──┴──┐
        │                          │
   ● ● │●  ●  ●  ●  ●  ●  ●  ●  │ ● ●   ← 行A（ピン1-8側）
        │           溝             │
   ● ● │●  ●  ●  ●  ●  ●  ●  ●  │ ● ●   ← 行E/F（ピン9-16側）
        │                          │
        └──────────────────────────┘
```

### Step 2: 電源接続

| ピン | 接続先 | 備考 |
|------|--------|------|
| ピン16 (VDD) | 3.3Vレール | デジタル電源 |
| ピン15 (VREF) | 3.3Vレール | 基準電圧 |
| ピン14 (AGND) | GNDレール | アナログGND |
| ピン9 (DGND) | GNDレール | デジタルGND |

### Step 3: デカップリングコンデンサ

**VDDデカップリング（ピン16近傍）:**
1. ピン16と同じ列に0.1µFの片足
2. もう片足をGNDレールに

**VREFデカップリング（ピン15近傍）:**
1. ピン15と同じ列に0.1µFの片足
2. もう片足をAGND（ピン14）と同じ列に

💡 **VREFのデカップリングは重要!** ADCの精度に直結します。

### Step 4: XIAO ESP32C3とのSPI接続

| MCP3008ピン | 信号名 | XIAO ピン | GPIO | 備考 |
|-------------|--------|-----------|------|------|
| ピン13 (CLK) | SPI_SCK | D8 | GPIO8 | クロック |
| ピン12 (DOUT) | SPI_MISO | D9 | GPIO9 | データ出力 |
| ピン11 (DIN) | SPI_MOSI | D10 | GPIO10 | データ入力 |
| ピン10 (CS) | SPI_CS | D7 | GPIO20 | チップセレクト |

### Step 5: アナログ入力接続

| MCP3008ピン | 接続元 | 備考 |
|-------------|--------|------|
| ピン1 (CH0) | MCP6022 OUT B | 受信アンプ出力 |
| ピン2-8 (CH1-7) | 未接続 | 将来3軸用 |

### Step 6: 入力保護（オプション）

BAT43ショットキーダイオードで過電圧からADCを保護:

```
        3.3V
          │
        [BAT43] (カソード→3.3V)
          │
CH0 ──────┼────── アンプ出力
          │
        [BAT43] (アノード→GND)
          │
         GND
```

**配線:**
1. BAT43 #1: アノード→CH0の列、カソード→3.3Vレール
2. BAT43 #2: カソード→CH0の列、アノード→GNDレール

💡 **ダイオードの向き:** BAT43には帯（バンド）があり、これがカソード側です。

---

## 動作確認

### XIAO ESP32C3用テストコード

Arduino IDEまたはPlatformIOで以下のコードを書き込み:

```cpp
// XIAO ESP32C3 + MCP3008 テスト
#include <SPI.h>

// XIAO ESP32C3 SPI pins
#define SPI_SCK  8   // D8
#define SPI_MISO 9   // D9
#define SPI_MOSI 10  // D10
#define SPI_CS   20  // D7

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH);
  
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
  
  Serial.println("MCP3008 Test Started");
}

int readADC(int channel) {
  byte command = 0b11000000 | (channel << 3);
  
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(0x01);  // Start bit
  byte high = SPI.transfer(command);
  byte low = SPI.transfer(0x00);
  digitalWrite(SPI_CS, HIGH);
  
  return ((high & 0x03) << 8) | low;
}

void loop() {
  int value = readADC(0);  // CH0を読む
  float voltage = value * 3.3 / 1023.0;
  
  Serial.printf("ADC: %4d, Voltage: %.3fV\n", value, voltage);
  delay(100);
}
```

### 期待される結果

| 入力状態 | ADC値 | 電圧 |
|----------|-------|------|
| アンプ出力 = Vmid (1.65V) | 約512 | 約1.65V |
| CH0をGNDに接続 | 0〜10 | 約0V |
| CH0を3.3Vに接続 | 1013〜1023 | 約3.3V |

---

## トラブルシューティング

| 症状 | 原因 | 対策 |
|------|------|------|
| 常に0が返る | SPI接続ミス、CSが常にHigh | 配線確認、CSがLowになっているか確認 |
| 値が不安定 | デカップリング不足、配線が長い | 0.1µFを確認、配線を短くする |
| 512付近にならない | Vmidの問題 | モジュール1, 2を再確認 |

---

## 回路図まとめ

```
                          MCP3008
                    ┌───────────────┐
アンプ出力 ────────→│CH0        VDD│←── 3.3V
                    │              │    │
        未使用 ─────│CH1       VREF│←───┼── 3.3V
        未使用 ─────│CH2           │    │
        未使用 ─────│CH3       AGND│──┬─│── GND
        未使用 ─────│CH4           │  │ │
        未使用 ─────│CH5        CLK│──│─│──→ XIAO D8
        未使用 ─────│CH6       DOUT│──│─│──→ XIAO D9
        未使用 ─────│CH7        DIN│←─│─│─── XIAO D10
                    │          CS │←─│─│─── XIAO D7
                    │        DGND │──│─│── GND
                    └───────────────┘  │ │
                                      │ │
                         [0.1µF]←─────┘ │
                            │           │
                           GND    [0.1µF]
                                    │
                                   GND
```

---

## 次のステップ

✅ ADCの動作確認が完了したら、[04_tx_driver.md](04_tx_driver.md) へ進みます。

