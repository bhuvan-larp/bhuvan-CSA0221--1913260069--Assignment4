# Task 3 – Unions and Memory Layout (9 marks, K4)
Run: `gcc -Wall -Wextra -o t3c task3_complete_code.c && ./t3c` and `gcc -Wall -Wextra -o t3 task3_packets.c && ./t3`. Outputs in `complete_code_output.txt` (matches expected) and `output.txt`.

## 3.1 Structure or union
* Structure: `{enum kind type; float celsius; int percent; char status[8];}` – holds all three, but only one is ever used.
* Tagged union: `{enum kind type; union {float celsius; int percent; char status[8];} v;}` – members share storage; `type` says which is valid.
* The station stores thousands of packets with little memory -> **tagged union** (measured below).

## 3.2 Predicted vs measured (64-bit GCC 13.3)
| Design | Prediction | Measured |
|---|---|---|
| union only | max(4, 4, 8) = 8, alignment 4 | 8 |
| struct (all three fields) | 4 + 4 + 4 + 8 = 20, alignment 4, no padding | 20 |
| tagged union | tag 4 + union 8 = 12 | 12 |
10,000 packets: 200,000 B (struct) vs 120,000 B (union) -> **80,000 B saved**.

## 3.3 Implementation
`print_packet()` uses `switch (p->type)` and a `default` branch for an unknown tag. Tested: temperature, humidity, 7-character status `RAINING` (fills 8 bytes with NUL) and an empty status (boundary).

## 3.4 Misuse
Stored `31.5f`, read `percent`: prints **1107034112 (0x41fc0000)**. The union has one storage area; 31.5 in IEEE-754 single precision is the bit pattern 0x41FC0000, and reading it as `int` shows those bits as an integer. The result is meaningless as a humidity (reading a different member than last written is type punning – defined bit reinterpretation in C, but wrong data here).

## Complete the Code
Why 12 and not 4 + 4 + 4 + 8? The three union members **overlap**, so the union is only as large as its largest member, `char status[8]` = 8 bytes, with alignment 4 (from `float`/`int`). Struct = 4-byte tag + 8-byte union = 12, already a multiple of 4, so no padding.

## Alternative designs
* **Three separate arrays** (one per type): each element is only 4 or 8 bytes (no tag), but the arrival order of packets is lost unless an extra index/tag array is kept; reading packet k needs a lookup of which array holds it. Time to read: O(1) with the index, space about the sum of the payloads plus the index.
* **`malloc` + `void *`**: `struct packet_ptr` is 16 bytes (tag + 8-byte pointer, measured) plus a heap block per packet (malloc rounds up and adds a header; typically around 32 bytes per chunk on 64-bit glibc – not measured here). Read: O(1) but one extra pointer hop and a cast; `free()` is needed, and leaks/dangling pointers become possible. Per packet roughly 16 + 32 = 48 bytes vs 12 for the tagged union.
* Justification: tagged union – smallest, one allocation (a plain array), O(1) read, and the type tag makes misuse checkable.

Viva: What does the tag do? Why is union size 8? What if the tag and the member disagree?
