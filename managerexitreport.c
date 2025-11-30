#include "managerexitreport.h"

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
    printf("| ID   | Name                | Quantity Left|\n");
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

    // 3.  FINAL MESSAGE
    printf("\n### A Message to Our Relief Partners\n");
    printf("> We recognize the weight of the tasks you face every day. \n");
    printf("> Every item requested, every order fulfilled, represents a life touched, a community supported. \n");
    printf("> Thank you for choosing us to be a small part of your massive effort. \n");
    printf("> Your dedication is the true inventory of hope. Keep up the essential work!\n");
    printf("============================================\n");
}
