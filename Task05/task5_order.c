/* printf with three calls in one statement: argument evaluation order is unspecified */
#include <stdio.h>
int next_token(void) { static int t = 1000; return ++t; }
int main(void)
{
    printf("%d %d %d\n", next_token(), next_token(), next_token());
    return 0;
}
