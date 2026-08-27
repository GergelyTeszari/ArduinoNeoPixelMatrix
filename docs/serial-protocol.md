# Serial firmware and protocol

The repository contains two Arduino sketches that receive a 16 x 16 RGB image
over a 115200 baud serial connection. Both expect colors in six-character
`RRGGBB` hexadecimal notation and both map ordinary top-left-origin image
coordinates to the installed serpentine matrix wiring.

The protocols are deliberately documented separately because they are not
interchangeable.

## Quick comparison

| Behavior | Legacy receiver | Optimized receiver |
| --- | --- | --- |
| Sketch | `NeoPixel_serial` | `NeoPixel_serial_optimised` |
| Data per line | One pixel, 6 characters | One row, 96 characters |
| Data lines per frame | 256 | 16 |
| `strip.show()` calls per frame | 256 | 16 |
| Reset recognition | Any 10-character line | Exactly `0123456789` |
| Reset clears LEDs | Yes | No |
| Invalid input | Ignored by length | Ignored by length/content |

Use the optimized receiver for normal image transfer. The legacy version is
kept for compatibility with an older sender and is useful when watching an
image arrive one pixel at a time.

## Shared configuration

- Baud rate: `115200`
- Color format: `RRGGBB`, without `#` or separators
- Logical dimensions: 16 columns by 16 rows
- Logical order: top row to bottom row, left to right within each row
- Data pin: Arduino digital pin 6
- LED type: `NEO_GRB + NEO_KHZ800`

For example, the following values represent individual pixels:

```text
FF0000  red
00FF00  green
0000FF  blue
000000  off
FFFFFF  white
```

Although data is expressed as RGB, the Adafruit NeoPixel library handles the
matrix's physical GRB channel order.

## Legacy pixel-by-pixel receiver

Sketch:
`firmware/serial/NeoPixel_Serial/NeoPixel_Serial.ino`

### Starting a frame

Send one line containing exactly ten characters. For compatibility with the
original implementation, the actual characters are not checked. Using the
same token as the optimized protocol is recommended:

```text
0123456789
```

The receiver then:

1. moves its logical write cursor to pixel 0;
2. clears all 256 LEDs;
3. displays the cleared matrix immediately.

### Sending pixels

Send 256 lines. Each line contains exactly one six-character color:

```text
FF0000
00FF00
0000FF
...
```

The first line writes logical coordinate `(0, 0)`, the second `(1, 0)`, and so
on. After `(15, 0)`, the next value writes `(0, 1)`. Each accepted value is
shown immediately. Values sent after the 256th pixel are ignored until another
reset line arrives.

The legacy sketch accepts either LF or CRLF line endings. It uses Arduino
`String` and refreshes after every pixel, so it is less memory- and
bandwidth-efficient than the optimized version.

## Optimized row-by-row receiver

Sketch:
`firmware/serial/NeoPixel_Serial_optimised/NeoPixel_Serial_optimised.ino`

### Starting a frame

Send this exact line:

```text
0123456789
```

This resets the logical row cursor to row 0. It intentionally does not clear
the LEDs, allowing the old image to remain visible while the new frame arrives.

### Sending rows

Send exactly 16 lines after the reset. Each line contains 16 adjacent `RRGGBB`
values with no spaces or separators, for a total of 96 characters:

```text
FF000000FF000000FF000000FF000000FF000000FF000000FF000000FFFFFF
```

That example is one row containing 16 pixels. The firmware validates the row
length and hexadecimal characters, fills all 16 pixels, then calls
`strip.show()` once. Invalid rows are ignored without advancing the row cursor.

After 16 valid rows, further row data is ignored until the reset command is
received. Both LF and CRLF line endings are accepted.

## Minimal Python sender outline

The important part of a sender is to preserve row-major ordering and terminate
every command or data record with a newline:

```python
serial_port.write(b"0123456789\n")

for row in image_rows:
    line = "".join(f"{red:02X}{green:02X}{blue:02X}"
                   for red, green, blue in row)
    serial_port.write((line + "\n").encode("ascii"))
```

For the legacy receiver, iterate over every pixel and send each formatted color
as its own line instead.

## Current protocol limitations

- There is no acknowledgement from the Arduino.
- There is no checksum or frame number.
- A malformed legacy six-character value is not fully validated.
- The optimized receiver displays each completed row rather than swapping a
  complete frame atomically.
- Matrix size, data pin and baud rate are compile-time constants.

These constraints are acceptable for a direct USB serial prototype but should
be revisited before using an unreliable transport or continuous animation.
