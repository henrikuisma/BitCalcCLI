#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

/* -------------------- Output -------------------- */

/* Smallest of 4/8/16/32/64 bits that fits the value (two's complement for negatives). */
int display_bits(uint64_t v) {
    int needed = 0;
    while (v) {
        needed++;
        v >>= 1;
    }
    int width = 4;
    while (width < needed)
        width <<= 1;
    return width;
}

void print_binary(uint64_t v, int width) {
    printf("0b");
    for (int i = width - 1; i >= 0; i--)
        putchar('0' + (int)((v >> i) & 1u));
}

void print_hex(uint64_t v, int width) {
    printf("0x%0*" PRIX64, width / 4, v);
}

void print_decimal(FILE *f, Value v) {
    if (v.is_int)
        fprintf(f, "%" PRId64, v.i);
    else
        fprintf(f, "%.15g", v.d);
}

void show_number(int64_t number) {
    uint64_t u = (uint64_t)number;      /* negatives show as 64-bit two's complement */
    int width = display_bits(u);

    printf("Decimal: %" PRId64 "\n", number);
    printf("Binary : ");
    print_binary(u, width);
    printf("\nHex    : ");
    print_hex(u, width);
    printf("\n");
}

/* -------------------- Parsing -------------------- */

bool parse_value(const char *str, Value *out) {
    const char *p = str;
    bool negative = false;
    char *end;

    if (*p == '-' || *p == '+') {
        negative = (*p == '-');
        p++;
    }

    int base = 0;
    if (p[0] == '0' && (p[1] == 'b' || p[1] == 'B'))
        base = 2;
    else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
        base = 16;

    errno = 0;

    if (base) {
        p += 2;
        if (!isxdigit((unsigned char)*p))   /* also rejects whitespace and signs */
            return false;
        uint64_t u = strtoull(p, &end, base);
        if (end == p || *end != '\0' || errno == ERANGE)
            return false;
        if (negative)
            u = 0 - u;                      /* unsigned wrap-around is well defined */
        out->is_int = true;
        out->i = (int64_t)u;                /* 0xFFFFFFFFFFFFFFFF == -1 */
        out->d = (double)out->i;
        return true;
    }

    /* Decimal: must start with a digit or ".digit" (rejects "inf", "nan", " 5", ...) */
    if (!isdigit((unsigned char)*p) && !(*p == '.' && isdigit((unsigned char)p[1])))
        return false;

    if (strpbrk(p, ".eE")) {
        double d = strtod(str, &end);
        if (*end != '\0' || errno == ERANGE || !isfinite(d))
            return false;
        out->is_int = false;
        out->i = 0;
        out->d = d;
    } else {
        int64_t v = strtoll(str, &end, 10);
        if (*end != '\0' || errno == ERANGE)
            return false;
        out->is_int = true;
        out->i = v;
        out->d = (double)v;
    }
    return true;
}

bool value_as_int(Value v, int64_t *out) {
    if (v.is_int) {
        *out = v.i;
        return true;
    }
    /* Range check first: casting an out-of-range double to int64_t is undefined. */
    if (!(v.d >= -9223372036854775808.0 && v.d < 9223372036854775808.0))
        return false;
    int64_t t = (int64_t)v.d;
    if ((double)t != v.d)
        return false;
    *out = t;
    return true;
}

/* -------------------- Arithmetic -------------------- */

static double to_double(Value v) {
    return v.is_int ? (double)v.i : v.d;
}

static CalcStatus calc_double(double x, double y, char op, Value *out) {
    double r;
    switch (op) {
        case '+': r = x + y; break;
        case '-': r = x - y; break;
        case '*': r = x * y; break;
        case '/':
            if (y == 0.0)
                return CALC_ERR_DIV_ZERO;
            r = x / y;
            break;
        default:
            return CALC_ERR_BAD_OP;
    }
    if (!isfinite(r))
        return CALC_ERR_OVERFLOW;
    out->is_int = false;
    out->i = 0;
    out->d = r;
    return CALC_OK;
}

/* __builtin_*_overflow: GCC/Clang. */
CalcStatus calculate(Value a, Value b, char op, Value *out) {
    if (a.is_int && b.is_int) {
        int64_t r;
        switch (op) {
            case '+':
                if (__builtin_add_overflow(a.i, b.i, &r)) return CALC_ERR_OVERFLOW;
                break;
            case '-':
                if (__builtin_sub_overflow(a.i, b.i, &r)) return CALC_ERR_OVERFLOW;
                break;
            case '*':
                if (__builtin_mul_overflow(a.i, b.i, &r)) return CALC_ERR_OVERFLOW;
                break;
            case '/':
                if (b.i == 0)
                    return CALC_ERR_DIV_ZERO;
                if (a.i == INT64_MIN && b.i == -1)
                    return CALC_ERR_OVERFLOW;
                if (a.i % b.i != 0)         /* inexact: 7 / 2 = 3.5 */
                    return calc_double((double)a.i, (double)b.i, op, out);
                r = a.i / b.i;
                break;
            default:
                return CALC_ERR_BAD_OP;
        }
        out->is_int = true;
        out->i = r;
        out->d = (double)r;
        return CALC_OK;
    }
    return calc_double(to_double(a), to_double(b), op, out);
}

const char *calc_strerror(CalcStatus status) {
    switch (status) {
        case CALC_OK:           return "ok";
        case CALC_ERR_DIV_ZERO: return "division by zero";
        case CALC_ERR_OVERFLOW: return "result out of range";
        case CALC_ERR_BAD_OP:   return "unknown operator (use + - * /)";
    }
    return "unknown error";
}
