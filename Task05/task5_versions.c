/* Task 5.1 (a)(b)(d) and Task 5.2 two-counter versions; (c) extern is in extern_main.c + extern_counter.c */
#include <stdio.h>

/* (a) automatic */
int token_auto(void) { int t = 1000; return ++t; }
/* (b) static local */
int token_static(void) { static int t = 1000; return ++t; }

/* 5.2 two counters */
int token_auto2(int veg)   { int t = veg ? 1000 : 2000; return ++t; }                 /* still broken */
int token_static2(int veg) { static int tv = 1000, tn = 2000; return veg ? ++tv : ++tn; }  /* 1 extra variable + 1 test */

int main(void)
{
    int a1 = token_auto(), a2 = token_auto(), a3 = token_auto();      /* declarations: left to right */
    printf("(a) auto   : %d %d %d\n", a1, a2, a3);

    printf("(b) static : ");
    for (int i = 0; i < 3; i++) { int t = token_static(); printf("%d ", t); }
    printf("\n");

    printf("(d) register loop variable printing tokens: ");
    for (register int i = 0; i < 3; i++) { int t = token_static(); printf("%d ", t); }  /* continues 1004.. */
    printf("\n    (&i would be a compile error: address of register variable requested)\n");

    printf("\n--- 5.2 two counters ---\n");
    int v1 = token_auto2(1), v2 = token_auto2(1), n1 = token_auto2(0), n2 = token_auto2(0);
    printf("auto   veg: %d %d  nonveg: %d %d  (resets every call)\n", v1, v2, n1, n2);
    v1 = token_static2(1); v2 = token_static2(1); n1 = token_static2(0); n2 = token_static2(0);
    printf("static veg: %d %d  nonveg: %d %d  (independent)\n", v1, v2, n1, n2);
    return 0;
}
