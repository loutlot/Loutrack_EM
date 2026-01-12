# 仕様・要件

## フォルダ構成

```
requirements/
├── README.md          # このファイル
├── m0_poc/           # M0: 1軸ディスクリートPoC
│   ├── specification_v0.9.md
│   └── acceptance_criteria.md
├── m1_3axis/         # M1: 3軸TDM + LIS3DH統合
│   └── lis3dh_integration.md
├── m1.5_coil_power/  # M1.5: 自作コイル＆ドライバ強化
│   └── specification.md
├── m2_smd/           # M2: SMD基板化
│   └── specification.md
├── lis3dh/           # LIS3DHデータシート
│   └── lis3dh.pdf
└── common/           # 共通仕様
```

## 各フェーズの仕様書

| フェーズ | 仕様書 | 達成基準 | 追加仕様 |
|----------|--------|----------|----------|
| M0 | [specification_v0.9.md](m0_poc/specification_v0.9.md) | [acceptance_criteria.md](m0_poc/acceptance_criteria.md) | - |
| M1 | (M0仕様の拡張) | TBD | [LIS3DH統合](m1_3axis/lis3dh_integration.md) |
| M1.5 | [specification.md](m1.5_coil_power/specification.md) | TBD | - |
| M2 | [specification.md](m2_smd/specification.md) | TBD | - |

## Hemispheric Ambiguity解決

M1以降で使用するLIS3DH（3軸加速度センサー）による解決方法は以下を参照:
- [M1 LIS3DH統合仕様書](m1_3axis/lis3dh_integration.md)
- [M0仕様書 10.3節](m0_poc/specification_v0.9.md#103-hemispheric-ambiguity解決)

## 命名規則

- `specification_vX.Y.md` - 仕様書（バージョン付き）
- `acceptance_criteria.md` - 達成基準
- `design_notes.md` - 設計メモ（オプション）


