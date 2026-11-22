/* Task 4.2 - address evidence: only the compile error (p.number) is fixed; the by-value bug stays */
#include <stdio.h>
struct account { int number; float balance; };

void addInterest(struct account a, float rate) {
    a.balance = a.balance + a.balance * rate / 100;
    printf("  inside addInterest : &a = %p, a.balance = %.2f\n", (void *)&a, a.balance);
}

int main(void) {
    struct account list[3] = {{101, 5000}, {102, 12000}, {103, 800}};
    struct account *p = list;
    for (int i = 0; i < 3; i++) {
        printf("account %d: &list[%d] = %p, balance before = %.2f\n", list[i].number, i, (void *)&list[i], list[i].balance);
        addInterest(list[i], 4);
        printf("  back in main       : balance after  = %.2f  (unchanged)\n", list[i].balance);
    }
    printf("%d %f\n", p->number, p->balance);
    return 0;
}
