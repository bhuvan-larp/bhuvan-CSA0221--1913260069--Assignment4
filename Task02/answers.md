# Task 2 – From Requirement to Nested Structures (8 marks, K4)
Run: `gcc -Wall -Wextra -o t2c task2_complete_code.c && ./t2c` (Complete the Code) and `gcc -Wall -Wextra -o t2 task2_parcels.c && ./t2`. Outputs: `complete_code_output.txt` (equals the expected output exactly), `output.txt`.

## 2.1 Entities
* **Parcel**: id, weight, booked (Date), sender (Person), receiver (Person), service (Service), charge.
* **Person** (nested twice): name, city, pincode. **Date** (nested): day, month, year. **Service**: enum NORMAL / EXPRESS.
* Nesting: `Parcel` contains `Date` once and `Person` twice (sender, receiver).

## 2.2 Definitions (`task2_parcels.c`)
`typedef struct {int day,month,year;} Date;` `typedef struct {char name[20]; char city[20]; int pincode;} Person;` `typedef enum {NORMAL, EXPRESS} Service;` `typedef struct {int id; float weight; Date booked; Person sender, receiver; Service service; float charge;} Parcel;` and `static Parcel day[MAX];` for the day's bookings.

## 2.3 Pseudocode (my layout of a standard pseudocode form)
```
ALGORITHM SortByWeight(P[0..n-1])
BEGIN
  FOR i <- 0 TO n-2 DO
    min <- i
    FOR j <- i+1 TO n-1 DO
      IF P[j].weight < P[min].weight THEN min <- j
    END FOR
    IF min != i THEN SWAP(P[i], P[min])        // whole structure
  END FOR
END

ALGORITHM ListByCity(P[0..n-1], city)
BEGIN
  found <- 0
  FOR i <- 0 TO n-1 DO
    IF P[i].receiver.city = city THEN PRINT P[i]; found <- found + 1
  END FOR
  RETURN found
END
```
Tested: sorted order 504, 502, 503, 501, 505; Chennai -> 3 parcels; Ooty -> 0 parcels (special case, handled).

## 2.4 Access paths (3rd parcel, index 2; verified = 641001)
(i) `day[2].receiver.pincode`  (ii) `Parcel *p = day; (p + 2)->receiver.pincode`

## Complete the Code
TODO 1 uses `p->id`, `p->weight`, `p->to.city`, `p->to.pincode`, `p->booked.day` ... (note `.` after the first `->` because `to` and `booked` are structures, not pointers). TODO 2 is selection sort swapping whole `struct parcel`s. Each swap copies `sizeof(struct parcel)` bytes: **116** for the nested `Parcel` in `task2_parcels.c`; in `task2_complete_code.c` it is 4 + 4 + (20 + 4) + 12 = 44 bytes (a smaller structure with only the receiver).

## Alternative – flat structure
Flat fields (`r_city`, `r_pin`, `bd`, `bm`, `by`...) give `flat[2].r_pin` – shorter but the names must be invented for sender and receiver, and an address function cannot be reused for both. Measured: `sizeof(Parcel)` nested = **116**, `sizeof(FlatParcel)` = **116** – no space difference (no padding is lost either way); nested wins on reuse (`print_person(&p->sender)` works for both), flat only on a slightly shorter access path. Sorting is O(n²) compares, O(n) swaps of 116 bytes either way; space O(n).

Viva: Why `.` after `->` in `p->to.city`? What is copied in a swap?
