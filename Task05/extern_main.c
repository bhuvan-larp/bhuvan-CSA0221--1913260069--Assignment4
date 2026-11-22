#include <stdio.h>
extern int counter;                 /* declaration: defined in extern_counter.c */
int next_token_ext(void);
int main(void)
{
    int a = next_token_ext(), b = next_token_ext(), c = next_token_ext();
    printf("extern: %d %d %d\n", a, b, c);
    counter = 5;                    /* RISK: any file can overwrite the counter */
    printf("after accidental counter = 5 in main: next token = %d (duplicate-prone)\n", next_token_ext());
    return 0;
}
