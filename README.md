# MML Transposer Win32 CUI (MMLTRC32)

A Win32 console edition of the MML Transposer tool.
It transposes plain‑text MML source files used in NSF (NES Sound Format) music creation.
This program does not process NSF data directly.

This edition uses the 32‑bit engine `mmleng32.c`.

## Supported Environment

- Windows 95 and later Win32 console environments
- Verified on Windows 95 (Virtual PC 2007) and Windows 7 Professional

## Usage

```bash
mmltrc32 [options] <input.mml> [shift] [output.mml]
```

## Options

- `-i <file>` — Specify input file.
- `-o <file>` — Specify output file.
- `-s <shift>` — Transpose amount (-12 to +12). `0` means no transpose. Leading `+` is optional.
- `-p`, `--pure` — Pure mode (no formatting).
- `-f`, `--fmt` — FMT mode (formatted output).
- `-r`, `--relative` — Relative octave mode (`<>`).
- `-a`, `--absolute` — Absolute octave mode (`oX`).
- `-d`, `--dch` — Transpose D‑channel (noise channel included). Octave is fixed to `o0`.
- `-h`, `--help` — Show English help.
- `-hjp` — Show Japanese help.

## Notes

- This program transposes MML source code used for NSF (NES Sound Format) music creation.
- For detailed help, use:
  - `mmltrc32 -h | more` — English help
  - `mmltrc32 -hjp | more` — Japanese help

## Examples

```bash
mmltrc32 input.mml
mmltrc32 input.mml output.mml
mmltrc32 input.mml -s 0
mmltrc32 input.mml 5
mmltrc32 input.mml -2 output.mml -p
mmltrc32 input.mml +3 output.mml -p -a
mmltrc32 input.mml +7 output.mml -f
mmltrc32 input.mml -5 output.mml -f -r -d
```

## Mode Description

### Pure / FMT Mode
Notes and octaves are automatically reassigned while preserving the original intent of the MML.
- `-p` / `--pure` — no formatting  
- `-f` / `--fmt` — formatted output

### Octave Mode
- `-r` / `--relative` — relative octave (`<>`)  
- `-a` / `--absolute` — absolute octave (`oX`)  
These can be combined with Pure / FMT modes.

### D‑Channel Transpose
- `-d` / `--dch` — transposes the D‑channel (noise channel included).  
- The D‑channel octave is always fixed to `o0`.

## Notes

- This program transposes MML source code used for NSF (NES Sound Format) music creation.
- For detailed help, use:  
  `mmltrc32 -h | more`

All source code in this project was created with the assistance of Copilot.
Unexpected issues may occur.
