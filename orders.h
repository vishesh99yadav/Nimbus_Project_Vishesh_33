#ifndef ORDERS_H
#define ORDERS_H

#define MAX_ORDERS 50

typedef struct {
    int order_id;
    char customer[50];
    int item_count;
    int item_ids[3];
    char item_names[3][50];
    int quantities[3];
} Order;

extern Order order_log[MAX_ORDERS];
extern int order_index;
extern int next_id;
extern int total_orders;
extern int total_dispatched;

void place_order();
void show_order_history();
void final_report();

#endif
