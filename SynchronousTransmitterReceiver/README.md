# Synchronous Transmitter / Receiver

A mockup of a synchronous transmitter (server) and receiver (client)

## Protocol

There is no real protocol for this project. The "server" takes as input a file or string from standard input.
It frames the payload and "encodes" it into ascii `0` and `1` bytes with a single parity bit. This project
only accepts as input bytes from `0x00` to `0x7F` to conform with the project spec. An example "frame" is below:

| Byte 0 | Byte 1 | Bytes 2..`<len>` | Byte 2 + `<len>` + 1 |
| ------ | ------ | ---------------- | -------------------- |
| `0x16` | Payload Length in bytes (`<len>`) | Payload bytes ( each bit in the byte is expanded into a `'0'` or `'1'` with the MSB in the data byte overwritten with a parity bit) | `0x16` |

To test parity, you can optionally pass the data through the `faulty-wire` program to flip one or more "bits"
before handing off the data to the "client".
