/* Task 1.2 - repaired patient register + Alternative analysis (AoS vs SoA) */
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PATIENTS 100

struct date { int day, month, year; };
struct bp   { int systolic, diastolic; };          /* 120/80 -> two ints */
struct patient {
    char name[20];
    int age;
    char phone[11];                                /* 10 digits + '\0'  */
    struct bp pressure;
    struct date visit;
};

/* Returns 1 on success, 0 if the register is full (limit check). */
int addPatient(struct patient reg[], int *count, const struct patient *p)
{
    if (*count >= MAX_PATIENTS) return 0;
    reg[*count] = *p;
    (*count)++;
    return 1;
}

void sort_by_age(struct patient reg[], int n)       /* selection sort */
{
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++)
            if (reg[j].age < reg[m].age) m = j;
        if (m != i) { struct patient t = reg[i]; reg[i] = reg[m]; reg[m] = t; }
    }
}

/* ---- parallel-array version (the junior's design) for comparison ---- */
#define N 3000
static char  s_name[N][20]; static int s_age[N]; static char s_phone[N][11];
static float s_bp[N];       static char s_date[N][9];
static long  moved_soa;

static void swap_soa(int i, int j)
{
    char tn[20], tp[11], td[9]; int ta; float tb;
    memcpy(tn, s_name[i], 20);  memcpy(s_name[i], s_name[j], 20);  memcpy(s_name[j], tn, 20);
    ta = s_age[i]; s_age[i] = s_age[j]; s_age[j] = ta;
    memcpy(tp, s_phone[i], 11); memcpy(s_phone[i], s_phone[j], 11); memcpy(s_phone[j], tp, 11);
    tb = s_bp[i]; s_bp[i] = s_bp[j]; s_bp[j] = tb;
    memcpy(td, s_date[i], 9);   memcpy(s_date[i], s_date[j], 9);   memcpy(s_date[j], td, 9);
    moved_soa += 20 + 4 + 11 + 4 + 9;               /* bytes moved per swap (one side) */
}
static void sort_soa(int n)
{
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (s_age[j] < s_age[m]) m = j;
        if (m != i) swap_soa(i, m);
    }
}

int main(void)
{
    struct patient reg[MAX_PATIENTS]; int count = 0;
    struct patient in[3] = {
        {"Meena", 45, "9840012345", {120, 80}, {5, 11, 2026}},
        {"Ravi",  23, "9444098765", {110, 70}, {6, 11, 2026}},
        {"Kumar", 67, "9941011223", {145, 95}, {6, 11, 2026}} };
    for (int i = 0; i < 3; i++) addPatient(reg, &count, &in[i]);
    sort_by_age(reg, count);
    printf("Sorted by age:\n");
    for (int i = 0; i < count; i++) {
        struct patient *p = &reg[i];
        printf("  %-6s %2d  %s  BP %d/%d  visit %02d/%02d/%d\n", p->name, p->age,
               p->phone, p->pressure.systolic, p->pressure.diastolic,
               p->visit.day, p->visit.month, p->visit.year);
    }
    /* limit check test */
    int added = 0;
    for (int i = 0; i < 105; i++) added += addPatient(reg, &count, &in[0]);
    printf("Limit test: tried to add 105 more, accepted %d, count = %d\n", added, count);

    printf("\n--- sizeof measurements ---\n");
    printf("sizeof(struct date)=%zu sizeof(struct bp)=%zu sizeof(struct patient)=%zu\n",
           sizeof(struct date), sizeof(struct bp), sizeof(struct patient));
    printf("Raw field bytes per patient = %d (name20+age4+phone11+bp8+date12)\n", 20+4+11+8+12);
    printf("Padding in struct patient   = %zu bytes\n", sizeof(struct patient) - (20+4+11+8+12));
    printf("Parallel arrays (char date[9], float bp) bytes per patient = %d\n", 20+4+11+4+9);

    /* sort n = 3000 patients in both designs, same ages */
    static struct patient big[N];
    unsigned s = 12345;
    for (int i = 0; i < N; i++) {
        s = s * 1103515245u + 12345u; int a = (int)((s >> 16) % 90) + 1;
        big[i].age = a; s_age[i] = a;
    }
    long moved_aos = 0; clock_t t0 = clock();
    for (int i = 0; i < N - 1; i++) {
        int m = i;
        for (int j = i + 1; j < N; j++) if (big[j].age < big[m].age) m = j;
        if (m != i) { struct patient t = big[i]; big[i] = big[m]; big[m] = t; moved_aos += sizeof t; }
    }
    double t_aos = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;
    t0 = clock(); sort_soa(N);
    double t_soa = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;
    printf("Sort n=%d by age: AoS bytes moved/side = %ld, time %.1f ms\n", N, moved_aos, t_aos);
    printf("Sort n=%d by age: SoA bytes moved/side = %ld, time %.1f ms (5 separate copies per swap)\n", N, moved_soa, t_soa);
    return 0;
}
