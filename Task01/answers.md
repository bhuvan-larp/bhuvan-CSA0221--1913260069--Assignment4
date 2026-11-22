# Task 1 – Data Design Review (6 marks, K4)
Run: `gcc -Wall -Wextra -o t1 task1_patient.c && ./t1` (real output in `output.txt`).

## 1.1 Diagnosis (what goes wrong in the health centre)
| # | Deficiency | What could go wrong |
|---|---|---|
| 1 | Parallel arrays (`name[]`, `age[]`, ... ) | One patient lives in 5 places; sorting by age and swapping only `age[]` attaches Ravi's age to Meena's name -> wrong medical record. |
| 2 | Phone number in `char[10]` | 10 digits leave no room for `'\0'`; `strcpy` writes 1 byte past the row and corrupts the next patient's phone. |
| 3 | Blood pressure in one `float` | `120/80` is two values; a float cannot hold it (120/80 in C is the integer 1). Systolic and diastolic cannot be checked separately (e.g. flag 145/95). |
| 4 | Date as `char[8]` text | `"12/11/26"` is 8 chars + `'\0'` = 9 -> overflow; two-digit year; cannot compare or find "visits in the last 30 days". |
| 5 | No limit check on `count` | The 101st patient overwrites memory outside the arrays (undefined behaviour). |
| 6 | Global variables | Any function can change `count` or the arrays; hard to test or reuse. |
| 7 | Sorting by age is hard | Every swap must be repeated for five arrays; forgetting one corrupts records. |
| 8 | `strcpy` without length check | A 25-letter name overflows `name[20]`. |

## 1.2 Repair
See `task1_patient.c`: `struct date`, `struct bp`, `struct patient` (nested `bp` and `date`), `int addPatient(struct patient reg[], int *count, const struct patient *p)` with a limit check. Limit test result: after 3 patients, 105 more were attempted and **97 accepted, count = 100** (the register stops at MAX_PATIENTS). Sorted by age: Ravi 23, Meena 45, Kumar 67.

## 1.3 Design defence (≤100 words)
One structure holds one patient, so a swap moves the whole record and the fields can never get out of step. The array of structures can be sorted by any field with one comparison. A patient is passed to a function as one pointer instead of five arguments. New fields (e.g. weight) are added once in the structure without changing any function signature. Nested `date` and `bp` give each value its own meaning and let us compare dates and check both pressures.

## Alternative – parallel arrays (structure of arrays) vs array of structures
* Parallel arrays are still preferred when a program scans **one field across all records** (e.g. average age): only `age[]` is read, so the cache is used well and data can be vectorised; also when different fields are stored in different files/places.
* Measured (`output.txt`, n = 3000, 2955 swaps): `sizeof(struct patient)` = **56** bytes (55 bytes of fields + 1 byte padding); parallel arrays need 48 bytes per patient with the corrected `char date[9]` and `float bp`. AoS moves 56 bytes per swap in one copy (165,480 bytes per side); SoA moves 48 bytes per swap in five copies (141,840). Time was 4.6 ms (AoS) vs 3.4 ms (SoA) – small difference on this machine (timings vary).
* Big-O: both sorts are O(n²) comparisons and O(n) swaps; one swap costs O(s) with s = bytes per record (constant). Space O(n) for both (about 56n vs 48n bytes). My two designs differ in how the fields are represented (nested int date = 12 bytes vs 9-byte text, bp 8 vs 4 bytes), so the 8-byte gap is mostly representation, not the layout itself.
* Justification: AoS for correctness and maintainability (all of this is about one record at a time).

Viva: Why is `char phone[10]` wrong? What does `sizeof(struct patient)` include besides the fields? Why is `reg[*count] = *p` legal?
