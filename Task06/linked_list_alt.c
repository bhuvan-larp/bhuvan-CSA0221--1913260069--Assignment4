/* Alternative: items in a singly linked list (dynamic structures) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct node { int id; char name[24]; float price; int stock; struct node *next; };

struct node *add_front(struct node *head, int id, const char *name, float price, int stock)  /* O(1) */
{
    struct node *n = malloc(sizeof *n);
    if (!n) return head;
    n->id = id; strncpy(n->name, name, 23); n->name[23] = 0; n->price = price; n->stock = stock;
    n->next = head; return n;
}
struct node *find(struct node *h, int id) { while (h && h->id != id) h = h->next; return h; }  /* O(n) */
struct node *remove_id(struct node *h, int id)                                                   /* O(n) find, O(1) unlink */
{
    struct node *prev = NULL, *cur = h;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) return h;
    if (prev) prev->next = cur->next; else h = cur->next;
    free(cur); return h;
}

struct arr_item { int id; char name[24]; float price; int stock; };

int main(void)
{
    enum { N = 20000 };
    struct node *head = NULL;
    clock_t t0 = clock();
    for (int i = 0; i < N; i++) head = add_front(head, i, "item", 10, 5);
    double t_add_ll = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;

    static struct arr_item a[N]; int cnt = 0;
    t0 = clock();                                  /* insert at front of array: shift all */
    for (int i = 0; i < N; i++) { memmove(&a[1], &a[0], cnt * sizeof a[0]); a[0].id = i; cnt++; }
    double t_add_ar = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;

    t0 = clock(); int hits = 0;
    for (int i = 0; i < 200; i++) hits += find(head, i) != NULL;     /* worst-ish: ids near the tail */
    double t_find_ll = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;
    t0 = clock();
    for (int i = 0; i < 200; i++) for (int j = 0; j < cnt; j++) if (a[j].id == i) { hits++; break; }
    double t_find_ar = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;

    t0 = clock();
    for (int i = 0; i < 200; i++) head = remove_id(head, i);
    double t_rem_ll = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;
    t0 = clock();
    for (int i = 0; i < 200; i++) { int j = 0; while (j < cnt && a[j].id != i) j++;
        if (j < cnt) { memmove(&a[j], &a[j+1], (cnt - j - 1) * sizeof a[0]); cnt--; } }
    double t_rem_ar = (double)(clock() - t0) * 1000 / CLOCKS_PER_SEC;

    printf("n = %d items (hits=%d)\n", N, hits);
    printf("add at front : list %.2f ms (O(1) each)   array %.2f ms (O(n) shift each)\n", t_add_ll, t_add_ar);
    printf("find 200 ids : list %.2f ms (O(n))        array %.2f ms (O(n))\n", t_find_ll, t_find_ar);
    printf("remove 200   : list %.2f ms               array %.2f ms (find + shift)\n", t_rem_ll, t_rem_ar);
    printf("sizeof(struct node) = %zu, sizeof(struct arr_item) = %zu -> next pointer + padding = %zu extra bytes per item\n",
           sizeof(struct node), sizeof(struct arr_item), sizeof(struct node) - sizeof(struct arr_item));
    return 0;
}
