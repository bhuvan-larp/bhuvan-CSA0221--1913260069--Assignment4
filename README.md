# CSA02 – C Programming – Assignment 4 (CO4)
**Modelling Real-World Data with Structures and Unions (50 marks)**

| | |
|---|---|
| Name | **<YOUR NAME HERE>** |
| Register number | **<YOUR REGISTER NUMBER HERE>** |
| Compiler | gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0, standard C |
| Compile command | `gcc -Wall -Wextra -o <name> <file>.c` (no warnings) |
| GitHub repo | `CSA02-<RegisterNumber>-Assignment4` |

## How to run (from inside the repository folder)
```
cd Task01 && gcc -Wall -Wextra -o t1 task1_patient.c && ./t1
cd Task02 && gcc -Wall -Wextra -o t2c task2_complete_code.c && ./t2c ; gcc -Wall -Wextra -o t2 task2_parcels.c && ./t2
cd Task03 && gcc -Wall -Wextra -o t3c task3_complete_code.c && ./t3c ; gcc -Wall -Wextra -o t3 task3_packets.c && ./t3
cd Task04 && gcc -Wall -Wextra -o t4f task4_fixed.c && ./t4f ; gcc -Wall -Wextra -o t4e task4_evidence.c && ./t4e   # task4_buggy.c does not compile (by design)
cd Task05 && gcc -Wall -Wextra -o t5c task5_complete_code.c && ./t5c ; gcc -Wall -Wextra -o t5v task5_versions.c && ./t5v
          gcc -Wall -Wextra -o t5e extern_main.c extern_counter.c && ./t5e     # two files, extern
cd Task06 && gcc -Wall -Wextra -o canteen canteen.c && ./canteen ; gcc -Wall -Wextra -o ll linked_list_alt.c && ./ll
```
Each task folder has the source, `output*.txt` (real output), `answers.md` (explanations, pseudocode, viva questions) and `screenshots/` (put your screenshots there).

## Result notes
| Task | Result |
|---|---|
| 1 | 8 design faults found; repaired with nested `date`/`bp` structs; limit check accepted 97 of 105 extra patients (count 100); `sizeof(struct patient)` = 56 vs 48 bytes per patient for parallel arrays. |
| 2 | Complete-the-code output equals the expected output; nested and flat parcel are both 116 bytes; access paths give 641001. |
| 3 | Complete-the-code output equals expected (union 8, struct 12); plain struct 20 vs tagged union 12; misuse prints 1107034112 (0x41fc0000). |
| 4 | By-value copy proven by addresses; `p.number` is a compile error; pointer version gives 5200 / 12480 / 832; pointer 0.42 s vs value+return 1.49 s for 200M calls. |
| 5 | Static 1001 1002 1003, auto 1001 ×3; extern shared across 2 files; gcc printed 1003 1002 1001 for the three-call printf; register address is a compile error. |
| 6 | Sales, insufficient stock, expiry warning/expired and revenue-sorted summary all demonstrated (total 526.00); linked list adds in O(1) (0.49 ms vs 131 ms) but scans slower. |

## Assumptions
Task 6: today is fixed at 12/10/2026 for repeatable output; combo stock is derived from its three parts; "expiring" = 0 to 2 days. Task 4: `#include <stdio.h>` added to the given buggy program. Pseudocode follows a standard ALGORITHM/BEGIN/END layout – adjust to the exact SIMATS standard if your copy differs. Timings and stack addresses differ between machines and runs.
