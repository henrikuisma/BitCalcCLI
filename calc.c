#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

// -------------------- Show numbers --------------------
void show_number(int64_t number) {

    printf("Decimal: %lld\n", number);  // Print decimal value

    uint64_t unsigned_number = (uint64_t)number;    // For negative numbers, shows 2's complement binary

    // Count the number of significant bits
    int bits_needed = (unsigned_number == 0) ? 1 : 64 - __builtin_clzll(unsigned_number);  

    // Round up to the next power-of-2 for display (4, 8, 16, 32, 64)
    int display_bits = 4;
    while (display_bits < bits_needed)
        display_bits <<= 1;  // Multiply by 2

    // Print binary representation
    printf("Binary : 0b");
    for (int i = display_bits - 1; i >= 0; i--) {
        printf("%llu", (unsigned_number >> i) & 1ULL);  // Extract each bit
    }
    printf("\n");

    // Print hexadecimal representation
    printf("Hex    : 0x%08llX\n", unsigned_number & 0xFFFFFFFF);
    
}

void show_number_double(double number) {

    printf("Decimal: %g\n", number);    // Always show decimal

    if (number == (int64_t)number) {    // Only show binary/hex if number is integer
        show_number((int64_t)number);   // Reuse integer function
    }
}

void print_binary(int64_t number) {

    uint64_t u = (uint64_t)number;

    int bits_needed = (u == 0) ? 1 : 64 - __builtin_clzll(u);

    int width = 4;
    while (width < bits_needed)
        width <<= 1;

    printf("0b");
    for (int i = width - 1; i >= 0; i--) {
        printf("%llu", (u >> i) & 1ULL);
    }
    printf("\n");
}


// -------------------- Parsing --------------------
int64_t parse_number(const char* str) {

    if (strncmp(str, "0b", 2) == 0)         // Checks for binary prefix and converts binary string to number
        return (int64_t)strtoull(str + 2, NULL, 2);
    else if (strncmp(str, "0x", 2) == 0)    // Same with hex
        return (int64_t)strtoull(str + 2, NULL, 16);
    else                                    // Otherwise, parse as decimal
        return strtoull(str, NULL, 10);

}

double parse_number_double(const char* str) {

    if (strncmp(str, "0b", 2) == 0)
        return (double)strtoull(str + 2, NULL, 2);
    else if (strncmp(str, "0x", 2) == 0)
        return (double)strtoull(str + 2, NULL, 16);
    else
        return strtod(str, NULL);          // Supports decimal with fraction
}

// -------------------- Arithmetic --------------------
int64_t calculate_int(int64_t a, int64_t b, char op) {

    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return 0;
    }

}

double calculate_double(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0.0 ? a / b : 0.0;
        default: return 0.0;
    }
}