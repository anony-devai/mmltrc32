# MML Transposer Win32 Console (MMLTRC32)

NSF 用 MML ファイル移調ツール「MML Transposer」の  
Win32 コンソール版（CUI版）です。

## 対応環境

- Windows 95 以降の Win32 コンソール環境

## エンジン構成

- 32bit 版エンジン `mmleng32.c` を使用しています。

## 使い方

```bash
mmltrc32 [options] <input.mml> [shift] [output.mml]
```

### オプション

- `-i <file>` 入力ファイル  
- `-o <file>` 出力ファイル  
- `-s <shift>` 移調量（-12～+12、0 は移調なし、`+` は省略可）  
- `-p`, `--pure` Pure モード（整形なし）  
- `-f`, `--fmt` FMT モード（整形あり）  
- `-r`, `--relative` 相対音域指定（先頭のみ `oX`、以降は `<` `>`）  
- `-a`, `--absolute` 絶対音域指定（すべて `oX`）  
- `-d`, `--dch` D チャンネル移調（ノイズ ch も移調量に応じて移調）  
- `-h`, `--help` ヘルプを表示

### 使用例

```bash
mmltrc32 input.mml
# → そのまま標準出力へ出力（移調なし）

mmltrc32 input.mml output.mml
# → そのまま output.mml へコピー（移調なし）

mmltrc32 input.mml -s 0
# → 移調量 0 で処理（モード省略時は Pure）結果を標準出力へ

mmltrc32 input.mml 5
# → +5 移調して標準出力へ（-s は省略可）

mmltrc32 input.mml -2 output.mml -p
# → -2 移調 / Pure（整形なし）/ 音域は自動振り直し

mmltrc32 input.mml +3 output.mml -p -a
# → +3 移調 / Pure / 絶対音域[oX]

mmltrc32 input.mml +7 output.mml -f
# → +7 移調 / FMT（整形あり）/ 音域は自動振り直し

mmltrc32 input.mml -5 output.mml -f -r -d
# → -5 移調 / FMT / 相対音域[<>] / Dチャンネルも移調
```

### モード説明

- **Pure / FMT モード**  
  元の MML の意図を保ちながら、音符とオクターブを自動的に振り直します。  
  - `-p` / `--pure` 整形なし  
  - `-f` / `--fmt` 整形あり  

- **音域指定**  
  - `-r` / `--relative` 相対音域（`<>`）  
  - `-a` / `--absolute` 絶対音域（`oX`）  
  これらは `-p` / `-f` と組み合わせて使用できます。

- **D チャンネル移調**  
  - `-d` / `--dch` を指定すると、ノイズチャンネルも移調します。  
  - オクターブは `o0` 固定です。

## 備考

このプログラムは NSF 用 MML 移調ツールです。  
ヘルプは `mmltrc32 -h | more` でページ送りしながら読むことを推奨します。

このコードは全て Copilot を活用して作成しています。  
予期せぬ不具合はご容赦ください。
