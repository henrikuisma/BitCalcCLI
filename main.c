#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "--show") != 0) {
        printf("Usage: %s --show <number>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[2]);
    show_number(num);
    return 0;
}