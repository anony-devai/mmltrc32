/* ==================================================================
 * mmltrc32.c  (MML Transposer Win32 CUI - Win32 Console Application)
 * ================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmleng32.h"

char text[MAX_TEXT];
char outbuf[MAX_OUT];

/* Check if the argument is an integer (with optional +/- sign) */
int is_integer_arg(const char* s)
{
    int i = 0;

    if (s[0] == '+' || s[0] == '-') {
        i = 1;
    }

    if (s[i] == '\0') return 0;

    for (; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }

    return 1;
}

/* Short usage help */
void print_usage(void) {
    fprintf(stdout,
        "Usage:\n"
        "  mmltrc32 [options] <input.mml> [shift] [output.mml]\n"
        "\n"
        "Options:           (all optional)\n"
        "  -i <file>        Specify input file\n"
        "  -o <file>        Specify output file\n"
        "  -s <shift>       Transpose amount (-12 to +12)\n"
        "  -p, --pure       Pure mode (no formatting)\n"
        "  -f, --fmt        FMT mode (formatted output)\n"
        "  -r, --relative   Relative octave mode\n"
        "  -a, --absolute   Absolute octave mode\n"
        "  -d, --dch        Transpose D-channel (noise channel included)\n"
        "\n"
        "== MML Transposer Win32 CUI - Win32 Console Application ==\n"
        "\n"
        "This program transposes MML source code written for NSF music.\n"
        "It does not handle NSF files.\n"
        "\n"
        "For detailed help, use:\n"
        "  mmltrc32 -h | more    English  help (/h, /? also supported)\n"
        "  mmltrc32 -hjp | more  Japanese help (/hjp also supported)\n"
    );
}

/* Detailed help (for use with -h, /h, /?) */
void print_help_detail(void)
{
    fprintf(stdout,
        "Usage:"
        "  mmltrc32 [options] <input.mml> [shift] [output.mml]\n"
        "\n"
        "Options:           (all optional)\n"
        "  -i <file>        Specify input file\n"
        "  -o <file>        Specify output file\n"
        "  -s <shift>       Transpose amount (-12 to +12).\n"
        "                   0 means no transpose (leading '+' is optional).\n"
        "  -p, --pure       Pure mode (no formatting)\n"
        "  -f, --fmt        FMT mode (formatted output)\n"
        "  -r, --relative   Relative octave mode\n"
        "                   (first note uses oX, subsequent notes use < >)\n"
        "  -a, --absolute   Absolute octave mode (all notes use oX)\n"
        "  -d, --dch        Transpose D-channel\n"
        "                   (noise channel is also transposed accordingly)\n"
        "  -h, --help       Show this help (/h, /? also supported)\n"
        "\n"
        "Examples:\n"
        "  mmltrc32 input.mml\n"
        "         -> Output directly to standard output\n"
        "            (no transpose, just display)\n"
        "\n"
        "  mmltrc32 input.mml output.mml\n"
        "         -> Copy input.mml to output.mml\n"
        "            (no transpose, simple file copy)\n"
        "\n"
        "  mmltrc32 input.mml -s 0\n"
        "         -> Process with shift = 0 and output to standard output\n"
        "            (when mode is omitted, Pure mode is used)\n"
        "\n"
        "  mmltrc32 input.mml 5\n"
        "         -> Transpose by +5 and output to standard output\n"
        "            (-s can be omitted when specifying a bare integer)\n"
        "\n"
        "Mode description:\n"
        "  In Pure / FMT modes, notes and octaves are automatically\n"
        "  reassigned while preserving the original intent of the MML.\n"
        "\n"
        "  -p / -f specify formatting style:\n"
        "     Pure = no formatting\n"
        "     FMT  = formatted output\n"
        "\n"
        "  -r / -a specify octave mode:\n"
        "     Relative octave [<>]\n"
        "     Absolute octave [oX]\n"
        "  These can be combined with Pure / FMT.\n"
        "\n\n"
        "Examples:\n"
        "  mmltrc32 input.mml -2 output.mml -p\n"
        "         -> Transpose by -2, Pure mode (no formatting),\n"
        "            octaves are automatically adjusted\n"
        "\n"
        "  mmltrc32 input.mml +3 output.mml -p -a\n"
        "         -> Transpose by +3, Pure mode (no formatting),\n"
        "            octaves are output in absolute form [oX]\n"
        "\n"
        "  mmltrc32 input.mml +7 output.mml -f\n"
        "         -> Transpose by +7, FMT mode (formatted),\n"
        "            octaves are automatically adjusted\n"
        "\n"
        "  mmltrc32 input.mml -5 output.mml -f -r -d\n"
        "         -> Transpose by -5, FMT mode, relative octave [<>],\n"
        "            and D-channel is also transposed\n"
        "\n"
        "Note:\n"
        "  Even when using -d to transpose the D-channel,\n"
        "  its octave is fixed to o0.\n"
    );
}

// Display Japanese help (-hjp, /hjp)
void print_help_jp(void)
{
    fprintf(stdout,
        "使い方:"
        "  mmltrc32 [options] <input.mml> [shift] [output.mml]\n"
        "\n"
        "オプション:        （すべて省略可）\n"
        "  -i <file>        入力ファイルを指定\n"
        "  -o <file>        出力ファイルを指定\n"
        "  -s <shift>       移調量（-12～+12） 0 は移調なし（+ は省略可）\n"
        "  -p, --pure       Pure モード（整形なし）\n"
        "  -f, --fmt        FMT  モード（整形あり）\n"
        "  -r, --relative   相対音域指定（先頭のみ oX、以降は < >）\n"
        "  -a, --absolute   絶対音域指定（すべて oX）\n"
        "  -d, --dch        D チャンネル移調（ノイズchも移調量に応じて移調）\n"
        "  -h, --help       英語ヘルプを出力（日本語ヘルプは -hjp, /hjp）\n"
        "例:\n"
        "  mmltrc32 input.mml\n"
        "         → そのまま  標準出力  へ 出力  （移調処理なし画面出力）\n"
        "\n"
        "  mmltrc32 input.mml output.mml\n"
        "         → そのまま output.mml へ コピー（移調処理なし単純コピー）\n"
        "\n"
        "  mmltrc32 input.mml -s 0\n"
        "         → 移調量 0 で 処理結果を 標準出力へ 出力（モード省略時は Pure）\n"
        "\n"
        "  mmltrc32 input.mml 5\n"
        "         → +5 移調して 処理結果を 標準出力へ 出力（-s は省略可）\n"
        "\n"
        "モード説明:\n"
        "※ Pure / FMT モードでは、音符とオクターブを\n"
        "   元の MML の意図を保ちながら自動的に振り直します。\n"
        "\n"
        "※ -p / -f は 整形方式（ Pure＝整形なし / FMT＝整形あり ）を指定します。\n"
        "   -r / -a は 音域方式（ 相対音域 [<>]  / 絶対音域 [oX] ）を指定します。\n"
        "   これらは組み合わせて使用できます。\n"
        "\n"
        "例:\n"
        "  mmltrc32 input.mml -2 output.mml -p\n"
        "         → -2 移調して Pure（整形なし）で 音域は 自動振り直しで 出力\n"
        "\n"
        "  mmltrc32 input.mml +3 output.mml -p -a\n"
        "         → +3 移調して Pure（整形なし）で 音域は 絶対音域[oX]で 出力\n"
        "\n"
        "  mmltrc32 input.mml +7 output.mml -f\n"
        "         → +7 移調して FMT （整形あり）で 音域は 自動振り直しで 出力\n"
        "\n"
        "  mmltrc32 input.mml -5 output.mml -f -r -d\n"
        "         → -5 移調して FMT、相対音域[<>]、Dチャンネルも移調して 出力\n"
        "\n"
        "※ -d で D チャンネル移調しても オクターブは o0 固定となります。\n"
    );
}

int main(int argc, char* argv[])
{
    const char* infile = NULL;
    const char* outfile = NULL;

    FILE* fp;
    size_t len;
    size_t written; /* for debug / completeness */

    int shift = 0;
    int shift_specified = 0;

    int fmt_flag  = 0;
    int pure_flag = 0;
    int rel_flag  = 0;
    int abs_flag  = 0;
    int dch_flag  = 0; /* [NEW] flag for -d / --dch (formerly noise_flag) */

    int i;
    int show_help = 0;
    FILE* outfp;

    const char* file_candidates[16];
    int file_count = 0;
    int outlen;

    /* Error detail structure (C89: must be at top of block) */
    MMLErrorInfo err_info;

    /* No arguments -> show usage */
    if (argc < 2) {
        print_usage();
        return 1;
    }

    /* Help has highest priority (also supports DOS-style /h /? /hjp) */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-hjp") == 0 ||
            strcmp(argv[i], "/hjp") == 0) {
            show_help = 2;
            break;
        }
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0 ||
            strcmp(argv[i], "/h") == 0 ||
            strcmp(argv[i], "/?") == 0) {
            show_help = 1;
            break;
        }
    }
    if (show_help == 1) {
        print_help_detail();
        return 0;
    }
    if (show_help == 2){
        print_help_jp();
        return 0;
    }

    /* ------------------------------------------------------------
       Process -i / -o first
       ------------------------------------------------------------ */
    for (i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: Please specify a file name after -i.\n");
                return 1;
            }
            infile = argv[i + 1];
            i++;
            continue;
        }

        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: Please specify a file name after -o.\n");
                return 1;
            }
            outfile = argv[i + 1];
            i++;
            continue;
        }
    }

    /* ------------------------------------------------------------
       Process remaining arguments
       ------------------------------------------------------------ */
    for (i = 1; i < argc; i++) {

        if (argv[i][0] == '-') {

            /* Skip already processed -i / -o and their arguments */
            if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "-o") == 0) {
                i++;
                continue;
            }

            /* Pure / FMT */
            if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pure") == 0) {
                if (fmt_flag) {
                    fprintf(stderr, "Error: --pure and --fmt cannot be used together.\n");
                    return 1;
                }
                pure_flag = 1;
                continue;
            }

            if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fmt") == 0) {
                if (pure_flag) {
                    fprintf(stderr, "Error: --pure and --fmt cannot be used together.\n");
                    return 1;
                }
                fmt_flag = 1;
                continue;
            }

            /* relative / absolute */
            if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--relative") == 0) {
                if (abs_flag) {
                    fprintf(stderr, "Error: -r and -a cannot be used together.\n");
                    return 1;
                }
                rel_flag = 1;
                continue;
            }

            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--absolute") == 0) {
                if (rel_flag) {
                    fprintf(stderr, "Error: -r and -a cannot be used together.\n");
                    return 1;
                }
                abs_flag = 1;
                continue;
            }

            /* [NEW] -d / --dch (D-channel shift) */
            if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dch") == 0) {
                dch_flag = 1;
                continue;
            }

            /* -s <shift> */
            if (strcmp(argv[i], "-s") == 0) {
                long s;
                char* endptr;

                if (i + 1 >= argc) {
                    fprintf(stderr, "Error: Please specify a value after -s.\n");
                    return 1;
                }

                if (shift_specified) {
                    fprintf(stderr, "Error: Shift value specified more than once.\n");
                    return 1;
                }

                s = strtol(argv[i + 1], &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "Error: Shift value must be an integer.\n");
                    return 1;
                }
                if (s < -12 || s > 12) {
                    fprintf(stderr, "Error: Shift value must be between -12 and +12.\n");
                    return 1;
                }

                shift = (int)s;
                shift_specified = 1;
                i++;
                continue;
            }

            /* Bare integer treated as shift */
            if (is_integer_arg(argv[i])) {
                long s;
                if (shift_specified) {
                    fprintf(stderr, "Error: Shift value specified more than once.\n");
                    return 1;
                }
                s = strtol(argv[i], NULL, 10);
                if (s < -12 || s > 12) {
                    fprintf(stderr, "Error: Shift value must be between -12 and +12.\n");
                    return 1;
                }
                shift = (int)s;
                shift_specified = 1;
                continue;
            }

            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            return 1;
        }

        /* Non-option arguments */

        /* Bare integer -> shift */
        if (is_integer_arg(argv[i])) {
            long s;
            if (shift_specified) {
                fprintf(stderr, "Error: Shift value specified more than once.\n");
                return 1;
            }
            s = strtol(argv[i], NULL, 10);
            if (s < -12 || s > 12) {
                fprintf(stderr, "Error: Shift value must be between -12 and +12.\n");
                return 1;
            }
            shift = (int)s;
            shift_specified = 1;
            continue;
        }

        /* Save as file candidate */
        if (file_count < 16) {
            file_candidates[file_count++] = argv[i];
        }
    }

    /* If -i / -o are not specified, infer from remaining arguments */
    if (!infile) {
        if (file_count >= 1) infile = file_candidates[0];
    }

    if (!outfile) {
        if (file_count >= 2) outfile = file_candidates[1];
    }

    if (!infile) {
        fprintf(stderr, "Error: No input file specified.\n");
        print_usage();
        return 1;
    }

    if (outfile && strcmp(infile, outfile) == 0) {
        fprintf(stderr, "Error: Input and output files are the same.\n");
        return 1;
    }

    /* ------------------------------------------------------------
       Read input (with size check tied to MAX_TEXT)
       ------------------------------------------------------------ */
    fp = fopen(infile, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file: %s\n", infile);
        return 1;
    }

    /* File size check */
    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: Cannot get file size: %s\n", infile);
        fclose(fp);
        return 1;
    }

    {
        long fsize = ftell(fp);
        if (fsize < 0) {
            fprintf(stderr, "Error: Cannot get file size: %s\n", infile);
            fclose(fp);
            return 1;
        }

        if (fsize >= MAX_TEXT) {
            fprintf(stderr,
                "Error: Input file is too large (%ld bytes).\n"
                "       Maximum size is %d bytes.\n",
                fsize, MAX_TEXT - 1);
            fclose(fp);
            return 1;
        }

        if (fseek(fp, 0, SEEK_SET) != 0) {
            fprintf(stderr, "Error: Cannot reset file position: %s\n", infile);
            fclose(fp);
            return 1;
        }
    }

    /* Actual read */
    len = fread(text, 1, MAX_TEXT - 1, fp);
    fclose(fp);
    text[len] = '\0';

    /* ------------------------------------------------------------
       Decide whether processing is needed
       (C89: variable declarations at block start)
       ------------------------------------------------------------ */
    {
        int need_process = 0;

        if (shift_specified || fmt_flag || pure_flag || rel_flag || abs_flag || dch_flag) {
            need_process = 1;
        }

        /* No processing needed -> simple copy */
        if (!need_process) {
            outfp = outfile ? fopen(outfile, "w") : stdout;
            if (!outfp) {
                fprintf(stderr, "Error: Cannot open output file.\n");
                return 1;
            }
            fprintf(outfp, "%s", text);
            if (outfile) fclose(outfp);
            return 0;
        }
    }

    /* If shift is not specified, treat as 0 (used as trigger for Pure/FMT) */
    if (!shift_specified) {
        shift = 0;
    }

    /* ------------------------------------------------------------
       Call mml_process()
       (C89: variable declarations at block start)
       ------------------------------------------------------------ */
    {
        int mode;
        int base_mode;

        /* Decide base mode: Pure / FMT */
        if (fmt_flag) {
            base_mode = MODE_FMT;      /* 4 */
        } else {
            base_mode = MODE_PURE;     /* 0 */
        }

        mode = base_mode;

        /* Add Rel / Abs bits */
        if (rel_flag) {
            mode |= 2;                 /* REL bit */
        } else if (abs_flag) {
            mode |= 1;                 /* ABS bit */
        }

        /* Add D-channel shift bit (8) */
        if (dch_flag) {
            mode |= 8;
        }

        /* Pass &err_info as the 6th argument */
        outlen = mml_process(text, shift, mode, outbuf, sizeof(outbuf), &err_info);
    }

    /* Detailed error handling for octave out-of-range (-10), etc. */
    if (outlen < 0) {
        if (outlen == MML_ERR_OCTAVE_OUT_OF_RANGE) {
            fprintf(stderr, "Error: Octave limit exceeded during transpose.\n");
            fprintf(stderr, "       Location: channel '%c', line %d (calculated: o%d)\n",
                    err_info.channel_char, err_info.line_number, err_info.calculated_value);
        } else {
            fprintf(stderr, "Error: MML processing failed (code %d)\n", outlen);
        }
        return 1;
    }

    if (outlen >= (int)sizeof(outbuf)) {
        fprintf(stderr, "Error: Output buffer is too small.\n");
        return 1;
    }
    outbuf[outlen] = '\0';

    outfp = outfile ? fopen(outfile, "w") : stdout;
    if (!outfp) {
        fprintf(stderr, "Error: Cannot open output file.\n");
        return 1;
    }

    written = fwrite(outbuf, 1, outlen, outfp);
    (void)written; /* suppress unused warning if not used */

    if (outfile) {
        fclose(outfp);
    }

    return 0;
}
