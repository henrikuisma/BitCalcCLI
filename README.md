# BitCalcCLI

A lightweight **CLI calculator** for performing arithmetic and converting numbers between **decimal, binary, and hexadecimal** formats.

## Features

- Evaluate basic arithmetic expressions: `+`, `-`, `*` (or `x`), `/`
- Parse and convert numbers in **decimal**, **binary** (`0b` prefix), and **hexadecimal** (`0x` prefix)
- Show a number in all formats at once or in a chosen format
- **Show calculation**: see how the operation is performed in all formats (decimal, binary, hex)
- Exact 64-bit integer arithmetic with overflow and division-by-zero detection
- Clear error messages and exit codes

## Build

```bash
make    # builds ./calc
```

## Commands

| Command | Description | Example |
|---------|-------------|---------|
| `--show <number>` | Show a single number in all formats (dec, bin with `0b`, hex with `0x`) | `$ calc --show 123` |
| `<a> <op> <b>` | Evaluate a simple arithmetic expression (`+`, `-`, `*`, `/`). Numbers can be decimal, binary (`0b`) or hexadecimal (`0x`). Integer results are shown in all formats | `$ calc 0b1011 + 0b110` → `Decimal: 17`, `Binary : 0b00010001`, `Hex    : 0x11` |
| `<a> <op> <b> --format <bin\|hex\|dec>` | Show only the result, in a specific format | `$ calc 10 + 5 --format bin` → `0b1111` |
| `--showcalc <a> <op> <b>` | Show calculation steps in all formats, including prefixes | `$ calc --showcalc 5 + 3` → see below |
| `--help` | Show usage information | `$ calc --help` |

### Notes

- Numbers can be entered in **decimal**, **binary** (`0b1010`), or **hexadecimal** (`0xA`), with an optional sign.
- Binary and hexadecimal inputs are automatically recognized by their prefixes. Values up to 64 bits are accepted, so `0xFFFFFFFFFFFFFFFF` is `-1`.
- Binary and hex output is padded to 4, 8, 16, 32 or 64 bits (the smallest that fits). Negative numbers are shown as 64-bit two's complement.
- Integer operands are calculated with exact 64-bit arithmetic; overflow is reported as an error. `7 / 2` gives `3.5`, while `8 / 2` gives the integer `4`.
- Operands with a fraction (`1.5`, `2e3`) are calculated as floating point. Binary and hex are only shown for integer values.
- The shell treats `*` specially: write `'*'` or `\*`, or use `x` instead (`calc 0xA x 3`).

## Usage Examples

```bash
# Show a number in all formats
$ calc --show 123
Decimal: 123
Binary : 0b01111011
Hex    : 0x7B

# Perform an arithmetic operation
$ calc 0b1011 + 0b110 --format dec
17

$ calc 0xA \* 0x3 --format bin
0b00011110

# Show calculation steps
$ calc --showcalc 5 + 3
Decimal: 5 + 3 = 8
Binary : 0b0101 + 0b0011 = 0b1000
Hex    : 0x5 + 0x3 = 0x8

# Errors go to stderr
$ calc 1 / 0
Error: division by zero
```