#include <iostream>

#include "include/ListaEnc.hpp"

#include <gtk-3.0/gtk/gtk.h>

using namespace std;

GtkWidget *window;
GtkBuilder *builder;

static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr, gpointer data, int x) {
	 cairo_set_source_rgb(cr, 0, 0, 0);
	 cairo_set_line_width(cr, 0.5);

	 cairo_move_to(cr, 0,0);
	 cairo_line_to(cr, 500,500);

	 cairo_stroke(cr);

	return FALSE;
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data)
{
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    return TRUE;
}

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	GtkViewport* viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport1"));
	g_signal_connect (G_OBJECT(viewport), "draw", G_CALLBACK (draw_cb), NULL);

	g_signal_connect (window, "button-press-event", G_CALLBACK (clicked), NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
