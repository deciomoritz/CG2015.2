#include <iostream>

#include "include/Viewport.hpp"
#include "include/DisplayFile.hpp"

#include <gtk-3.0/gtk/gtk.h>

using namespace std;

GtkWidget *window;
GtkBuilder *builder;

DisplayFile* displayFile;

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {

	Viewport view;
	view.setCairo(cr);


	return FALSE;
}

int main(int argc, char* argv[]) {

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	GtkViewport* viewport = GTK_VIEWPORT(
			gtk_builder_get_object(builder, "viewport1"));
	g_signal_connect(G_OBJECT(viewport), "draw", G_CALLBACK (draw_cb), NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
