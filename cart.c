#include "cart.h"
#include <string.h>

CartItem cart[MAX_ITEMS];
int cart_count = 0;

void add_to_cart(const char *name, int qty, double price) {
    for (int i = 0; i < cart_count; i++) {
        if (strcmp(cart[i].name, name) == 0) {
            cart[i].quantity += qty;
            return;
        }
    }
    strcpy(cart[cart_count].name, name);
    cart[cart_count].quantity = qty;
    cart[cart_count].price = price;
    cart_count++;
}
