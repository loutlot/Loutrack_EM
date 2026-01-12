# 回路図

## フォルダ構成

```
schematics/
├── README.md          # このファイル
├── m0_poc/           # M0: 1軸ディスクリートPoC
│   ├── beacon/       # 送信回路
│   ├── tracker/      # 受信回路
│   └── wiring_table.md
├── m1_3axis/         # M1: 3軸TDM + LIS3DH
├── m1.5_coil_power/  # M1.5: 自作コイル＆ドライバ強化
└── m2_smd/           # M2: SMD基板化
    └── tracker/
        ├── power_schematic.md   # 電源回路
        └── lis3dh_schematic.md  # LIS3DH回路
```

## 各フェーズの回路図

| フェーズ | 送信回路 | 受信回路 | 配線表 |
|----------|----------|----------|--------|
| M0 | [beacon/](m0_poc/beacon/) | [tracker/](m0_poc/tracker/) | [wiring_table.md](m0_poc/wiring_table.md) |
| M1 | TBD | TBD | TBD |
| M2 | TBD | [tracker/](m2_smd/tracker/) | - |

## M2 Tracker回路図

| 回路 | ファイル | 概要 |
|------|----------|------|
| 電源回路 | [power_schematic.md](m2_smd/tracker/power_schematic.md) | バッテリー監視、Wake-up回路 |
| LIS3DH回路 | [lis3dh_schematic.md](m2_smd/tracker/lis3dh_schematic.md) | 3軸加速度センサー（Hemispheric ambiguity解決用） |

## 回路図作成ツール（推奨）

- **KiCad** - オープンソース、無料
- **Fritzing** - ブレッドボード図に最適
- **draw.io** - 簡易ブロック図向け
