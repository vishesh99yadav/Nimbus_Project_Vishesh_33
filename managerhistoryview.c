#include "managerhistoryview.h"

// -----------------------------------------------------------
// Function: manager_history_view()
// Purpose : Display history of fulfilled orders with customer name and remaining stock
// -----------------------------------------------------------
void manager_history_view() {
    printF("\n=== Manager: Order History Log ===\n");
    
    if (history_count == 0) {
        printf("No successful orders have been recorded yet.\n");
        return;
    }

    printf("Total Orders Registered: %d\n", total_orders_made);
    printf("------------------------------------------\n");

    for (int i = 0; i < history_count; i++) {
        printf("ORDER ID: %d | Customer: %s (Items: %d)\n", 
               order_history[i].order_id, order_history[i].customer_name, order_history[i].items_in_order);
        printf("  Fulfillment Details:\n");
        for (int j = 0; j < order_history[i].items_in_order; j++) {
            // Assumed defined in another file (e.g., inventory.c)
            int remaining_qty = get_remaining_stock(order_history[i].item_ids[j]); 
            
            // Display Allocated Qty AND Remaining Stock
            printf("  -> %s (ID %d): Allocated %d | **Stock Left: %d**\n", 
                    order_history[i].item_names[j], 
                    order_history[i].item_ids[j], 
                    order_history[i].quantities[j],
                    remaining_qty
            );
        }
        printf("------------------------------------------\n");
    }
}
