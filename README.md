# BitCalcCLI

A lightweight **CLI calculator** for performing arithmetic and converting numbers between **decimal, binary, and hexadecimal** formats.

## Features

- Evaluate basic arithmetic expressions: `+`, `-`, `*`, `/`  
- Parse and convert numbers in **decimal**, **binary** (`0b` prefix), and **hexadecimal** (`0x` prefix)  
- Show a number in all formats at once or in a chosen format  
- TODO! **Show calculation**: see how the operation is performed in all formats (decimal, binary, hex)

## Commands

| Command | Description | Example |
|---------|-------------|---------|
| `--show <number>` | Show a single number in all formats (dec, bin with `0b`, hex with `0x`) | `$ calc --show 123` |
| `--format <bin\|hex\|dec>` | Show the result in a specific format | `$ calc "10 + 5" --format bin` → `0b1111` |
| `<expr>` | Evaluate a simple arithmetic expression (`+`, `-`, `*`, `/`). Numbers can be decimal, binary (`0b`) or hexadecimal (`0x`) | `$ calc "0b1011 + 0b110"` → `17` |
| `--showcalc` | Show calculation steps in all formats, including prefixes | `$ calc "0b101 + 0b11" --showcalc` → `Decimal: 5 + 3 = 8, Binary: 0b101 + 0b011 = 0b1000, Hex: 0x5 + 0x3 = 0x8` |
| `--help` | Show usage information | `$ calc --help` |

### Notes

- Numbers can be entered in **decimal**, **binary** (`0b1010`), or **hexadecimal** (`0xA`).  
- Binary and hexadecimal inputs are automatically recognized by their prefixes.  
- Works the same way in **bash** and **zsh**.  

## Usage Examples

```bash
# Show a number in all formats
$ calc --show 123
Decimal: 123
Binary : 0b1111011
Hex    : 0x7B

# Perform an arithmetic operation
$ calc 0b1011 + 0b110 --format dec
17

$ calc 0xA \* 0x3 --format bin
0b11110

# Show calculation steps
$ calc --showcalc 5 + 3
Decimal: 5 + 3 = 8
Binary : 0b101 + 0b011 = 0b1000
Hex    : 0x5 + 0x3 = 0x8
