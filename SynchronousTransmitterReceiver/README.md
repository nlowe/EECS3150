# Synchronous Transmitter / Receiver

A mockup of a synchronous transmitter (server) and receiver (client)

## Protocol

There is no real protocol for this project. The "server" takes as input a file or string from standard input.
It frames the payload and "encodes" it into ascii `0` and `1` bytes with a single parity bit. This project
only accepts as input bytes from `0x00` to `0x7F` to conform with the project spec. An example "frame" is below:

| Byte 0 | Byte 1 | Bytes 2..`<len>` | Byte 2 + `<len>` + 1 |
| ------ | ------ | ---------------- | -------------------- |
| `0x16` | Payload Length in bytes (`<len>`) | Payload bytes | `0x16` |

The seven least-significant bits in each byte in the frame are expanded into an ascii '0' or '1'. The most-significant
bit is replaced with an odd-parity bit.

To test parity, you can optionally pass the data through the `faulty-wire` program to flip one or more "bits"
before handing off the data to the "client".
