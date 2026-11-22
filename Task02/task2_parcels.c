/* Task 2.2 / 2.3 / 2.4 + flat-structure alternative */
#include <stdio.h>
#include <string.h>

typedef struct { int day, month, year; } Date;
typedef struct { char name[20]; char city[20]; int pincode; } Person;
typedef enum { NORMAL, EXPRESS } Service;
typedef struct { int id; float weight; Date booked; Person sender, receiver; Service service; float charge; } Parcel;

/* flat alternative: every nested field written directly */
typedef struct { int id; float weight; int bd, bm, by;
    char s_name[20], s_city[20]; int s_pin; char r_name[20], r_city[20]; int r_pin;
    Service service; float charge; } FlatParcel;

#define MAX 50
static Parcel day[MAX];

void sort_by_weight(Parcel p[], int n)
{ for (int i = 0; i < n-1; i++) { int m = i;
    for (int j = i+1; j < n; j++) if (p[j].weight < p[m].weight) m = j;
    if (m != i) { Parcel t = p[i]; p[i] = p[m]; p[m] = t; } } }

int list_by_city(const Parcel p[], int n, const char *city)
{ int found = 0;
  for (int i = 0; i < n; i++) if (strcmp(p[i].receiver.city, city) == 0) {
      printf("  parcel %d  %.1f kg  to %s, %s-%d\n", p[i].id, p[i].weight,
             p[i].receiver.name, p[i].receiver.city, p[i].receiver.pincode); found++; }
  return found; }

int main(void)
{
    Parcel in[5] = {
      {501, 4.5f,{12,10,2026},{"Arun","Madurai",625001},{"Bala","Chennai",600077},NORMAL,90},
      {502, 1.2f,{12,10,2026},{"Cathy","Salem",636001},{"Dev","Chennai",600020},EXPRESS,150},
      {503, 2.8f,{13,10,2026},{"Esha","Trichy",620001},{"Farid","Coimbatore",641001},NORMAL,70},
      {504, 0.5f,{13,10,2026},{"Gita","Madurai",625002},{"Hari","Chennai",600042},EXPRESS,120},
      {505, 7.0f,{13,10,2026},{"Ila","Erode",638001},{"Jay","Madurai",625001},NORMAL,160} };
    int n = 5; for (int i = 0; i < n; i++) day[i] = in[i];

    sort_by_weight(day, n);
    printf("Sorted by weight:\n");
    for (int i = 0; i < n; i++) printf("  %d  %.1f kg\n", day[i].id, day[i].weight);
    printf("Parcels for Chennai:\n");  int c = list_by_city(day, n, "Chennai");
    printf("  (%d found)\n", c);
    printf("Parcels for Ooty:\n");     c = list_by_city(day, n, "Ooty");
    printf("  (%d found)  <- special case: none\n", c);

    printf("\nAccess paths for 3rd parcel (index 2):\n");
    Parcel *p = day;
    printf("  (i)  day[2].receiver.pincode  = %d\n", day[2].receiver.pincode);
    printf("  (ii) (p+2)->receiver.pincode  = %d\n", (p + 2)->receiver.pincode);

    printf("\nsizeof(Date)=%zu sizeof(Person)=%zu sizeof(Parcel nested)=%zu sizeof(FlatParcel)=%zu\n",
           sizeof(Date), sizeof(Person), sizeof(Parcel), sizeof(FlatParcel));
    printf("Bytes copied per swap in sort = sizeof(Parcel) = %zu\n", sizeof(Parcel));
    printf("Flat path would be: flat[2].r_pin ; nested path: day[2].receiver.pincode\n");
    return 0;
}
