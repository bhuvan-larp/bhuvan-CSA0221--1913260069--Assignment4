/* Task 4.3 - repaired: pointer + arrow operator; Task 4.4 const pointer; alternative by-value-return */
#include <stdio.h>
#include <time.h>

struct account { int number; float balance; };

void addInterest(struct account *a, float rate) {
    a->balance = a->balance + a->balance * rate / 100;
    printf("  inside addInterest : a = %p (same as &list[i])\n", (void *)a);
}
void show(const struct account *a) { printf("%d %.2f\n", a->number, a->balance); }  /* read-only access */

/* alternative: by value in, modified copy out */
struct account addInterestV(struct account a, float rate) { a.balance += a.balance * rate / 100; return a; }

__attribute__((noinline)) void bump_ptr(struct account *a) { a->balance += 0.0001f; }
__attribute__((noinline)) struct account bump_val(struct account a) { a.balance += 0.0001f; return a; }

int main(void) {
    struct account list[3] = {{101, 5000}, {102, 12000}, {103, 800}};
    struct account *p = list;
    for (int i = 0; i < 3; i++) {
        printf("account %d: &list[%d] = %p\n", list[i].number, i, (void *)&list[i]);
        addInterest(&list[i], 4);
    }
    for (int i = 0; i < 3; i++) show(p + i);

    printf("\n--- alternative: pass by value and return the copy ---\n");
    struct account v[3] = {{101, 5000}, {102, 12000}, {103, 800}};
    for (int i = 0; i < 3; i++) v[i] = addInterestV(v[i], 4);
    for (int i = 0; i < 3; i++) show(&v[i]);

    printf("\n--- boundary: zero balance, zero rate ---\n");
    struct account z = {104, 0}; addInterest(&z, 4); show(&z);
    struct account y = {105, 1000}; addInterest(&y, 0); show(&y);

    printf("\nsizeof(struct account) = %zu bytes; pointer = %zu bytes\n", sizeof(struct account), sizeof(struct account *));
    struct account t = {1, 1.0f}; struct account u = {1, 1.0f};
    long N = 200000000L; clock_t c0 = clock();
    for (long i = 0; i < N; i++) bump_ptr(&t);
    double tp = (double)(clock() - c0) / CLOCKS_PER_SEC; c0 = clock();
    for (long i = 0; i < N; i++) u = bump_val(u);
    double tv = (double)(clock() - c0) / CLOCKS_PER_SEC;
    printf("%ld calls: pointer %.2f s, value+return %.2f s (-O0, machine dependent; t=%.1f u=%.1f)\n", N, tp, tv, t.balance > 0 ? 1.0 : 0.0, u.balance > 0 ? 1.0 : 0.0);
    return 0;
}
