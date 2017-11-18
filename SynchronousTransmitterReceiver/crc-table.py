#!/usr/bin/env python3

WIDTH = 16
POLYNOMIAL = 0xA001

lookup = []


def crc16ansi(data, sz):
    r = 0xFFFF
    for idx in range(sz):
        r = (r >> 8) ^ lookup[(r ^ (ord(data[idx]) & 0xff)) & 0xff]

    return r & 0xffff


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

        lookup.append(remainder & 0xFFFF)
        line += "{0:#0{1}x}, ".format(remainder, 6)
        if (i+1) % 8 == 0:
            print(line.rstrip(" "))
            line = "\t"
    print("};")

    print("CRC-16-ANSI of 'test' is: {0:#0{1}x} (should be 0xdc2e)".format(crc16ansi('test', 4), 6))
