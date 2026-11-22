# Task 5 – Storage Classes (7 marks, K5)
Run: `gcc -Wall -Wextra -o t5c task5_complete_code.c`; `gcc -Wall -Wextra -o t5v task5_versions.c`; `gcc -Wall -Wextra -o t5e extern_main.c extern_counter.c`; `gcc ... task5_order.c`; `gcc ... task5_pointer.c`. Outputs: `complete_code_output.txt` (equals expected), `output.txt`, `output_extern.txt`, `output_order.txt`, `output_pointer.txt`, `register_error.txt`.

## 5.1 Four implementations (tokens recorded)
| Version | Code | Tokens |
|---|---|---|
| (a) auto | `int t = 1000; return ++t;` | 1001 1001 1001 |
| (b) static | `static int t = 1000; return ++t;` | 1001 1002 1003 |
| (c) extern | `int counter = 1000;` defined in `extern_counter.c`, `extern int counter;` in `extern_main.c` | 1001 1002 1003 |
| (d) register | `for (register int i...)` loop variable | loop runs 3 times and the static counter issues 1004 1005 1006; a register variable can only hold a loop counter, not a counter that must survive calls |

## 5.2 Two counters (veg 1001.., non-veg 2001..)
* auto – cannot work at all: both counters restart every call (veg 1001 1001, non-veg 2001 2001); the design has to change (counter must live outside the function).
* static – small change: two static variables and a parameter/branch (`return veg ? ++tv : ++tn;`) -> veg 1001 1002, non-veg 2001 2002, independent.
* extern – a second global and a second function (or parameter) and the declaration in **every** file that uses it: two files edited.
* register – not applicable.

## 5.3 Comparison
| | auto | static local | global/extern | register |
|---|---|---|---|---|
| Scope | block | block | all files that declare it | block |
| Lifetime | one call | whole program | whole program | one block execution |
| Initial value | garbage unless set | 0 (initialised once) | 0 (initialised once) | garbage unless set |
| Accidental change | none (private, but lost) | only that function | any file (demo: `counter = 5` made the next token 6) | none |
Taking the address of a register variable is not allowed because it may live in a CPU register, which has no memory address (gcc: "address of register variable 'r' requested", see `register_error.txt`).

## Complete the Code + argument order
TODO 1: `static int token = 1000; return ++token;` – output `static: 1001 1002 1003 / auto  : 1001 1001 1001`. With `printf("%d %d %d", next_token(), next_token(), next_token())` gcc 13.3 on this x86-64 machine printed **1003 1002 1001** at both -O0 and -O2: the order of evaluating function arguments is unspecified, so the compiler may evaluate right to left (another compiler or machine may give 1001 1002 1003). Safe fix: store each call in its own variable first (`int a = next_token(), b = ...;` – commas in a declaration are sequence points).

## Alternative – counter passed by pointer
`int next_token(int *counter) { return ++*counter; }` with `int veg = 1000, nonveg = 2000;` in the caller: veg 1001 1002 1003, non-veg 2001 2002, veg again 1004 (verified in `output_pointer.txt`). Time O(1) in every version. Space: one int per counter; lifetime = the caller's variable; two counters need no code change (just another variable); no hidden state, easy to test. Drawback: every caller must remember to pass the right counter. For two independent counters this is the best of the four.

Viva: Why does the static variable keep its value? Where is a static variable stored? Why can't a register be addressed?
