#include "payment.h"
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>

static GtkWidget *entry_card, *entry_exp, *entry_cvv;

int validate_card_number(const char *num) {
    int len = strlen(num);
    int sum = 0, alt = 0;
    for (int i = len - 1; i >= 0; i--) {
        int n = num[i] - '0';
        if (alt) {
            n *= 2;
            if (n > 9) n -= 9;
        }
        sum += n;
        alt = !alt;
    }
    return (sum % 10 == 0);
}

void on_pay(GtkButton *button, gpointer data) {
    const char *card = gtk_entry_get_text(GTK_ENTRY(entry_card));
    const char *exp = gtk_entry_get_text(GTK_ENTRY(entry_exp));
    const char *cvv = gtk_entry_get_text(GTK_ENTRY(entry_cvv));

    if (strlen(card) < 13 || !validate_card_number(card)) {
        gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid card number!");
        return;
    }
    if (strlen(exp) != 5 || exp[2] != '/') {
        gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid expiry date (MM/YY)!");
        return;
    }
    if (strlen(cvv) != 3) {
        gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid CVV!");
        return;
    }

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Payment Successful!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_payment_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Payment");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *lbl_card = gtk_label_new("Card Number:");
    GtkWidget *lbl_exp = gtk_label_new("Expiry (MM/YY):");
    GtkWidget *lbl_cvv = gtk_label_new("CVV:");

    entry_card = gtk_entry_new();
    entry_exp = gtk_entry_new();
    entry_cvv = gtk_entry_new();

    GtkWidget *pay_btn = gtk_button_new_with_label("Pay Now");
    g_signal_connect(pay_btn, "clicked", G_CALLBACK(on_pay), NULL);

    gtk_grid_attach(GTK_GRID(grid), lbl_card, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_card, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lbl_exp, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_exp, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lbl_cvv, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_cvv, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), pay_btn, 0, 3, 2, 1);

    gtk_widget_show_all(window);
}
