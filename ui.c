#include "ui.h"
#include "cart.h"
#include "payment.h"
#include <gtk/gtk.h>

GtkWidget *cart_list;
GtkWidget *total_label;

void update_cart_display() {
    gtk_list_store_clear(GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(cart_list))));
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(cart_list)));

    double total = 0.0;
    for (int i = 0; i < cart_count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, cart[i].name,
                           1, cart[i].quantity,
                           2, cart[i].price,
                           3, cart[i].quantity * cart[i].price,
                           -1);
        total += cart[i].quantity * cart[i].price;
    }

    char total_text[100];
    sprintf(total_text, "Total: ₹%.2f", total);
    gtk_label_set_text(GTK_LABEL(total_label), total_text);
}

void on_add_item(GtkButton *button, gpointer data) {
    const char *item = (const char *)data;
    add_to_cart(item, 1, 50.0);
    update_cart_display();
}

void on_checkout(GtkButton *button, gpointer data) {
    show_payment_window();
}

void create_main_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Shopping Cart");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    GtkWidget *add1 = gtk_button_new_with_label("Add Apple");
    GtkWidget *add2 = gtk_button_new_with_label("Add Banana");
    g_signal_connect(add1, "clicked", G_CALLBACK(on_add_item), "Apple");
    g_signal_connect(add2, "clicked", G_CALLBACK(on_add_item), "Banana");
    gtk_box_pack_start(GTK_BOX(button_box), add1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), add2, TRUE, TRUE, 0);

    GtkListStore *store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE, G_TYPE_DOUBLE);
    cart_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col1 = gtk_tree_view_column_new_with_attributes("Item", renderer, "text", 0, NULL);
    GtkTreeViewColumn *col2 = gtk_tree_view_column_new_with_attributes("Qty", renderer, "text", 1, NULL);
    GtkTreeViewColumn *col3 = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", 2, NULL);
    GtkTreeViewColumn *col4 = gtk_tree_view_column_new_with_attributes("Subtotal", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(cart_list), col1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(cart_list), col2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(cart_list), col3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(cart_list), col4);
    gtk_box_pack_start(GTK_BOX(vbox), cart_list, TRUE, TRUE, 0);

    total_label = gtk_label_new("Total: ₹0.00");
    gtk_box_pack_start(GTK_BOX(vbox), total_label, FALSE, FALSE, 0);

    GtkWidget *checkout = gtk_button_new_with_label("Checkout");
    g_signal_connect(checkout, "clicked", G_CALLBACK(on_checkout), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), checkout, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}
