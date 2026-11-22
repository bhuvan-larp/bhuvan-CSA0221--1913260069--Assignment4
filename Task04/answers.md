# Task 4 – Structure Forensics (7 marks, K4)
Files: `task4_buggy.c` (given program; I added `#include <stdio.h>` because the snippet omits it), `compile_error.txt`, `task4_evidence.c` / `output_evidence.txt`, `task4_fixed.c` / `output.txt`.
Compile: `gcc -Wall -Wextra -o t4f task4_fixed.c`

## 4.1 Defects
1. **Balances unchanged** – `addInterest(struct account a, ...)` receives a **copy** of `list[i]`; the interest is added to the copy, which disappears when the function returns.
2. **Does not compile** – `p` is a pointer, so `p.number` / `p.balance` is invalid; use `p->number`, `p->balance` (gcc: "'p' is a pointer; did you mean to use '->'?" – see `compile_error.txt`).

## 4.2 Evidence (addresses from one run; they change between runs)
| account | `&list[i]` in main | `&a` inside addInterest | balance in function | balance back in main |
|---|---|---|---|---|
| 101 | …b7b0 | …b788 | 5200.00 | 5000.00 |
| 102 | …b7b8 | …b788 | 12480.00 | 12000.00 |
| 103 | …b7c0 | …b788 | 832.00 | 800.00 |
Different addresses = a different object (the copy); the copy has the same address each call because it reuses the same stack slot.

## 4.3 Repair
`void addInterest(struct account *a, float rate) { a->balance += a->balance * rate / 100; }` called as `addInterest(&list[i], 4)`. Result: 101 -> 5200.00, 102 -> 12480.00, 103 -> 832.00. Inside the function the printed address equals `&list[i]`. Boundary tests: balance 0 stays 0.00; rate 0 leaves 1000.00.

## 4.4 When to pass how
* **By value** – small structure, the function only needs its own copy, or must not change the caller's data.
* **By pointer** – the function must modify the caller's structure, or the structure is large (copy cost).
* **By const pointer** – large structure the function only reads (`const struct account *a`); the compiler rejects accidental writes.

## Alternative – by value and return the copy
`list[i] = addInterestV(list[i], 4);` gives the same balances (verified). The structure is copied in (8 bytes) and out (8 bytes). Time is O(sizeof(struct)) = O(1) for 8 bytes but grows with structure size; pointer passing is always O(1) (8-byte address). Measured at -O0, 200,000,000 calls: pointer 0.42 s, value+return 1.49 s (about 3.5× slower; machine dependent). Stack space: pointer version 8 bytes of argument; value version a copy of the structure for the argument (8 bytes) plus the returned copy. For a 1 KB structure that becomes about 2 KB copied per call. Preferred: pointer.

Viva: Why is the address inside the function different? Which operator for a pointer to structure?
