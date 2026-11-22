/* Task 6 - College Canteen Inventory and Billing */
#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 20
#define WARN_DAYS 2

struct date { int day, month, year; };
struct clock_time { int hour, minute; };

enum kind { PACKAGED, COOKED, COMBO };

struct item {
    int   id;
    char  name[24];
    enum kind type;                         /* tag */
    float price;
    int   stock;                            /* not used for COMBO (derived from parts) */
    float revenue;                          /* total sales today */
    union {
        struct { struct date expiry; }          pack;
        struct { struct clock_time prepared; }  cook;
        struct { int part_id[3]; int count; }   combo;
    } u;
};

static const struct date TODAY = {12, 10, 2026};   /* fixed for repeatable output */

/* ---- prototypes (structure pointers) ---- */
long days_from_civil(const struct date *d);
int  days_until_expiry(const struct item *it, const struct date *today);
struct item *find_item(struct item menu[], int n, int id);
int  sell(struct item menu[], int n, int id, int qty, const struct date *today);
void expiry_report(const struct item menu[], int n, const struct date *today);
void print_summary(struct item menu[], int n);

long days_from_civil(const struct date *d)          /* day number, handles month/year ends */
{
    int y = d->year, m = d->month;
    if (m <= 2) { y--; m += 12; }
    return 365L * y + y / 4 - y / 100 + y / 400 + (153 * (m - 3) + 2) / 5 + d->day;
}
int days_until_expiry(const struct item *it, const struct date *today)
{ return (int)(days_from_civil(&it->u.pack.expiry) - days_from_civil(today)); }

struct item *find_item(struct item menu[], int n, int id)
{ for (int i = 0; i < n; i++) if (menu[i].id == id) return &menu[i]; return NULL; }

/* Records a sale. Returns 1 if sold, 0 if refused. */
int sell(struct item menu[], int n, int id, int qty, const struct date *today)
{
    static int bill_no = 0;                         /* lives for the whole run */
    struct item *it = find_item(menu, n, id);
    if (!it)      { printf("  REFUSED: item %d does not exist\n", id); return 0; }
    if (qty <= 0) { printf("  REFUSED: %s quantity %d is not valid\n", it->name, qty); return 0; }

    if (it->type == COMBO) {
        struct item *part[3];
        for (int k = 0; k < it->u.combo.count; k++) {
            part[k] = find_item(menu, n, it->u.combo.part_id[k]);
            if (!part[k] || part[k]->stock < qty) {
                printf("  REFUSED: %s needs %d of %s but only %d in stock\n", it->name, qty,
                       part[k] ? part[k]->name : "?", part[k] ? part[k]->stock : 0);
                return 0;
            }
            if (part[k]->type == PACKAGED && days_until_expiry(part[k], today) < 0) {
                printf("  REFUSED: %s contains expired %s\n", it->name, part[k]->name); return 0; }
        }
        for (int k = 0; k < it->u.combo.count; k++) part[k]->stock -= qty;   /* update only after all checks */
    } else {
        if (it->stock < qty) {
            printf("  REFUSED: %s requested %d, only %d in stock (stock unchanged)\n", it->name, qty, it->stock);
            return 0; }
        if (it->type == PACKAGED && days_until_expiry(it, today) < 0) {
            printf("  REFUSED: %s expired %d day(s) ago\n", it->name, -days_until_expiry(it, today)); return 0; }
        it->stock -= qty;
    }
    float amount = it->price * qty;
    it->revenue += amount;
    printf("  Bill #%d: %d x %s @ %.2f = %.2f\n", ++bill_no, qty, it->name, it->price, amount);
    return 1;
}

void expiry_report(const struct item menu[], int n, const struct date *today)
{
    printf("Expiry check (today %02d/%02d/%d, warn within %d days):\n", today->day, today->month, today->year, WARN_DAYS);
    int any = 0;
    for (int i = 0; i < n; i++) {
        const struct item *it = &menu[i];
        if (it->type != PACKAGED) continue;
        int d = days_until_expiry(it, today);
        if (d < 0)            { printf("  EXPIRED : %-10s expired %d day(s) ago, stock %d\n", it->name, -d, it->stock); any = 1; }
        else if (d <= WARN_DAYS) { printf("  WARNING : %-10s expires in %d day(s), stock %d\n", it->name, d, it->stock); any = 1; }
    }
    if (!any) printf("  nothing expiring\n");
}

void print_summary(struct item menu[], int n)
{
    struct item *ord[MAX_ITEMS];                    /* sort pointers, not 90-byte structures */
    for (int i = 0; i < n; i++) ord[i] = &menu[i];
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (ord[j]->revenue > ord[m]->revenue) m = j;
        struct item *t = ord[i]; ord[i] = ord[m]; ord[m] = t;
    }
    float total = 0;
    printf("Day summary (sorted by revenue, highest first):\n");
    for (int i = 0; i < n; i++) { printf("  %-12s Rs %7.2f\n", ord[i]->name, ord[i]->revenue); total += ord[i]->revenue; }
    printf("  %-12s Rs %7.2f\n", "TOTAL", total);
}

int main(void)
{
    struct item menu[MAX_ITEMS] = {
        {1, "Biscuits", PACKAGED, 10, 20, 0, .u.pack  = {{13, 10, 2026}}},
        {2, "Curd cup", PACKAGED, 25, 10, 0, .u.pack  = {{11, 10, 2026}}},
        {3, "Chips",    PACKAGED, 20, 15, 0, .u.pack  = {{30, 11, 2026}}},
        {4, "Samosa",   COOKED,   12, 30, 0, .u.cook  = {{10, 30}}},
        {5, "Tea",      COOKED,    8, 50, 0, .u.cook  = {{11, 0}}},
        {6, "Snack combo", COMBO, 28,  0, 0, .u.combo = {{4, 5, 1}, 3}},
    };
    int n = 6;

    printf("== Normal sales ==\n");
    sell(menu, n, 4, 5, &TODAY);
    sell(menu, n, 5, 10, &TODAY);
    sell(menu, n, 3, 2, &TODAY);
    sell(menu, n, 6, 2, &TODAY);                    /* combo: deducts Samosa, Tea, Biscuits */
    printf("== Expiring item sale ==\n");
    sell(menu, n, 1, 3, &TODAY);
    printf("== Insufficient stock ==\n");
    sell(menu, n, 5, 60, &TODAY);
    printf("== Special cases ==\n");
    sell(menu, n, 2, 1, &TODAY);                    /* expired */
    sell(menu, n, 3, 0, &TODAY);                    /* zero quantity */
    sell(menu, n, 99, 1, &TODAY);                   /* unknown id */
    sell(menu, n, 3, 13, &TODAY);                   /* boundary: exactly the remaining stock (15-2=13) */
    sell(menu, n, 3, 1, &TODAY);                    /* now stock = 0 */
    printf("\n");
    expiry_report(menu, n, &TODAY);
    printf("\nStock left: Biscuits %d, Samosa %d, Tea %d, Chips %d\n",
           menu[0].stock, menu[3].stock, menu[4].stock, menu[2].stock);
    printf("\n");
    print_summary(menu, n);
    printf("\nsizeof(struct item) = %zu, sizeof(union) = %zu, array of %d = %zu bytes\n",
           sizeof(struct item), sizeof(menu[0].u), MAX_ITEMS, sizeof menu);
    return 0;
}
