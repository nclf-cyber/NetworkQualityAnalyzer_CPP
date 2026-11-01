# 通信品質分析ツール (C++)

## 概要

通信テストなどで得た応答時間を入力し、結果を集計するC++のコンソールプログラムです。

- 測定ごとの応答時間を入力（応答がない場合は `-1`）
- 成功率・応答なし率を計算
- 最短・最長・平均応答時間を計算
- 応答時間の標準偏差と、連続した成功測定間の平均変動を計算
- 測定結果をCSVファイルに保存

> このプログラム自体がネットワーク通信を実行するわけではありません。別途測定した値を入力して分析します。PlayStation Portalのストリーミング品質を直接測定・保証するものではありません。

## 必要なもの

- C++コンパイラ（C++17対応）
- 例：GCC / MinGW-w64、またはVisual StudioのMSVC

VS CodeだけではC++コンパイラは含まれないため、コンパイラを別途インストールしてください。

## コンパイルと実行

### GCC / MinGW-w64

```bash
g++ -std=c++17 -Wall -Wextra -pedantic NetworkQualityAnalyzer.cpp -o NetworkQualityAnalyzer
./NetworkQualityAnalyzer
```

Windows PowerShellでは、コンパイル後に次で実行できます。

```powershell
.\NetworkQualityAnalyzer.exe
```

### Visual Studio Developer Command Prompt (MSVC)

```bat
cl /std:c++17 /EHsc /W4 NetworkQualityAnalyzer.cpp
NetworkQualityAnalyzer.exe
```

## 入力例

```text
測定 1 回目の応答時間 (ms / 応答なし=-1 / 終了=q): 24.5
測定 2 回目の応答時間 (ms / 応答なし=-1 / 終了=q): 31
測定 3 回目の応答時間 (ms / 応答なし=-1 / 終了=q): -1
測定 4 回目の応答時間 (ms / 応答なし=-1 / 終了=q): 27.8
測定 5 回目の応答時間 (ms / 応答なし=-1 / 終了=q): q
```

## 学習できるC++の要素

- `struct` によるデータのまとめ方
- `std::vector` によるデータ管理
- 関数による処理の分割
- `if` / `while` / `for` による条件分岐と繰り返し
- ファイル出力（`std::ofstream`）
- 平均・標準偏差などの基本的なデータ集計

## 注意

このプログラムは学習用の簡易ツールです。出力される統計値は入力データの集計であり、ネットワーク品質の合否判定やPlayStationの内部品質基準を示すものではありません。
