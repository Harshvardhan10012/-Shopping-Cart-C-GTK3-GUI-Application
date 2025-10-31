#ifndef CART_H
#define CART_H

#define MAX_ITEMS 50

typedef struct {
    char name[64];
    int quantity;
    double price;
    double subtotal;
} CartItem;

typedef struct {
    CartItem items[MAX_ITEMS];
    int count;
    double total;
} Cart;

/* Cart functions */
void cart_init(Cart *c);
void cart_add_item(Cart *c, const char *name, double price);
void cart_remove_item(Cart *c, const char *name);
void save_cart_to_file(Cart *c, const char *filename);

#endif /* CART_H */
