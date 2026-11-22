/* Task 3.1-3.4 + alternatives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

enum kind { TEMPERATURE, HUMIDITY, STATUS };

/* Design A: plain structure holding all three fields */
struct packet_struct { enum kind type; float celsius; int percent; char status[8]; };
/* Design B: tagged union */
struct packet_union  { enum kind type; union { float celsius; int percent; char status[8]; } v; };
/* Alternative: malloc'd payload behind a void pointer */
struct packet_ptr    { enum kind type; void *data; };

void print_packet(const struct packet_union *p)
{
    switch (p->type) {
    case TEMPERATURE: printf("Temp %.1f C\n", p->v.celsius); break;
    case HUMIDITY:    printf("Humidity %d %%\n", p->v.percent); break;
    case STATUS:      printf("Status %s\n", p->v.status); break;
    default:          printf("Unknown tag %d\n", (int)p->type);
    }
}

int main(void)
{
    printf("sizeof(struct packet_struct) = %zu\n", sizeof(struct packet_struct));
    printf("sizeof(union only)           = %zu\n", sizeof(((struct packet_union *)0)->v));
    printf("sizeof(struct packet_union)  = %zu\n", sizeof(struct packet_union));
    printf("sizeof(struct packet_ptr)    = %zu (+ malloc payload 4..8 and malloc header, usually 16-24)\n", sizeof(struct packet_ptr));
    printf("offsetof(packet_union, v)    = %zu (4-byte tag, union needs only 4-byte alignment, so no padding)\n", offsetof(struct packet_union, v));
    printf("Memory for 10000 packets: struct=%zu B, union=%zu B, saving=%zu B\n",
           10000*sizeof(struct packet_struct), 10000*sizeof(struct packet_union),
           10000*(sizeof(struct packet_struct)-sizeof(struct packet_union)));

    struct packet_union a = {TEMPERATURE, {.celsius = 31.5f}};
    struct packet_union b = {HUMIDITY,    {.percent = 68}};
    struct packet_union c; c.type = STATUS; strcpy(c.v.status, "RAINING");   /* 7 chars + NUL */
    struct packet_union d = {STATUS, {.status = ""}};                         /* boundary: empty text */
    print_packet(&a); print_packet(&b); print_packet(&c); print_packet(&d);

    printf("\n--- Task 3.4 misuse: store temperature, read humidity ---\n");
    struct packet_union m; m.type = TEMPERATURE; m.v.celsius = 31.5f;
    printf("celsius = %.1f, percent (wrong member) = %d (hex 0x%08x)\n", m.v.celsius, m.v.percent, (unsigned)m.v.percent);

    printf("\n--- Alternative: void* payload ---\n");
    struct packet_ptr q; q.type = HUMIDITY; q.data = malloc(sizeof(int));
    *(int *)q.data = 68;
    printf("humidity via void*: %d (needs cast, extra pointer hop, free() needed)\n", *(int *)q.data);
    free(q.data);
    return 0;
}
