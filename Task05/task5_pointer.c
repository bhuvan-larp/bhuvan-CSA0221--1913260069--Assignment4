/* Alternative: counter owned by the caller, passed by pointer; two counters = two variables */
#include <stdio.h>
int next_token(int *counter) { return ++*counter; }
int main(void)
{
    int veg = 1000, nonveg = 2000;
    int a = next_token(&veg), b = next_token(&veg), c = next_token(&veg);
    int d = next_token(&nonveg), e = next_token(&nonveg);
    printf("veg   : %d %d %d\n", a, b, c);
    printf("nonveg: %d %d\n", d, e);
    printf("veg again: %d\n", next_token(&veg));
    return 0;
}
