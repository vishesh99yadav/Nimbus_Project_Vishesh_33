// JUST DEFINING THE INVENTORY FUNCTION

#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_ITEMS 100

typedef struct {
    int id;
    char name[50];
    int qty;
} Item;

extern Item inventory_list[MAX_ITEMS];
extern int inventory_count;

void add_stock();
void show_inventory();
int stock_left(int id);

#endif
