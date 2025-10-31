#include "payment.h"
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static GtkWidget *entry_card = NULL;
static GtkWidget *entry_exp = NULL;
static GtkWidget *entry_cvv = NULL;
static double cart_total_local = 0.0;

/* Luhn algorithm */
static int validate_card_number(const char *num) {
    int len = (int)strlen(num);
    if (len < 13 || len > 19) return 0;
    int sum = 0;
    int alt = 0;
    for (int i = len - 1; i >= 0; --i) {
        if (!isdigit((unsigned char)num[i])) return 0;
        int n = num[i] - '0';
        if (alt) {
            n *= 2;
            if (n > 9) n -= 9;
        }
        sum += n;
        alt = !alt;
    }
    return (sum % 10) == 0;
}

/* helper to show message dialogs */
static void show_message(GtkMessageType type, const char *title, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK, "%s", msg);
    gtk_window_set_title(GTK_WINDOW(d), title);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

/* pay button callback */
static void on_pay_clicked(GtkButton *btn, gpointer data) {
    const char *card = gtk_entry_get_text(GTK_ENTRY(entry_card));
    const char *exp = gtk_entry_get_text(GTK_ENTRY(entry_exp));
    const char *cvv = gtk_entry_get_text(GTK_ENTRY(entry_cvv));

    if (!validate_card_number(card)) {
        show_message(GTK_MESSAGE_ERROR, "Invalid Card", "Card number appears invalid. Enter digits only (13-19) and a valid card number.");
        return;
    }

    if (strlen(exp) != 5 || exp[2] != '/' || !isdigit((unsigned char)exp[0]) || !isdigit((unsigned char)exp[1]) ||
        !isdigit((unsigned char)exp[3]) || !isdigit((unsigned char)exp[4])) {
        show_message(GTK_MESSAGE_ERROR, "Invalid Expiry", "Expiry must be in MM/YY format.");
        return;
    }

    if (strlen(cvv) < 3 || strlen(cvv) > 4) {
        show_message(GTK_MESSAGE_ERROR, "Invalid CVV", "CVV must be 3 or 4 digits.");
        return;
    }

    /* Append a payment note to cart.txt */
    FILE *f = fopen("cart.txt", "a");
    if (f) {
        fprintf(f, "\n--- Payment Completed ---\n");
        int L = (int)strlen(card);
        if (L >= 4) {
            fprintf(f, "Card (last4): %c%c%c%c\n", card[L-4], card[L-3], card[L-2], card[L-1]);
        } else {
            fprintf(f, "Card: (invalid length)\n");
        }
        fprintf(f, "Paid: ₹%.2f\n", cart_total_local);
        fprintf(f, "Status: SUCCESS\n");
        fclose(f);
    }

    char buf[128];
    snprintf(buf, sizeof(buf), "Payment of ₹%.2f successful!\nReceipt appended to cart.txt", cart_total_local);
    show_message(GTK_MESSAGE_INFO, "Payment Successful", buf);

    /* close payment window */
    GtkWidget *w = gtk_widget_get_toplevel(GTK_WIDGET(btn));
    gtk_widget_destroy(w);
}

/* show payment window and set local total */
void show_payment_window(double total) {
    cart_total_local = total;

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Payment");
    gtk_window_set_default_size(GTK_WINDOW(win), 360, 220);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(win), grid);

    GtkWidget *lbl_total = gtk_label_new(NULL);
    char total_markup[80];
    snprintf(total_markup, sizeof(total_markup), "<b>Total: ₹%.2f</b>", total);
    gtk_label_set_markup(GTK_LABEL(lbl_total), total_markup);

    GtkWidget *lbl_card = gtk_label_new("Card Number:");
    GtkWidget *lbl_exp = gtk_label_new("Expiry (MM/YY):");
    GtkWidget *lbl_cvv = gtk_label_new("CVV:");

    entry_card = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_card), "1234123412341234");

    entry_exp = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_exp), "MM/YY");

    entry_cvv = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_cvv), "123");
    gtk_entry_set_visibility(GTK_ENTRY(entry_cvv), FALSE);

    GtkWidget *btn_pay = gtk_button_new_with_label("Pay Now");
    g_signal_connect(btn_pay, "clicked", G_CALLBACK(on_pay_clicked), NULL);

    gtk_grid_attach(GTK_GRID(grid), lbl_total, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), lbl_card, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_card, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lbl_exp, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_exp, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lbl_cvv, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_cvv, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_pay, 0, 4, 2, 1);

    gtk_widget_show_all(win);
}
