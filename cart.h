#ifndef CART_H
#define CART_H

#define MAX_ITEMS 50

typedef struct {
    char name[50];
    int quantity;
    double price;
} CartItem;

extern CartItem cart[MAX_ITEMS];
extern int cart_count;

void add_to_cart(const char *name, int qty, double price);

#endif
