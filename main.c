#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_ORDERS 50

// Structure to store information about one product
typedef struct {
    int id;
    char name[50];
    int qty;
} Stock;

// Structure to store order transaction details with customer name
typedef struct {
    int order_id;
    char customer_name[50]; 
    int items_in_order;
    int item_ids[3]; 
    char item_names[3][50]; 
    int quantities[3];      
} OrderRecord;


Stock inventory[MAX_ITEMS];
int total_items = 0;

OrderRecord order_history[MAX_ORDERS];
int next_order_id = 1;
int history_count = 0;

// Global variables to track overall fulfillment
int total_orders_made = 0;
int total_quantity_fulfilled = 0;

// Function declarations
void add_items();
void query_menu();
void show_item_by_id(int id);
void display_all_inventory(void);
int get_remaining_stock(int item_id); 
void process_request(); 
void manager_history_view(); 
void manager_exit_report();

// -----------------------------------------------------------
// Helper Function: get_remaining_stock()
// Purpose : Finds the current quantity of an item by ID
// -----------------------------------------------------------
int get_remaining_stock(int item_id) {
    for (int i = 0; i < total_items; i++) {
        if (inventory[i].id == item_id) {
            return inventory[i].qty;
        }
    }
    return 0; // Return 0 if item is not found (or potentially deleted)
}

// -----------------------------------------------------------
// Function: add_items()
// Purpose : Allows manager to enter stock details
// -----------------------------------------------------------
void add_items() {
    int count;
    printf("\n=== Manager: Add Stock Items ===\n");

    printf("How many items do you want to add? ");
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Invalid number! Returning to main menu.\n");
        while (getchar() != '\n');
        return;
    }

    if (total_items + count > MAX_ITEMS) {
        printf("Only %d slots available. Adding only that much.\n", MAX_ITEMS - total_items);
        count = MAX_ITEMS - total_items;
    }

    for (int i = 0; i < count; i++) {
        printf("\nAdding Item %d:\n", total_items + 1);

        inventory[total_items].id = 101 + total_items;   // Auto ID
        printf("Assigned ID: %d\n", inventory[total_items].id);

        printf("Enter item name: ");
        scanf("%49s", inventory[total_items].name);

        printf("Enter quantity: ");
        if (scanf("%d", &inventory[total_items].qty) != 1 || inventory[total_items].qty < 0) {
            printf("Invalid quantity. Setting to 0.\n");
            inventory[total_items].qty = 0;
        }

        while (getchar() != '\n');

        total_items++;
    }

    printf("\n✔ Stock entry completed. Total items = %d\n", total_items);
}

// -----------------------------------------------------------
// Function: show_item_by_id() (Retained for internal/manager use)
// Purpose : Display details of a specific ID
// -----------------------------------------------------------
void show_item_by_id(int id) {
    for (int i = 0; i < total_items; i++) {
        if (inventory[i].id == id) {
            printf("\n--- Item Details ---\n");
            printf("ID       : %d\n", inventory[i].id);
            printf("Name     : %s\n", inventory[i].name);
            printf("Quantity : %d\n", inventory[i].qty);
            printf("---------------------\n");
            return;
        }
    }
    printf("\n❌ No item found with ID %d\n", id);
}

// -----------------------------------------------------------
// Function: display_all_inventory()
// Purpose : Display all items in inventory
// -----------------------------------------------------------
void display_all_inventory(void) {
    if (total_items == 0) {
        printf("Inventory empty. Nothing to display.\n");
        return;
    }
    printf("\n----- Full Inventory -----\n");
    printf("-------------------------------------------\n");
    printf("| ID   | Name              | Quantity     |\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < total_items; i++) {
        printf("| %-4d | %-17s | %-11d |\n",
            inventory[i].id,
            inventory[i].name,
            inventory[i].qty
        );
    }
    printf("-------------------------------------------\n");
}


// -----------------------------------------------------------
// Function: process_request()
// Purpose : Handle multi-item order request, check stock, reduce quantity, and record customer name
// -----------------------------------------------------------
void process_request() {
    int item_id, requested_qty;
    int index, num_items_to_order;
    int items_fulfilled = 0;
    int total_qty_for_order = 0;
    
    OrderRecord current_order = {0}; 
    current_order.order_id = next_order_id;

    printf("\n=== User: Place Multi-Item Order Request ===\n");
    
    if (total_items == 0) {
        printf("Inventory is empty. Cannot process orders.\n");
        return;
    }

    // Get customer name
    printf("Enter your name (e.g., Param): ");
    scanf("%49s", current_order.customer_name);
    while (getchar() != '\n'); 

    display_all_inventory(); // Show available items FIRST so the user knows the IDs!

    printf("\nHow many DIFFERENT types of items do you need (Max 3)? ");
    if (scanf("%d", &num_items_to_order) != 1 || num_items_to_order <= 0 || num_items_to_order > 3) {
        printf("Invalid number of items. Order cancelled.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); 

    // Loop through each item the user wants to order
    for (int i = 0; i < num_items_to_order; i++) {
        index = -1;
        printf("\n--- Item %d of %d ---\n", i + 1, num_items_to_order);

        printf("Enter Item ID (from list above): ");
        if (scanf("%d", &item_id) != 1) {
            printf("Invalid ID format. Skipping this item.\n");
            while (getchar() != '\n');
            continue;
        }

        // Find the item index
        for (int j = 0; j < total_items; j++) {
            if (inventory[j].id == item_id) {
                index = j;
                break;
            }
        }

        if (index == -1) {
            printf("❌ Item ID %d not found. Skipping this item.\n", item_id);
            continue;
        }

        printf("Enter quantity for '%s' (Available: %d): ", inventory[index].name, inventory[index].qty);
        if (scanf("%d", &requested_qty) != 1 || requested_qty <= 0) {
            printf("Invalid quantity. Skipping this item.\n");
            continue;
        }

        // Check stock and fulfill
        if (requested_qty <= inventory[index].qty) {
            // SUCCESS: Reduce stock and record details
            inventory[index].qty -= requested_qty;
            
            // Record to temporary OrderRecord (Saving ID, Name, and Quantity)
            current_order.item_ids[items_fulfilled] = item_id;
            strcpy(current_order.item_names[items_fulfilled], inventory[index].name);
            current_order.quantities[items_fulfilled] = requested_qty;
            
            total_qty_for_order += requested_qty;
            items_fulfilled++;
            
            printf("   -> ✅ %d units of %s fulfilled. Remaining: %d\n", requested_qty, inventory[index].name, inventory[index].qty);

        } else {
            // FAILURE: Insufficient stock
            printf("   -> ⚠️ Insufficient stock! Only %d units available. Item NOT added to order.\n", inventory[index].qty);
        }
        while (getchar() != '\n');
    }

    // Finalize the transaction
    if (items_fulfilled > 0) {
        // Update global counters and save history
        total_orders_made++;
        total_quantity_fulfilled += total_qty_for_order;
        
        current_order.items_in_order = items_fulfilled;
        
        if (history_count < MAX_ORDERS) {
            order_history[history_count++] = current_order;
        } else {
            printf("\n⚠️ Order History full. Current order not saved to log.\n");
        }
        next_order_id++;

        printf("\n⭐ ORDER #%d COMPLETED for %s! Total %d units shipped. ⭐\n", 
               current_order.order_id, current_order.customer_name, total_qty_for_order);
    } else {
        printf("\n❌ Order Cancelled: No items could be fulfilled (or none requested).\n");
    }
}


// -----------------------------------------------------------
// Function: query_menu()
// Purpose : Lets users check available stock and place order (REMOVED ID SEARCH)
// -----------------------------------------------------------
void query_menu() {
    int option;

    printf("\n=== User: Stock Query & Order Menu ===\n");

    if (total_items == 0) {
        printf("Inventory empty. Nothing to display or order.\n");
        return;
    }

    printf("1. View All Items\n");
    printf("2. **Place a Multi-Item Order Request**\n"); // Ordering inherently requires viewing the list first
    printf("Choose an option: ");

    if (scanf("%d", &option) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    switch (option) {
        case 1:
            display_all_inventory();
            break;
            
        case 2:
            process_request();
            break;

        default:
            printf("Unknown option. Please choose 1 or 2.\n");
    }

    while (getchar() != '\n');
}

// -----------------------------------------------------------
// Function: manager_history_view()
// Purpose : Display history of fulfilled orders with customer name and remaining stock
// -----------------------------------------------------------
void manager_history_view() {
    printf("\n=== Manager: Order History Log ===\n");
    
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


// -----------------------------------------------------------
// Function: manager_exit_report()
// Purpose : Prints final inventory, sales, and emotional closing
// -----------------------------------------------------------
void manager_exit_report() {
    int total_remaining_qty = 0;
    
    printf("\n============================================\n");
    printf("📦 FINAL MANAGER & DISASTER RELIEF REPORT 📦\n");
    printf("============================================\n");

    // 1. Order Summary
    printf("## Sales/Fulfillment Summary\n");
    printf("-------------------------------------------\n");
    printf("Total Successful Orders Registered: %d\n", total_orders_made);
    printf("Total Quantity Fulfilled: %d units\n", total_quantity_fulfilled);
    printf("-------------------------------------------\n");

    // 2. Remaining Stock
    printf("\n## Remaining Inventory\n");
    printf("-------------------------------------------\n");
    printf("| ID   | Name              | Quantity Left|\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < total_items; i++) {
        printf("| %-4d | %-17s | %-12d |\n",
            inventory[i].id,
            inventory[i].name,
            inventory[i].qty
        );
        total_remaining_qty += inventory[i].qty;
    }
    printf("-------------------------------------------\n");
    printf("Total remaining units across all items: %d\n", total_remaining_qty);

    // 3. Emotional Connection
    printf("\n### A Message to Our Relief Partners\n");
    printf("> We recognize the weight of the tasks you face every day. \n");
    printf("> Every item requested, every order fulfilled, represents a life touched, a community supported. \n");
    printf("> Thank you for choosing us to be a small part of your massive effort. \n");
    printf("> Your dedication is the true inventory of hope. Keep up the essential work!\n");
    printf("============================================\n");
}


// -----------------------------------------------------------
// Main Driver
// -----------------------------------------------------------
int main() {
    int choice;

    printf("=== Simple Inventory Management System ===\n");

    do {
        printf("\nSelect your role:\n");
        printf("1. Manager\n");
        printf("2. User (View/Request Stock)\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        while (getchar() != '\n');
        
        // Manager Sub-Menu
        if (choice == 1) {
            int manager_choice;
            printf("\n--- Manager Menu ---\n");
            printf("1. Add New Stock\n");
            printf("2. View Order History\n");
            printf("Enter manager choice: ");
            
            if (scanf("%d", &manager_choice) != 1) {
                printf("Invalid input. Returning to main menu.\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            switch (manager_choice) {
                case 1: add_items(); break;
                case 2: manager_history_view(); break;
                default: printf("Invalid manager choice.\n");
            }
        } else {
            // Main Menu Options
            switch (choice) {
                case 2: query_menu(); break;
                case 3: 
                    manager_exit_report(); 
                    break;
                default: printf("Invalid choice.\n");
            }
        }

    } while (choice != 3);

    return 0;
}
