#!/usr/bin/env python3

WIDTH = 16
POLYNOMIAL = 0xA001

if __name__ == "__main__":
    print("uint16_t lookup[256] =")
    print("{")
    line = "\t"
    for i in range(256):
        remainder = i

        for b in range(8):
            if remainder & 1 == 1:
                remainder = (remainder >> 1) ^ POLYNOMIAL
            else:
                remainder >>= 1

        line += "{0:#0{1}x}, ".format(remainder, 6)
        if (i+1) % 8 == 0:
            print(line.rstrip(" "))
            line = "\t"
    print("};")
