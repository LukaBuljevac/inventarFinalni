#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"

#define FILENAME "inventory.bin"

Product* inventory = NULL;
int productCount = 0;

static void toLowerCase(char* str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

static int idExists(int id) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            return 1;
        }
    }
    return 0;
}

int getValidInt(const char* prompt) {
    if (prompt == NULL) return -1;
    int value;
    char input[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0';
            if (sscanf(input, "%d", &value) == 1) {
                return value;
            }
        }
        printf("Neispravan unos. Molimo unesite valjan broj.\n\n");
    }
}

float getValidFloat(const char* prompt) {
    if (prompt == NULL) return -1.0f;
    float value;
    char input[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0';
            if (sscanf(input, "%f", &value) == 1) {
                return value;
            }
        }
        printf("Neispravan unos. Molimo unesite valjan broj.\n\n");
    }
}

void getValidString(const char* prompt, char* output, int maxLength) {
    if (prompt == NULL || output == NULL || maxLength <= 0) return;
    char input[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin)) {
            if (strlen(input) <= maxLength) {
                input[strcspn(input, "\n")] = '\0';
                strncpy(output, input, maxLength);
                return;
            }
        }
        printf("Neispravan unos. Molimo unesite niz od najvise %d znakova.\n\n", maxLength);
    }
}

void insertProduct() {
    int id;
    do {
        id = getValidInt("Unesite ID proizvoda: ");
        if (idExists(id)) {
            printf("ID vec postoji. Molimo unesite drugi ID.\n\n");
        }
    } while (idExists(id));

    productCount++;
    Product* temp = realloc(inventory, productCount * sizeof(Product));
    if (!temp) {
        perror("Neuspjelo alociranje memorije");
        productCount--;
        return;
    }
    inventory = temp;

    inventory[productCount - 1].id = id;

    getValidString("Unesite naziv proizvoda: ", inventory[productCount - 1].name, MAX_NAME);
    getValidString("Unesite tip proizvoda: ", inventory[productCount - 1].type, MAX_TYPE);
    getValidString("Unesite proizvodaca: ", inventory[productCount - 1].manufacturer, MAX_MANUFACTURER);

    inventory[productCount - 1].price = getValidFloat("Unesite cijenu: ");
    inventory[productCount - 1].amount = getValidInt("Unesite kolicinu: ");

    saveProducts();
    printf("\n");
}

void printProducts() {
    for (int i = 0; i < productCount; i++) {
        printf("ID: %d\nNaziv: %s\nTip: %s\nProizvodac: %s\nCijena: %.2f\nKolicina: %d\n\n",
            inventory[i].id, inventory[i].name, inventory[i].type, inventory[i].manufacturer,
            inventory[i].price, inventory[i].amount);
    }
    printf("\n");
}

void deleteProduct() {
    int id = getValidInt("Unesite ID proizvoda za brisanje: ");
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            for (int j = i; j < productCount - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            productCount--;
            if (productCount > 0) {
                Product* temp = realloc(inventory, productCount * sizeof(Product));
                if (!temp) {
                    perror("Neuspjelo alociranje memorije");
                    productCount++;
                    return;
                }
                inventory = temp;
            }
            else {
                free(inventory);
                inventory = NULL;
            }
            saveProducts();
            printf("Proizvod izbrisan.\n\n");
            return;
        }
    }
    printf("Proizvod nije pronaden.\n\n");
}

void deleteAllProducts() {
    char choice;
    printf("Jeste li sigurni da zelite izbrisati sve proizvode? (d/n): ");
    choice = getchar();
    getchar();

    if (choice == 'd' || choice == 'D') {
        free(inventory);
        inventory = NULL;
        productCount = 0;
        saveProducts();
        printf("Svi proizvodi su izbrisani.\n\n");
    }
    else {
        printf("Operacija otkazana.\n\n");
    }
}

void editProduct() {
    int id = getValidInt("Unesite ID proizvoda za uredivanje: ");
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            getValidString("Unesite novi naziv proizvoda: ", inventory[i].name, MAX_NAME);
            getValidString("Unesite novi tip proizvoda: ", inventory[i].type, MAX_TYPE);
            getValidString("Unesite novog proizvodaca: ", inventory[i].manufacturer, MAX_MANUFACTURER);

            inventory[i].price = getValidFloat("Unesite novu cijenu: ");
            inventory[i].amount = getValidInt("Unesite novu kolicinu: ");

            saveProducts();
            printf("Proizvod ureden.\n\n");
            return;
        }
    }
    printf("Proizvod nije pronaden.\n\n");
}

int comparePriceAscending(const void* a, const void* b) {
    float priceA = ((Product*)a)->price;
    float priceB = ((Product*)b)->price;
    return (priceA > priceB) - (priceA < priceB);
}

int comparePriceDescending(const void* a, const void* b) {
    float priceA = ((Product*)a)->price;
    float priceB = ((Product*)b)->price;
    return (priceB > priceA) - (priceB < priceA);
}

void sortProductsByPriceAscending() {
    qsort(inventory, productCount, sizeof(Product), comparePriceAscending);
    printf("Proizvodi sortirani po cijeni (uzlazno).\n\n");
}

void sortProductsByPriceDescending() {
    qsort(inventory, productCount, sizeof(Product), comparePriceDescending);
    printf("Proizvodi sortirani po cijeni (silazno).\n\n");
}

void searchByType() {
    char type[MAX_TYPE];
    getValidString("Unesite tip proizvoda za pretragu: ", type, MAX_TYPE);
    toLowerCase(type);

    int found = 0;
    for (int i = 0; i < productCount; i++) {
        char productType[MAX_TYPE];
        strcpy(productType, inventory[i].type);
        toLowerCase(productType);
        if (strcmp(productType, type) == 0) {
            printf("ID: %d\nNaziv: %s\nTip: %s\nProizvodac: %s\nCijena: %.2f\nKolicina: %d\n\n",
                inventory[i].id, inventory[i].name, inventory[i].type, inventory[i].manufacturer,
                inventory[i].price, inventory[i].amount);
            found = 1;
        }
    }
    if (!found) {
        printf("Nema proizvoda tipa: %s\n\n", type);
    }
    else {
        printf("\n");
    }
}

void searchByManufacturer() {
    char manufacturer[MAX_MANUFACTURER];
    getValidString("Unesite proizvodaca za pretragu: ", manufacturer, MAX_MANUFACTURER);
    toLowerCase(manufacturer);

    int found = 0;
    for (int i = 0; i < productCount; i++) {
        char productManufacturer[MAX_MANUFACTURER];
        strcpy(productManufacturer, inventory[i].manufacturer);
        toLowerCase(productManufacturer);
        if (strcmp(productManufacturer, manufacturer) == 0) {
            printf("ID: %d\nNaziv: %s\nTip: %s\nProizvodac: %s\nCijena: %.2f\nKolicina: %d\n\n",
                inventory[i].id, inventory[i].name, inventory[i].type, inventory[i].manufacturer,
                inventory[i].price, inventory[i].amount);
            found = 1;
        }
    }
    if (!found) {
        printf("Nema proizvoda od proizvodaca: %s\n\n", manufacturer);
    }
    else {
        printf("\n");
    }
}

void searchByID() {
    int id = getValidInt("Unesite ID proizvoda za pretragu: ");
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            printf("ID: %d\nNaziv: %s\nTip: %s\nProizvodac: %s\nCijena: %.2f\nKolicina: %d\n\n",
                inventory[i].id, inventory[i].name, inventory[i].type, inventory[i].manufacturer,
                inventory[i].price, inventory[i].amount);
            return;
        }
    }
    printf("Proizvod nije pronaden.\n\n");
}

void saveProducts() {
    FILE* file = fopen(FILENAME, "wb");
    if (!file) {
        perror("Neuspjelo otvaranje datoteke za pisanje");
        exit(EXIT_FAILURE);
    }
    fwrite(inventory, sizeof(Product), productCount, file);

    // Provjera pozicije pokazivaca
    long pos = ftell(file);
    printf("Upisano je %ld bajtova u datoteku.\n", pos);

    fclose(file);
}

void loadProducts() {
    FILE* file = fopen(FILENAME, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        productCount = fileSize / sizeof(Product);
        if (productCount > 0) {
            inventory = malloc(productCount * sizeof(Product));
            if (!inventory) {
                perror("Neuspjelo alociranje memorije");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            fread(inventory, sizeof(Product), productCount, file);
        }
        fclose(file);
    }
}

void deleteInventoryFile() {
    if (remove(FILENAME) == 0) {
        printf("Datoteka %s uspjesno izbrisana.\n", FILENAME);
    }
    else {
        perror("Greska pri brisanju datoteke");
    }
}

void renameInventoryFile() {
    char newFilename[100];
    getValidString("Unesite novo ime datoteke: ", newFilename, sizeof(newFilename));
    if (rename(FILENAME, newFilename) == 0) {
        printf("Datoteka %s uspjesno preimenovana u %s.\n", FILENAME, newFilename);
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}
