# データシート

## 概要

本プロジェクトで使用する部品のデータシートを保存するフォルダです。

---

## フォルダ構成

```
datasheets/
├── README.md                              # このファイル
├── drv8835/                               # モータドライバ
│   ├── AE-DRV8835-S_akizuki.pdf          ✅ DL済み
│   └── DRV8835_TI.pdf                    ✅ DL済み
├── 3d11_coil/                             # 3軸コイル
│   └── 3D11-Series_SHINHOM.pdf           ✅ DL済み
├── mcp6022/                               # オペアンプ
│   └── (要手動DL)
├── mcp3008/                               # ADC
│   └── MCP3008_Microchip.pdf             ✅ DL済み
├── bat43/                                 # ショットキーダイオード
│   └── (要手動DL)
└── esp32c3/                               # MCU
    └── (要手動DL)
```

---

## ダウンロード済み ✅

| フォルダ | ファイル | サイズ | 内容 |
|----------|----------|--------|------|
| drv8835/ | AE-DRV8835-S_akizuki.pdf | 1.0MB | 秋月モジュール説明書 |
| drv8835/ | DRV8835_TI.pdf | 871KB | TI公式データシート |
| 3d11_coil/ | 3D11-Series_SHINHOM.pdf | 514KB | 3軸コイルシリーズ |
| mcp3008/ | MCP3008_Microchip.pdf | 764KB | ADC公式データシート |

---

## 要手動ダウンロード ⚠️

以下のファイルはサイトの制限により自動ダウンロードできませんでした。
ブラウザでダウンロードして該当フォルダに保存してください。

### MCP6022（オペアンプ）

**保存先:** `mcp6022/MCP6022_Microchip.pdf`

**ダウンロードリンク:**
- [Microchip公式](https://www.microchip.com/en-us/product/MCP6022) → "Documentation" → "Data Sheets"

### BAT43（ショットキーダイオード）

**保存先:** `bat43/BAT43_ST.pdf`

**ダウンロードリンク:**
- [ST公式](https://www.st.com/en/diodes-and-rectifiers/bat43.html) → "Resources" → "Datasheet"

### ESP32-C3（MCU）

**保存先:** `esp32c3/ESP32-C3_Datasheet.pdf`

**ダウンロードリンク:**
- [Espressif公式](https://www.espressif.com/en/products/socs/esp32-c3) → "Resources" → "Datasheet"
- 直接リンク: https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf

---

## 主要スペック抜粋

### DRV8835モジュール (秋月 AE-DRV8835-S)

| 項目 | 値 |
|------|-----|
| モータ電源電圧 (VM) | 0〜11V |
| ロジック電源電圧 (VCC) | 2〜7V |
| Hブリッジ出力電流 | 0〜1.5A（並列で3A） |
| 外部入力PWM周波数 | 0〜250kHz |
| オン抵抗 (HS+LS) | 305mΩ |

**ピン配置（反時計回り）:**

| ピン | 名称 | 機能 |
|------|------|------|
| 1 | VM | モータ電源 |
| 2 | AOUT1 | A出力1 |
| 3 | AOUT2 | A出力2 |
| 4 | BOUT1 | B出力1 |
| 5 | BOUT2 | B出力2 |
| 6 | GND | グランド |
| 7 | BIN2 | B入力2/BENBL |
| 8 | BIN1 | B入力1/BPHASE |
| 9 | AIN2 | A入力2/AENBL |
| 10 | AIN1 | A入力1/APHASE |
| 11 | MODE | モード設定 |
| 12 | VCC | ロジック電源 |

**IN/INモード (MODE = GND):**

| xIN1 | xIN2 | xOUT1 | xOUT2 | 動作 |
|------|------|-------|-------|------|
| 0 | 0 | HiZ | HiZ | 空転 |
| 0 | 1 | L | H | 逆転 |
| 1 | 0 | H | L | 正転 |
| 1 | 1 | L | L | ブレーキ |

### 3D11-722J (SHINHOM)

| 項目 | 値 |
|------|-----|
| サイズ | 11.8 × 11.8 × 2.9 mm |
| インダクタンス (Lx,y,z) | 7.20 mH |
| Q値 | 30 |
| 自己共振周波数 (SRFx,y) | 330 kHz (typ) |
| 自己共振周波数 (SRFz) | 700 kHz (min) |
| DC抵抗 (RDCx,y) | 150Ω max |
| DC抵抗 (RDCz) | 210Ω max |
| 動作温度 | -40°C 〜 +85°C |

### MCP3008 (Microchip)

| 項目 | 値 |
|------|-----|
| 分解能 | 10bit |
| チャンネル数 | 8 |
| サンプルレート | 200ksps @ 5V, 75ksps @ 2.7V |
| 電源電圧 | 2.7〜5.5V |
| インターフェース | SPI |

---

## 外部リンク

### メーカーサイト

| 部品 | メーカー | 製品ページ |
|------|----------|------------|
| DRV8835 | Texas Instruments | [TI](https://www.ti.com/product/DRV8835) |
| AE-DRV8835-S | 秋月電子 | [秋月](https://akizukidenshi.com/catalog/g/g109848/) |
| 3D11-722J | SHINHOM | [SHINHOM](https://www.shinhom.com/) |
| MCP6022 | Microchip | [Microchip](https://www.microchip.com/en-us/product/MCP6022) |
| MCP3008 | Microchip | [Microchip](https://www.microchip.com/en-us/product/MCP3008) |
| BAT43 | STMicroelectronics | [ST](https://www.st.com/en/diodes-and-rectifiers/bat43.html) |
| ESP32-C3 | Espressif | [Espressif](https://www.espressif.com/en/products/socs/esp32-c3) |
| XIAO ESP32C3 | Seeed Studio | [Wiki](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) |
