#include <string.h>
#include <stdio.h>
#include "cart.h"

/* Initialize cart */
void cart_init(Cart *c) {
    c->count = 0;
    c->total = 0.0;
    for (int i = 0; i < MAX_ITEMS; ++i) {
        c->items[i].name[0] = '\0';
        c->items[i].quantity = 0;
        c->items[i].price = 0.0;
        c->items[i].subtotal = 0.0;
    }
}

/* Add item (if exists increment qty) */
void cart_add_item(Cart *c, const char *name, double price) {
    if (!c || !name) return;
    for (int i = 0; i < c->count; ++i) {
        if (strcmp(c->items[i].name, name) == 0) {
            c->items[i].quantity += 1;
            c->items[i].subtotal = c->items[i].quantity * c->items[i].price;
            /* recalc total */
            c->total = 0.0;
            for (int j = 0; j < c->count; ++j) c->total += c->items[j].subtotal;
            return;
        }
    }
    /* add new item */
    if (c->count < MAX_ITEMS) {
        strncpy(c->items[c->count].name, name, sizeof(c->items[c->count].name)-1);
        c->items[c->count].name[sizeof(c->items[c->count].name)-1] = '\0';
        c->items[c->count].quantity = 1;
        c->items[c->count].price = price;
        c->items[c->count].subtotal = price * 1;
        c->count++;
        /* recalc total */
        c->total = 0.0;
        for (int j = 0; j < c->count; ++j) c->total += c->items[j].subtotal;
    }
}

/* Remove item by name (first match) */
void cart_remove_item(Cart *c, const char *name) {
    if (!c || !name) return;
    for (int i = 0; i < c->count; ++i) {
        if (strcmp(c->items[i].name, name) == 0) {
            /* shift left */
            for (int j = i; j < c->count - 1; ++j) {
                c->items[j] = c->items[j + 1];
            }
            c->count--;
            break;
        }
    }
    /* recalc total */
    c->total = 0.0;
    for (int j = 0; j < c->count; ++j) c->total += c->items[j].subtotal;
}

/* Save cart to a human-readable text file */
void save_cart_to_file(Cart *c, const char *filename) {
    if (!c || !filename) return;
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "Shopping Cart Summary\n");
    fprintf(f, "---------------------\n");
    for (int i = 0; i < c->count; ++i) {
        fprintf(f, "Item: %s | Qty: %d | Price: ₹%.2f | Subtotal: ₹%.2f\n",
                c->items[i].name, c->items[i].quantity, c->items[i].price, c->items[i].subtotal);
    }
    fprintf(f, "\nTotal Amount: ₹%.2f\n", c->total);
    fprintf(f, "---------------------\n");
    fclose(f);
}
