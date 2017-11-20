# Synchronous Transmitter / Receiver

A mockup of a synchronous transmitter (server) and receiver (client)

## Protocol

The "client" reads a file and uploads it to the "server". After the server receives the file, it responds to the
client with a message containing the length of the received payload. To signal the end of a transmission, each
endpoint on the network send a frame containing an `ETB` byte (hex `0x17`). 

The client and server pick either a hamming code or CRC for error correction and detection.

### Hamming

| Byte 0 | Byte 1 | Bytes 2..(2 *`<len>`) | Byte 2 + (2 *`<len>`) + 1 |
| ------ | ------ | --------------------- | ------------------------- |
| `0x16` | Payload Length in bytes (`<len>`) | Payload bytes encoded with hamming-7-4 | `0x16` |

For simplicity, we just encode each nibble in each payload btye with hamming-7-4:

```c
// MSB of each byte is reserved for parity
// for simplicity, we just do hamming 7-4 twice which still allows
// us to do an overall parity bit

// 15 14 13 12 11 10 09 08    07 06 05 04 03 02 01 00
//                            00 d6 d5 d4 d3 d2 d1 d0
//
//                     expands to
//
// __ p1 p2 00 p4 d6 d5 d4    __ p1 p2 d3 p4 d2 d1 d0
//    xx    xx    xx    xx       xx    xx    xx    xx
//       xx xx       xx xx          xx xx       xx xx
//             xx xx xx xx                xx xx xx xx
```

In the frame, the high byte is placed first.

### CRC

| Byte 0 | Byte 1 | Bytes 2..`<len>` | Byte 2 + `<len>` + 1..2 | Byte 2 + `<len>` + 3 |
| ------ | ------ | ---------------- | ----------------------- | -------------------- |
| `0x16` | Payload Length in bytes (`<len>`) | Payload bytes | The CRC-16-ANSI checksum of the payload. High byte first | `0x16` |

A lookup table is used to generate the CRC-16-ANSI checksum of the payload bytes. The high byte of the checksum
appears first in the frame.
