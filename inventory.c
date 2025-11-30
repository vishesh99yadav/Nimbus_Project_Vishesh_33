#include <stdio.h>
#include <string.h>
#include "inventory.h"
#include "utils.h"

Item inventory_list[MAX_ITEMS];
int inventory_count = 0;

int stock_left(int id) {
    for (int i = 0; i < inventory_count; i++) {
        if (inventory_list[i].id == id)
            return inventory_list[i].qty;
    }
    return 0;
}

void add_stock() {
    int count;
    printf("\nEnter number of items to add: ");
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Invalid amount.\n");
        clear_input();
        return;
    }
    clear_input();

    if (inventory_count + count > MAX_ITEMS)
        count = MAX_ITEMS - inventory_count;

    for (int i = 0; i < count; i++) {
        int idx = inventory_count;

        inventory_list[idx].id = 100 + idx + 1;

        printf("\nItem %d ID: %d\n", idx + 1, inventory_list[idx].id);
        printf("Name: ");
        scanf("%49s", inventory_list[idx].name);

        printf("Quantity: ");
        scanf("%d", &inventory_list[idx].qty);
        clear_input();

        inventory_count++;
    }

    printf("\nStock successfully updated.\n");
}

void show_inventory() {
    if (inventory_count == 0) {
        printf("No stock available.\n");
        return;
    }

    printf("\nAvailable Stock:\n");
    printf("-----------------------------\n");
    printf("ID    Name           Qty\n");
    printf("-----------------------------\n");

    for (int i = 0; i < inventory_count; i++) {
        printf("%-5d %-15s %d\n",
               inventory_list[i].id,
               inventory_list[i].name,
               inventory_list[i].qty);
    }
}
