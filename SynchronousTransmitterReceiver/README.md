# Synchronous Transmitter / Receiver

A mockup of a synchronous transmitter (server) and receiver (client)

## Protocol

The protocol consists of control frames and data frames. The client imitates communication with
a control frame and communication follows this sequence:

![](http://www.plantuml.com/plantuml/png/dL0xQyCm4DxrAxun6K8z6nG8BNb02I4fsO_gx9WSEbTIIlFlgzZ6GqepPD3olNNMOXMOK-Fh26lDgpJiqsn-uF3D0ILw06iMKTEZi_a0bXXaFMijzlnruPXGQS0R9NeKEMWDIS96X6kIqV-2hIOizUBTlwLxZgtwo4Ww-YOxkegIAr7y5CNKi6J-ID8uT8oxZUo6VGJoiSenUU5p3U_iwtI2Ld0vSFhtdulFin7L5Z8gszvyrrXsa-u2El_j47GsdC-Gtoy0)

### Control Frames

Each control "frame" consists of two ASCII `SYN` characters (`0x16`) followed by a verb and
optional "arguments"

|    0-1   |      2       |        3..x       |
| -------- | ------------ | ----------------- |
| `0x1616` | Command Verb | Command Arguments |

#### Commands
 
| Byte | Command | Arguments | 
| ---- | ------- | --------- |
| `0x41` | Client Hello | |
| `0x42` | Server Hello | |
| `0x43` | Client Request | A one-byte length-prefixed string for a filename or message id |
| `0x44` | Server Response to request | The length of the response payload. `0x00` if the request was illegal, not found, or invalid |


### Data Frames

Each "frame" consists of two ASCII `SYN` Characters (`0x16`), followed by a one byte length of
the payload and then the payload itself. The catch is each byte of the data payload is literally
an ASCII '`0`' or '`1`'...

In the schema below, the first byte to appear in the stream is on the left and is indicated as
byte 0.

|    0-1   |                      2                      |          3-(3+len)         |
| -------- | ------------------------------------------- | -------------------------- |
| `0x1616` | Payload Length in "bytes" (`0x00` - `0x40`) | Payload "bits" (lsb first) |

#### Payload

The payload is transmitted 7 bits at a time plus a parity bit. Thus, at most 56 data bits (or 7
data bytes)can be packed into a frame.

#### Checksum

The end of each frame contains a 8-byte CRC64 checksum of the payload bits

#### Errors

If any parity check fails or the CRC checksum does not match, the receiving party must respond
with a `NAK` as defined below. Otherwise, the receiving party must respond with an `ACK`.

##### Ack

An `ACK` is a one-byte payload consisting of the byte `0x00`:

|    0-1   |    2   |    3   |
| -------- | ------ | ------ |
| `0x1616` | `0x01` | `0x00` |

##### Nak

A `NAK` is a one-byte payload consisting of anything other than `0x00`. However, all transmitters
**MUST** use a payload of `0xCD` when sending a `NAK`. This way, a receiver is able to interpret
a `NAK` if any corruption in the transport occurs.

|    0-1   |    2   |    3   |
| -------- | ------ | ------ |
| `0x1616` | `0x01` | `0xCD` |