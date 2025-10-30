#include "ui.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    create_main_window();
    gtk_main();
    return 0;
}
