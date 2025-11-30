#include "querymenu.h"

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
    printf("2. **Place a Multi-Item Order Reqt**\n");
    printf("Choose an option: ");

    if (scanf("%d", &option) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    switch (option) {
        case 1:
            display_all_inventory(); // Assumed defined in another file (e.g., inventory.c)
            break;
            
        case 2:
            process_request(); // Assumed defined in another file (e.g., inventory.c)
            break;

        default:
            printf("Unknown option. Please choose 1 or 2.\n");
    }

    while (getchar() != '\n');
}
