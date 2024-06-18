#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"

void displayMenu() {
    printf("1. Unos proizvoda\n");
    printf("2. Ispis proizvoda\n");
    printf("3. Brisanje proizvoda\n");
    printf("4. Uredjivanje proizvoda\n");
    printf("5. Sortiranje proizvoda po cijeni (uzlazno)\n");
    printf("6. Sortiranje proizvoda po cijeni (silazno)\n");
    printf("7. Pretraga po tipu\n");
    printf("8. Pretraga po proizvodacu\n");
    printf("9. Pretraga po ID-u\n");
    printf("10. Brisanje svih proizvoda\n");
    printf("11. Izlaz\n");
    printf("12. Brisanje datoteke\n");
    printf("13. Preimenovanje datoteke\n\n");
}

int main() {
    loadProducts();
    int choice;
    do {
        displayMenu();
        choice = getValidInt("Unesite svoj izbor: ");
        switch (choice) {
        case 1:
            insertProduct();
            break;
        case 2:
            printProducts();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            editProduct();
            break;
        case 5:
            sortProductsByPriceAscending();
            printProducts();
            break;
        case 6:
            sortProductsByPriceDescending();
            printProducts();
            break;
        case 7:
            searchByType();
            break;
        case 8:
            searchByManufacturer();
            break;
        case 9:
            searchByID();
            break;
        case 10:
            deleteAllProducts();
            break;
        case 11:
            printf("Izlaz...\n\n");
            break;
        case 12:
            deleteInventoryFile();
            break;
        case 13:
            renameInventoryFile();
            break;
        default:
            printf("Neispravan izbor. Molimo pokusajte ponovno.\n\n");
        }
    } while (choice != 11);

    if (inventory != NULL) {
        free(inventory);
        inventory = NULL;
    }

    return 0;
}
