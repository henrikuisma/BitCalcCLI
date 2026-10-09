#ifndef CALC_H
#define CALC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* A parsed number: either an exact 64-bit integer or a double. */
typedef struct {
    bool    is_int;
    int64_t i;      /* valid when is_int */
    double  d;      /* valid when !is_int */
} Value;

typedef enum {
    CALC_OK = 0,
    CALC_ERR_DIV_ZERO,
    CALC_ERR_OVERFLOW,
    CALC_ERR_BAD_OP
} CalcStatus;

/* Parsing: decimal, 0b binary, 0x hex, optional sign. Returns false on bad input. */
bool parse_value(const char *str, Value *out);

/* Returns true if v is exactly representable as int64_t (writes it to *out). */
bool value_as_int(Value v, int64_t *out);

/* Arithmetic: integers stay integers (checked for overflow); otherwise doubles. */
CalcStatus calculate(Value a, Value b, char op, Value *out);
const char *calc_strerror(CalcStatus status);

/* Output helpers. width is in bits (see display_bits). */
int  display_bits(uint64_t v);              /* 4, 8, 16, 32 or 64 */
void print_binary(uint64_t v, int width);   /* 0b0101, no newline */
void print_hex(uint64_t v, int width);      /* 0x5,    no newline */
void print_decimal(FILE *f, Value v);       /* no newline */
void show_number(int64_t number);           /* Decimal / Binary / Hex lines */

#endif
