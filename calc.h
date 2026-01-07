#ifndef CALC_H
#define CALC_H

#include <stdint.h>

void show_number(int64_t number);
void show_number_double(double number);
void print_binary(int64_t number);

int64_t parse_number(const char* str);
double parse_number_double(const char* str);

int64_t calculate_int(int64_t a, int64_t b, char op);
double calculate_double(double a, double b, char op);

#endif