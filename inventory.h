#define _CRT_SECURE_NO_WARNINGS
#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_TYPE 50
#define MAX_MANUFACTURER 50

typedef struct {
    int id;
    char name[MAX_NAME];
    char type[MAX_TYPE];
    char manufacturer[MAX_MANUFACTURER];
    float price;
    int amount;
} Product;

extern Product* inventory;
extern int productCount;

void insertProduct();
void printProducts();
void deleteProduct();
void deleteProductByID();
void editProduct();
void sortProductsByPriceAscending();
void sortProductsByPriceDescending();
void searchByType();
void searchByManufacturer();
void searchByID();
void deleteAllProducts();
void saveProducts();
void loadProducts();
int getValidInt(const char* prompt);
float getValidFloat(const char* prompt);
void getValidString(const char* prompt, char* output, int maxLength);
void deleteInventoryFile();
void renameInventoryFile();

#endif 
