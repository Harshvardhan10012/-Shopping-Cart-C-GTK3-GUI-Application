#include "ui.h"
#include "cart.h"
#include "payment.h"
#include <gtk/gtk.h>
#include <stdio.h>

static GtkWidget *treeview;
static GtkWidget *total_label;
static GtkListStore *store;
static Cart cart;

/* Forward declarations */
static void on_add_item_clicked(GtkButton *button, gpointer data);
static void on_remove_item_clicked(GtkButton *button, gpointer data);
static void on_checkout_clicked(GtkButton *button, gpointer data);

void create_main_window(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *vbox, *hbox, *button_apple, *button_banana, *button_remove, *button_checkout;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Shopping Cart");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    button_apple = gtk_button_new_with_label("Add Apple");
    button_banana = gtk_button_new_with_label("Add Banana");
    button_remove = gtk_button_new_with_label("Remove Selected");

    g_signal_connect(button_apple, "clicked", G_CALLBACK(on_add_item_clicked), "Apple");
    g_signal_connect(button_banana, "clicked", G_CALLBACK(on_add_item_clicked), "Banana");
    g_signal_connect(button_remove, "clicked", G_CALLBACK(on_remove_item_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), button_apple, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_banana, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_remove, TRUE, TRUE, 0);

    /* list store columns: 0=item,1=qty,2=price,3=subtotal */
    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE, G_TYPE_DOUBLE);
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Item", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Qty", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Price (₹)", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Subtotal (₹)", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled), treeview);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 6);

    total_label = gtk_label_new("Total: ₹0.00");
    gtk_box_pack_start(GTK_BOX(vbox), total_label, FALSE, FALSE, 6);

    button_checkout = gtk_button_new_with_label("Checkout / Pay");
    g_signal_connect(button_checkout, "clicked", G_CALLBACK(on_checkout_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_checkout, FALSE, FALSE, 0);

    /* initialize cart */
    cart_init(&cart);

    gtk_widget_show_all(window);
}

/* helper to refresh the tree view from cart struct */
static void refresh_cart_view(void) {
    gtk_list_store_clear(store);
    for (int i = 0; i < cart.count; ++i) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, cart.items[i].name,
                           1, cart.items[i].quantity,
                           2, cart.items[i].price,
                           3, cart.items[i].subtotal,
                           -1);
    }
    char buf[64];
    snprintf(buf, sizeof(buf), "Total: ₹%.2f", cart.total);
    gtk_label_set_text(GTK_LABEL(total_label), buf);
}

/* add item handler */
static void on_add_item_clicked(GtkButton *button, gpointer data) {
    const gchar *item = (const gchar *)data;
    double price = (g_strcmp0(item, "Apple") == 0) ? 50.0 : 30.0;
    cart_add_item(&cart, item, price);
    refresh_cart_view();
}

/* remove selected row handler */
static void on_remove_item_clicked(GtkButton *button, gpointer data) {
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
        gchar *name = NULL;
        gtk_tree_model_get(model, &iter, 0, &name, -1);
        if (name) {
            cart_remove_item(&cart, name);
            g_free(name);
            refresh_cart_view();
        }
    } else {
        /* nothing selected - show info */
        GtkWidget *d = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "No item selected to remove.");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
    }
}

/* checkout handler: save and open payment window (passing total) */
static void on_checkout_clicked(GtkButton *button, gpointer data) {
    save_cart_to_file(&cart, "cart.txt");
    show_payment_window(cart.total);
}
