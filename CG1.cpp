#include <iostream>
#include "include/ListaEnc.hpp"

#include <gtk-3.0/gtk/gtk.h>

using namespace std;

int main(int argc,char* argv[]) {

	ListaEnc<int> lista;

	cout << "!!!Hello World!!!" << endl;

	gtk_init(&argc, &argv);

	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_widget_set_size_request(window, 800, 600);

	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show (window);


	gtk_main();

	return 0;
}
