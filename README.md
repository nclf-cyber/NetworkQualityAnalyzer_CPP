
## ② C++：NetworkQualityAnalyzer

<badge color="success">修正版</badge>

```markdown
# 通信品質分析ツール（C++）

## 概要

通信テストなどで得た応答時間を入力し、測定結果を集計・保存するC++のコンソールプログラムです。測定値のばらつきや応答がなかった回数を整理し、通信状況を振り返るための簡易ツールとして作成しました。

このプログラム自体はネットワーク通信を行いません。別のツールなどで測定した応答時間を入力して分析します。

## 主な機能

- 測定ごとの応答時間の入力（応答がなかった場合は -1）
- 測定結果の一覧表示
- 成功率・応答なし率の計算
- 最短・最長・平均応答時間の計算
- 応答時間の標準偏差の計算
- 隣接する成功測定間の応答時間の平均変動を計算
- 測定結果のCSVファイルへの保存

## 使用技術

- C++17
- std::vector：測定データの管理
- struct：測定データの保持
- 関数：入力処理、結果表示、集計、CSV保存の処理分割
- std::ofstream：CSVファイルへの出力
- 標準ライブラリによる数値計算・文字列処理

## 必要なもの

- C++17に対応したコンパイラ
- 例：GCC / MinGW-w64、またはVisual StudioのMSVC

VS Codeを使用する場合も、C++コンパイラは別途インストールする必要があります。

## コンパイルと実行

### GCC / MinGW-w64

```bash
g++ -std=c++17 -Wall -Wextra -pedantic NetworkQualityAnalyzer.cpp -o NetworkQualityAnalyzer
./NetworkQualityAnalyzer
