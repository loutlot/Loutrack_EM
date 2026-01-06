# 回路図

## フォルダ構成

```
schematics/
├── README.md          # このファイル
├── m0_poc/           # M0: 1軸ディスクリートPoC
│   ├── beacon/       # 送信回路
│   ├── tracker/      # 受信回路
│   └── wiring_table.md
└── m1_3axis/         # M1: 3軸TDM
```

## 各フェーズの回路図

| フェーズ | 送信回路 | 受信回路 | 配線表 |
|----------|----------|----------|--------|
| M0 | [beacon/](m0_poc/beacon/) | [tracker/](m0_poc/tracker/) | [wiring_table.md](m0_poc/wiring_table.md) |
| M1 | TBD | TBD | TBD |

## 回路図作成ツール（推奨）

- **KiCad** - オープンソース、無料
- **Fritzing** - ブレッドボード図に最適
- **draw.io** - 簡易ブロック図向け
