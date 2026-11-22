# Task 6 – College Canteen Inventory and Billing (13 marks, K5)
Run: `gcc -Wall -Wextra -o canteen canteen.c && ./canteen` (output in `output.txt`); alternative: `gcc -Wall -Wextra -o ll linked_list_alt.c && ./ll` (`output_linked_list.txt`).

## Stage 1 – Interpret the problem
Entities: **Item** (id, name, kind, price, stock, revenue) of three kinds – packaged (+expiry date), cooked (+prepared time), combo (+up to 3 item ids); **Date**; **Clock time**; **Sale/bill**.
Testable requirements:
R1 Selling q of an item with stock ≥ q reduces its stock by q and adds price×q to its revenue.
R2 If q > stock the sale is refused and nothing changes.
R3 q ≤ 0 or an unknown id is refused.
R4 A combo sale needs every part in stock for q; parts are deducted only after all checks pass; revenue is credited to the combo.
R5 A packaged item expiring in 0..2 days is reported as WARNING; one already expired is reported as EXPIRED and cannot be sold.
R6 The summary lists all items by revenue, highest first, with a total.

## Stage 2 – Data model
Nested structures: `struct date` and `struct clock_time` inside the item's union members. Array of structures: `struct item menu[MAX_ITEMS]`. **Tagged union**: `enum kind type` + `union u {pack{expiry}, cook{prepared}, combo{part_id[3], count}}` – each item is exactly one of the three kinds.

## Stage 3 – Critical decisions
* Stock safety: check first, then update (R2, R4); for combos check all parts before deducting any.
* Expiry: `days_from_civil()` converts a `struct date` to a day number, so month/year boundaries work; difference = days until expiry.
* Static/global: `static int bill_no` inside `sell()` (keeps counting between calls, private); `static const struct date TODAY` file-level constant (fixed date for repeatable tests). Menu is a local array passed by pointer – no mutable globals.

## Stage 4 – Structures, prototypes, pseudocode
Structures: see `canteen.c` (`struct item` is 60 bytes, union 16 bytes, 20 items = 1200 bytes).
```
long days_from_civil(const struct date *d);
int  days_until_expiry(const struct item *it, const struct date *today);
struct item *find_item(struct item menu[], int n, int id);
int  sell(struct item menu[], int n, int id, int qty, const struct date *today);
void expiry_report(const struct item menu[], int n, const struct date *today);
void print_summary(struct item menu[], int n);
```
```
ALGORITHM Sell(menu, n, id, qty, today)
BEGIN
  it <- FindItem(menu, n, id)
  IF it = NULL OR qty <= 0 THEN RETURN refused
  IF it.type = COMBO THEN
     FOR each part k: IF part missing OR stock < qty OR part expired THEN RETURN refused
     FOR each part k: stock <- stock - qty
  ELSE
     IF it.stock < qty THEN RETURN refused
     IF it.type = PACKAGED AND DaysUntilExpiry(it) < 0 THEN RETURN refused
     it.stock <- it.stock - qty
  it.revenue <- it.revenue + it.price * qty
  RETURN sold
END

ALGORITHM ExpiryWarning(menu, n, today)
BEGIN
  FOR i <- 0 TO n-1 DO
    IF menu[i].type = PACKAGED THEN
       d <- DaysUntilExpiry(menu[i], today)
       IF d < 0 THEN PRINT "EXPIRED" ELSE IF d <= 2 THEN PRINT "WARNING"
END
```

## Stage 5 – Demonstration (real output, `output.txt`)
| Test | Result |
|---|---|
| Normal sales | 5 Samosa 60.00, 10 Tea 80.00, 2 Chips 40.00, 2 combos 56.00 (Samosa, Tea and Biscuits stock reduced for the combos) |
| Sale near expiry | 3 Biscuits sold; expiry report: WARNING, expires in 1 day |
| Insufficient stock | 60 Tea requested, 38 available -> REFUSED, stock unchanged |
| Expired item | Curd cup (expired yesterday) REFUSED; also listed as EXPIRED |
| Special cases | quantity 0 refused; unknown id 99 refused; exactly remaining stock (13 Chips) sold; next Chips sale refused (stock 0) |
| Summary | Chips 300, Tea 80, Samosa 60, Snack combo 56, Biscuits 30, Curd cup 0, TOTAL 526.00 (sorted by revenue) |
The summary sorts an array of **pointers** so the 60-byte structures are never moved.

## Alternative – linked list of dynamically allocated items
`struct node {id, name, price, stock, struct node *next}` allocated with `malloc`, freed on removal.
| Operation | Linked list | Array |
|---|---|---|
| Add (front) | O(1) | O(n) shifting (O(1) at end if room) |
| Remove (by id) | O(n) find + O(1) unlink | O(n) find + O(n) shift |
| Search by id | O(n) | O(n) (O(log n) if sorted) |
Measured (n = 20,000, `output_linked_list.txt`): add at front 0.49 ms for the list vs 131 ms for the array; searching 200 ids: 9.1 ms vs 5.3 ms; removing 200: 7.5 ms vs 5.2 ms (list nodes are scattered in memory, so scanning is slower). Space: `struct node` 48 bytes vs 36 bytes per item in the test array structure – 12 extra bytes (an 8-byte `next` pointer plus padding), plus malloc overhead per node. Sorting by revenue is also easier on an array.
**Final design: array of structures** – the canteen menu is small and fixed in size, searched far more often than changed, so contiguous storage, no malloc failures and no leaks beat the list's cheap insertion.

Viva: Why check all combo parts before deducting? What does the tag protect? Why sort pointers?
