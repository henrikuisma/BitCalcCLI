#include <stdio.h>
#include <stdint.h>
#include "calc.h"


void show_number(uint64_t number) {
    printf("Decimal: %llu\n", number);

    printf("Binary : 0b");
    int started = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t bit = (number >> i) & 1ULL;
        if (bit) started = 1;
        if (started) printf("%llu", bit);
    }
    if (!started) printf("0");
    printf("\n");

    printf("Hex    : 0x%llX\n", number);
}