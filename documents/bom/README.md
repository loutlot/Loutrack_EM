# 部品リスト（BOM）

## フォルダ構成

```
bom/
├── README.md          # このファイル
├── m0_poc/           # M0: 1軸ディスクリートPoC
│   └── bom.md
└── m1_3axis/         # M1: 3軸TDM
    └── bom.md
```

## 各フェーズのBOM

| フェーズ | BOM |
|----------|-----|
| M0 | [bom.md](m0_poc/bom.md) |
| M1 | [bom.md](m1_3axis/bom.md) |

## 共通部品（購入済み）

| 部品 | 型番 | 入手先 |
|------|------|--------|
| MCU | XIAO ESP32C3 | AliExpress |
| 3軸コイル | 3D11-722J | AliExpress |
| モータドライバ | DRV8835モジュール | 秋月電子 |
| オペアンプ | MCP6022-I/P | 秋月電子 |
| ADC | MCP3008-I/P | 秋月電子 |


