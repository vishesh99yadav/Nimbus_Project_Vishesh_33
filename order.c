#include <stdio.h>
#include <string.h>
#include "orders.h"
#include "inventory.h"
#include "utils.h"

Order order_log[MAX_ORDERS];
int order_index = 0;
int next_id = 1;

int total_orders = 0;
int total_dispatched = 0;

void place_order() {
    if (inventory_count == 0) {
        printf("Stock empty.\n");
        return;
    }

    Order current = {0};
    current.order_id = next_id;

    printf("Customer name: ");
    scanf("%49s", current.customer);
    clear_input();

    show_inventory();

    int count;
    printf("How many different items? (max 3): ");
    scanf("%d", &count);
    clear_input();

    if (count < 1 || count > 3) {
        printf("Invalid count.\n");
        return;
    }

    int fulfilled = 0;
    int total_qty = 0;

    for (int i = 0; i < count; i++) {
        int id, req;
        printf("\nItem ID: ");
        scanf("%d", &id);

        printf("Quantity: ");
        scanf("%d", &req);
        clear_input();

        int index = -1;

        for (int j = 0; j < inventory_count; j++) {
            if (inventory_list[j].id == id)
                index = j;
        }

        if (index == -1) {
            printf("Invalid ID.\n");
            continue;
        }

        if (req <= inventory_list[index].qty) {
            inventory_list[index].qty -= req;

            current.item_ids[fulfilled] = id;
            strcpy(current.item_names[fulfilled], inventory_list[index].name);
            current.quantities[fulfilled] = req;

            total_qty += req;
            fulfilled++;

            printf("✔ Fulfilled %d of %s\n",
                   req,
                   inventory_list[index].name);
        } else {
            printf("Not enough stock.\n");
        }
    }

    if (fulfilled > 0) {
        current.item_count = fulfilled;

        order_log[order_index++] = current;
        next_id++;
        total_orders++;
        total_dispatched += total_qty;

        printf("\nOrder %d completed for %s\n",
               current.order_id,
               current.customer);
    } else {
        printf("No items fulfilled.\n");
    }
}

void show_order_history() {
    if (order_index == 0) {
        printf("No orders placed yet.\n");
        return;
    }

    printf("\nOrder History:\n");
    printf("-------------------------------\n");

    for (int i = 0; i < order_index; i++) {
        printf("Order %d | Customer: %s\n",
               order_log[i].order_id,
               order_log[i].customer);

        for (int j = 0; j < order_log[i].item_count; j++) {
            int rem = stock_left(order_log[i].item_ids[j]);
            printf("  %s: %d (Remaining: %d)\n",
                   order_log[i].item_names[j],
                   order_log[i].quantities[j],
                   rem);
        }
        printf("-------------------------------\n");
    }
}

void final_report() {
    printf("\n====== Summary Report ======\n");
    printf("Total Orders: %d\n", total_orders);
    printf("Total Units Dispatched: %d\n", total_dispatched);

    printf("\nRemaining Stock:\n");
    show_inventory();
}
