#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "calc.h"

static void print_help(void) {
    printf("\nUsage:\n");
    printf("  calc --show <number>                   Show number in decimal, binary and hex\n");
    printf("  calc <a> <op> <b>                      Calculate (+, -, *, /; 'x' also means multiply)\n");
    printf("  calc <a> <op> <b> --format <dec|bin|hex>\n");
    printf("                                         Show the result in one format only\n");
    printf("  calc --showcalc <a> <op> <b>           Show the calculation in all formats\n");
    printf("  calc --help                            Show this help\n\n");
    printf("Numbers: decimal (255), binary (0b1010), hex (0xFF), optional sign.\n");
    printf("Note: '*' must be quoted or escaped in the shell ('*' or \\*), or use 'x'.\n\n");
}

static void print_usage_error(void) {
    fprintf(stderr, "Invalid input. Use --help for command list.\n");
}

/* Parses "<a> <op> <b>". Prints an error and returns false on failure. */
static bool parse_expression(const char *sa, const char *sop, const char *sb,
                             Value *a, char *op, Value *b) {
    if (!parse_value(sa, a)) {
        fprintf(stderr, "Error: invalid number '%s'\n", sa);
        return false;
    }
    if (!parse_value(sb, b)) {
        fprintf(stderr, "Error: invalid number '%s'\n", sb);
        return false;
    }
    if (strlen(sop) != 1 || !strchr("+-*/xX", sop[0])) {
        fprintf(stderr, "Error: invalid operator '%s' (use + - * /)\n", sop);
        return false;
    }
    *op = (sop[0] == 'x' || sop[0] == 'X') ? '*' : sop[0];
    return true;
}

static int cmd_show(const char *s) {
    Value v;
    int64_t n;

    if (!parse_value(s, &v)) {
        fprintf(stderr, "Error: invalid number '%s'\n", s);
        return 1;
    }
    if (value_as_int(v, &n)) {
        show_number(n);
    } else {
        printf("Decimal: ");
        print_decimal(stdout, v);
        printf("\n");
    }
    return 0;
}

static int cmd_calc(const char *sa, const char *sop, const char *sb, const char *fmt) {
    Value a, b, r;
    char op;
    int64_t n;

    if (!parse_expression(sa, sop, sb, &a, &op, &b))
        return 1;

    CalcStatus st = calculate(a, b, op, &r);
    if (st != CALC_OK) {
        fprintf(stderr, "Error: %s\n", calc_strerror(st));
        return 1;
    }

    if (fmt == NULL) {                      /* default: everything, if integer */
        if (value_as_int(r, &n)) {
            show_number(n);
        } else {
            printf("Decimal: ");
            print_decimal(stdout, r);
            printf("\n");
        }
        return 0;
    }

    if (strcmp(fmt, "dec") == 0) {
        print_decimal(stdout, r);
        printf("\n");
        return 0;
    }
    if (strcmp(fmt, "bin") != 0 && strcmp(fmt, "hex") != 0) {
        fprintf(stderr, "Error: unknown format '%s' (use dec, bin or hex)\n", fmt);
        return 1;
    }
    if (!value_as_int(r, &n)) {
        fprintf(stderr, "Error: result ");
        print_decimal(stderr, r);
        fprintf(stderr, " is not an integer, cannot show as %s\n", fmt);
        return 1;
    }

    uint64_t u = (uint64_t)n;
    int width = display_bits(u);
    if (strcmp(fmt, "bin") == 0)
        print_binary(u, width);
    else
        print_hex(u, width);
    printf("\n");
    return 0;
}

static int cmd_showcalc(const char *sa, const char *sop, const char *sb) {
    Value a, b, r;
    char op;
    int64_t x, y, z;

    if (!parse_expression(sa, sop, sb, &a, &op, &b))
        return 1;

    CalcStatus st = calculate(a, b, op, &r);
    if (st != CALC_OK) {
        fprintf(stderr, "Error: %s\n", calc_strerror(st));
        return 1;
    }

    if (!(value_as_int(a, &x) && value_as_int(b, &y) && value_as_int(r, &z))) {
        printf("Decimal: ");
        print_decimal(stdout, a);
        printf(" %c ", op);
        print_decimal(stdout, b);
        printf(" = ");
        print_decimal(stdout, r);
        printf("\n");
        fprintf(stderr, "Note: binary and hex are only shown for integer calculations.\n");
        return 0;
    }

    uint64_t ux = (uint64_t)x, uy = (uint64_t)y, uz = (uint64_t)z;

    /* One common width so the columns line up */
    int width = display_bits(ux);
    if (display_bits(uy) > width) width = display_bits(uy);
    if (display_bits(uz) > width) width = display_bits(uz);

    printf("Decimal: %" PRId64 " %c %" PRId64 " = %" PRId64 "\n", x, op, y, z);

    printf("Binary : ");
    print_binary(ux, width);
    printf(" %c ", op);
    print_binary(uy, width);
    printf(" = ");
    print_binary(uz, width);
    printf("\n");

    printf("Hex    : ");
    print_hex(ux, width);
    printf(" %c ", op);
    print_hex(uy, width);
    printf(" = ");
    print_hex(uz, width);
    printf("\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: calc [command] [args] (use --help for full command list)\n");
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }
    if (strcmp(argv[1], "--show") == 0) {
        if (argc != 3) {
            print_usage_error();
            return 1;
        }
        return cmd_show(argv[2]);
    }
    if (strcmp(argv[1], "--showcalc") == 0) {
        if (argc != 5) {
            print_usage_error();
            return 1;
        }
        return cmd_showcalc(argv[2], argv[3], argv[4]);
    }
    if (argc == 4)
        return cmd_calc(argv[1], argv[2], argv[3], NULL);
    if (argc == 6 && strcmp(argv[4], "--format") == 0)
        return cmd_calc(argv[1], argv[2], argv[3], argv[5]);

    print_usage_error();
    return 1;
}
