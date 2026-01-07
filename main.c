#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"


int main(int argc, char* argv[]) {

    // Check if enough arguments are provided
    if (argc < 2) {
        printf("Usage: calc [command] [args] (use --help for full command list)\n");
        return 1;
    }

    // --help
    if (argc >= 2 && strcmp(argv[1], "--help") == 0) {

        printf("\nCommands:\n");
        printf("  --show <number>                  Show number in decimal, binary, and hex\n");
        printf("  <number> <op> <number>           Perform arithmetic operation (+, -, *, /)\n");
        printf("                                   Note: use '*' for multiplication (quote or escape in shell)\n");
        printf("  <number> <op> <number> --format <dec|bin|hex>  Show result in specific format\n");
        printf("TODO  --showcalc <number> <op> <number>  Show calculation steps\n\n");

        return 0;
    }
 
    // --show <number>
    if (argc == 3 && strcmp(argv[1], "--show") == 0) {
        int64_t num = parse_number(argv[2]);
        show_number(num);
        return 0;
    }
    // <number> <op> <number>
    if (argc >= 4) {
        char op = argv[2][0];
        double a = parse_number_double(argv[1]);
        double b = parse_number_double(argv[3]);
        double result = calculate_double(a, b, op);

        // Checks for optional --format
        if (argc == 6 && strcmp(argv[4], "--format") == 0) {
            if (strcmp(argv[5], "dec") == 0) {
                printf("%g\n", result);
            } else if ((strcmp(argv[5], "bin") == 0 || strcmp(argv[5], "hex") == 0) 
                        && result == (int64_t)result) {
                // Only show bin/hex if result is integer
                if (strcmp(argv[5], "dec") == 0) {
                    printf("%lld\n", (int64_t)result);
                
                } else if (strcmp(argv[5], "bin") == 0) {
                    print_binary((int64_t)result);
                
                } else if (strcmp(argv[5], "hex") == 0) {
                    printf("0x%llX\n", (int64_t)result);
                }
            } else {
                // fallback: decimal for non-integer result with bin/hex
                printf("%g\n", result);
            }
        } else {
            // Default behavior: show full number if integer, decimal if not
            if (result == (int64_t)result)
                show_number((int64_t)result);
            else
                printf("Decimal: %g\n", result);
        }

        return 0;
    }


    printf("Invalid input. Use --help for command list.\n");
    return 1;

}